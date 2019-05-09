var D = {1..10};
var A: [D] int;

var resized: atomic bool;

ref As = A[D];

sync  {
  begin on Locales[numLocales-1]{
    while (!resized.read()) { chpl_task_yield(); }
    As[11] = 1;
    writeln(As.domain);
    resized.write(false);
  }
  D = {11..20};
  resized.write(true);
  while (resized.read()) { chpl_task_yield(); }
  writeln(As);
}
