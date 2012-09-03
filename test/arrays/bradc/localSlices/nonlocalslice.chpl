var A: [1..10] real = [i in 1..10] i;

on rootLocale.getLocales()(1) {
  writeln(A.localSlice(1..10 by 2));
  writeln(A.localSlice({1..10 by 2}));
}
