// Write a signal to a WAV file.
//
// TODO:
// * Error checking!
// * Fix bit depth dependency in some locations
// * Integer lengths need checking
// * Command line options
//
// NOTDOING:
// * Output for non-files, e. g. pipelines, unless I need it

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

// Calculate lengths of arrays
#define A_LEN(var, ty) (sizeof(var)/sizeof(ty))

// Bet this is in a header somewhere, damned if I can find it...
#define PI (3.14159265358979323846L)
#define TAU (2.0L*PI)

char const static riff_hdr[4]="RIFF";
size_t const static riff_hdr_len=A_LEN(riff_hdr, char);
char const static wave_hdr[4]="WAVE";
size_t const static wave_hdr_len=A_LEN(wave_hdr, char);
char const static fmt_hdr[4]="fmt ";
char const static data_hdr[4]="data";
size_t const static fmt_hdr_len=A_LEN(fmt_hdr, char);
int16_t const static pcm_ident=1;

int32_t const static dflt_flen=0x41414141;
int32_t const static dflt_dlen=0x61616161;

struct wav_header {
  int32_t wfl;
  int32_t wdl;
  int16_t nc;
  int16_t bps;
  int32_t sr;
  int32_t ds;
  FILE * f;
};

char const static siggen_short_opts[]="+stqwi";
struct option const static siggen_long_opts [] = {
  {
    "sine",
    no_argument,
    NULL,
    's'
  },
  {
    "square",
    no_argument,
    NULL,
    'q'
  },
  {
    "triangle",
    no_argument,
    NULL,
    't'
  },
  {
    "sawtooth",
    no_argument,
    NULL,
    'w'
  },
  {
    "invert",
    no_argument,
    NULL,
    'i'
  },
  {
    NULL,
    0,
    NULL,
    0
  }
};

void sg_error(int const sg_errno, char const * sg_errstr) {
  fprintf(stderr, "%s\n", sg_errstr);
  exit(sg_errno);
}

struct options {
  long double const (* wave_function)(long double const);
  int invert;
};

void init_options(struct options * opts) {
  opts->wave_function=&sinl;
  opts->invert=0;
}

long double const wave_square(long double const omega) {
  return 0.0L;
}

long double const wave_sawtooth(long double const omega) {
  return 0.0L;
}

long double const wave_triangle(long double const omega) {
  return 0.0L;
}

int parse_options(struct options * sg_opts, int argc, char * argv[]) {
  int opt;

  while(
      (opt=getopt_long(
        argc,
        argv,
        siggen_short_opts,
        siggen_long_opts,
        NULL
        )
      )!=-1) {
    switch(opt) {
      case 'i':
        sg_opts->invert=!sg_opts->invert;
      break;

      case 's':
        sg_opts->wave_function=&sinl;
      break;

      case 'q':
        sg_opts->wave_function=&wave_square;
      break;

      case 'w':
        sg_opts->wave_function=&wave_sawtooth;
      break;

      case 't':
        sg_opts->wave_function=&wave_triangle;
      break;

      default:
        sg_error(1, "Unknown option");
      break;
    }
  }
}

void wav_set_n_channels(struct wav_header * const wh,
                        int16_t const n_channels) {
  wh->nc=n_channels;
}

void wav_set_sample_rate(struct wav_header * const wh,
                         int32_t const sample_rate) {
  wh->sr=sample_rate;
}

void wav_set_bits_per_sample(struct wav_header * const wh,
                             int16_t const bits_per_sample) {
  wh->bps=bits_per_sample;
}

// This section is a bit dependent on the bit depth being 16, and should
// be fixed at some point

void wav_write_sample(struct wav_header * const wh,
                      int16_t const sample) {
  fwrite(&sample, sizeof(int16_t), 1, wh->f);
}

void wav_write_square(struct wav_header * const wh,
                      long double const frequency,
                      long double const length) {
  long double const omega=TAU*frequency; 
  long double const delta_omega=omega/wh->sr;
  long double omega_pos=0.0L;
  for(int pos=0; pos<wh->sr*length; ++pos) {
    int const sample=omega_pos<PI?32767:-32767;
    for(int i=wh->nc; i; --i)
      wav_write_sample(wh, sample);
//    fprintf(stderr, "%d\n", (int)sample);
    omega_pos+=delta_omega;
    if(omega_pos>=TAU)
      omega_pos-=TAU;
  }
}

// end of bit depth 16 section

void wav_write_initial_header(struct wav_header * const wh,
                              FILE * f) {
  union {
    int32_t i32;
    int16_t i16;
  } tmp;
  wh->f=f;
  // riff header, bytes 0-3
  fwrite(riff_hdr, sizeof(char), riff_hdr_len, f);
  // record position for overall length. Info is contradictory as to
  // whether this is 4 or 8 bytes; alignment makes 4 seem more likely
  wh->wfl=ftell(f);
  fprintf(stderr, "%d\n", (int)wh->wfl);
  // placeholder for file length, bytes 4-7
  fwrite(&dflt_flen, sizeof(int32_t), 1, f);
  // wave filetype header, bytes 8-11
  fwrite(wave_hdr, sizeof(char), wave_hdr_len, f);
  // format header, bytes 12-15
  fwrite(fmt_hdr, sizeof(char), fmt_hdr_len, f);
  // We have to write the header length in here. Yes, I don't know
  // either; it seems useless at the end ... bytes 16-19
  tmp.i32=16;
  fprintf(stderr, "%d\n", (int)tmp.i32);
  fwrite(&tmp.i32, sizeof(int32_t), 1, f);
  // encoding type, pcm. Bytes 20-21
  fwrite(&pcm_ident, sizeof(int16_t), 1, f);
  // No. of channels, bytes 22-23
  fwrite(&wh->nc, sizeof(int16_t), 1, f);
  // Sample rate, bytes 24-27
  fwrite(&wh->sr, sizeof(int32_t), 1, f);
  // Appears to be bytes per second, bytes 28-31
  tmp.i32=wh->sr*wh->bps*wh->nc/8;
  fwrite(&tmp.i32, sizeof(int32_t), 1, f);
  // Appears to be the data "stride", i. e. length from one sample set
  // to the next. Bytes 32-33
  tmp.i16=wh->bps*wh->nc/8;
  fwrite(&tmp.i16, sizeof(int16_t), 1, f);
  // Bits per sample. Bytes 34-35
  fwrite(&wh->bps, sizeof(int16_t), 1, f);
  // Data header. Bytes 36-39
  fwrite(data_hdr, sizeof(char), A_LEN(data_hdr, char), f);
  // Placeholder for data size. Bytes 40-43
  wh->wdl=ftell(f);
  fwrite(&dflt_dlen, sizeof(int32_t), 1, f);
  wh->ds=ftell(f);
}

void wav_write_header_closure(struct wav_header * const wh) {
  int32_t tmp_i32=ftell(wh->f)-4;
  fprintf(stderr, "%d\n", (int)tmp_i32);
  fseek(wh->f, wh->wfl, SEEK_SET);
  fwrite(&tmp_i32, sizeof(int32_t), 1, wh->f);
  fseek(wh->f, wh->wdl, SEEK_SET);
  tmp_i32=tmp_i32-wh->ds;
  fwrite(&tmp_i32, sizeof(int32_t), 1, wh->f);
}

int main(int argc, char * argv[]) {
  struct wav_header wh;
  struct options sgo;
  init_options(&sgo);
  parse_options(&sgo, argc, argv);
  wav_set_sample_rate(&wh, 48000);
  wav_set_n_channels(&wh, 2);
  wav_set_bits_per_sample(&wh, 16);
  wav_write_initial_header(&wh, stdout);
  wav_write_square(&wh, 440.0, 5.0);
  wav_write_header_closure(&wh);

  return 0;
}
  
