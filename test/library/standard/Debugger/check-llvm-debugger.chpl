use Debugger;

proc begin_print() {}
proc end_print() {}
proc begin_loop() {}
proc end_loop() {}

proc main() {
  // CHECK: @begin_print
  // CHECK: @llvm.debugtrap
  // CHECK: @writeln
  // CHECK: @llvm.debugtrap
  // CHECK: @writeln
  // CHECK: @end_print
  begin_print();
  breakpoint;
  writeln("hello");
  breakpoint;
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
    breakpoint;
    writeln("Elm ", a);
    end_loop();
  }
}
