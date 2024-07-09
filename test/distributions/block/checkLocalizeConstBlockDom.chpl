use BlockDist;

const D = {1..10} dmapped new blockDist({1..10});

var A: [D] real;

on Locales[numLocales-1] {
  // This should not cause the localization optimization to fire since
  // 'A' is distributed
  var B = A;
  writeln(B);
}
