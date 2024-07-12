config param testnum = 1;

proc foo(x:int, y:int = 7) {
  return x+y;
}
var A = [1,2,3,4,5,6,7,8,9];

if testnum == 1 {
  // promotion and not capturing the result
  foo(A);
}
else if testnum == 2{
  // promotion and capturing the result
  var x = foo(A);
}
else if testnum == 3 {
  // promotion and first capturing the result, then not capturing the result
  var x = foo(A);
  foo(A);
}
else if testnum == 4 {
  // no promotion and not capturing the result
  foo(7);
}
