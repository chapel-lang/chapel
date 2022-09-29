
use IO;

config const trywrite = false;

var logFile: fileWriter(kind=iokind.dynamic, locking=true);
if trywrite then logFile.write("X");
logFile.close();
