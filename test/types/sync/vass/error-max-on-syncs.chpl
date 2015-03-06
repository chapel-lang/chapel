// Currently min/max are not allowed on sync/single/atomic vars.

config param
  doSync = false,
  doSingle = false,
  doAtomic  = false;

proc test(type T) {
  var a, b: T;
  return max(a,b);
}

if doSync then writeln(test(sync int));
if doSingle then writeln(test(single int));
if doAtomic  then writeln(test(atomic int));
compilerError("should not get here"); // ensure we do not execute this
