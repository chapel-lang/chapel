// Checks that it's okay for the import subexpressions to all be different forms
// of import
module A {
  var first: int;
  var second: bool;
  var third = "blah blah";
}
module B {
  var aVar = 17;
  proc aFunc() {
    writeln("In B.aFunc()");
  }
}
module C {
  param justALonesomeParam = true;
}
module User {
  import A.{first, second}, B, C.justALonesomeParam;

  proc main() {
    writeln(first);
    writeln(second);
    writeln(B.aVar);
    B.aFunc();
    writeln(justALonesomeParam);
  }
}
