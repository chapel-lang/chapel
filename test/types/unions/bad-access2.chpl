union U {
  var x: int;
  var y: real;
}


var u: U;
writeln(u.x); // runtime error: x is not active
