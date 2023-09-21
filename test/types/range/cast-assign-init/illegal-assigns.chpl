// error messages for illegal assignments with different idxTypes

enum color { red };

config type to, from;

var rFrom: range(from);
var rTo:   range(to);
rTo; rFrom;                    // no split init, please

rTo = rFrom;                   // assignment

writeln("OK");

