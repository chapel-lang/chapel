proc gimme(type t): [] {
  var x: [1..5] t;
  return x;
}

var myReals = gimme(real);
var myInts = gimme(int);
writeln(myReals);
writeln(myInts);
