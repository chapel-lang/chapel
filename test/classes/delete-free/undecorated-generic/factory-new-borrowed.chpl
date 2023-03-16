class MyClass { }

proc factory(param case, type t) {
  var y = new t();
  compilerWarning("factory(" + case  + ") : " + y.type:string, errorDepth=1);
  return y;
}

proc main() {
  var a = new MyClass();
  compilerWarning("new MyClass() : " + a.type:string, errorDepth=0);

  var b = new owned MyClass();
  compilerWarning("new owned MyClass() : " + b.type:string, errorDepth=0);
  var cOwn = new owned MyClass();
  var c = cOwn.borrow();
  compilerWarning("cOwn.borrow() : " + c.type:string, errorDepth=0);

  factory("MyClass", MyClass);

  factory("owned MyClass", owned MyClass);

  // illegal to make a 'new borrowed MyClass'
  //factory("borrowed MyClass", borrowed MyClass); // lifetime error in this call
}
