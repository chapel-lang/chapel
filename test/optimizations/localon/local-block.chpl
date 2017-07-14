
class Foo { var x : int = 5; }

var f : Foo;
on Locales[numLocales-1] do f = new Foo();

local do on here do writeln(f);

// Should not register as a local-on, and should throw a runtime error
// for a remote access inside a local block
local {
  on here do writeln(f);
}
