proc main {
  writeln("here 1");

  var tup: 300*int;

  on Locales(1) {
    begin {
      var t = tup;
    }
  }

  writeln("here 2");
}
