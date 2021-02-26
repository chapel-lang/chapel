class C { var x : int; }

record R {
  var c : unmanaged C?;
  proc length return c!.x;
  proc deinit() { delete c; }
}

pragma "init copy fn"
proc chpl__initCopy(other : R, definedConst: bool) {
  pragma "no auto destroy"
  var ret : R;
  ret.c = new unmanaged C(other.length);
  return ret;
}

record D {
  proc member(i) {
    return i >= 50 && i <= 100;
  }
}

iter foo() {
  var A : [1..10] R;
  for a in A do a.c = new unmanaged C(5);

  for a in A do yield a;
}

iter foo(param tag:iterKind) where tag == iterKind.standalone {
  var A : [1..10] R;
  for a in A do a.c = new unmanaged C(5);

  for a in A do yield a;
}

proc main() {
  var x = new D();
  forall f in foo() with (const ref x) {
    writeln(x.member(f.length));
  }
}
