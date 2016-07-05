/*var x = 1;*/

/*proc foo(value1, value2) {*/
  /*return value1*value2;*/
/*}*/

/*proc bar(z, y, t) {*/
  /*return z*y+t;*/
/*}*/

/*writeln(bar(10, foo(x*3.0, x*5.0), 2));*/

const space = {0..10};
var arr: [space] int;
for i in arr.domain do arr[i] = i;
proc foo(ref y) ref{
  y *= 2;
  var loc = y;
  return arr[5];
}

proc bar(z) {
  return z*2;
}

var x = 5;
writeln(foo(x));
writeln(x);

writeln(bar(arr[foo(x)-2]));
