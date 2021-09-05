#include "ttoa.h"
#include <stdio.h>

int main(int argc, char *args[])
{
  if (argc < 2) {
    printf("Supply a file to convert\n");
    return -1;
  }

  if (ANIMD_ttoa(args[1])) {
    printf("Could not convert\n");
    return -1;
  }

  return 0;
}
