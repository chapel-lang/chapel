use Debugger;

proc begin_print() { return 1; }
proc end_print() { return 2; }
proc begin_loop() { return 3; }
proc end_loop() { return 4; }

proc main() {
  // CHECK: @begin_print
  // CHECK: @llvm.debugtrap
  // CHECK: @writeln
  // CHECK: @llvm.debugtrap
  // CHECK: @writeln
  // CHECK: @end_print
  begin_print();
  debugTrap;
  writeln("hello");
  debugTrap;
  writeln("world");
  end_print();

  var A: [1..10] int = 1..10;

  var a: int;
  for i in A.domain {
    // CHECK: @begin_loop
    // CHECK: @llvm.debugtrap
    // CHECK: @end_loop
    begin_loop();
    a = A[i];
    debugTrap;
    writeln("Elm ", a);
    end_loop();
  }
}
