module taskpool {

use blockindices;

class taskpool {
  const poolSize;
  var taskarr : [0..poolSize-1] sync unmanaged blockIndices?;
  var head, tail : sync int = 0;
  proc add(blk: unmanaged blockIndices?) {
    const pos = tail.readFE();
    tail.writeEF((pos+1)%poolSize);
    taskarr(pos).writeEF(blk);
  }
  proc remove() {
    const pos = head.readFE();
    head.writeEF((pos+1)%poolSize);
    return taskarr(pos).readFE();
  }
}
}
