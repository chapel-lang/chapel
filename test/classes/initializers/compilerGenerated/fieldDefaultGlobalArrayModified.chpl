// Modified copy of
// test/multilocale/diten/nolocalArgDefaultGlobal/fieldDefaultGlobalArray.chpl
var A: [1..5] int;
A(2) = 1;

class C {
  var a = A(2) + 2;
}

var c = (new owned C()).borrow();
A(2) = 2;
var d = (new owned C()).borrow();

writeln(c);
writeln(d);
