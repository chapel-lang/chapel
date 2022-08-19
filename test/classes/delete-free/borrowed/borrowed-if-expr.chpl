class MyClass {
  var x:int;
}

config const c = true;

proc main() {
 
  var x = if c then (new owned MyClass(1) else new borrowed MyClass(2)).borrow();

  writeln(x);
}
