#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "png.h"

extern void crush(char *infilename, char *outfilename);
extern void decrush(char *infilename, char *outfilename);

int willcrush = -1;

void usage() {
  printf("ios pngcrush/decrush utility\n\n");
  printf("usage:\n");
  printf("-c <files>: crush png files\n");
  printf("-d <files>: de-crush png files\n");
  printf("-h        : show this message\n");
  printf("<files>   : crush/decrush png automatically\n");
  exit(0);
}

int iscrushed(char *file)
{
  if(!file)
    return -1;
  FILE *png = fopen(file, "rb");
  if(!png)
    return -1;
  png_byte header[8];
  fread(header, 1, 8, png);
  if(png_sig_cmp(header, 0, 8) != 0)
    return -1;
  fseek(png, 12, SEEK_SET);
  png_byte cgbi[4];
  fread(cgbi, 1, 4, png);
  if(strncmp((char*) cgbi, "CgBI", 4) == 0) {
    fclose(png);
    return 1;
  }
  else {
    fclose(png);
    return 0;
  }
}

int main(int argc, char **argv)
{
  char *cmd_name = argv[0];
  if(argc == 1)
    usage();

  char optflag;
  while((optflag = getopt(argc, argv, "hcd")) != -1) {
    switch(optflag) {
    case 'c':
      willcrush = 1;
      break;
    case 'd':
      willcrush = 0;
      break;
    case 'h':
    default:
      usage();
    }
  }

  argc = argc - optind;
  argv = argv + optind;
  int i;
  for(i = 0; i < argc; i++) {
    if(willcrush == 1 && iscrushed(argv[i]) == 0) { 
      crush(argv[i],NULL);
    }
    else if(willcrush == 0 && iscrushed(argv[i]) == 1) { 
      decrush(argv[i],NULL);
    }
    else if(willcrush == -1 && iscrushed(argv[i]) != -1){//crush untouched.crush uncrush png, decrush crush png.
      if(!iscrushed(argv[i]))
        crush(argv[i],NULL);
      else
        decrush(argv[i],NULL);
    }
    else 
        continue;
  }
  return 0;
}
