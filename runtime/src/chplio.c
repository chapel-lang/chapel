#include "chplio.h"
#include "domain.h"


void _write_linefeed(FILE* outfile) {
  fprintf(outfile, "\n");
}


void _write_integer64(FILE* outfile, char* format, _integer64 val) {
  fprintf(outfile, format, val);
}


void _write_float64(FILE* outfile, char* format, _float64 val) {
  fprintf(outfile, format, val);
}


void _write_string(FILE* outfile, char* format, _string val) {
  fprintf(outfile, format, val);
}


void _write_domain1(FILE* outfile, char* format, _domain1 val) {
  fprintf(outfile, format, val.dim[0].lo, val.dim[0].hi, val.dim[0].str);
}


void _write_array1_integer64(FILE* outfile, char* format, _array1_integer64 arr) {
  _domain1 dom;
  int i;

  dom = *(arr.domain);

  for (i=dom.dim[0].lo; i<=dom.dim[0].hi; i+=dom.dim[0].str) {
    fprintf(outfile, format, _ACC1(arr, i));
    if (i<dom.dim[0].hi) {
      fprintf(outfile, " ");
    }
  }
}


void _write_array2_float64(FILE* outfile, char* format, _array2_float64 arr) {
  _domain2 dom;
  int i, j;

  dom = *(arr.domain);

  for (i=dom.dim[0].lo; i<=dom.dim[0].hi; i+=dom.dim[0].str) {
    for (j=dom.dim[1].lo; j<=dom.dim[1].hi; j+=dom.dim[1].str) {
      fprintf(outfile, format, _ACC2(arr, i, j));
      if (j<dom.dim[1].hi) {
	fprintf(outfile, " ");
      }
    }
    fprintf(outfile, "\n");
  }
}
