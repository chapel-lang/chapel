use IO;

var f = openTempFile();
var style = defaultIOStyleInternal();
var ch = f.writer(style=style);
ch.close();
var ch2 = f.reader(style=style);
ch2.close();
