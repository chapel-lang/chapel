union U {
  var x: int;
  var y: real;
}


var u: U;
u.x = 10;
writeln(u.x);
u.y = 3.14;
writeln(u.y);
writeln(u.x); // runtime error: x is not active
