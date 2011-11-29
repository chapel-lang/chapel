cobegin {
  {
    var x = 0;
    var limit = 10000000;
    for i in (0..limit) {
      x = x + 17;
    }
    writeln ("Four peanuts were walking through Chapel code.");
  }
  writeln ("One was assaulted (nut)!");
}
