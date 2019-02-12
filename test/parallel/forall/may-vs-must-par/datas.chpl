// Helper datatypes.

config const nd = 2;
const RNGd = 1..nd;

const inst1ser = new owned class1ser();
const inst1SA  = new owned class1SA();
const inst1LF  = new owned class1LF();
const inst1par = new owned class1par();

const inst2ser = new owned class2ser();
const inst2SA  = new owned class2SA();
const inst2LF  = new owned class2LF();
const inst2par = new owned class2par();

var cnt1d, cnt2d: atomic int;
proc inc1d        { cnt1d.add(1); }
proc inc2d        { cnt2d.add(1); }
proc resetCountsD { cnt1d.write(0); cnt2d.write(0); }
proc showCountsD  { writeln("followers: ", cnt1d.read(), " ", cnt2d.read());
                    writeln(); resetCountsD; }

/////////// serial-only ///////////

class class1ser {
  iter these() {
    writeln("class1ser");
    for idx in RNGd do
      yield idx;
  }
  proc chpl__promotionType() type return int;
}

class class2ser {
  iter these() {
    writeln("class2ser");
    for idx in RNGd do
      yield -idx;
  }
  proc chpl__promotionType() type return int;
}

/////////// serial+standalone ///////////

class class1SA {
  iter these() {
    writeln("class1SA serial");
    for idx in RNGd do
      yield idx;
  }
  iter these(param tag) where tag == iterKind.standalone {
    writeln("class1SA standalone");
    for idx in RNGd.these(tag) do
      yield idx;
  }
  proc chpl__promotionType() type return int;
}

class class2SA {
  iter these() {
    writeln("class2SA serial");
    for idx in RNGd do
      yield -idx;
  }
  iter these(param tag) where tag == iterKind.standalone {
    writeln("class2SA standalone");
    for idx in RNGd.these(tag) do
      yield -idx;
  }
  proc chpl__promotionType() type return int;
}

/////////// serial+leader/follower ///////////

class class1LF {
  iter these() {
    writeln("class1LF serial");
    for idx in RNGd do
      yield idx;
  }
  iter these(param tag) where tag == iterKind.leader {
    writeln("class1LF leader");
    for idx in RNGd.these(tag) do
      yield idx;
  }
  iter these(param tag, followThis) where tag == iterKind.follower {
    inc1d;
    for idx in RNGd.these(tag, followThis) do
      yield idx;
  }
  proc chpl__promotionType() type return int;
}

class class2LF {
  iter these() {
    writeln("class2LF serial");
    for idx in RNGd do
      yield -idx;
  }
  iter these(param tag) where tag == iterKind.leader {
    writeln("class2LF leader");
    for idx in RNGd.these(tag) do
      yield idx;
  }
  iter these(param tag, followThis) where tag == iterKind.follower {
    inc2d;
    for idx in RNGd.these(tag, followThis) do
      yield -idx;
  }
  proc chpl__promotionType() type return int;
}

/////////// serial + LF + SA ///////////

class class1par {
  iter these() {
    writeln("class1par serial");
    for idx in RNGd do
      yield idx;
  }
  iter these(param tag) where tag == iterKind.standalone {
    writeln("class1par standalone");
    for idx in RNGd.these(tag) do
      yield idx;
  }
  iter these(param tag) where tag == iterKind.leader {
    writeln("class1par leader");
    for idx in RNGd.these(tag) do
      yield idx;
  }
  iter these(param tag, followThis) where tag == iterKind.follower {
    inc1d;
    for idx in RNGd.these(tag, followThis) do
      yield idx;
  }
  proc chpl__promotionType() type return int;
}

class class2par {
  iter these() {
    writeln("class2par serial");
    for idx in RNGd do
      yield -idx;
  }
  iter these(param tag) where tag == iterKind.standalone {
    writeln("class2par standalone");
    for idx in RNGd.these(tag) do
      yield -idx;
  }
  iter these(param tag) where tag == iterKind.leader {
    writeln("class2par leader");
    for idx in RNGd.these(tag) do
      yield idx;
  }
  iter these(param tag, followThis) where tag == iterKind.follower {
    inc2d;
    for idx in RNGd.these(tag, followThis) do
      yield -idx;
  }
  proc chpl__promotionType() type return int;
}
