use Map;

class C {
  var i: int;
}

var m = new map(int, owned C?);

var o = new owned C(1)?;

writeln("before assignment o is: ", o);
m[1] = o;
writeln("after assignment o is: ", o);
writeln("before next assignment m[1] is: ", m[1]);
o = m[1];
writeln("after next assignment m[1] is: ", m[1]);
