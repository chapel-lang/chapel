record Base {
  var x : record {
     var y : integer;
     var z : float;
  }
}

var a : Base;
a.x.z = 5;
writeln(a.x.z);
