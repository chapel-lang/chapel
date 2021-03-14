use UnorderedCopy;

config const never = false;

var a: int;
on Locales[1] {
  a = 1;                      // cached PUT
  unorderedCopy(a, 2);        // unordered PUT to same addr (should flush cache)
  unorderedCopyTaskFence();
}
writeln(a);

on Locales[1] {
  var b: int;
  b = a;                     // GET and store in cache
  if never then writeln(b);  // prevent dead code elim and other opts
  unorderedCopy(a, 3);       // unordered PUT do same addr (should flush cache)
  unorderedCopyTaskFence();
  b = a;                     // real GET, should not be from cache
  writeln(b);
}
