// return_of_new_string.chpl
//
// This test case demonstrates a bug in AMM, regarding where autoDestroys are
// inserted.
//
// The final case, fubar(true) causes the unnamed temporary capturing the
// string result to be freed without having first been initialized.  This
// causes an illegal free under valgrind followed by a crash.
//

proc get_literal() : string return "Free Willy!";

// This one should work in both cases.
// It demonstrates the source-code workaround for the problem, which is to
// insert an explicit else clause.
proc bar(cond:bool, out empty:bool) : string {
  if (cond) {
    empty = true;
    return "";
  }
  else {
    empty = false;
    return get_literal();
  }
}

// This one demonstrates the problem when cond == true.
// The problem does not surface without the out argument.
proc fubar(cond:bool, out empty:bool) : string {
  if (cond) {
    empty = true;
    return "";
  }
  empty = false;
  return get_literal();
}

var is_empty : bool;

writeln(bar(false, is_empty));
writeln(bar(true, is_empty));
writeln(fubar(false, is_empty));
writeln(fubar(true, is_empty));

