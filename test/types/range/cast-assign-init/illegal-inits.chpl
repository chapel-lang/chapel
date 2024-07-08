// error messages for illegal initializations with different idxTypes

enum color { red };

config type to, from;

var rFrom: range(from);
var rTo:   range(to) = rFrom;  // initialization

writeln("OK");
