use IO;

var filename = "shortfile.txt";
var f = open(filename, iomode.r);
var chnl = f.reader(kind=ionative);
// If I remove kind=ionative, the program completes correctly

var line: string;
var lineSizeLimit = 1024;
while (chnl.readstring(line, lineSizeLimit)) {
  write(line);
}
f.close();
