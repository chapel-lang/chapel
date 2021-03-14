class Foo {
  var x : int = 1;
}

proc main() {
  var f    = new borrowed Foo();
  var LM   = here._instance:LocaleModel;
  var wide = false;

  // If the variable `f` is represented as a wide pointer within the local-on,
  // then the optimization is not performing correctly.
  for i in 1..LM.getChildCount() {
    local do on LM.getChild(i-1) {
      wide = wide || __primitive("is wide pointer", f);
    }
  }

  writeln(wide);
}
