use IO.FormattedIO;

var counter = 0;
proc writelnCnt(const args ...?k) throws {
  writelnFix((...args));
  counter += 1;
}

proc writelnFix(const args ...?k) throws {
  var s: string = "%3i ".format(counter);
  try writeln(s, (...args));
}


proc getSubloc() {
  if localeModelHasSublocales {
    return chpl_getSubloc();
  }
  return 0;
}

if(here.getChildCount() != 2) {
  writeln("----------------------------------------------------------------------------");
  writeln("We expect to run this test using the NUMA locale model on a machine with two");
  writeln("NUMA domains.");
  writeln("----------------------------------------------------------------------------");
  exit(1);
}

writelnCnt("NUM LOCALES: ", numLocales-1);
writelnCnt("CHILD COUNT: ", here.getChildCount());
writelnCnt("-------------");

{
  writelnCnt("Range iteration:");
  var r1 = 1..4;
  forall i in r1 {
    writelnFix("  i = ", i, "\t", here, "\t", getSubloc());
  }
}

{
  writelnCnt("Default rectangular domain zippered iteration:");
  var D : domain(2) = {1..2, 1..2};
  forall i in zip(D, D) {
    // TODO: For some reason if we use here (the constant) rather than "here"
    // the string we get the following error: use of 'here' before encountering
    // its definition, type unknown.  Looks like this is a bug that should be
    // fixed.  Once this is resolved remove the double quotes.
    // FIXME: See https://github.com/chapel-lang/chapel/issues/18778
    writelnFix("  i = ", i, "\t", "here", "\t", getSubloc());
  }
}

{
  writelnCnt("Array view slice iteration:");
  const A = [1, 2, 3, 4, 5, 6];
  const D = {2..5};
  forall i in A[D] {
    writelnFix("  i = ", i, "\t", here, "\t", getSubloc());
  }
}

{
  writelnCnt("Array view reindex iteration:");
  const A = [1, 2];
  const D = {4..5};
  forall i in A.reindex(D) {
    writelnFix("  i = ", i, "\t", here, "\t", getSubloc());
  }
}

{
  writelnCnt("Array view rank change iteration:");
  var A : [1..2, 1..2] int;
  A[1, 1] = 1; A[1, 2] = 2; A[2, 1] = 3; A[2, 2] = 4;
  const B = A[1, ..];
  forall i in B {
    writelnFix("  i = ", i, "\t", here, "\t", getSubloc());
  }
}

