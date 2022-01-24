use IO;

const filename1 = "blah.txt";
var style = defaultIOStyleInternal();
var f = open(filename1, iomode.cw, style=style);
f.close();
