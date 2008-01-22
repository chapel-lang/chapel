
/*-------------------------------------------------
Code 5 Task pool of integral blocks - Chapel
-------------------------------------------------*/
class taskpool {
  const poolSize;
  var taskarr : [0..poolSize-1] sync blockIndices; // describe blockIndices?
  var head, tail : sync int = 0;
  def add(blk: blockIndices) {
    const pos = tail;
    tail = (pos+1)%poolSize;
    taskarr(pos) = blk;
  }
  def remove() {
    const pos = head;
    head = (pos+1)%poolSize;
    return taskarr(pos);
  }
}

/*-------------------------------------------------
Code 6 Top-level driver for task pool - Chapel
-------------------------------------------------*/
config const numConsumers = numLocales * Locales(0).numCores,
             poolSize = numConsumers;
const t = taskpool(poolSize);
cobegin {
  coforall loc in 1..numConsumers do
    consumer();
  producer();
}


/*-------------------------------------------------
Code 7 Producer of integral blocks - Chapel
-------------------------------------------------*/
def producer() {
  forall blk in genBlocks() do
    t.add(blk);
}


/*-------------------------------------------------
Code 8 Fock index space iterator - Chapel
-------------------------------------------------*/
def genBlocks() {
  forall (iat, jat, kat) in [1..natom, 1..iat, 1..iat] {
    const lattop = if (kat==iat) then jat
                                 else kat;
    forall lat in 1..lattop do
      yield blockIndices(...);  // what is "..."?
  }
  forall loc in 1..numConsumers do
    yield blockIndices(0,0,0,0,0,0,0,0); // should/could this be four ranges?
}


/*-------------------------------------------------
Code 9 Consumer of integral blocks - Chapel
-------------------------------------------------*/
def consumer() {
  var blk = t.remove();
  while (blk.ilo != 0) {
    const copyofblk = blk;
    cobegin {
      buildjk_atom4(copyofblk);
      blk = t.remove();
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
