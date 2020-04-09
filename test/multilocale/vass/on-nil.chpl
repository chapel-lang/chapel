// This test exercises the various cases of the on-clause,
// i.e. the EXPR in "on EXPR do STMT;"
// questioning which locale STMT should execute on, in each case.


// Need a couple of extra locales for this test.
assert(numLocales >= 3);


// PART 1: EXPR is a literal
// (a) These execute STMT on the current locale.
// (b) Get a compiler warning "this does not do anything useful".
//
// While 'nil' is not a literal in the spec,
// we seem to agree that it should behave like a literal in this context.

writeln("PART 1");
on Locales[1] {
  on 0.0 do
    writeln("on 0.0: ", here);
  on "hello" do
    writeln("on \"hello\": ", here);
  on nil do
    writeln("on nil: ", here);
}


// PART 2: where EXPR is of a "value" type
// (a) if EXPR is a variable, execute STMT on that variable's locale.
//     Ditto for any l-value EXPR?
// (b) if EXPR is an r-value, execute STMT on the current locale
//     and issue a compiler warning as above.

writeln("PART 2");
on Locales[1] {
  var x: int;
  on Locales[2] {
    on x do
      writeln("on x: ", here);
    on 3+4 do
      writeln("on 3+4: ", here);
  }
}

// PART 3: EXPR is nil

writeln("PART 3b");
{
  var obj:object? = new object();
  on Locales[1] {
    obj = nil;
    on Locales[2] {
      on obj do
        writeln("on obj: ", here);
    }
  }
}
