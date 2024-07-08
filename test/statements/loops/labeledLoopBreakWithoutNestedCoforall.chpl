label outerer for qq in 1..3 do {
  coforall loc in Locales[0..0] do on loc {
    coforall tid in 0..<1 {
      var j = 0;
      label outer while j < 10 {
	var i = 1;
	do {
	  writeln((i, j));
	  if i == 3 then
	    break outerer;
	  i += 1;
	  j += 1;
	} while (i < 100);
      }
    }
  }
}
