extern proc chpl_task_sleep(t:uint):void;
proc slp(msg:string) { chpl_task_sleep(2); writeln(msg, " done\n"); }

writeln();
testBO(); slp("testBO");
testOB(); slp("testOB");
testBwO(); slp("testBwO");
testOwB(); slp("testOwB");

proc testBO() {
  writeln("testBO start");
  var D = {1..3};
  writeln("testBO before begin");
  begin on Locales[numLocales-1] {
    chpl_task_sleep(1);
    writeln(D);
    writeln("testBO finished begin");
  }
  writeln("testBO after begin");
}

proc testOB() {
  writeln("testOB start");
  var D = {1..3};
  writeln("testOB before begin");
  on Locales[numLocales-1] do begin {
    chpl_task_sleep(1);
    writeln(D);
    writeln("testOB finished begin");
  }
  writeln("testOB after begin");
}

proc testBwO() {
  writeln("testBwO start");
  var D = {1..3};
  writeln("testBwO before begin");
  begin { writeln(); on Locales[numLocales-1] {
    writeln(D);
    writeln("testBwO finished begin");
  }}
  chpl_task_sleep(1);
  writeln("testBwO after begin");
  writeln(D);
}

// currently works correctly
proc testOwB() {
  writeln("testOwB start");
  var D = {1..3};
  writeln("testOwB before begin");
  on Locales[numLocales-1] { writeln(); begin {
    writeln(D);
    writeln("testOwB finished begin");
  }}
  chpl_task_sleep(1);
  writeln("testOwB after begin");
  writeln(D);
}
