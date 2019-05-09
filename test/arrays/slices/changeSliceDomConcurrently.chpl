var D = {1..10};
var A: [D] int;

var resized: atomic bool;

sync  {
  begin on Locales[numLocales-1]{
    while (!resized.read()) { chpl_task_yield(); }
    A[11] = 1;
    writeln(A.domain);
  }
  D = {11..20};
  resized.write(true);
  writeln(A);
}
