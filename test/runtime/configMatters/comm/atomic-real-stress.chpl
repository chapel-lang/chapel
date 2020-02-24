// Stress some floating point operations that we know have tricky
// implementations on some platforms/configurations.

use Time;

config const numTasks = here.maxTaskPar;
config const numIters = 50_000;
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
inline proc AtomicT.mult(multiplier: T, param exchangeType: ExchangeType) {
  var oldValue = this.read();
  select (exchangeType) {
    when ExchangeType.cmpxchgW {
      while !this.compareExchangeWeak(oldValue, oldValue * multiplier) { }
    }
    when ExchangeType.cmpxchg {
      while !this.compareExchange(oldValue, oldValue * multiplier) { }
    }
    when ExchangeType.cas {
      while !this.compareAndSwap(oldValue, oldValue * multiplier) {
        oldValue = this.read();
      }
    }
  }
}

inline proc RAtomicT.mult(multiplier: T, param exchangeType: ExchangeType) {
  var oldValue = this.read();
  select (exchangeType) {
    when ExchangeType.cmpxchgW {
      while !this.compareExchangeWeak(oldValue, oldValue * multiplier) { }
    }
    when ExchangeType.cmpxchg {
      while !this.compareExchange(oldValue, oldValue * multiplier) { }
    }
    when ExchangeType.cas {
      while !this.compareAndSwap(oldValue, oldValue * multiplier) {
        oldValue = this.read();
      }
    }
  }
}

proc testMult(type t, param exchangeType: ExchangeType) {
  const timesVal:t = (1.0 + 1.0/numItersActual:real):t;
  var a: atomic t = 1.0;
  coforall loc in Locales do on loc do
    coforall 1..numTasks do
      for 1..numItersPerTask do
        a.mult(timesVal, exchangeType);

  var aSer: t = 1.0;
  for 1..numItersActual do aSer *= timesVal;

  assert(isclose(a.read(), aSer));
}

var t: Timer; t.start();

testAddSub(real(32));
testAddSub(real(64));
if printTimers { writeln("addSub   ", t.elapsed()); t.clear(); }

testMult(real(32), ExchangeType.cmpxchgW);
testMult(real(64), ExchangeType.cmpxchgW);
if printTimers { writeln("cmpxchgW ", t.elapsed()); t.clear(); }

testMult(real(32), ExchangeType.cmpxchg);
testMult(real(64), ExchangeType.cmpxchg);
if printTimers { writeln("cmpxchgW ", t.elapsed()); t.clear(); }

testMult(real(32), ExchangeType.cas);
testMult(real(64), ExchangeType.cas);
if printTimers { writeln("cmpxchgW ", t.elapsed()); t.clear(); }
