const s: string = "hi";

on Locales[(here.id + 1) % numLocales] {
  writeln("s is ", s, " and is on locale: ", s.locale.id);
}
