// A basic test with reduce intents.

config param allowStandalone = true;

iter myiter() {
  writeln("Serial");
  yield 111;
}

iter myiter(param tag) where tag == iterKind.standalone && allowStandalone {
  writeln("Standalone");
//not implemented:  yield 100;
  cobegin {
    {
      yield 222;
      cobegin {
        yield 333;
        var jjj:int;
      }
    }
    var kkk: int;
  }
}

// leader
iter myiter(param tag) where tag == iterKind.leader {
  writeln("Leader");
  cobegin {
    yield 11;
    yield 22;
  }
}

// follower
iter myiter(followThis, param tag: iterKind) where tag == iterKind.follower {
  writeln("Follower");
  yield followThis + 1;
  yield followThis + 2;
}

proc main {
  var xxx, yyy: int;
  var zzz = 1;
  forall iii in myiter() with (+ reduce xxx) {
    xxx = iii * 10 + zzz;
  }
  writeln(xxx);
  forall jjj in myiter() with (+ reduce xxx, + reduce yyy, zzz) {
    xxx += zzz;
    yyy = zzz;
  }
  writeln(xxx, " loop iterations   ", yyy, " tasks");
}
