
class Foo {
  var x : int;
}

proc reflect(f : Foo) {
  writeln(f);
  return f;
}

proc assignReflect(ref f : Foo) {
  writeln(f);
  on Locales[numLocales-1] {
    f = new Foo(1337);
  }
  return f;
}

proc main() {
  writeln("Starting reflect()...");
  {
    var loc = new Foo(11);
    var locRet = reflect(loc);
    writeln(locRet);
    writeln("Local var was returned as wide pointer: ", __primitive("is wide pointer", locRet));

    var remote : Foo;

    on Locales[numLocales-1] {
      remote = new Foo(42);
    }
    var remoteRet = reflect(remote);
    writeln(remoteRet);
    writeln("Remote var was returned as wide pointer: ", __primitive("is wide pointer", remoteRet));
  }
  writeln("Starting assignReflect()...");
  {
    var loc = new Foo(11);
    var locRet = assignReflect(loc);
    writeln(locRet);
    writeln("Local var was returned as wide pointer: ", __primitive("is wide pointer", locRet));

    var remote : Foo;

    on Locales[numLocales-1] {
      remote = new Foo(42);
    }
    var remoteRet = assignReflect(remote);
    writeln(remoteRet);
    writeln("Remote var was returned as wide pointer: ", __primitive("is wide pointer", remoteRet));
  }
}
