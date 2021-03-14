
class Foo {
  var x : int;
}

proc helper(const ref c : borrowed Foo) {
  writeln("Inside helper, c = ", c);
  writeln("is 'c' wide? ", __primitive("is wide pointer", c));
}

proc main() {
  var loc = new borrowed Foo(42);
  var rem = new borrowed Foo(100);
  helper(loc);

  on Locales[numLocales-1] {
    helper(rem);
  }

  // Without optimizations, this will return 'true'
  writeln("is 'loc' wide? ", __primitive("is wide pointer", loc));
}
