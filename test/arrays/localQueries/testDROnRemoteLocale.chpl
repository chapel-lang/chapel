// Note that this domain will be broadcated and serialized if the
// --remote-serialization optimization is enabled.
var D = {1..10};
var A: [D] real;

testit("testing D from locale 0", D);
testit("testing A from locale 0", A);
on Locales[numLocales-1] {
  testit("testing D from locale "+ (numLocales-1):string, D);
  testit("testing A from locale "+ (numLocales-1):string, A);

  var B: [D] real;
  testit("testing B from locale "+ (numLocales-1):string, B);

  on Locales[0] {
    testit("testing B from locale 0", B);
  }
}

proc testit(lbl, obj) {
  writeln(lbl);
  writeln("targetLocales = ", obj.targetLocales());
  if obj.hasSingleLocalSubdomain() then
    writeln("local subdomain = ", obj.localSubdomain());
  write("local subdomains =");
  for sbd in obj.localSubdomains() do
    write(" ", sbd);
  writeln();
  writeln();
}
