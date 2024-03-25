
/*****
      ex_range.chpl -
      Test program demonstrating ranges through an image, covering the entire
      array, one column, or one row.  You can set the column or row by the
      command line.

      Call:
        ex_range
          --x     column to print
          --y     row to print
          if neither x nor y provided, iterates over the entire image

      c 2015-2018 Primordial Machine Vision Systems
*****/

config const x : int = -1;              /* column to print */
config const y : int = -1;              /* row to print */

const ncol = 10;                        /* image width (number columns) */
const nrow = 10;                        /* image height (number rows) */
const npix = ncol * nrow;               /* number of pixels */
const lastpix = npix - 1;               /* index of last pixel (incl.) */

if (ncol <= x) {
  writeln("column ", x, " bigger than image (max. ", ncol-1, ")");
  exit(0);
}
if (nrow <= y) {
  writeln("row ", y, " bigger than image (max. ", nrow-1, ")");
  exit(0);
}

if ((x < 0) && (y < 0)) {
  for xy in 0 .. lastpix {
    if ((0 < xy) && (0 == (xy % ncol))) then writef("\n");
    writef(" %3i", xy);
  }
  writef("\n");
} else if (y < 0) {
  writef("column %2i:", x);
  /* To count backwards use 'by -ncol'.  To show the column only over the
     first 5 rows use '(0 .. lastpix by ncol align x) # 5'.  To show over
     the last 5 rows use '(0 .. lastpix by -ncol align x) # 5'. */
  for xy in 0 .. lastpix by ncol align x do writef(" %3i", xy);
  writef("\n");
} else if (x < 0) {
  writef("row %2i:", y);
  for xy in (y*ncol) .. ((y+1) * ncol - 1) do writef(" %3i", xy);
  writef("\n");
} else {
  var xy = (y * ncol) + x;              /* pixel index */
  writef("index at %2i, %2i: %3i\n", x,y, xy);
}
