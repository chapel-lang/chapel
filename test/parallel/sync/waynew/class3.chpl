// sync should be on the class, not the fields.

class C {
  var x: int;
  var y: int;
  var z: int;
}

var s: sync C;

s = new C();
s.x = 1;
s.x = 2;
s.y = 3;
s.y = 4;
s.z = 5;
s.z = 6;
var c = s;
writeln( c);
delete c;

