// Leader recurses into itself.

var firstTime = true;

proc main {
  [ idx in zip(asdf()) ]
    writeln(idx);
}

iter asdf() {
  writeln("asdf serial");
  yield 555;
}
iter asdf(param tag):int {
  if firstTime {
    writeln("asdf ldr/sa firstTime {");
    firstTime = false;
    forall xdi in zip(asdf(),jjj()) {
      writeln("  asdf 1w: ", xdi);
    }
    forall xdi in zip(asdf(),jjj()) {
      writeln("  asdf 1y: ", xdi);
      yield xdi(0)+xdi(1);
    }
    writeln("  asdf 1z: 661");
    yield 661;
    writeln("asdf ldr/sa firstTime }");
  } else {
    writeln("asdf ldr/sa start");
    yield 666;
    writeln("asdf ldr/sa finish");
  }
}
iter asdf(param tag, followThis) {
  writeln("asdf follower ", followThis);
  yield 777;
}

iter jjj() {
  writeln("jjj serial");
  yield 322;
}
iter jjj(param tag, followThis) {
  writeln("jjj follower ", followThis);
  yield 223;
}
