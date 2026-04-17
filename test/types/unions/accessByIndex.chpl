union U {
  var x: int;
  var y: string;
}

var u = new U();
u.x = 1;
writeln(u(0));
u.y = "hello";
writeln(u(1));


writeln(u(0)); // runtime error: x is not active
