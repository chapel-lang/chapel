//
// Lock in a fix for a dead-module bug
//
// The module main "uses" the module internal but then doesn't
// actually access any of the symbols in internal.
//
// The module internal is logically dead and so will be subject to
// dead module elimination.  However the module "uses" itself.


// Until 2016/03/23
//
// The use of internal within internal creates a self-reference in
// the mod-use list that confused dead module elimination and ultimately
// lead to an internal compiler error
//
// The fix is to catch the insertion of the circular use when handling
// the use statement
//

module selfReference1 {
  use internal;

  proc main() {
    writeln('Hello');
  }
}

module internal {
  use internal;

  proc test0() {
    test1();
  }

  proc test1() {

  }
}
