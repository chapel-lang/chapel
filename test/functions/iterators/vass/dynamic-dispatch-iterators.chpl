class Superclass {
  proc parens() { writeln("parens() in Superclass"); }
  proc noparns { parens(); }
  iter itest() { yield "Superclass"; }
}

class Subclass:Superclass {
  override proc parens() { writeln("parens() in Subclass"); }
  override iter itest() { yield "Subclass"; }
}

var tmp: unmanaged Superclass?;
tmp = if numLocales > 1 then new unmanaged Superclass() else new unmanaged Subclass();
var c = tmp:unmanaged Superclass;

// check whether dynamic dispatch within a paren-less function works
c.noparns;

// check whether dynamic dispatch for iterators works
for n in c.itest() do writeln(n);

delete c;
