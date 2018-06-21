// Modified copy of
// test/multilocale/diten/nolocalArgDefaultGlobalArray/fieldDefaultGlobalArray
var A: [1..5] int;
A(2) = 1;

class C {
  var a = A(2) + 2;
}

var c = new borrowed C();
A(2) = 2;
var d = new borrowed C();

writeln(c);
writeln(d);
