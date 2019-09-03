// Functions to test the atomics API

// TODO Add functions to test non sequentially consistent memory orders:
// - fences
// - non seqCst memory_orders passed to atomic methods

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
                b = a.compareAndSwap(t, t);         writeab("cas     ", a, b);
                b = a.compareAndSwap(f, t);         writeab("cas     ", a, b);
                i = a.testAndSet();                 writeai("test&Set", a, i);
                    a.clear();                      writea ("clear   ", a);
  asyncSet(a);      a.waitFor(true);                writea ("waitFor ", a);
  writeln();
}

proc testOrderAtomicBool(a, ref i, ref b, param order: memoryOrder) {
  param t = true, f = false;

  writeln("Testing 'atomic bool' with order '", order, "':");
                                                           writea ("init    ", a);
                var initval : a.type = true;               writeai("init=   ", a, initval);
                var initval2 : a.type = initval;           writeai("init=(v)", a, initval2);
                var initval3 = initval;                    writeai("init=(v)", a, initval3);
                assert(initval3.type == atomic bool);
                i = a.read(order);                         writeai("read    ", a, i);
                    a.write(t, order);                     writea ("write   ", a);
                i = a.exchange(f, order);                  writeai("xchg    ", a, i);
                b = a.compareAndSwap(t, t, order);         writeab("cas     ", a, b);
                b = a.compareAndSwap(f, t, order);         writeab("cas     ", a, b);
                i = a.testAndSet(order);                   writeai("test&Set", a, i);
                    a.clear(order);                        writea ("clear   ", a);
  asyncSet(a);      a.waitFor(true, order);                writea ("waitFor ", a);
  writeln();
}

proc asyncAdd(a) { begin { a.add(1); } }
proc testAtomicT(a, ref i, ref b, type basetype) {
  param isInt = isIntegral(basetype);

  writeln("Testing 'atomic ", basetype:string, "':");
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
                b = a.compareAndSwap(1, 5);         writeab("cas     ", a, b);
                b = a.compareAndSwap(2, 5);         writeab("cas     ", a, b);
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

proc testOrderAtomicT(a, ref i, ref b, type basetype, param order: memoryOrder) {
  param isInt = isIntegral(basetype);

  writeln("Testing 'atomic ", basetype:string, "' with order '", order, "':");
                                                           writea ("init    ", a);
                if isArray(a) == false {
                  var initval : a.type = 1;                writeai("init=   ", a, initval);
                  var initval2 : a.type = initval;         writeai("init=(v)", a, initval2);
                  var initval3 = initval;                  writeai("init=(v)", a, initval3);
                  assert(initval3.type == atomic basetype);
                }
                i = a.read(order);                         writeai("read    ", a, i);
                    a.write(1, order);                     writea ("write   ", a);
                i = a.exchange(2, order);                  writeai("xchg    ", a, i);
                b = a.compareAndSwap(1, 5, order);         writeab("cas     ", a, b);
                b = a.compareAndSwap(2, 5, order);         writeab("cas     ", a, b);
                i = a.fetchAdd(1, order);                  writeai("fetchAdd", a, i);
                    a.add(1, order);                       writea ("add     ", a);
                i = a.fetchSub(1, order);                  writeai("fetchSub", a, i);
                    a.sub(1, order);                       writea ("sub     ", a);
  if isInt {    i = a.fetchOr(8, order);                   writeai("fetchOr ", a, i);   }
  if isInt {        a.or(10, order);                       writea ("or      ", a);      }
  if isInt {    i = a.fetchAnd(7, order);                  writeai("fetchAnd", a, i);   }
  if isInt {        a.and(5, order);                       writea ("and     ", a);      }
  if isInt {    i = a.fetchXor(5, order);                  writeai("fetchXor", a, i);   }
  if isInt {        a.xor(5, order);                       writea ("xor     ", a);      }
  asyncAdd(a);      a.waitFor(6, order);                   writea ("waitFor ", a);
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
