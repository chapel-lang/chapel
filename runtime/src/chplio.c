#include "chplio.h"
#include "domain.h"


void _write_linefeed(FILE* outfile) {
  fprintf(outfile, "\n");
}


void _write_integer64(FILE* outfile, char* format, _integer64 val) {
  fprintf(outfile, format, val);
}


void _write_string(FILE* outfile, char* format, _string val) {
  fprintf(outfile, format, val);
}


void _write_domain1(FILE* outfile, char* format, _domain1 val) {
  fprintf(outfile, format, val.dim[0].lo, val.dim[0].hi, val.dim[0].str);
}
