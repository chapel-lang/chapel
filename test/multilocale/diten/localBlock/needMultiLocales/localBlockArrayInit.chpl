// Originally from https://github.com/chapel-lang/chapel/issues/10536

on Locales[numLocales-1] {
  var s, l: int;
  local {
    var smallArr: [1..1] int;
    s = smallArr.size;

    // allocate 1/2 GB array (more than large enough to trigger parallel init)
    var largeArr: [1..(64 * 1024 * 1024)] int;
    l = largeArr.size;
  }
  writeln(s);
  writeln(l);
}
