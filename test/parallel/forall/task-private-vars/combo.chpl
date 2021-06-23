
var s$ : sync int;

iter mydriver() {
  for i in 1..3 {
    s$.writeEF(i);
    yield i;
  }
}

iter myIter() { yield 555; }
iter myIter(param tag) {
  yield 666;
  coforall i in mydriver() {
    yield i * 1111;
    s$.readFE();
  }
}

var VVV = 10000;
proc update() {
  VVV += 1;
  writeln("global VVV ", VVV);
}

var cnt: atomic int;
cnt.write(1);

proc nextCnt(arg: int) {
  const curr = cnt.fetchAdd(1);
  const ret  = curr + s$.readXX()*100;
  writeln("curr ", curr, "   ret ", ret, "   arg ", arg);
  return ret;
}

proc main {

  forall idxVar in myIter() with (
    ref VVV,
    var v111 = nextCnt(numLocales),
    var v222 = nextCnt(v111),
    var v333 = nextCnt(v222 + VVV)
  ) {
    writeln("idxVar ", idxVar);
    update();
    writeln(v111);
    writeln(v222);
    writeln(v333);
    writeln(numLocales);
    writeln(VVV);
    writeln();
  }

}
