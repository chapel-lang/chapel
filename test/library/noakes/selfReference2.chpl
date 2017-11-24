//
// Lock in a fix for a dead-module bug
//
// The module main "uses" the module internal but then doesn't
// actually access any of the symbols in internal.
//
// The module internal is logically dead and so will be subject to
// dead module elimination.  However the function internal.test0()
// calls internal.test1() using a fully qualified name.


// Until 2016/03/23
//
// the fully qualified reference in test0() causes the module
// internal to add a "mod use" to itself during scopeResolve
// and causes a cycle that confuses dead module elimination.
//
// The fix is to catch the insertion of the circular use
//

module selfReference {
  use internal;

  proc main() {
    writeln('Hello');
  }
}

module internal {
  proc test0() {
    internal.test1();
  }

  proc test1() {

  }
}
