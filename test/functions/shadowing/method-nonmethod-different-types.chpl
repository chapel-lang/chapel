// Example 2 from 21668
module M {
  record R { }
  proc R.bar(arg: real) { writeln("in M.R.bar()"); }
  proc bar(arg: int) { writeln("in M.bar()"); }
  proc R.foo() {
    var x = 32;
    bar(x); // does it refer to M.R.bar() or M.bar() ?
  }
  proc main() { var r = new R(); r.foo(); }
}
