union U {
  var x: int;
  var y: real;
  var z: string;
}

var u1: U;

u1.x = 1;

writeln("u1.x is: ", u1.x);

u1.y = 2.3;

writeln("u1.y is: ", u1.y);

u1.z = "hello";

writeln("u1.z is: ", u1.z);
