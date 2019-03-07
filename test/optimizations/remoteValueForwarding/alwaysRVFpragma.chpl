class C {
  var loc: locale = here;
}

pragma "always RVF"
record R {
  var x: int;
  var y = new shared C();;
}

record S {
  var x: int;
  var y = new shared C();
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
    writeln("From locale ", here);
    writeln("------");
    writeln(myR);
    writeln(myR.locale);
    writeln(myR.x.locale);
    writeln(myS);
    writeln(myS.locale);
    writeln(myS.x.locale);
    writeln();
    writeln("Writing");
    writeln("-------");
    myR.x = 43;
    myR.y = new shared C();
    myS.x = 24;
    myS.y = new shared C();
    writeln(myR);
    writeln(myS);
    writeln();
  }

  writeln("Back home");
  writeln("---------");
  writeln(myR);
  writeln(myS);
}
