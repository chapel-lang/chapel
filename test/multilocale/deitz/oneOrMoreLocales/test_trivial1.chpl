proc main {
  var x: int;
  coforall i in 0..numLocales-1 with (ref x) {
    var y: int;
    on Locales(i) {
      y = i+3628800;
    }
    x += y;
  }
  writeln((x-(numLocales*(numLocales-1)/2))/numLocales);
}
