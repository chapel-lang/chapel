#include <stdlib.h>
#include <string.h>
#include "chplio.h"
#include "domain.h"


void _write_linefeed(FILE* outfile) {
  fprintf(outfile, "\n");
}


int _string_to_boolean(char* valueString, _boolean* val) {
  int validBoolean = 0;

  if (valueString) {
    if (strcmp(valueString, "true") == 0) {
      *val = true;
      validBoolean = 1;
    } else if (strcmp(valueString, "false") == 0){
      *val = false;
      validBoolean = 1;
    }
  }
  return validBoolean;
}


void _read_boolean(FILE* infile, char* format, _boolean* val) {
  char* inputString = NULL;
  int validBoolean = 0;

  _read_string(stdin, format, &inputString);
  validBoolean = _string_to_boolean(inputString, val);

  if (!validBoolean) {
    fflush(stdout);
    fprintf(stderr, "***Error:  Not of boolean type***\n");
    exit(0);
  }
}


void _write_boolean(FILE* outfile, char* format, _boolean val) {
  if (val) {
    fprintf(outfile, format, "true");
  } else {
    fprintf(outfile, format, "false");
  }
}


void _read_integer64(FILE* infile, char* format, _integer64* val) {
  fscanf(infile, format, val);
}


void _write_integer64(FILE* outfile, char* format, _integer64 val) {
  fprintf(outfile, format, val);
}


void _read_float64(FILE* infile, char* format, _float64* val) {
  fscanf(infile, format, val);
}


void _write_float64(FILE* outfile, char* format, _float64 val) {
  fprintf(outfile, format, val);
}


void _read_string(FILE* infile, char* format, _string* val) {
  char localVal[_default_string_length];

  fscanf(infile, format, localVal);
  if (strlen(localVal) == (_default_string_length - 1)) {
    fprintf(stderr, "***Error:  The maximum string length is %d***\n", 
	    _default_string_length);
    exit(0);
  }
  _copy_string(val, localVal);
}


void _read_complex128(FILE* infile, char* format, _complex128* val) {
  char imaginaryI = 'i';
  int numScans;

  // NOTE:  The imaginary "i" is stored and checked in the if statement below
  // to ensure that it has been entered directly after the second double of 
  // the complex number, with no intervening space.
  numScans = fscanf(infile, format, &(val->re), &(val->im), &imaginaryI);
  if ((imaginaryI != 'i') || (numScans < 2)) {
    fprintf(stderr, "***Error:  Incorrect format for complex numbers***\n");
    exit(0);
  }
}


void _write_complex128(FILE* outfile, char* format, _complex128 val) {
  fprintf(outfile, format, val.re, val.im);
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
