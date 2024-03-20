class C {
  var sy: sync int;
  proc init() {}
}

var c = new unmanaged C();
on Locales[numLocales-1] {
  delete c;
}
