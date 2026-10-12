union u {
  var w: int;
  var x: int;
  var y: real;
  var z: string;
}

var u0 = new u(),
    u1 = new u(w=45),
    u2 = new u(x=78),
    u3 = new u(y=33.3),
    u4 = new u(z="hi");

writeln(u0);
writeln(u1);
writeln(u2);
writeln(u3);
writeln(u4);
