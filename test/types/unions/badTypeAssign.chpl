union U {
  var x: string;
  var y: real;
}
var u: U;
u.x = "hello";
var s: string = U.x;
writeln(s);
