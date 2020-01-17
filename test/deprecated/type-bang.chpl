class MyClass { var x: int; }

proc printTypes(type t, type tt) {
  writeln(t:string, " ! -> ", tt:string);
}
proc testType(type t) {
  printTypes(t, t!);
}

proc main() {
  testType(owned MyClass);
  testType(owned MyClass?);
  testType(shared MyClass);
  testType(shared MyClass?);
  testType(unmanaged MyClass);
  testType(unmanaged MyClass?);
  testType(borrowed MyClass);
  testType(borrowed MyClass?);
}
