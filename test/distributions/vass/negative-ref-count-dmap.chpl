
use BlockDist;

const d1 = {1..4};

proc main() {
  var bd1 = new unmanaged Block(d1);
  var dm1 = new dmap(bd1);

  if bd1.pid >= -1 then  // dereference bd1, doesn't matter how
    writeln("test: bd1 dereference OK");

  // we want 'dm1' to wrap 'bd1' directoy
  if bd1 == dm1._value then
    writeln("test: bd1 wrap OK");
}
