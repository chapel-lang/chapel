record R { }

proc b() : R(?) {
  return new R();
}
b();
