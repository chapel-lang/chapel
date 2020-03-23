enum color {red, green, blue};
enum size {small, medium, large};

proc foo(x: enum, y: x.type) {
  writeln("In foo()");
}

foo(color.red, size.small);

proc bar(x: integral, y: x.type) {
  writeln("In bar()");
}

var i8 = 1: int(8);
var i64 = max(int);

bar(i8, i64);
