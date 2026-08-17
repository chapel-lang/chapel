union u {
  var x: int;
  var y: int;
}

var myU = new u(x=42);
writeln(myU);
myU.y = myU.x;
writeln(myU);
myU.x = myU.y;
writeln(myU);
