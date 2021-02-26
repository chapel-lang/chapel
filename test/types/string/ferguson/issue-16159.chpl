proc write(s) { writeln((s, s.size)); }

var a = "Hello World";
write(a);
var b = "Hello\x00World";
write(b);

var x = "Hello\x0World";
write(x);
var y = "Hello\x1World";
write(y);
var z = "Hello\x2World";
write(z);
