// Copied from
// test/multilocale/diten/nolocalArgDefaultGlobalArray/fieldDefaultGlobalArray
var A: [1..5] int;
A(2) = 1;

class C {
  var a = A(2);
}

var c = new C();
A(2) = 2;
var d = new C();

writeln(c);
writeln(d);

delete c;
delete d;
