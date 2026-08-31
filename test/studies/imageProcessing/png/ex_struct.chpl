
/*****
      ex_struct.chpl -
      Program demonstrating classes, records, and unions.  See test for
      description.

      Call:
        ex_struct

      c 2015-2018 Primordial Machine Vision Systems
*****/

use CTypes;


writeln("\nDemonstrating unions");
union numbers {
  var svar : int(16);
  var ivar : int(32);
  var lvar : int(64);
  var rvar : real;
  var cvar : complex;
}

var exunion : numbers;                  /* example */
var cpunion : numbers;                  /* copy of example */

exunion.ivar = 1234;
writef("  assigned union int %i\n", exunion.ivar);

cpunion = exunion;
writef("  copy has int field %i\n", cpunion.ivar);

exunion.cvar = 1.0 + 2.0i;
writef("  changed union to complex %z\n", exunion.cvar);
writef("  copied union still int   %i\n", cpunion.ivar);

/* This will cause the program to crash. */
/*
writef("union now has no int     %i\n", exunion.ivar);
*/


writeln("\nDemonstrating records");
record rgbimage_rcd {
  var ncol : c_int;
  var nrow : c_int;
  var npix : c_int;
  var r : c_ptr(c_uchar) = nil;
  var g : c_ptr(c_uchar) = nil;
  var b : c_ptr(c_uchar) = nil;
}

var exrcd = new rgbimage_rcd(nrow=400, ncol=300);    /* example */
var cprcd : rgbimage_rcd;                            /* copy of example */

cprcd.npix = 100;
writef("  exrcd: %4i x %4i (= %6i pix)   nil r? %s\n", exrcd.ncol,exrcd.nrow,
       exrcd.npix, (nil == exrcd.r));
writef("  cprcd: %4i x %4i (= %6i pix)   nil r? %s\n", cprcd.ncol,cprcd.nrow,
       cprcd.npix, (nil == cprcd.r));

writef("  ex == cp? %s\n", (exrcd == cprcd));
cprcd = exrcd;
writeln("  copy exrcd to cprcd");
writef("  exrcd: %4i x %4i (= %6i pix)   nil r? %s\n", exrcd.ncol,exrcd.nrow,
       exrcd.npix, (nil == exrcd.r));
writef("  cprcd: %4i x %4i (= %6i pix)   nil r? %s   == exrcd? %s\n",
       cprcd.ncol,cprcd.nrow, cprcd.npix, (nil == cprcd.r), (exrcd == cprcd));
exrcd.ncol = 600;
exrcd.nrow = 800;
writeln("  set exrcd ncol, nrow");
writef("  exrcd: %4i x %4i (= %6i pix)   nil r? %s\n", exrcd.ncol,exrcd.nrow,
       exrcd.npix, (nil == exrcd.r));
writef("  cprcd: %4i x %4i (= %6i pix)   nil r? %s   == exrcd? %s\n",
       cprcd.ncol,cprcd.nrow, cprcd.npix, (nil == cprcd.r), (exrcd == cprcd));


writeln("\nDemonstrating classes");
class rgbimg_cls {
  var ncol : c_int;
  var nrow : c_int;
  var npix : c_int;
  var r : c_ptr(c_uchar) = nil;
  var g : c_ptr(c_uchar) = nil;
  var b : c_ptr(c_uchar) = nil;
};

var excls : unmanaged rgbimg_cls?;
writef("  excls nil post-declaration? %s\n", (nil == excls));
excls = new unmanaged rgbimg_cls(200, npix=1000, ncol=5);
writef("  new excls:     %4i x %4i (= %6i npix)   nil r? %s\n",
       excls!.ncol,excls!.nrow, excls!.npix, (nil == excls!.r));
excls!.ncol = 10;
excls!.npix = 2000;
writef("  excls changed: %4i x %4i (= %6i npix)   nil r? %s\n",
       excls!.ncol,excls!.nrow, excls!.npix, (nil == excls!.r));

var cpcls = excls;
writef("  make cpcls:    %4i x %4i (= %6i npix)   nil r? %s   == excls? %s\n",
       cpcls!.ncol,cpcls!.nrow, cpcls!.npix, (nil == excls!.r),
       (cpcls == excls));
excls!.nrow = 400;
excls!.npix = 4000;
writef("  excls changed: %4i x %4i (= %6i npix)   nil r? %s\n",
       excls!.ncol,excls!.nrow, excls!.npix, (nil == excls!.r));
writef("  cpcls now:     %4i x %4i (= %6i npix)   nil r? %s   == excls? %s\n",
       cpcls!.ncol,cpcls!.nrow, cpcls!.npix, (nil == excls!.r),
       (cpcls == excls));

delete excls;

writeln("\nDemonstrating constructors");
class image_cls {
  var ncol, nrow, npix : int;

  proc init(numcol : int, numrow : int) {
    ncol = numcol;
    nrow = numrow;
    npix = numcol * numrow;
  }

  proc deinit() {
    writeln("freeing allocated image");
  }
}

var eximg = new unmanaged image_cls(10, 20);
writef("image size %4i x %4i (= %6i npix)\n",
       eximg.ncol,eximg.nrow, eximg.npix);


writeln("\nDemonstrating memory management");
class test_cls {
  var tmp : int;

  proc deinit() {
    writeln("  called class destructor, tmp was ", tmp);
  }
}
record test_rcd {
  var tmp : int;

  proc deinit() {
    writeln("  called record destructor, tmp was ", tmp);
  }
}

proc test_destruct1() {
  var c = new unmanaged test_cls(3);
  var r = new test_rcd(2);

  writeln("in test_destruct1:");
  /* This leaks c. */
}

proc test_destruct2() {
  var c = new unmanaged test_cls(2);
  var r = new test_rcd(3);

  writeln("in test_destruct2:");
  delete c;
}

test_destruct1();
test_destruct2();
