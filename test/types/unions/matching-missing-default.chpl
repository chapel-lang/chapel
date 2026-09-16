union U {
  var x: int;
  var y: real;
  var z: string;
  var w: int;
}
var u = new U(x=10);
union select u {
  when x do x += 1;
  when y do y += 1.0;
}
writeln("after union select: ", u);
