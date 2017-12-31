#include "debug.h"
#include "utf.h"
#include "wrappers.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int infile, outfile;
  int in_flags, out_flags;
  parse_args(argc, argv);
  check_bom();
  print_state();
  in_flags = O_RDONLY;
  out_flags = O_WRONLY | O_CREAT;
  FILE *file;
  if((file = fopen(program_state->out_file, "w"))) {
    remove(program_state->out_file);
    fclose(file);
  }
  infile = Open(program_state->in_file, in_flags);
  outfile = Open(program_state->out_file, out_flags);
  lseek(SEEK_SET, program_state->bom_length, infile); /* Discard BOM */
  get_encoding_function()(infile, outfile);
  //printf("%s\n", "I AM HERE IN MAIN!!");

   if(program_state != NULL) {
     //free(program_state->in_file);
     //free(program_state->out_file);
     free(program_state);
   }
  //I think this is how this works
  close(outfile);
  close(infile);
  return EXIT_SUCCESS;
}