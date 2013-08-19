config const printResults = false;

proc doit(type myType) {
  var ax: atomic myType;
  var x = ax.read();

  if printResults then writeln(x);

  if numBits(x.type) != numBits(myType) then
    writeln(typeToString(myType), ": ERROR: numBits=", numBits(x.type),
            " (should be ", numBits(myType), ")");
  if printResults then writeln(numBits(x.type));

  on Locales[numLocales-1] do ax.write(min(myType));
  if ax.read() != min(myType) then
    writeln(typeToString(myType), ": ERROR: (read) ax=", ax.read(),
            " (should be ", min(myType), ")");
  if printResults then writeln(ax.read());

  for i in 1..99 {
    var a = if i%100 then max(myType) else 0:myType;
    ax.exchange(a);
  }
  if printResults then writeln(ax.read());
  if ax.read() != max(myType) then
    writeln(typeToString(myType), ": ERROR: (exchange) ax=", ax.read(),
            " (should be ", max(myType), ")");

  ax.write(0:myType);
  coforall i in 1..15 do on Locales[i%numLocales] { // 15 is max for int(8)
      var a = (ax.fetchAdd(i:myType)+i:myType):myType;
      ax.compareExchange(a, a);
    }
  x = ax.read();
  if x != 120:myType then
    writeln(typeToString(myType), ": ERROR: (fetchAdd/compareExchange) x=", x, " (should be 120)");
  if printResults then writeln(x);

  ax.write(0:myType);
  coforall i in 1..15 do on Locales[i%numLocales] { // 15 is max for int(8)
      var a = (ax.fetchAdd(i:myType)+i:myType):myType;
      ax.compareExchangeStrong(a, a);
    }
  x = ax.read();
  if x != 120:myType then
    writeln(typeToString(myType), ": ERROR: (fetchAdd/compareExchangeStrong) x=", x, " (should be 120)");
  if printResults then writeln(x);

  ax.write(0:myType);
  coforall i in 1..15 do on Locales[i%numLocales] { // 15 is max for int(8)
      var a = (ax.fetchAdd(i:myType)+i:myType):myType;
      ax.compareExchangeWeak(a, a);
    }
  x = ax.read();
  if x != 120:myType then
    writeln(typeToString(myType), ": ERROR: (fetchAdd/compareExchangeWeak) x=", x, " (should be 120)");
  if printResults then writeln(x);

  var b: atomic bool;
  ax.write(0:myType);
  begin {
    ax.waitFor(120:myType);
    const x = ax.read();
    if x != 120:myType then
      writeln(typeToString(myType), ": ERROR: (waitFor/add) x=", x, " (should be 120)");
    if printResults then writeln(x);
    b.testAndSet();
  }
  coforall i in 1..15 do on Locales[i%numLocales] { // 15 is max for int(8)
      ax.add(i:myType);
    }
  b.waitFor(true);

  b.clear();
  begin {
    ax.waitFor(0:myType);
    const x = ax.read();
    if x != 0:myType then
      writeln(typeToString(myType), ": ERROR: (waitFor/sub) x=", x, " (should be 0)");
    if printResults then writeln(x);
    b.testAndSet();
  }
  coforall i in 1..15 do on Locales[i%numLocales] { // 15 is max for int(8)
      ax.sub(i:myType);
    }
  b.waitFor(true);

  if _isIntegralType(myType) {
    ax.write(max(int(8)):myType);
    b.clear();
    begin {
      ax.waitFor(0:myType);
      const x = ax.read();
      if x != 0:myType then
        writeln(typeToString(myType), ": ERROR: (waitFor/and) x=", x, " (should be 1)");
      if printResults then writeln(x);
      b.testAndSet();
    }
    coforall i in 0..6 do on Locales[i%numLocales] { // 7 is max for int(8)
        const v = (max(int(8))^(1<<i)):myType;
        ax.and(v);
      }
    b.waitFor(true);

    ax.write(0:myType);
    b.clear();
    begin {
      ax.waitFor(max(int(8)):myType);
      const x = ax.read();
      if x != max(int(8)):myType then
        writeln(typeToString(myType), ": ERROR: (waitFor/or) x=", x, " (should be ", max(int(8)), ")");
      if printResults then writeln(x);
      b.testAndSet();
    }
    coforall i in 0..6 do on Locales[i%numLocales] { // 7 is max for int(8)
        const v = (1<<i):myType;
        ax.or(v);
      }
    b.waitFor(true);

    ax.write(0:myType);
    b.clear();
    begin {
      ax.waitFor(max(int(8)):myType);
      const x = ax.read();
      if x != max(int(8)):myType then
        writeln(typeToString(myType), ": ERROR: (waitfOr/xor) x=", x, " (should be ", max(int(8)), ")");
      if printResults then writeln(x);
      b.testAndSet();
    }
    coforall i in 0..6 do on Locales[i%numLocales] { // 7 is max for int(8)
        const v = (1<<i):myType;
        ax.xor(v);
      }
    b.waitFor(true);
  }

  const D = {7..39};
  var aA: [D] atomic myType;

  for i in D do aA[i].write(i:myType);
  var A = [a in aA] a.read();
  for i in D {
    if A[i-D.dim(1).low+1] != i:myType then
      writeln(typeToString(myType), ": ERROR: A[", i-D.dim(1).low+1, "]=",
              A[i-D.dim(1).low+1], " (should be ", i, ")");
  }
  if printResults then writeln(A);

  for i in D do aA[i].write((i+1):myType);
  A = aA.read();
  for i in D {
    if A[i-D.dim(1).low+1] != (i+1):myType then
      writeln(typeToString(myType), ": ERROR: A[", i-D.dim(1).low+1, "]=",
              A[i-D.dim(1).low+1], " (should be ", i+1, ")");
  }
  if printResults then writeln(A);
}

doit(uint(8));
doit(uint(16));
doit(uint(32));
doit(uint(64));
doit(uint);

doit(int(8));
doit(int(16));
doit(int(32));
doit(int(64));
doit(int);
doit(real(32));
doit(real(64));
doit(real);
