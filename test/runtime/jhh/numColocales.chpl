// Verify that here.numColocales equals CHPL_RT_LOCALES_PER_NODE if it is
// set.
use OS.POSIX;
use IO.FormattedIO;

var status = "Success";
var value = getenv("CHPL_RT_LOCALES_PER_NODE");
if value {
    var n = string.createCopyingBuffer(value):int;
    if n != here.numColocales {
        writef("CHPL_RT_LOCALES_PER_NODE != here.numColocales (%i != %i)\n",
               n, here.numColocales);
        status = "Failure";
    }
}
writeln(status);
