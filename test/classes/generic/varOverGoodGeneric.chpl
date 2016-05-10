class C {
  type t = int;
  var x: t;
}

var myC: C;

myC = new C();

var myC2: C = new C();

writeln(myC);
writeln(myC2);


delete myC2;
delete myC;

