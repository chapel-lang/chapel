var a = [5,7];
var b = a;
foo(b);

proc foo(arr: [?D] int) {
  return arr[1];
}
