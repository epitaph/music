/* From stackoverflow:
   https://stackoverflow.com/questions/6866103/finding-all-the-devices-i-can-use-to-play-pcm-with-alsa

   adapted by epitaph
*/

#include <alsa/asoundlib.h>
#include <stdio.h>

int main(int argc, char argv[]) {
  char **hints;
  /* Enumerate sound devices */
  int err = snd_device_name_hint(-1, "pcm", (void***)&hints);
  if (err != 0)
     return 1;//Error! Just return
  
  char** n = hints;
  while (*n != NULL) {
  
      char *name = snd_device_name_get_hint(*n, "NAME");
  
      if (name != NULL && 0 != strcmp("null", name)) {
          //Copy name to another buffer and then free it
          printf("available: %s\n", name);
          free(name);
      }
      n++;
  }//End of while
  
  //Free hint buffer too
  snd_device_name_free_hint((void**)hints);
  return 0;
}

