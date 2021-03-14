
use IO;

config const trywrite = false;

var logFile: channel(true, kind=iokind.dynamic, locking=true);
if trywrite then logFile.write("X");
logFile.close();
