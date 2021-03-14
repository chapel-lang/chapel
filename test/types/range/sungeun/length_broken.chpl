config const lo = min(int)+1;
config const hi = max(int);
config const noisy = false;

proc checkLen(lo, hi) {
  var r1: range = (lo..hi);
  var len = if lo<=hi then hi-lo+1 else 0;
  if noisy {
    writeln("lo=", lo, ", hi=", hi);
    writeln("\tr1.size=", r1.size, ", len=", len);
  }
  return(r1.size==len);
}

writeln(checkLen(lo, hi));
writeln(checkLen(hi, lo));
writeln(checkLen(lo, 0));
writeln(checkLen(0, hi));
