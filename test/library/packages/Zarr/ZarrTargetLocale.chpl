use Zarr;
use Random;
use BlockDist;
use IO;


const D = {0..<10, 0..<10};
var A: [D] real(32);
fillRandom(A);

// write the store that we will read in a distributed fashion
writeZarrArrayLocal("TestTargetLocale", A, (3,3));

// read with the default
var defaultArray = readZarrArray("TestTargetLocale", real(32), 2);

// read with custom locale distribution
const myTargetLocales = reshape(Locales, {0..0, 0..#numLocales});
var targetedArray = readZarrArray("TestTargetLocale", real(32), 2, targetLocales=myTargetLocales);

on Locales[0] {
  var defaultLocale0 = defaultArray.localSubdomain();
  assert(defaultLocale0 == {0..<5, 0..<10}, "Locale 0 subdomain incorrect for default locale distribution: %?".format(defaultLocale0));
  var targetLocale0 = targetedArray.localSubdomain();
  assert(targetLocale0 == {0..<10, 0..<5}, "Locale 0 subdomain incorrect for targeted locale distribution: %?".format(targetLocale0));
}

on Locales[1] {
  var defaultLocale1 = defaultArray.localSubdomain();
  assert(defaultLocale1 == {5..<10, 0..<10}, "Locale 1 subdomain incorrect for default locale distribution: %?".format(defaultLocale1));
  var targetLocale1 = targetedArray.localSubdomain();
  assert(targetLocale1 == {0..<10, 5..<10}, "Locale 1 subdomain incorrect for targeted locale distribution: %?".format(targetLocale1));
}

for i in D {
  assert(defaultArray[i] == A[i], "Default locale distribution read incorrect at index %?".format(i));
  assert(targetedArray[i] == A[i], "Targeted locale distribution read incorrect at index %?".format(i));
}
writeln("Pass");