
proc main {
  var result: int;

  forall aaa in AAA() with (+ reduce result) {
    result += aaa;
  }

  writeln(result);
}

iter AAA(): nothing {
  halt("do not invoke me");
}

iter AAA(param tag) where tag == iterKind.standalone {
  on Locales[1] do
    yield 5;
}
