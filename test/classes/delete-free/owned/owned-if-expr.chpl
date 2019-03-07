class MyClass {
  var x:int;
}

config const c = true;

proc main() {
 
  var x = if c then new owned MyClass(1) else new owned MyClass(2);

  writeln(x);
}
