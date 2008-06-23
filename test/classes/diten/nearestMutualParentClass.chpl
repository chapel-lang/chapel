class A {
  var name: string = "A";
  def ddName() { return name; }
}

class B:A {
  var name: string = "B";
  def ddName() { return name; }
}

class C:B {
  var name: string = "C";
  def ddName() { return name; }
}

class C2:C {
  var name: string = "C2";
  def ddName() { return name; }
}

class D:C {
  var name: string = "D";
  def ddName() { return name; }
}

class E:C {
  var name: string = "E";
  def ddName() { return name; }
}

class F {
  var name: string = "F";
  def ddName() { return name; }
}

def object.name return "object";
def object.ddName() { return "object"; }

//
// Begin interesting code
//

def get_cdr(type car, type cdr...?k) type {
  return cdr;
}

def isSubType(type sub, type sup) param {
  def isSubTypeHelp(v:sub = nil) param {
    def ist(v:sup) param {
      return true;
    }
    def ist(v) param {
      return false;
    }
    return ist(v);
  }
  return isSubTypeHelp();
}

def getSuperType(type t) type {
  def st(v:t = nil) type {
    if (t == object) then
      return t;
    else
      return v.super.type;
  }
  return st();
}

def nearestMutualParentClass(type t1, type t2) type {
  if t1 == t2 then
    return t1;
  else if isSubType(t1, t2) then
    return t2;
  else if isSubType(t2, t1) then
    return t1;
  else
    return nearestMutualParentClass(getSuperType(t1), getSuperType(t2));
}

def nearestMutualParentClass(type car, type cdr...?k) type where k != 1 {
  type first = nearestMutualParentClass(car, cdr(1));
  return nearestMutualParentClass(first, (...get_cdr((...cdr))));
}

def main {
  var c: nearestMutualParentClass(E, D, C, C2, F) = new E();
  var d: nearestMutualParentClass(E, D, C) = new E();
  writeln(c.name);
  writeln(c.ddName());
  writeln(d.name);
  writeln(d.ddName());
}
