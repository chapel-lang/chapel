on Locales(1 % numLocales) {
  var x: int;
  atomic on Locales (2 % numLocales) {
    var y: [1..10] int = 1;
    for i in 1..10 do 
      x += y(i);
  }
  writeln (Locales(1 % numLocales), " ", x, " ", Locales(2 % numLocales));
}
