use IO;

var filename = "blah3.txt";
var style = defaultIOStyleInternal();
var chW = openWriter(filename, style=style);
chW.close();
var chR = openReader(filename, style=style);
chR.close();
