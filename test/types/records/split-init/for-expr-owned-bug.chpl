class C { }
record R {
  var ptr: owned C = new owned C();
}

proc main() {
  var A = for i in 0..1 do new R();
}
