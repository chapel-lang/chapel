record Base {
  var x : record {
     var y : integer;
     var z : float;
  }
}

var a : Base;
a.x = (z = 1.4, y = 5, m = 2);
writeln("expect 1.4 ", a.x.z);
