use BlockDist;

const space = {1..10};

proc scanArr(Dom) {
  var X: [Dom] int = space;

  var Y = + scan X;
  writeln(Y);
}

const nonFirstLocDom = space dmapped blockDist(space, targetLocales=Locales[1..<numLocales]);
const nonLastLocDom  = space dmapped blockDist(space, targetLocales=Locales[0..<numLocales-1]);
const evenLocDom     = space dmapped blockDist(space, targetLocales=Locales[0..<numLocales by 2]);
const allLocDom      = space dmapped blockDist(space, targetLocales=Locales[0..<numLocales]);

scanArr(nonFirstLocDom);
scanArr(nonLastLocDom);
scanArr(evenLocDom);
scanArr(allLocDom);
