#include <stdio.h>

int main(int argv, char ** argc) {
  char ** argp;

  for(argp=argc; *argp; ++argp) {
    unsigned char * sp;
    printf("Arg %2d:", (int)(argp-argc));
    for(sp=*argp; *sp; ++sp)
      printf(" %2x", (unsigned int)*sp);
    printf("\n");
  }

  return 0;
}

