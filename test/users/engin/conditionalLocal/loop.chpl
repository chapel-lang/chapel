proc inc(ref x) {
  local x.locale == here do x += 1;
}

var a = 0;
for l in Locales do inc(a);

if a!=numLocales then halt("Something went wrong");
