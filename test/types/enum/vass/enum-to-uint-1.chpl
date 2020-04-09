
enum e1 { a = 1 };
enum e2 { b = 0, c, d};
config var e1var: e1;
config var e2var: e2;

// simple example
var k = e2var: uint(8);
writeln(k);

// bonus testing
proc test(param w) {

  type tt = uint(w);
  var i1 = e1.a: tt;
  var i2 = e1var: tt;
  var j1 = e2.b: tt;
  var j2 = e2var: tt;

  writeln((tt:string, i1,i2,j1,j2));
}

test(8);
test(16);
test(32);
test(64);
