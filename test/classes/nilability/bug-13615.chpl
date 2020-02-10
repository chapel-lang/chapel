class MyClass {	var x: int; }

proc foo(in arg: MyClass?) {
  compilerWarning("arg:", isNilableClass(arg):string);
}


{
  var o1: owned MyClass?;
  foo(o1);

  var o2 = new owned MyClass();
  foo(o2);
}
