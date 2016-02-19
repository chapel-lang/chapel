// This test verifies that you can use a record type you've renamed, have that
// type as an argument to a function you've created, and access methods on the
// renamed type without incident.
module Foo {
  record bar {
    var a = 15;

    proc baz () {
      writeln("In Foo's bar.baz() method, with a = ", a);
    }
  }
}

module M {
  use Foo only bar as bleep, _defaultOf;
  // Ideally, I wouldn't have to name _defaultOf here, but since our new
  // constructor story is heading towards implementation, this seems fine for
  // now.

  proc main() {
    var instance: bleep;
    instance.baz();

    changeA(instance, 2);
    instance.baz();
  }

  proc changeA(ref x: bleep, newA) {
    x.a = newA * 3;
    writeln("I have alterred the deal, pray I do not alter it further");
  }
}
