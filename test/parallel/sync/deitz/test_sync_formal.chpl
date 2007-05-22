def foo(s: sync int) {
  writeln(s); // first read
}

var s: sync int;

s = 2; // first write
foo(s); // pass sync by reference

def bar(s: int) {
  writeln(s);
}

s = 2; // second write
bar(s); // second read

foo(2); // create a new sync, inside foo is only read
