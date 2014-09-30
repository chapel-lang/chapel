/*
 * Open a file and confirm its FS type is lustre.
 */

use IO;

const fsTypes: [{0..3}] string = [
  "none",
  "hdfs",
  "UNKNOWN", // lustre is LUSTRE_SUPER_MAGIC (whatever that is)
  "curl"
];

var fp = open("SKIPIF", iomode.r),
  fileType = fp.fstype();

if fileType == FTYPE_LUSTRE {
  writeln("SUCCESS");
} else {
  writef("FAILURE: Expected FTYPE_LUSTRE (%n), actual: %s (%n)\n",
         FTYPE_LUSTRE, fsTypes[fileType], fileType);
}
