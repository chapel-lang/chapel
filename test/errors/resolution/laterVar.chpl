proc helper(arg: int) {
  var a = b;
  var b = arg;
  return b;
}

proc other(x: int) {
  var a = x;
  var x = 2.0;
  return x;
}

var x = helper(2);
var y = other(2);

var ax = g;
var g = 1;
