config var i: int = 0;
config var f: float = 1.2;
config var b: bool = true;

class C {
  var u: int;
  var v: int;
  var w: int;
  var x: int;
  var y: int;
  var z: int;
}

function foo() {
  var s: string = "1234567890";

  s = "";
}

var c = C();
foo();

writeln(i);
writeln(f);
writeln(b);
