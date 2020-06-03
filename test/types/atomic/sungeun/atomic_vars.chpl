config const printResults = false;
param mo = memoryOrder.seqCst;

proc doit(type myType) {
  var ax: atomic myType;
  var x = ax.read();

  if printResults then writeln(x);

  if numBits(x.type) != numBits(myType) then
    writeln(myType:string, ": ERROR: numBits=", numBits(x.type),
            " (should be ", numBits(myType), ")");
  if printResults then writeln(numBits(x.type));

  on Locales[numLocales-1] do ax.write(min(myType), mo);
  if ax.read(mo) != min(myType) then
    writeln(myType:string, ": ERROR: (read) ax=", ax.read(mo),
            " (should be ", min(myType), ")");
  if printResults then writeln(ax.read(mo));

  for i in 1..99 {
    var a = if i%100 then max(myType) else 0:myType;
    ax.exchange(a, mo);
  }
  if printResults then writeln(ax.read(mo));
  if ax.read() != max(myType) then
    writeln(myType:string, ": ERROR: (exchange) ax=", ax.read(mo),
            " (should be ", max(myType), ")");

  ax.write(0:myType, mo);
  coforall i in 1..15 do on Locales[i%numLocales] { // 15 is max for int(8)
      var a = (ax.fetchAdd(i:myType, mo)+i:myType):myType;
      ax.compareAndSwap(a, a, mo);
    }
  x = ax.read(mo);
  if x != 120:myType then
    writeln(myType:string, ": ERROR: (fetchAdd/compareAndSwap) x=", x, " (should be 120)");
  if printResults then writeln(x);

  var b: atomic bool;
  ax.write(0:myType, mo);
  begin {
    ax.waitFor(120:myType);
    const x = ax.read(mo);
    if x != 120:myType then
      writeln(myType:string, ": ERROR: (waitFor/add) x=", x, " (should be 120)");
    if printResults then writeln(x);
    b.testAndSet();
  }
  coforall i in 1..15 do on Locales[i%numLocales] { // 15 is max for int(8)
      ax.add(i:myType, mo);
    }
  b.waitFor(true);

  b.clear();
  begin {
    ax.waitFor(0:myType);
    const x = ax.read(mo);
    if x != 0:myType then
      writeln(myType:string, ": ERROR: (waitFor/sub) x=", x, " (should be 0)");
    if printResults then writeln(x);
    b.testAndSet();
  }
  coforall i in 1..15 do on Locales[i%numLocales] { // 15 is max for int(8)
      ax.sub(i:myType, mo);
    }
  b.waitFor(true);

  if isIntegralType(myType) {
    ax.write(max(int(8)):myType, mo);
    b.clear();
    begin {
      ax.waitFor(0:myType);
      const x = ax.read(mo);
      if x != 0:myType then
        writeln(myType:string, ": ERROR: (waitFor/and) x=", x, " (should be 1)");
      if printResults then writeln(x);
      b.testAndSet();
    }
    coforall i in 0..6 do on Locales[i%numLocales] { // 7 is max for int(8)
        const v = (max(int(8))^(1<<i)):myType;
        ax.and(v, mo);
      }
    b.waitFor(true);

    ax.write(0:myType, mo);
    b.clear();
    begin {
      ax.waitFor(max(int(8)):myType);
      const x = ax.read(mo);
      if x != max(int(8)):myType then
        writeln(myType:string, ": ERROR: (waitFor/or) x=", x, " (should be ", max(int(8)), ")");
      if printResults then writeln(x);
      b.testAndSet();
    }
    coforall i in 0..6 do on Locales[i%numLocales] { // 7 is max for int(8)
        const v = (1<<i):myType;
        ax.or(v, mo);
      }
    b.waitFor(true);

    ax.write(0:myType, mo);
    b.clear();
    begin {
      ax.waitFor(max(int(8)):myType);
      const x = ax.read(mo);
      if x != max(int(8)):myType then
        writeln(myType:string, ": ERROR: (waitfOr/xor) x=", x, " (should be ", max(int(8)), ")");
      if printResults then writeln(x);
      b.testAndSet();
    }
    coforall i in 0..6 do on Locales[i%numLocales] { // 7 is max for int(8)
        const v = (1<<i):myType;
        ax.xor(v, mo);
      }
    b.waitFor(true);
  }

  const D = {7..39};
  var aA: [D] atomic myType;

  for i in D do aA[i].write(i:myType, mo);
  var A = [a in aA] a.read(mo);
  for i in D {
    if A[i] != i:myType then
      writeln(myType:string, ": ERROR: A[", i, "]=",
              A[i], " (should be ", i, ")");
  }
  if printResults then writeln(A);

  for i in D do aA[i].write((i+1):myType);
  A = aA.read(mo);
  for i in D {
    if A[i] != (i+1):myType then
      writeln(myType:string, ": ERROR: A[", i, "]=",
              A[i], " (should be ", i+1, ")");
  }
  if printResults then writeln(A);
}

if CHPL_NETWORK_ATOMICS == "none" {
  doit(uint(8));
  doit(uint(16));
}
doit(uint(32));
doit(uint(64));
doit(uint);

if CHPL_NETWORK_ATOMICS == "none" {
  doit(int(8));
  doit(int(16));
}
doit(int(32));
doit(int(64));
doit(int);

if CHPL_NETWORK_ATOMICS == "none" {
  doit(real(32));
}
doit(real(64));
doit(real);
