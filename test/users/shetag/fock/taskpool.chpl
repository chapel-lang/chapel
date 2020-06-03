
class blockIndices {
  const ilo, ihi, jlo, jhi, klo, khi, llo, lhi : int;
}

class taskpool {
  const poolSize;
  var taskarr : [0..poolSize-1] sync unmanaged blockIndices?;
  var head, tail : sync int = 0;

  proc add(bI) {
    const pos = tail;
    tail = (pos+1)%poolSize;
    taskarr(pos) = bI;
  }

  proc remove() {
    const pos = head;
    head = (pos+1)%poolSize;
    const bI = taskarr(pos);
    return bI;
  }
}
