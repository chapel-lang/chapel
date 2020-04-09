
proc main {
  var result: int;

  forall aaa in AAA() with (+ reduce result) {
    result += aaa;
  }

  writeln(result);
}

iter AAA() {
  halt("do not invoke me");
  yield 1;
}

iter AAA(param tag) where tag == iterKind.standalone {
  on Locales[1] do
    yield 5;
}
