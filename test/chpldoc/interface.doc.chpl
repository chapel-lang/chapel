/*
  An interface without formals
*/
interface myInterface {
  /* The foo method */
  proc Self.foo();
  @deprecated("bar is deprecated")
  proc Self.bar();
}

/*
  An interface with formals
*/
@unstable("I am unstable")
interface myInterfaceWithFormals(T1, T2) {
  proc Self.foo(x: T1);
  /* The bar method */
  proc Self.bar(y: T2);
  @chpldoc.nodoc
  proc Self.hidden();
}

@chpldoc.nodoc
interface hiddenInterface {
  proc Self.hidden();
}

