
class Foo {
  var x : int;
}

proc reflect(f : unmanaged Foo) {
  writeln(f);
  return f;
}

proc assignReflect(ref f : unmanaged Foo) {
  writeln(f);
  on Locales[numLocales-1] {
    f = new unmanaged Foo(1337);
  }
  return f;
}

proc main() {
  writeln("Starting reflect()...");
  {
    var loc = new unmanaged Foo(11);
    var locRet = reflect(loc);
    writeln(locRet);
    writeln("Local var was returned as wide pointer: ", __primitive("is wide pointer", locRet));

    var remote : unmanaged Foo?;

    on Locales[numLocales-1] {
      remote = new unmanaged Foo(42);
    }
    var remoteRet = reflect(remote!);
    writeln(remoteRet);
    writeln("Remote var was returned as wide pointer: ", __primitive("is wide pointer", remoteRet));
  }
  writeln("Starting assignReflect()...");
  {
    var loc = new unmanaged Foo(11);
    var locRet = assignReflect(loc);
    writeln(locRet);
    writeln("Local var was returned as wide pointer: ", __primitive("is wide pointer", locRet));

    var remote : unmanaged Foo?;

    on Locales[numLocales-1] {
      remote = new unmanaged Foo(42);
    }
    var remoteB = remote!;
    var remoteRet = assignReflect(remoteB);
    writeln(remoteRet);
    writeln("Remote var was returned as wide pointer: ", __primitive("is wide pointer", remoteRet));
  }
}
