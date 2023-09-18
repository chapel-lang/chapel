
use IO;

config const trywrite = false;

var logFile: fileWriter(locking=true);
if trywrite then logFile.write("X");
logFile.close();
