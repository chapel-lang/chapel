class MyClass {
var len: int;
var dom: domain(1);
var arr: [dom] int;
proc MyClass(length: int) {
  this.len = length;
    this.dom = {1..length};
      forall (a,i) in zip(this.arr, 13..) do
        a = i;
        }

proc MyClass(array: [] int, length: int) {
  this.len = length;
    this.dom = {1..length};
      forall (a,b) in zip(this.arr, array) do
        a = b;
        }
}


proc |(lhs: MyClass, rhs: MyClass): MyClass {
  var res = new MyClass(min(lhs.len, rhs.len));
  forall (r,a,b) in zip(res.arr, lhs.arr, rhs.arr) do
    r = a | b;
    return res;
      }

var X = [4, 8, 15, 16, 23, 42];
var a = new MyClass(X, 6);
var b = new MyClass(6);
var c = a | b;
writeln("c is: ", c);

delete a;
delete b;
delete c;
