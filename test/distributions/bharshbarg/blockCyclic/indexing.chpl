use BlockCycDist;

config param rank = 1;

config const n     = 10;
config const debug = false;

proc test(Dom, bs) {
  if debug then writeln("### Testing ", Dom, " with blocksize ", bs, " ###");

  const start = if isTuple(Dom.low) then Dom.low else (Dom.low,);
  const Space = Dom dmapped BlockCyclic(startIdx=start, blocksize=bs);
  var A : [Space] int;

  forall i in Space do A[i] = here.id + 1;

  for a in A do assert(a >= 1);
}

iter blockIter(param r : int) {
  const first = (1,); // base case
  var e = 2;
  while n % e != 0 do e += 1;
  var o = 2;
  while n % o == 0 do o += 1;
  if r == 1 {
    yield (1,);
    yield (e,);
    yield (o,);
    yield (n,);
  } else {
    for b in blockIter(r-1) {
      yield (1, (...b));
      yield (e, (...b));
      yield (o, (...b));
      yield (n, (...b));
    }
  }
}

proc main() {
  if n < 1 then halt("config 'n' must be >= 1.");

  var dims : rank*range;
  for param i in 0..rank-1 {
    dims(i) = 1..n;
  }

  const Dom = {(...dims)};

  for bs in blockIter(rank) {
    test(Dom, bs);
  }

  writeln("SUCCESS");
}
