label outerer for qq in 1..3 do {
  on Locales[0] {
    var i = 1;
    do {
      writeln(i);
      if i == 3 then
	continue outerer;
      i += 1;
    } while (i < 100);
  }
}
