use impl;
use memTrackSupport;

config const noisy = false;
config const n = 25;

const m0 = allMemoryUsed();
{
  var s:string_impl = "blah";

  for i in 1..#n do on Locales[i%numLocales] do s += s;

  if noisy then writeln(s);
}
checkMemLeaks(m0);
