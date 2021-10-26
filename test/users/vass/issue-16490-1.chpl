class MyClass { }
var myC = new MyClass()?;

proc libFun(myArg) {
  var myClass = myArg: class; // error here
}
proc main {
  libFun(myC);
}
