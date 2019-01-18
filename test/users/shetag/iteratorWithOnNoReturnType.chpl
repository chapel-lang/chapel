config const n = 10;

extern proc printf(fmt:c_string, x...);

iter g() {
  var loc = 0;
  for i in 0..n {
    on Locales(loc) do yield i;
    loc = (loc+1)%numLocales;
  }
}

for i in g() do
  printf("%s\n", (here.id + ":i=" + i).c_str());
