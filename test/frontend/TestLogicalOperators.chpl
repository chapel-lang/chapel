use Print;

var a0 = false;
var a1 = true;
var b0 = false;
var b1 = true;
var c0 = false;
var c1 = true;

proc test1() {
  var x1 = a0 && b0 && c0;
  println(x1); // false
  var x2 = a1 && b1 && c1;
  println(x2); // true
  var x3 = a0 && b1 && c1;
  println(x3); // false
  var x4 = a1 && b0 && c0;
  println(x4); // false
}

proc test2() {
  var x1 = a0 || b0 || c0;
  println(x1); // false
}

proc test3() {
  var x = false && b1 && c1;
  println(x); // false
}

proc test4() {
  var x = false || b0 || c1;
  println(x); // true
}

proc test5() {
  var x = true && b1 && c1;
  println(x); // true

}

proc test6() {
  var x = true || b0 || c0;
  println(x); // true
}

test1();
test2();
test3();
test4();
test5();
test6();
