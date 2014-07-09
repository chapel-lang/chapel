use Memory;

class C {
  var i: int;
}

proc main() {
  for loc in Locales do on loc {
    startVerboseMemHere();
    var c = new C();
    delete c;
    stopVerboseMemHere();
  }
}
