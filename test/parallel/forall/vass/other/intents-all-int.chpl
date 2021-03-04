// This test applies, to forall intents, the pattern of:
//   test/parallel/taskPar/vass/taskintents-all.chpl
//
// The pattern is:
//
//   cobegin with (ref i) { <-- always 'ref', to allow parallel updates to 'i'
//     task 1 updates 'i'
//     task 2 runs a 'forall' with a specific intent,
//            checking whether task 1's update to 'i' is visible
//   }
//

// configure the complexity of the parallel iterator
config param allowStandalone = true;
config param leaderSimple = true;

iter myiter() {
  writeln("Serial");
  yield 111;
}

// standalone
iter myiter(param tag) where tag == iterKind.standalone && allowStandalone {
  if leaderSimple then
    yield 333;
  else cobegin {
    yield 333;
    var jjj: int;
  }
}

// leader
iter myiter(param tag) where tag == iterKind.leader {
  if leaderSimple then
    yield 333;
  else cobegin {
    yield 333;
    var jjj: int;
  }
}

// follower
iter myiter(followThis, param tag: iterKind) where tag == iterKind.follower {
  yield followThis;
}


var i = 111;
var s1$, s2$: sync int;

/* blank intents are not available at present
s1$.reset(); s2$.reset();
writeln(i, "  blank");
cobegin with (ref i) {
  {
    s1$.readFE();
    i = 222;
    s2$.writeEF(1);
  }{
    forall indvar in myiter() with (i) {
      s1$.writeEF(1);
      s2$.readFE();
      writeln(i, "    inside");
    }
  }
}
*/
i = 222;

s1$.reset(); s2$.reset();
writeln(i, "  in");
cobegin with (ref i) {
  {
    s1$.readFE();
    i = 333;
    s2$.writeEF(1);
  }{
    forall indvar in myiter() with (in i) {
      s1$.writeEF(1);
      s2$.readFE();
      i += 1;
      writeln(i, "    inside");
    }
  }
}


s1$.reset(); s2$.reset();
writeln(i, "  const");
cobegin with (ref i) {
  {
    s1$.readFE();
    i = 444;
    s2$.writeEF(1);
  }{
    forall indvar in myiter() with (const i) {
      s1$.writeEF(1);
      s2$.readFE();
      writeln(i, "    inside");
    }
  }
}


s1$.reset(); s2$.reset();
writeln(i, "  const in");
cobegin with (ref i) {
  {
    s1$.readFE();
    i = 555;
    s2$.writeEF(1);
  }{
    forall indvar in myiter() with (const in i) {
      s1$.writeEF(1);
      s2$.readFE();
      writeln(i, "    inside");
    }
  }
}


s1$.reset(); s2$.reset();
writeln(i, "  const ref");
cobegin with (ref i) {
  {
    s1$.readFE();
    i = 666;
    s2$.writeEF(1);
  }{
    forall indvar in myiter() with (const ref i) {
      s1$.writeEF(1);
      s2$.readFE();
      writeln(i, "    inside");
    }
  }
}


s1$.reset(); s2$.reset();
writeln(i, "  ref");
cobegin with (ref i) {
  {
    s1$.readFE();
    i = 777;
    s2$.writeEF(1);
  }{
    forall indvar in myiter() with (ref i) {
      s1$.writeEF(1);
      s2$.readFE();
      i += 1;
      writeln(i, "    inside");
    }
  }
}


writeln(i, "  done");
