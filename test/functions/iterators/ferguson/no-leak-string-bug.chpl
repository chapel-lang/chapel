iter foo() {
  var A : [1..10] string = "hello";

  for a in A do yield a;
}

iter foo(param tag:iterKind) where tag == iterKind.standalone {
  var A : [1..10] string = "hello";

  for a in A do yield a;
}

proc main() {
  var x = {50..100};
  forall f in foo() with (const ref x) {
    writeln(x.contains(f.size));
  }
}
