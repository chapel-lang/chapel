class C {
  type t = int;
  var x: t;
}

var myC: unmanaged C?;

myC = new unmanaged C();

var myC2: unmanaged C = new unmanaged C();

writeln(myC);
writeln(myC2);


delete myC2;
delete myC;

