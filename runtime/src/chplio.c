#include "chplio.h"

void _write_integer64(FILE* outfile, char* format, _integer64 val) {
  fprintf(outfile, format, val);
}
