class Superclass {
  proc parens() { writeln("parens() in Superclass"); }
  proc noparns { parens(); }
  iter itest() { yield "Superclass"; }
}

class Subclass:Superclass {
  proc parens() { writeln("parens() in Subclass"); }
  iter itest() { yield "Subclass"; }
}

var c: Superclass;
c = if numLocales > 1 then new Superclass() else new Subclass();

// check whether dynamic dispatch within a paren-less function works
c.noparns;

// check whether dynamic dispatch for iterators works
for n in c.itest() do writeln(n);
