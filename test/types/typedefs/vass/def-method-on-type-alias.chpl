/* Feature request: Allow declaring methods on type aliases */
type T = real;
proc T.foo() {
  writeln("T.foo() on " + this);
}
(4.0).foo();
