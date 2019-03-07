// This test verifies that the follow pattern
// of paren-less type and value methods returning param works.

// For easier test verification, it doesn't print out the real
// epsilon.

proc type (real(64)).epsilon param return 0x1.0p-02:real(64);
proc (real(64)).epsilon param return 0x1.0p-02:real(64);
proc type (real(32)).epsilon param return 0x1.0p-01:real(32);
proc (real(32)).epsilon param return 0x1.0p-01:real(32);

var v64: real(64);
var v32: real(32);
param et64 = real(64).epsilon;
param ev64 = v64.epsilon;
param et32 = real(32).epsilon;
param ev32 = v32.epsilon;
writeln(et64);
writeln(ev64);
writeln(et32);
writeln(ev32);
