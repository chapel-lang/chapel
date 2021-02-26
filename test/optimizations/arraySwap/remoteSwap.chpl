var D = {1..10, 1..10};
var A: [D] int = 1;
on Locales[numLocales-1] {
  var B: [D] int = 2;
  A <=> B;
}
writeln(A);
