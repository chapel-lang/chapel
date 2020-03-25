/*
 * Open a file and confirm its FS type is lustre.
 */

use IO;

var fp = open("SKIPIF", iomode.r),
  fileType = fp.fstype();

if fileType == FTYPE_LUSTRE {
  writeln("SUCCESS");
} else {
  writef("FAILURE: Expected FTYPE_LUSTRE (%n), actual: (%n)\n",
         FTYPE_LUSTRE, fileType);
}
