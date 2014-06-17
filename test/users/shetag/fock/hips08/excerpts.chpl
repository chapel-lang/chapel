
/*-------------------------------------------------
Code 5 Task pool of integral blocks - Chapel
-------------------------------------------------*/
class taskpool {
  const poolSize;
  var taskarr : [0..poolSize-1] sync blockIndices;
  var head, tail : sync int = 0;
  proc add(bI) {
    const pos = tail;
    tail = (pos+1)%poolSize;
    taskarr(pos) = bI;
  }
  proc remove() {
    const pos = head;
    head = (pos+1)%poolSize;
    return taskarr(pos);
  }
}

/*-------------------------------------------------
Code 6 Top-level driver for task pool - Chapel
-------------------------------------------------*/
config const numLocs = 10;
config const poolSize = 10;
const t = new taskpool(poolSize);
cobegin {
  coforall loc in 1..numLocs do
    consumer();
  producer();
}


/*-------------------------------------------------
Code 7 Producer of integral blocks - Chapel
-------------------------------------------------*/
proc producer() {
  forall bI in genBlocks() do
    t.add(bI);
}


/*-------------------------------------------------
Code 8 Fock index space iterator - Chapel
-------------------------------------------------*/
proc genBlocks() {
  forall iat in 1..natom do
    forall jat in 1..iat do
      forall kat in 1..iat {
        const lattop = if (kat==iat) then jat
        else kat;
        forall lat in 1..lattop do
          yield blockIndices(...);
      }
  forall loc in 1..numLocs do
    yield blockIndices(0,0,0,0,0,0,0,0);
}


/*-------------------------------------------------
Code 9 Consumer of integral blocks - Chapel
-------------------------------------------------*/
proc consumer() {
  var bI, copyofbI : blockIndices;
  bI = t.remove();
  while (bI.ilo != 0) {
    copyofbI = bI;
    cobegin {
      buildjk_atom4(copyofbI);
      bI = t.remove();
    }
  }
}


/*-------------------------------------------------
Code 10 Symmetrization of J and K - Chapel
-------------------------------------------------*/

cobegin {
  [(i,j) in D] jmat2T(i,j) = jmat2(j,i);
  [(i,j) in D] kmat2T(i,j) = kmat2(j,i);
}
jmat2 = (jmat2+jmat2T)*2;
kmat2 += kmat2T;
