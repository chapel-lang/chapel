config const n = 10;
var s$: [1..n] sync bool;

proc f(i) {
  var D = {1..10};
  begin {
    s$[i]; // wait until after f() returns
    var A: [D] real;
  }
}


proc main {
  for i in 1..n {
    f(i);
    s$[i] = true;
  }
}
