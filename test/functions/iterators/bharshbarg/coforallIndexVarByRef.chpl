
record R {
  var els : [1..4] int;
}

proc main() {
  var A : [1..4] R;

  //
  // 'a' should be a reference to the element in the array 'A'. This can go
  // wrong if argument intents are incorrectly set, or if the compiler
  // incorrectly handles FLAG_COFORALL_INDEX_VAR.
  //
  // BHARSH 2016-11-10: I ran into this issue after the introduction of
  // qualified references.
  //
  coforall a in A {
    a.els = 5;
  }

  for a in A do
    writeln(a.els);

}
