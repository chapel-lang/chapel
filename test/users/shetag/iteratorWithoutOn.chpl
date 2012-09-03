config const n = 10;

iter g() {
  for i in 0..n do
    yield i;
}

var loc = 0;
for i in g() {
  on rootLocale.getLocale(loc) do writeln(here.id + ":i=" + i);
  loc = (loc+1)%numLocales;
}
