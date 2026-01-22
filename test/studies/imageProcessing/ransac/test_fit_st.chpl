
/*****
      test_fit_st.chpl -
      Given a mapping on the command line, runs the core of the alignment
      (map_corners, match_corners) and prints the results.  For testing how
      we should expect the matching algorithm to behave with a shift/translate
      transform.

      Call:
        test_fit_st
          --inname1=<file>         first image to compare
          --inname2=<file>         second image to compare
          --sx                     x scaling image1 to 2 coords
          --dx                     x offset image1 to 2 coords
          --sy                     y scaling image1 to 2 coords
          --dy                     y offset

      c 2015-2018 Primordial Machine Vision Systems
*****/

use ransac_st;
use ip_color_v3;
use ip_corner;
use kdtree;
use Help;
use Random;
use CTypes;


/**** Command Line Arguments ****/

/* Mapping */
config const sx = 0.0;                  /* x scaling from image1 to image2 */
config const dx = 0.0;                  /* y offset from image1 to image2 */
config const sy = 0.0;                  /* y scaling from image1 to image2 */
config const dy = 0.0;                  /* y offset from image1 to image2 */


/**** Top Level ****/

/***
    usage:  Print an error message along with the system help, then exit.
    args:   msg - message to print
***/
proc usage(msg : string) {

  writeln("\nERROR");
  writeln("  " + msg);
  writeln("\nALLOWED VALLUES");
  writeln("  --space = LAB | YUV");
  writeln("  --radius must be >= 3");
  writeln("  note L, Y scales differ, default --thr set for L");
  printUsage();
  exit(1);
}

/***
    end_onerr:  Check the error code; if OK (>= 0) do nothing.  Else release
                any objects passed as additional arguments - anything can
                be passed and its type will determine the action that needs
                to be done - and exit with an non-zero error value.
    args:       retval - error code/return to value for exit
                inst - variable list of instances to free
***/
proc end_onerr(retval : int, inst ...?narg) : void {

  if (0 <= retval) then return;

  /* Note we skip the argument if we don't know how to clean it up. */
  for param i in 0..(narg-1) {
    if (inst(i).type == c_ptr(rgbimage)) then free_rgbimage(inst(i));
    else if isClass(inst(i)) then delete inst(i);
  }
  exit(1);
}

/***
    verify_setup:  Check that the config variables have been set correctly.
                   If not, print usage error and exit.
***/
proc verify_setup() {

  if ("" == inname1) then usage("missing --inname1");
  if (!PNG_isa(inname1.c_str())) then usage("input file 1 not a PNG picture");
  if ("" == inname2) then usage("missing --inname2");
  if (!PNG_isa(inname2.c_str())) then usage("input file 2 not a PNG picture");
  if ((clrspace.LAB != space) && (clrspace.YUV != space)) then
    usage("only use LAB or YUV color spaces");
  if (radius < 3) then usage("radius too small");
  /* Do not check the shifts since 0 is a valid value for them. */
  if ((0.0 == sx) || (0.0 == sy)) then
    usage("must provide mapping");
}


proc main() {
  var rgb : c_ptr(rgbimage);            /* image we've read */
  var disp : c_ptr(rgbimage);           /* marked corners */
  var clr1 : unmanaged clrimage?;       /* greyscale first image */
  var clr2 : unmanaged clrimage?;       /* greyscale second image */
  var spec : fastspec;                  /* FAST parameters */
  var corners1 : unmanaged chunkarray(corner)?;   /* corners found in clr1 */
  var corners2 : unmanaged chunkarray(corner)?;   /* corners found in clr2 */
  var testmap : mapinfo;                /* mapping with parameters provided */
  var retval : int;

  verify_setup();

  retval = PNG_read(inname1.c_str(), rgb);
  end_onerr(retval, rgb);

  rgb_convert(rgb, clr1, space);

  retval = PNG_read(inname2.c_str(), rgb);
  end_onerr(retval, rgb, clr1);

  rgb_convert(rgb, clr2, space);

  spec.radius = radius;
  spec.minlen = minlen;
  spec.maxlen = maxlen;
  spec.thr = thr;
  spec.suppsep = suppsep;

  corners1 = find_corners(clr1, spec);
  corners2 = find_corners(clr2, spec);

  retval = mark_corners(clr1, space, corners1, disp);
  end_onerr(retval, rgb, disp, clr1, clr2);
  retval = PNG_write("ctf1.png", disp, CLR_RGB);
  end_onerr(retval, rgb, disp, clr1, clr2);

  retval = mark_corners(clr2, space, corners2, disp);
  end_onerr(retval, rgb, disp, clr1, clr2);
  retval = PNG_write("ctf2.png", disp, CLR_RGB);
  end_onerr(retval, rgb, disp, clr1, clr2);

  testmap.sx = sx;
  testmap.dx = dx;
  testmap.sy = sy;
  testmap.dy = dy;

  var tree                              /* 2-dimensional sorted space */
    = new unmanaged kdtree(corners2!.size, int, 2);
  for i in corners2!().domain {
    tree.add_node(corners2!(i).center, i);
  }
  tree.assemble_tree();

  var mapped1 : [corners1!().domain] corner;
  map_corners(corners1!(), testmap, mapped1);

  var nmap = count_matches(mapped1, corners2!(), tree);

  writeln("\nMatching from fixed mapping");
  writef("  x2 = %6.3dr x1 %+7.1dr\n", testmap.sx, testmap.dx);
  writef("  y2 = %6.3dr y1 %+7.1dr\n", testmap.sy, testmap.dy);
  writef("  number matching corners  %i\n", nmap);

  free_rgbimage(rgb);
  free_rgbimage(disp);
  delete clr1;
  delete clr2;
  delete tree;

  return 0;
}
