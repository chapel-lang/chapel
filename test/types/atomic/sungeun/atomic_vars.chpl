config const printResults = false;

proc doit(type myType) {
  var ax: atomic myType;
  var x = ax.read();

  if printResults then writeln(x);

  if numBits(x.type) != numBits(myType) then
    writeln(typeToString(myType), ": ERROR: numBits=", numBits(x.type),
            " (should be ", numBits(myType), ")");
  if printResults then writeln(numBits(x.type));

  on rootLocale.getLocale(numLocales-1) do ax.write(min(myType));
  if ax.read() != min(myType) then
    writeln(typeToString(myType), ": ERROR: ax=", ax.read(),
            " (should be ", min(myType), ")");
  if printResults then writeln(ax.read());

  ax.exchange(max(myType));
  if printResults then writeln(ax.read());
  if ax.read() != max(myType) then
    writeln(typeToString(myType), ": ERROR: ax=", ax.read(),
            " (should be ", max(myType), ")");

  ax.write(0:myType);
  coforall i in 1..15 do on rootLocale.getLocale(i%numLocales) { // 15 is max for int(8)
      var a = (ax.fetchAdd(i:myType)+i:myType):myType;
      ax.compareExchange(a, a);
    }
  x = ax.read();
  if x != 120:myType then
    writeln(typeToString(myType), ": ERROR: x=", x, " (should be 120)");
  if printResults then writeln(x);

  var b: atomic bool;
  ax.write(0:myType);
  begin {
    ax.waitFor(120:myType);
    const x = ax.read();
    if x != 120:myType then
      writeln(typeToString(myType), ": ERROR: x=", x, " (should be 120)");
    if printResults then writeln(x);
    b.testAndSet();
  }
  coforall i in 1..15 do on rootLocale.getLocale(i%numLocales) { // 15 is max for int(8)
      ax.add(i:myType);
    }
  b.waitFor(true);

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
