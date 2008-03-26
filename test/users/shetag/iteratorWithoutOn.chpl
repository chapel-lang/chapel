config const n = 10;

def g() {
  for i in 0..n do
    yield i;
}

var loc = 0;
for i in g() {
  on Locales(loc) do _debugWriteln(here.id, ":i=", i);
  loc = (loc+1)%numLocales;
}
