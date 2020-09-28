proc foo(a: [] int, param p=false) {
  writeln("value of p: ", p);
}
var arr = [1,2,3];
var temp = true;
foo(arr, temp);
