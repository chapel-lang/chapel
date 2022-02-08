use IO;

var f = opentmp();
var style = defaultIOStyleInternal();
param typ:iokind = iokind.native;
var ch = f.writer(typ, style=style);
ch.close();
var ch2 = f.reader(typ, style=style);
ch2.close();
