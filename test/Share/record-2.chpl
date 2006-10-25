record Base {
  var x : record {
     var y : int;
     var z : real;
  }
}

var a : Base;
a.x.z = 5;
writeln(a.x.z);
