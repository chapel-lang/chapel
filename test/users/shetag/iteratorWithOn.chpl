config const n = 10;

def g() {
  var loc = 0;
  for i in 0..n {
    on Locales(loc) do yield i;
    loc = (loc+1)%numLocales;
  }
}

for i in g() do
  _debugWriteln(localeID(), ":i=", i);
