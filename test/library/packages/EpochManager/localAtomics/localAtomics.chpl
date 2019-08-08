use LocalAtomics;

class C {
  var x : int;
}

proc main() {
  var x = new unmanaged C(1);
  var atomicObj = new LocalAtomicObject(unmanaged C);
  atomicObj.write(x);
  var y = atomicObj.read();
  writeln(atomicObj.read().type:string);
  writeln(y);
  var z = atomicObj.readABA();
  writeln(z.type : string);
  writeln(z);
  var w = new unmanaged C(2);
  writeln(atomicObj.compareExchange(x, z.getObject()));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.compareExchangeABA(z, w));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.compareExchange(w, x));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());
  writeln(atomicObj.exchange(nil));
  writeln(atomicObj.read());
  writeln(atomicObj.readABA());

  writeln();

  var a = atomicObj.readABA();
  var b = atomicObj.readABA();

  writeln(a==b);
  atomicObj.compareExchangeABA(a, x);
  writeln(a==b);

  var c = atomicObj.readABA();

  writeln(a==c);
}
