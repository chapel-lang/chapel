var totNumLocales = + reduce [r in Realms] r.numLocales;
const allLocales: [lid in 0..#totNumLocales] locale = getNthLocale(lid);

def getNthLocale(in lid) {
  for r in Realms {
    if lid < r.numLocales then
      return r.Locales[lid];
    lid -= r.numLocales;
  }
  halt("lid out of range in getNthLocale()");
  return Realms[0].Locales[0];
}

