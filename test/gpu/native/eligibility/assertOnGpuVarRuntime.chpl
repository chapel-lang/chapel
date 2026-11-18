@assertOnGpu
var A = foreach i in 1..100 do i;

writeln("!!! This message should not be displayed unless there's a bug in");
writeln("    assertOnGpu.");
