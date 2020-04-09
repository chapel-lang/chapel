// Helper iterators.

config const ni = 2;
const RNGi = 1..ni;

var cnt1i, cnt2i: atomic int;
proc inc1i        { cnt1i.add(1); }
proc inc2i        { cnt2i.add(1); }
proc resetCountsI { cnt1i.write(0); cnt2i.write(0); }
proc showCountsI  { writeln("followers: ", cnt1i.read(), " ", cnt2i.read());
                    writeln(); resetCountsI; }

/////////// serial-only ///////////

iter iter1ser() {
  writeln("iter1ser");
  for idx in RNGi do
    yield idx;
}

iter iter2ser() {
  writeln("iter2ser");
  for idx in RNGi do
    yield -idx;
}

/////////// serial+standalone ///////////

iter iter1SA() {
  writeln("iter1SA serial");
  for idx in RNGi do
    yield idx;
}
iter iter1SA(param tag) where tag == iterKind.standalone {
  writeln("iter1SA standalone");
  for idx in RNGi.these(tag) do
    yield idx;
}

iter iter2SA() {
  writeln("iter2SA serial");
  for idx in RNGi do
    yield -idx;
}
iter iter2SA(param tag) where tag == iterKind.standalone {
  writeln("iter2SA standalone");
  for idx in RNGi.these(tag) do
    yield -idx;
}

/////////// serial+leader/follower ///////////

iter iter1LF() {
  writeln("iter1LF serial");
  for idx in RNGi do
    yield idx;
}
iter iter1LF(param tag) where tag == iterKind.leader {
  writeln("iter1LF leader");
  for idx in RNGi.these(tag) do
    yield idx;
}
iter iter1LF(param tag, followThis) where tag == iterKind.follower {
  inc1i;
  for idx in RNGi.these(tag, followThis) do
    yield idx;
}

iter iter2LF() {
  writeln("iter2LF serial");
  for idx in RNGi do
    yield -idx;
}
iter iter2LF(param tag) where tag == iterKind.leader {
  writeln("iter2LF leader");
  for idx in RNGi.these(tag) do
    yield idx;
}
iter iter2LF(param tag, followThis) where tag == iterKind.follower {
  inc2i;
  for idx in RNGi.these(tag, followThis) do
    yield -idx;
}

/////////// serial + LF + SA ///////////

iter iter1par() {
  writeln("iter1par serial");
  for idx in RNGi do
    yield idx;
}
iter iter1par(param tag) where tag == iterKind.standalone {
  writeln("iter1par standalone");
  for idx in RNGi.these(tag) do
    yield idx;
}
iter iter1par(param tag) where tag == iterKind.leader {
  writeln("iter1par leader");
  for idx in RNGi.these(tag) do
    yield idx;
}
iter iter1par(param tag, followThis) where tag == iterKind.follower {
  inc1i;
  for idx in RNGi.these(tag, followThis) do
    yield idx;
}

iter iter2par() {
  writeln("iter2par serial");
  for idx in RNGi do
    yield -idx;
}
iter iter2par(param tag) where tag == iterKind.standalone {
  writeln("iter2par standalone");
  for idx in RNGi.these(tag) do
    yield -idx;
}
iter iter2par(param tag) where tag == iterKind.leader {
  writeln("iter2par leader");
  for idx in RNGi.these(tag) do
    yield idx;
}
iter iter2par(param tag, followThis) where tag == iterKind.follower {
  inc2i;
  for idx in RNGi.these(tag, followThis) do
    yield -idx;
}
