var s = "Türkçe";

var (p1, p2, p3) = s.partition("r");

writeln(p1.size);  // must be 2
writeln(p2.size);  // must be 1
writeln(p3.size);  // must be 3
