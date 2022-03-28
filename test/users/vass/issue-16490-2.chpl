class MyClass { }
const c1 = new MyClass();

proc myFun(myArg) {
  var c2 = new MyClass();
  c2 = myArg;  // error here
}
proc main {
  myFun(c1);

  var c3 = new MyClass();
  c3 = c1;  // error here
}
