def foo(x, y) {
  if (x.type != y.type) then
    compilerError("foo() called with non-matching types: ", 
                  x.type, " != ", y.type);
  writeln("In 2-argument foo...");
}

def foo(x) {
  compilerWarning("1-argument version of foo called with type: ", x.type);
  writeln("In generic foo!");
}

foo(3.4);
foo("hi");
foo(1, 2);
foo(1.2, 3.4);
foo("hi", "bye");
foo(1, 2.3);
foo("hi", 2.3);
