use BlockDist;
use CyclicDist;

config const n = 2;

proc warning1() {

  var myDomain: domain(2) = {1..n, 1..n} dmapped Block({1..n, 1..n});
  writeln(myDomain);

}

proc warning2() {

  var myDomain: domain(2) = {1..n, 1..n} dmapped Cyclic(startIdx=(1,1));
  writeln(myDomain);

}


proc ok1() {

  var myDomain: domain(2);
  myDomain; // see issue #14746
  myDomain = {1..n, 1..n} dmapped Block({1..n, 1..n});
  writeln(myDomain);

}


warning1();
warning2();
ok1();
