pragma "always RVF"
record R {
  var x: int;

  proc chpl__serialize() {
    writeln(here.id, ": in R.serialize");
    return x;
  }

  proc type chpl__deserialize(x) {
    writeln(here.id, ": in R.deserialize");
    return new R(x+1);
  }
}

var myGlobR = new R(42);

on Locales[numLocales-1] {
  writeln(myGlobR);
}

{
  var myLocR = new R(33);
  
  on Locales[numLocales-1] {
    writeln(myLocR);
  }
}

proc main() {
  var myMainR = new R(78);
  
  on Locales[numLocales-1] {
    writeln(myMainR);
  }
}
