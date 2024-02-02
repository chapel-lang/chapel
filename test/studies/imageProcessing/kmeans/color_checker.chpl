
/*****
      color_checker.chpl -
      Generates an image with 24 colored squares separated by pure black
      borders.  The colors follow the Macbetch chart.  Image is written to
      "colors.png".

      Call:
        color_checker

      c 2015-2018 Primordial Machine Vision Systems
*****/

use ip_color_v3;
use CTypes;


/**** Constants - Internal ****/

const WBORDER = 25;                     /* width of border between squares */
const WSQUARE = 100;                    /* size of color squares */
const NCOL = 6;                         /* number of columns in grid */
const NROW = 4;                         /* number rows in grid */
const OUTNAME = "colors.png" : string;  /* file name for result */

/* sRGB values for squares from table on
     https://en.wikipedia.org/wiki/ColorChecker
   based on data from manufacturer of chart. */
var clrs : [1..NCOL*NROW] 3*uint(8);    /* sRGB values for color fields */
clrs(1) = (0x73 : uint(8), 0x52 : uint(8), 0x44 : uint(8));
clrs(2) = (0xc2 : uint(8), 0x96 : uint(8), 0x82 : uint(8));
clrs(3) = (0x62 : uint(8), 0x7a : uint(8), 0x9d : uint(8));
clrs(4) = (0x57 : uint(8), 0x6c : uint(8), 0x43 : uint(8));
clrs(5) = (0x85 : uint(8), 0x80 : uint(8), 0xb1 : uint(8));
clrs(6) = (0x67 : uint(8), 0xbd : uint(8), 0xaa : uint(8));
clrs(7) = (0xd6 : uint(8), 0x7e : uint(8), 0x2c : uint(8));
clrs(8) = (0x50 : uint(8), 0x5b : uint(8), 0xa6 : uint(8));
clrs(9) = (0xc1 : uint(8), 0x5a : uint(8), 0x63 : uint(8));
clrs(10) = (0x5e : uint(8), 0x3c : uint(8), 0x6c : uint(8));
clrs(11) = (0x9d : uint(8), 0xbc : uint(8), 0x40 : uint(8));
clrs(12) = (0xe0 : uint(8), 0xa3 : uint(8), 0x2e : uint(8));
clrs(13) = (0x38 : uint(8), 0x3d : uint(8), 0x96 : uint(8));
clrs(14) = (0x46 : uint(8), 0x94 : uint(8), 0x49 : uint(8));
clrs(15) = (0xaf : uint(8), 0x36 : uint(8), 0x3c : uint(8));
clrs(16) = (0xe7 : uint(8), 0xc7 : uint(8), 0x1f : uint(8));
clrs(17) = (0xbb : uint(8), 0x56 : uint(8), 0x95 : uint(8));
clrs(18) = (0x08 : uint(8), 0x85 : uint(8), 0xa1 : uint(8));
clrs(19) = (0xf3 : uint(8), 0xf3 : uint(8), 0xf2 : uint(8));
clrs(20) = (0xc8 : uint(8), 0xc8 : uint(8), 0xc8 : uint(8));
clrs(21) = (0xa0 : uint(8), 0xa0 : uint(8), 0xa0 : uint(8));
clrs(22) = (0x7a : uint(8), 0x7a : uint(8), 0x79 : uint(8));
clrs(23) = (0x55 : uint(8), 0x55 : uint(8), 0x55 : uint(8));
clrs(24) = (0x34 : uint(8), 0x34 : uint(8), 0x34 : uint(8));


proc main() {
  var rgb : c_ptr(rgbimage);                   /* color chart */
  var retval: int;

  retval = alloc_rgbimage(rgb, (NCOL*(WBORDER+WSQUARE)+WBORDER) : c_int,
                          (NROW*(WBORDER+WSQUARE)+WBORDER) : c_int);
  if (retval < 0) then return retval;

  for j in 1..NROW {
    const yl = WBORDER + (j-1) * (WBORDER+WSQUARE);       /* y ll corner */
    for i in 1..NCOL {
      const xl = WBORDER + (i-1) * (WBORDER+WSQUARE);     /* x ll corner */
      const sq = ((j - 1) * NCOL) + i;                    /* color square */
      for y in yl..yl+WSQUARE {
        var xy = (y * rgb.deref().ncol);                          /* pixel index */
        for x in xl..xl+WSQUARE {
          (rgb.deref().r(xy+x), rgb.deref().g(xy+x), rgb.deref().b(xy+x)) = clrs(sq);
        }
      }
    }
  }

  retval = PNG_write(OUTNAME.c_str(), rgb, CLR_RGB);

  free_rgbimage(rgb);

  return retval;
}
