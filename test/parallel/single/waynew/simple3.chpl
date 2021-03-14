use Time;

// Test multiple threads waiting at the same time.

var go: single int;

proc jam( i: int) {
  writeln( i, " Pizza!");
}

begin jam( go.readFF());
begin jam( go.readFF());
begin jam( go.readFF());
begin jam( go.readFF());
begin jam( go.readFF());

writeln( "I'm sleepy");
sleep( 3);
writeln( "I'm awake");
go.writeEF(9);
