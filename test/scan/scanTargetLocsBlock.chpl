use BlockDist;

const space = {1..10};

proc scanArr(Dom) {
  var X: [Dom] int = space;

  var Y = + scan X;
  writeln(Y);
}

const nonFirstLocDom = space dmapped new blockDist(space, targetLocales=Locales[1..<numLocales]);
const nonLastLocDom  = space dmapped new blockDist(space, targetLocales=Locales[0..<numLocales-1]);
const evenLocDom     = space dmapped new blockDist(space, targetLocales=Locales[0..<numLocales by 2]);
const allLocDom      = space dmapped new blockDist(space, targetLocales=Locales[0..<numLocales]);

scanArr(nonFirstLocDom);
scanArr(nonLastLocDom);
scanArr(evenLocDom);
scanArr(allLocDom);
