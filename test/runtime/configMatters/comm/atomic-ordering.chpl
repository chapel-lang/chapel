// Tests that atomics are ordered correctly, specifically that a non-fetching
// atomic (add) completes before a subsequent fetching atomic (fetchSub). This
// tests the fix for issue Cray/chapel-private#4140

config const trials = 25_000;
for i in 1..trials {
  var x: atomic int;
  on Locales[numLocales-1] {
    x.write(1);
    x.add(1);
    var y = x.fetchSub(1);
    if y != 2 then halt("i=", i," y=",y);
  }
}
writeln("success");