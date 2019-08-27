// Functions to test the atomics API

// TODO Add functions to test non sequentially consistent memory orders:
// - fences
// - memory_orders passed to functions

proc writea( op: string, a)    { writeln("  ",op,"    a=",a); }
proc writeai(op: string, a, i) { writeln("  ",op,"    a=",a,", i=",i); }
proc writeab(op: string, a, b) { writeln("  ",op,"    a=",a,", b=",b); }

proc asyncSet(a) { begin { a.write(true); } }
proc testAtomicBool(a, ref i, ref b) {
  param t = true, f = false;

  writeln("Testing 'atomic bool':");
                                                    writea ("init    ", a);
                var initval : a.type = true;        writeai("init=   ", a, initval);
                var initval2 : a.type = initval;    writeai("init=(v)", a, initval2);
                var initval3 = initval;             writeai("init=(v)", a, initval3);
                assert(initval3.type == atomic bool);
                i = a.read();                       writeai("read    ", a, i);
                    a.write(t);                     writea ("write   ", a);
                i = a.exchange(f);                  writeai("xchg    ", a, i);
                b = a.compareExchange(t, t);        writeab("cmpxchg ", a, b);
                b = a.compareExchange(f, t);        writeab("cmpxchg ", a, b);
                b = a.compareExchangeWeak(f, f);    writeab("cmpxchgW", a, b);
                b = a.compareExchangeWeak(t, f);    writeab("cmpxchgW", a, b);
                b = a.compareExchangeStrong(t, t);  writeab("cmpxchgS", a, b);
                b = a.compareExchangeStrong(f, t);  writeab("cmpxchgS", a, b);
                i = a.testAndSet();                 writeai("test&Set", a, i);
                    a.clear();                      writea ("clear   ", a);
  asyncSet(a);      a.waitFor(true);                writea ("waitFor ", a);
  writeln();
}

proc asyncAdd(a) { begin { a.add(1); } }
proc testAtomicT(a, ref i, ref b, type basetype) {
  param isInt = isIntegral(basetype);

  writeln("Testing 'atomic " + basetype:string + "':");
                                                    writea ("init    ", a);
                if isArray(a) == false {
                  var initval : a.type = 1;         writeai("init=   ", a, initval);
                  var initval2 : a.type = initval;  writeai("init=(v)", a, initval2);
                  var initval3 = initval;           writeai("init=(v)", a, initval3);
                  assert(initval3.type == atomic basetype);
                }
                i = a.read();                       writeai("read    ", a, i);
                    a.write(1);                     writea ("write   ", a);
                i = a.exchange(2);                  writeai("xchg    ", a, i);
                b = a.compareExchange(1, 3);        writeab("cmpxchg ", a, b);
                b = a.compareExchange(2, 3);        writeab("cmpxchg ", a, b);
                b = a.compareExchangeWeak(2, 4);    writeab("cmpxchgW", a, b);
                b = a.compareExchangeWeak(3, 4);    writeab("cmpxchgW", a, b);
                b = a.compareExchangeStrong(3, 5);  writeab("cmpxchgS", a, b);
                b = a.compareExchangeStrong(4, 5);  writeab("cmpxchgS", a, b);
                i = a.fetchAdd(1);                  writeai("fetchAdd", a, i);
                    a.add(1);                       writea ("add     ", a);
                i = a.fetchSub(1);                  writeai("fetchSub", a, i);
                    a.sub(1);                       writea ("sub     ", a);
  if isInt {    i = a.fetchOr(8);                   writeai("fetchOr ", a, i);   }
  if isInt {        a.or(10);                       writea ("or      ", a);      }
  if isInt {    i = a.fetchAnd(7);                  writeai("fetchAnd", a, i);   }
  if isInt {        a.and(5);                       writea ("and     ", a);      }
  if isInt {    i = a.fetchXor(5);                  writeai("fetchXor", a, i);   }
  if isInt {        a.xor(5);                       writea ("xor     ", a);      }
  asyncAdd(a);      a.waitFor(6);                   writea ("waitFor ", a);
  writeln();
}

proc testUnorderedAtomicT(a, ref i, ref b, type basetype) {
  use UnorderedAtomics;
  inline proc fence() { unorderedAtomicTaskFence(); }
  param isInt = isIntegral(basetype);

  writeln("Testing 'atomic " + basetype:string + "':");
                                                    writea ("init    ", a);
                    a.unorderedAdd(8); fence();     writea ("add     ", a);
                    a.unorderedSub(1); fence();     writea ("sub     ", a);
  if isInt {        a.unorderedOr(10); fence();     writea ("or      ", a); }
  if isInt {        a.unorderedAnd(5); fence();     writea ("and     ", a); }
  if isInt {        a.unorderedXor(5); fence();     writea ("xor     ", a); }
  writeln();
}
