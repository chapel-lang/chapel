#include "chplio.h"

void _write_integer64(FILE* outfile, integer64 val) {
  fprintf(outfile, _format_integer64, val);
}
