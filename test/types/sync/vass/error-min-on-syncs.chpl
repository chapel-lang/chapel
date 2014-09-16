// Currently min/max are not allows on sync/single/atomic vars.

config param
  doMin = false,
  doMax = false,
  doSync = false,
  doSingle = false,
  doAtomic  = false;

proc test(type T) {
  var a, b: T;
  if doMin then return min(a,b);  if doMax then return max(a,b);
}

if doSync then writeln(test(sync int));
if doSingle then writeln(test(single int));
if doAtomic  then writeln(test(atomic int));
