// It is hoped that we will find a better solution than _defaultOf for type
// defaults.  Leaning on this as training wheels for now (though I don't
// necessarily have a plan for how my initializers work will serve to remove the
// need for _defaultOf right now)

class Foo {
  var x: imag;
  var y = 7;

  proc init(a) {
    y = a;
  }

}

proc _defaultOf(type t) where t == imag {
  return 1.0i;
}

var r: Foo = new Foo(3);
writeln(r);
delete r;
