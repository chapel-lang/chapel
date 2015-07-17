//
// Syntactically, the compiler can't distinguish between standalone
// functions and primary methods, so some post-parsing pass ought to
// flag the following as illegal since there is no 'this' to add an
// intent to.
//
proc ref foo() {
  writeln("In foo()");
}

proc type bar() {
  writeln("In bar()");
}

foo();
bar();
