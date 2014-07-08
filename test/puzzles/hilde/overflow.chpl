// overflow.chpl
//
// What should be the behavior of Chapel w.r.t. signed integer overflow?
//
// This test is related to the behavior of -fstrict-overflow if GCC is used as
// the backend compiler.
//


// Let's try to get an integer overflow (in the "if" statement).
proc foo(i) {
  // If we assume that integer overflow cannot occur, then this routine should
  // always return false, and GCC can optimize it away.
  if i + 3 < i then return true;
  else return false;
}

var count = 0;
for j in 0.. by max(int)/3 
{
  write(foo(j));
  if (foo(j)) then break;

  // Put in a limit, so we don't loop forever.
  count += 1;
  if count >= 10 then break;
}

writeln();
