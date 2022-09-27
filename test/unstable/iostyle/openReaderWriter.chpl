use IO;

var filename = "blah3.txt";
var style = defaultIOStyleInternal();
var chW = openwriter(filename, style=style);
chW.close();
var chR = openreader(filename, style=style);
chR.close();
