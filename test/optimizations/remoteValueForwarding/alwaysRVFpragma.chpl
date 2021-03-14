pragma "always RVF"
record R {
  var x: int;
  var loc = here.id;
}

record S {
  var x: int;
  var loc = here.id;
}

proc main() {
  var myR = new R(42);
  var myS = new S(23);

  writeln("Initially");
  writeln("---------");
  writeln(myR);
  writeln(myS);
  writeln();

  on Locales[numLocales-1] {
    writeln("From locale ", here.id);
    writeln("------");
    writeln(myR);
    writeln(myR.locale.id);
    writeln(myR.x.locale.id);
    writeln(myS);
    writeln(myS.locale.id);
    writeln(myS.x.locale.id);
    writeln();
    writeln("Writing");
    writeln("-------");
    myR.x = 43;
    myR.loc = here.id;
    myS.x = 24;
    myS.loc = here.id;
    writeln(myR);
    writeln(myS);
    writeln();
  }

  writeln("Back home");
  writeln("---------");
  writeln(myR);
  writeln(myS);
}
