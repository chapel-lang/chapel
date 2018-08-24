class MyClass {
  var x:int;
}

config const c = true;

proc main() {
 
  var x = if c then new borrowed MyClass(1) else new borrowed MyClass(2);

  writeln(x);
}
