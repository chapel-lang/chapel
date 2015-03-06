/*
 * Open a HTTPS connection and confirm its FS type is curl.
 */

use IO;

const fsTypes: [{0..3}] string = [
  "none",
  "hdfs",
  "UNKNOWN",  // lustre is LUSTRE_SUPER_MAGIC (whatever that is)
  "curl"
];

// This open call is pretty awkward (due to non-optional argument mode:iomode,
// which is after optional argument path="". Also, it is awkward to require an
// iomode for a read-only request.
var resp = open("", iomode.r, url="https://status.github.com/api/status.json"),
  fileType = resp.fstype();

if fileType == FTYPE_CURL {
  writeln("SUCCESS");
} else {
  writef("FAILURE: Expected FTYPE_CURL (%n), actual: %s (%n)\n",
         FTYPE_CURL, fsTypes[fileType], fileType);
}
