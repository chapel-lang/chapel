/*
 * Open a file and confirm its FS type is lustre.
 */

use IO;

var fp = open("SKIPIF", iomode.r),
  fileType = fp.fstype();

if fileType == FTYPE_LUSTRE {
  writeln("SUCCESS");
} else {
  writeln("FAILURE");
}
