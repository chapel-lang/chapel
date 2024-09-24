class C { var x: int; }

proc doSomethingOwnedC() {
  writeln("Am I running on the last locale? ", here == Locales.last);
  return new owned C(42);
}

proc doSomethingSharedC() {
  writeln("Am I running on the last locale? ", here == Locales.last);
  return new shared C(42);
}

proc doSomethingInt() {
  writeln("Am I running on the last locale? ", here == Locales.last);
  return 42;
}

on Locales.last var myOwnedClassVar = doSomethingOwnedC();
writeln(myOwnedClassVar);
writeln(myOwnedClassVar.locale == Locales.last);

// perform a copy to make sure that the actual heap value in the shared
// wrapper is on the last locale
on Locales.last var mySharedClassVar = doSomethingSharedC();
writeln(mySharedClassVar);
var copy = mySharedClassVar;
writeln(copy.locale == Locales.last);

on Locales.last var myIntVar = doSomethingInt();
writeln(myIntVar);
writeln(myIntVar.locale == Locales.last);

// Same as above, but in a function to test the case where variables aren't global
proc foo() {
  on Locales.last var myOwnedClassVar = doSomethingOwnedC();
  writeln(myOwnedClassVar);
  writeln(myOwnedClassVar.locale == Locales.last);

  // perform a copy to make sure that the actual heap value in the shared
  // wrapper is on the last locale
  on Locales.last var mySharedClassVar = doSomethingSharedC();
  writeln(mySharedClassVar);
  var copy = mySharedClassVar;
  writeln(copy.locale == Locales.last);

  on Locales.last var myIntVar = doSomethingInt();
  writeln(myIntVar);
  writeln(myIntVar.locale == Locales.last);
}
foo();

// Same as foo, but explicitly annotate types.
proc bar() {
  on Locales.last var myOwnedClassVar: owned C = doSomethingOwnedC();
  writeln(myOwnedClassVar);
  writeln(myOwnedClassVar.locale == Locales.last);

  // perform a copy to make sure that the actual heap value in the shared
  // wrapper is on the last locale
  on Locales.last var mySharedClassVar: shared C = doSomethingSharedC();
  writeln(mySharedClassVar);
  var copy = mySharedClassVar;
  writeln(copy.locale == Locales.last);

  on Locales.last var myIntVar: int = doSomethingInt();
  writeln(myIntVar);
  writeln(myIntVar.locale == Locales.last);
}
bar();
