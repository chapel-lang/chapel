
use Print;

// Should just print 999 and return early
@dyno.printConvertedAst
proc helper(param first: bool) {
  if first {
    println(999);
    return;
  }

  println(100);
}

proc main() {
  helper(true);
}
