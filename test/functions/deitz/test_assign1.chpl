record C {
  var i : integer = 2;
}

record D {
  var j : integer = 3;
  function =(c : C) {
    j = c.i;
  }
}

var c : C;
var d : D;

writeln(c);
writeln(d);

d = c;

writeln(d);
