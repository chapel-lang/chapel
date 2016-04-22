class A {
  var name: string = "A";
  proc ddName() { return name; }
}

class B:A {
  var name: string = "B";
  proc ddName() { return name; }
}

class C:B {
  var name: string = "C";
  proc ddName() { return name; }
}

class C2:C {
  var name: string = "C2";
  proc ddName() { return name; }
}

class D:C {
  var name: string = "D";
  proc ddName() { return name; }
}

class E:C {
  var name: string = "E";
  proc ddName() { return name; }
}

class F {
  var name: string = "F";
  proc ddName() { return name; }
}

proc object.name: string return "object";
proc object.ddName(): string { return "object"; }

//
// Begin interesting code
//

proc get_cdr(type car, type cdr...?k) type {
  return cdr;
}

proc isSubType(type sub, type sup) param {
  proc isSubTypeHelp(v:sub = nil) param {
    proc ist(v:sup) param {
      return true;
    }
    proc ist(v) param {
      return false;
    }
    return ist(v);
  }
  return isSubTypeHelp();
}

proc getSuperType(type t) type {
  proc st(v:t = nil) type {
    if (t == object) then
      return t;
    else
      return v.super.type;
  }
  return st();
}

proc nearestMutualParentClass(type t1, type t2) type {
  if t1 == t2 then
    return t1;
  else if isSubType(t1, t2) then
    return t2;
  else if isSubType(t2, t1) then
    return t1;
  else
    return nearestMutualParentClass(getSuperType(t1), getSuperType(t2));
}

proc nearestMutualParentClass(type car, type cdr...?k) type where k != 1 {
  type first = nearestMutualParentClass(car, cdr(1));
  return nearestMutualParentClass(first, (...get_cdr((...cdr))));
}

proc main {
  var c: nearestMutualParentClass(E, D, C, C2, F) = new E();
  var d: nearestMutualParentClass(E, D, C) = new E();
  writeln(c.name);
  writeln(c.ddName());
  writeln(d.name);
  writeln(d.ddName());

  delete d;
  delete c;
}
