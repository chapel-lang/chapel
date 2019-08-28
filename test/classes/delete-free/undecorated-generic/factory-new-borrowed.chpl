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

  var c = new borrowed MyClass();
  compilerWarning("new borrowed MyClass() : " + c.type:string, errorDepth=0);

  factory("MyClass", MyClass);

  factory("owned MyClass", owned MyClass);

  factory("borrowed MyClass", borrowed MyClass); // lifetime error in this call
}
