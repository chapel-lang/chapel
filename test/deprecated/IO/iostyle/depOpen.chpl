use IO;

const filename1 = "blah.txt";
var style = defaultIOStyleInternal();
var f = open(filename1, ioMode.cw, style=style);
f.close();
