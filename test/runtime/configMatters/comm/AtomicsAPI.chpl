// Functions to test the atomics API

// TODO Add functions to test non sequentially consistent memory orders:
// - fences
// - non seqCst memory_orders passed to atomic methods

proc writea  (op: string, a)       { writeln("  ",op,"    a=",a); }
proc writeai (op: string, a, i)    { writeln("  ",op,"    a=",a,", i=",i); }
proc writeab (op: string, a, b)    { writeln("  ",op,"    a=",a,", b=",b); }
proc writex  (op: string, x)       { writeln("  ",op,"    x=",x); }
proc writeabx(op: string, a, b, x) { writeln("  ",op,"    a=",a,", b=",b,", x=",x); }

// !b (where b could be an array)
proc n(b) {
  if isArray(b) { for e in b do if !e then return true; return false; }
  else { return !b; }
}

proc asyncSet(a) { begin { a.write(true); } }
proc testAtomicBool(a, ref i, ref b) {
  param t = true, f = false;

  writeln("Testing 'atomic bool':");
                                                                writea  ("init    ", a);
                var initval : a.type = true;                    writeai ("init=   ", a, initval);
                var initval2 : a.type = initval;                writeai ("init=(v)", a, initval2);
                var initval3 = initval;                         writeai ("init=(v)", a, initval3);
                assert(initval3.type == atomic bool);
                i = a.read();                                   writeai ("read    ", a, i);
                    a.write(t);                                 writea  ("write   ", a);
                i = a.exchange(f);                              writeai ("xchg    ", a, i);
                var x = true;                                   writex  ("expected", x);
                b = a.compareExchange(x, t);                    writeabx("cmpxchg ", a, b, x);
                b = a.compareExchange(x, t);                    writeabx("cmpxchg ", a, b, x);
                b = a.compareExchangeWeak(x, f);                writeabx("cmpxchgW", a, b, x);
             do b = a.compareExchangeWeak(x, f); while n(b);    writeabx("cmpxchgW", a, b, x);
                b = a.compareAndSwap(t, t);                     writeab ("cas     ", a, b);
                b = a.compareAndSwap(f, t);                     writeab ("cas     ", a, b);
                i = a.testAndSet();                             writeai ("test&Set", a, i);
                    a.clear();                                  writea  ("clear   ", a);
  asyncSet(a);      a.waitFor(true);                            writea  ("waitFor ", a);
  writeln();
}

proc testOrderAtomicBool(a, ref i, ref b, param o: memoryOrder) {
  param t = true, f = false;

  writeln("Testing 'atomic bool' with order '", o, "':");
                                                                     writea  ("init    ", a);
                var initval : a.type = true;                         writeai ("init=   ", a, initval);
                var initval2 : a.type = initval;                     writeai ("init=(v)", a, initval2);
                var initval3 = initval;                              writeai ("init=(v)", a, initval3);
                assert(initval3.type == atomic bool);
                i = a.read(o);                                       writeai ("read    ", a, i);
                    a.write(t, o);                                   writea  ("write   ", a);
                i = a.exchange(f, o);                                writeai ("xchg    ", a, i);
                var x = true;                                        writex  ("expected", x);
                b = a.compareExchange(x, t, o);                      writeabx("cmpxchg ", a, b, x);
                b = a.compareExchange(x, t, o);                      writeabx("cmpxchg ", a, b, x);
                b = a.compareExchange(x, f, o, o);                   writeabx("cmpxchg ", a, b, x);
                b = a.compareExchange(x, f, o, o);                   writeabx("cmpxchg ", a, b, x);
                b = a.compareExchangeWeak(x, t, o);                  writeabx("cmpxchgW", a, b, x);
             do b = a.compareExchangeWeak(x, t, o); while n(b);      writeabx("cmpxchgW", a, b, x);
                b = a.compareExchangeWeak(x, f, o, o);               writeabx("cmpxchgW", a, b, x);
             do b = a.compareExchangeWeak(x, f, o, o); while n(b);   writeabx("cmpxchgW", a, b, x);
                b = a.compareAndSwap(t, t, o);                       writeab ("cas     ", a, b);
                b = a.compareAndSwap(f, t, o);                       writeab ("cas     ", a, b);
                i = a.testAndSet(o);                                 writeai ("test&Set", a, i);
                    a.clear(o);                                      writea  ("clear   ", a);
  asyncSet(a);      a.waitFor(true, o);                              writea  ("waitFor ", a);
  writeln();
}

proc asyncAdd(a) { begin { a.add(1); } }
proc testAtomicT(a, ref i, ref b, type basetype) {
  param isInt = isIntegral(basetype);
  type xType = if isArray(a) then [a.domain] basetype else basetype;

  writeln("Testing 'atomic ", basetype:string, "':");
                                                                writea  ("init    ", a);
                if isArray(a) == false {
                  var initval : a.type = 1;                     writeai ("init=   ", a, initval);
                  var initval2 : a.type = initval;              writeai ("init=(v)", a, initval2);
                  var initval3 = initval;                       writeai ("init=(v)", a, initval3);
                  assert(initval3.type == atomic basetype);     
                }
                i = a.read();                                   writeai ("read    ", a, i);
                    a.write(1);                                 writea  ("write   ", a);
                i = a.exchange(2);                              writeai ("xchg    ", a, i);
                var x: xType = 3:basetype;                      writex  ("expected", x);
                b = a.compareExchange(x, 4);                    writeabx("cmpxchg ", a, b, x);
                b = a.compareExchange(x, 4);                    writeabx("cmpxchg ", a, b, x);
                b = a.compareExchangeWeak(x, 2);                writeabx("cmpxchgW", a, b, x);
             do b = a.compareExchangeWeak(x, 2); while n(b);    writeabx("cmpxchgW", a, b, x);
                b = a.compareAndSwap(1, 5);                     writeab ("cas     ", a, b);
                b = a.compareAndSwap(2, 5);                     writeab ("cas     ", a, b);
                i = a.fetchAdd(1);                              writeai ("fetchAdd", a, i);
                    a.add(1);                                   writea  ("add     ", a);
                i = a.fetchSub(1);                              writeai ("fetchSub", a, i);
                    a.sub(1);                                   writea  ("sub     ", a);
  if isInt {    i = a.fetchOr(8);                               writeai ("fetchOr ", a, i);   }
  if isInt {        a.or(10);                                   writea  ("or      ", a);      }
  if isInt {    i = a.fetchAnd(7);                              writeai ("fetchAnd", a, i);   }
  if isInt {        a.and(5);                                   writea  ("and     ", a);      }
  if isInt {    i = a.fetchXor(5);                              writeai ("fetchXor", a, i);   }
  if isInt {        a.xor(5);                                   writea  ("xor     ", a);      }
  asyncAdd(a);      a.waitFor(6);                               writea  ("waitFor ", a);
  writeln();
}

proc testOrderAtomicT(a, ref i, ref b, type basetype, param o: memoryOrder) {
  param isInt = isIntegral(basetype);
  type xType = if isArray(a) then [a.domain] basetype else basetype;

  writeln("Testing 'atomic ", basetype:string, "' with order '", o, "':");
                                                                     writea  ("init    ", a);
                if isArray(a) == false {
                  var initval : a.type = 1;                          writeai ("init=   ", a, initval);
                  var initval2 : a.type = initval;                   writeai ("init=(v)", a, initval2);
                  var initval3 = initval;                            writeai ("init=(v)", a, initval3);
                  assert(initval3.type == atomic basetype);     
                }
                i = a.read(o);                                       writeai ("read    ", a, i);
                    a.write(1, o);                                   writea  ("write   ", a);
                i = a.exchange(2, o);                                writeai ("xchg    ", a, i);
                var x: xType = 3:basetype;                           writex  ("expected", x);
                b = a.compareExchange(x, 4, o);                      writeabx("cmpxchg ", a, b, x);
                b = a.compareExchange(x, 4, o);                      writeabx("cmpxchg ", a, b, x);
                b = a.compareExchange(x, 2, o, o);                   writeabx("cmpxchg ", a, b, x);
                b = a.compareExchange(x, 2, o, o);                   writeabx("cmpxchg ", a, b, x);
                b = a.compareExchangeWeak(x, 4, o, o);               writeabx("cmpxchgW", a, b, x);
             do b = a.compareExchangeWeak(x, 4, o, o); while n(b);   writeabx("cmpxchgW", a, b, x);
                b = a.compareExchangeWeak(x, 2, o);                  writeabx("cmpxchgW", a, b, x);
             do b = a.compareExchangeWeak(x, 2, o); while n(b);      writeabx("cmpxchgW", a, b, x);
                b = a.compareAndSwap(1, 5, o);                       writeab ("cas     ", a, b);
                b = a.compareAndSwap(2, 5, o);                       writeab ("cas     ", a, b);
                i = a.fetchAdd(1, o);                                writeai ("fetchAdd", a, i);
                    a.add(1, o);                                     writea  ("add     ", a);
                i = a.fetchSub(1, o);                                writeai ("fetchSub", a, i);
                    a.sub(1, o);                                     writea  ("sub     ", a);
  if isInt {    i = a.fetchOr(8, o);                                 writeai ("fetchOr ", a, i);   }
  if isInt {        a.or(10, o);                                     writea  ("or      ", a);      }
  if isInt {    i = a.fetchAnd(7, o);                                writeai ("fetchAnd", a, i);   }
  if isInt {        a.and(5, o);                                     writea  ("and     ", a);      }
  if isInt {    i = a.fetchXor(5, o);                                writeai ("fetchXor", a, i);   }
  if isInt {        a.xor(5, o);                                     writea  ("xor     ", a);      }
  asyncAdd(a);      a.waitFor(6, o);                                 writea  ("waitFor ", a);
  writeln();
}

proc testUnorderedAtomicT(a, ref i, ref b, type basetype) {
  use UnorderedAtomics;
  inline proc fence() { unorderedAtomicTaskFence(); }
  param isInt = isIntegral(basetype);

  writeln("Testing 'atomic ", basetype:string, "':");
                                                    writea ("init    ", a);
                    a.unorderedAdd(8); fence();     writea ("add     ", a);
                    a.unorderedSub(1); fence();     writea ("sub     ", a);
  if isInt {        a.unorderedOr(10); fence();     writea ("or      ", a); }
  if isInt {        a.unorderedAnd(5); fence();     writea ("and     ", a); }
  if isInt {        a.unorderedXor(5); fence();     writea ("xor     ", a); }
  writeln();
}
