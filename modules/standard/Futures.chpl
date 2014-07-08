
// future class
class future {
  type FutureType;
  var value$: sync FutureType;
  var resolved: atomic int;

  proc Future(type FutureType) {
    resolved = 0;
  }

  proc write(r: FutureType): void {
    resolved.add(1);
    var success: bool = (resolved.read() == 1);
    if (success) {
      value$.writeEF(r);
    } else {
      halt("attempted duplicate put on future!");
    }
  }

  proc read(): FutureType {
    return value$.readFF();
  }

  proc isResolved(): bool {
    return (resolved.read() >= 1);
  }
}

// factory method that uses lambdas
proc buildFuture(type t, lambdaOp) {
  var f = new future(t);
  begin {
    var r = lambdaOp();
    f.write(r); // guarantees future value is written
  }
  return f;
}

// primitive methods
proc +(a: future(?T1), b:future(?T2)) {
  return a.read() + b.read();
}
proc -(a: future(?T1), b:future(?T2)) {
  return a.read() - b.read();
}
proc *(a: future(?T1), b:future(?T2)) {
  return a.read() * b.read();
}
proc /(a: future(?T1), b:future(?T2)) {
  return a.read() / b.read();
}