type t = sync bool;
writeln("about to create sync tuple");
var a: (t, t, t);

writeln("created it");
a(1).writeEF(true);
writeln("set sync tuple field");
writeln("sync value is: ", a(1).readFE());
writeln("read it");
