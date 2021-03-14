class MyClass { var x:int; }
proc f( const ref arg:owned MyClass ) {
  return arg;
  // compiler adds copy initialization to this return
  // since it's returning a reference by value
  // is it surprising that arg stores nil after the return?
}

proc main() {
  const x = new owned MyClass(1);
  f(x);
  writeln(x); // is it surprising that x now stores nil?
}

