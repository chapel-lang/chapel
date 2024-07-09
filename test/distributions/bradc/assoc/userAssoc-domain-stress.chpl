use HashedDist;
use Random;

config const requestCapacity = true;
config const updatesPerLocale = 10000;

var D: domain(int) dmapped new hashedDist(idxType=int);

if requestCapacity then
  D.requestCapacity(updatesPerLocale*numLocales);

coforall loc in Locales with (ref D) {
  on loc {
    var rng = new randomStream(eltType=int, seed=100+loc.id);
    forall r in rng.next({1..updatesPerLocale}) with (ref D) {
      D += r;
    }
  }
}

// Now check that each update is present!
coforall loc in Locales {
  on loc {
    var rng = new randomStream(eltType=int, seed=100+loc.id);
    forall r in rng.next({1..updatesPerLocale}) {
      assert(D.contains(r));
    }
  }
}
