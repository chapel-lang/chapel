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
  import A.{first, second}, B.{aVar, aFunc}, C.{justALonesomeParam};

  proc main() {
    writeln(first);
    writeln(second);
    writeln(aVar);
    aFunc();
    writeln(justALonesomeParam);
  }
}
