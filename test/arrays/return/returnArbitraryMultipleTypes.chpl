proc gimme(type t): [] {
  var x: [1..5] t;
  return x;
}
proc gimme2(type t, type t2): ([], []) {
  var x: [1..5] t;
  var y: [1..5] t2;
  return (x, y);
}
proc gimme3(type t, type t2, type t3, type t4): ([], [], ([], [])) {
  var x: [1..5] t;
  var y: [1..5] t2;
  var z: [1..5] t3;
  var w: [1..5] t4;
  return (x, y, (z, w));
}


var myReals = gimme(real);
var myInts = gimme(int);
writeln(myReals);
writeln(myInts);

var myRealsAndInts = gimme2(real, int);
writeln(myRealsAndInts);
var manyRealsAndInts = gimme3(real, int, real, int);
writeln(manyRealsAndInts);
