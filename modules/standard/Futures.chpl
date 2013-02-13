class future {
  type FutureType;
  var value: single FutureType;

  proc Future(type FutureType) {
  }

  proc put(r: FutureType): void {
    // writeln("put(", r, ")");
    value = r;
  }

  proc get(): FutureType {
    // writeln("get() = ", value);
    return value;
  }
}