// Modified copy of
// test/multilocale/diten/nolocalArgDefaultGlobal/fieldDefaultGlobalArray.chpl
var A: [1..5] int;
A(2) = 1;

class C {
  var a = A(2) + 2;
}

var ownC = new owned C();
var c = ownC.borrow();
A(2) = 2;
var ownD = new owned C();
var d = ownD.borrow();

writeln(c);
writeln(d);
