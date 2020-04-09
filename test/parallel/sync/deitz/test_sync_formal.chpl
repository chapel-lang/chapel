proc foo(s: sync int) {
  writeln(s.readFE()); // first read
}

var s: sync int;

s = 1; // first write
foo(s); // pass sync by reference

proc bar(s: int) {
  writeln(s);
}

s = 2; // second write
bar(s); // second read

