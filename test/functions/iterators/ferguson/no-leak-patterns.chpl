iter yield3() {
  yield 1;
  yield 2;
  yield 3;
}
iter yield3(param tag: iterKind) where tag==iterKind.standalone
{
  yield 1;
  yield 2;
  yield 3;
}
iter yield3(param tag: iterKind) where tag==iterKind.leader
{
  yield 1..1;
  yield 2..2;
  yield 3..3;
}
iter yield3(param tag: iterKind, followThis) where tag==iterKind.follower
{
  yield followThis.low;
}

proc testReturn() {
  for i in yield3() {
    writeln(i);
    return;
  }
}

proc testBreak() {
  for i in yield3() {
    writeln(i);
    break;
  }
}

proc testThrow() {
  try {
    for i in yield3() {
      writeln(i);
      throw new owned Error();
    }
  } catch {
  }
}

proc testForallThrow() {
  try {
    forall i in yield3() {
      writeln(i);
      throw new owned Error();
    }
  } catch {
  }
}

proc testForallZipThrow() {
  try {
    forall (i,j) in zip(yield3(),yield3()) {
      assert(i==j);
      writeln(i);
      throw new owned Error();
    }
  } catch {
  }
}

iter yieldLoop() {
  var i = 1;
  while i <= 3 {
    yield i;
    i += 1;
  }
}
iter yieldLoop(param tag: iterKind) where tag==iterKind.standalone
{
  var i = 1;
  while i <= 3 {
    yield i;
    i += 1;
  }
}
iter yieldLoop(param tag: iterKind) where tag==iterKind.leader
{
  var i = 1;
  while i <= 3 {
    yield i..i;
    i += 1;
  }
}
iter yieldLoop(param tag: iterKind, followThis) where tag==iterKind.follower
{
  yield followThis.low;
}


proc testLoopReturn() {
  for i in yieldLoop() {
    writeln(i);
    return;
  }
}

proc testLoopBreak() {
  for i in yieldLoop() {
    writeln(i);
    break;
  }
}

proc testLoopThrow() {
  try {
    for i in yieldLoop() {
      writeln(i);
      throw new owned Error();
    }
  } catch {
  }
}

proc testLoopForallThrow() {
  try {
    forall i in yieldLoop() {
      writeln(i);
      throw new owned Error();
    }
  } catch {
  }
}

proc testLoopForallZipThrow() {
  try {
    forall (i,j) in zip(yieldLoop(),yieldLoop()) {
      assert(i==j);
      writeln(i);
      throw new owned Error();
    }
  } catch {
  }
}

config const test = 1;

select test {
  when 1 do testReturn();
  when 2 do testBreak();
  when 3 do testThrow();
  when 4 do testForallThrow();
  when 5 do testForallZipThrow();
  when 6 do testLoopReturn();
  when 7 do testLoopBreak();
  when 8 do testLoopThrow();
  when 9 do testLoopForallThrow();
  when 10 do testLoopForallZipThrow();
  otherwise do writeln("Unknown case");
}
