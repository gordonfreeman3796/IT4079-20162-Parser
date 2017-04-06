/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>

#include "reader.h"
#include "parser.h"

/******************************************************************/

FILE *outputStream;

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("Parser: no input file.\n");
    return -1;
  }

  outputStream = fopen("result.txt", "w");
  if (compile(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }
  fclose(outputStream);
  return 0;
}
