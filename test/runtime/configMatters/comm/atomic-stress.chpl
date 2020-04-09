// Stress some atomic operations that we know have tricky
// implementations on some platforms/configurations.

use Time;

config const numTasks = here.maxTaskPar;
const locAtomicDflt = 50_000;
const netAtomicDflt = 10_000;
const procAtomicDflt = 1_000;
const remAtomicDflt = if CHPL_NETWORK_ATOMICS != "none" then netAtomicDflt else procAtomicDflt;
config const numIters = if numLocales == 1 then locAtomicDflt else remAtomicDflt;
config const numItersPerTask = numIters / numLocales / numTasks;
const numItersActual = numItersPerTask * numLocales * numTasks;

config const printTimers = false;

proc testAddSub(type t) {
  var a: atomic t;

  coforall loc in Locales do on loc do
    coforall 1..numTasks do
      for 1..numItersPerTask do
        a.add(1:t);

  coforall loc in Locales do on loc do
    coforall 1..numTasks do
      for 1..numItersPerTask do
        a.sub(1:t);
  assert(isclose(a.read(), 0:t));
}

enum ExchangeType {cmpxchg, cmpxchgW, cas};
inline proc AtomicT.loopAdd(value: T, param exchangeType: ExchangeType) {
  var oldValue = this.read();
  select (exchangeType) {
    when ExchangeType.cmpxchgW {
      while !this.compareExchangeWeak(oldValue, oldValue + value) { }
    }
    when ExchangeType.cmpxchg {
      while !this.compareExchange(oldValue, oldValue + value) { }
    }
    when ExchangeType.cas {
      while !this.compareAndSwap(oldValue, oldValue + value) {
        oldValue = this.read();
      }
    }
  }
}
inline proc RAtomicT.loopAdd(value: T, param exchangeType: ExchangeType) {
  var oldValue = this.read();
  select (exchangeType) {
    when ExchangeType.cmpxchgW {
      while !this.compareExchangeWeak(oldValue, oldValue + value) { }
    }
    when ExchangeType.cmpxchg {
      while !this.compareExchange(oldValue, oldValue + value) { }
    }
    when ExchangeType.cas {
      while !this.compareAndSwap(oldValue, oldValue + value) {
        oldValue = this.read();
      }
    }
  }
}

proc testLoopAdd(type t, param exchangeType: ExchangeType) {
  var a: atomic t;

  coforall loc in Locales do on loc do
    coforall 1..numTasks do
      for 1..numItersPerTask do
        a.loopAdd(1:t, exchangeType);

  coforall loc in Locales do on loc do
    coforall 1..numTasks do
      for 1..numItersPerTask do
        a.loopAdd((-1):t, exchangeType);
  assert(isclose(a.read(), 0:t));
}

proc testLoopAdd(type t) {
  var timer: Timer; timer.start();
  testLoopAdd(t, ExchangeType.cmpxchgW);
  if printTimers { writef("cmpxchgW %8s: %.2dr\n", t:string, timer.elapsed()); timer.clear(); }

  testLoopAdd(t, ExchangeType.cmpxchg);
  if printTimers { writef("cmpxchg  %8s: %.2dr\n", t:string, timer.elapsed()); timer.clear(); }

  testLoopAdd(t, ExchangeType.cas);
  if printTimers { writef("cas      %8s: %.2dr\n\n", t:string, timer.elapsed()); timer.clear(); }
}


// Stress atomic real add/sub
testAddSub(real(32));
testAddSub(real(64));

// Stress compareExchange/compareAndSwap for numeric types
testLoopAdd(int(8));
testLoopAdd(int(16));
testLoopAdd(int(32));
testLoopAdd(int(64));

testLoopAdd(uint(8));
testLoopAdd(uint(16));
testLoopAdd(uint(32));
testLoopAdd(uint(64));

testLoopAdd(real(32));
testLoopAdd(real(64));
