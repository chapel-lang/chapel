config var i: integer = 0;
config var f: float = 1.2;
config var b: boolean = true;

class C {
  var u: integer;
  var v: integer;
  var w: integer;
  var x: integer;
  var y: integer;
  var z: integer;
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
