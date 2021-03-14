

proc mydoit(ref x)
{
  x = 0;
}

proc myreadwrite(loc:locale, ref x)
{
  on loc {
    mydoit(x);
  }
}

var x = 5;
myreadwrite(x.locale, x);
