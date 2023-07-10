use CTypes;

config const n = 10;

extern proc printf(fmt:c_ptrConst(c_char), x...);

iter g() : int {
  var loc = 0;
  for i in 0..n {
    on Locales(loc) do yield i;
    loc = (loc+1)%numLocales;
  }
}

for i in g() do
  printf(c_ptrToConst_helper("%s\n"), c_ptrToConst_helper(here.id:string + ":i=" + i:string));
