proc myadd(a: int, b:int) {
  return a + b;
}

proc f(x, y, param p, type t) {
  return myadd(x, y);
}

record MyRecord { }
record OtherRecord { }

proc main() {
  var a: MyRecord;
  var b: OtherRecord;

  var c = f(a, b, 1, real);
}
