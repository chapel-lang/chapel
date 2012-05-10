
enum e1 { a = 1 };
enum e2 { b = 0, c, d};
config var e1var: e1;
config var e2var: e2;

// simple example
var k:uint(8) = e2var;
writeln(k);

// bonus testing
proc test(param w) {

  type tt = uint(w);
  var i1: tt = e1.a;
  var i2: tt = e1var;
  var j1: tt = e2.b;
  var j2: tt = e2var;

  writeln((typeToString(tt), i1,i2,j1,j2));
}

test(8);
test(16);
test(32);
test(64);
