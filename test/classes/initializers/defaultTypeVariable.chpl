record LocalAtomicObject {
  type objType;
  type atomicType = uint(64);
  var _atomicVar: atomic atomicType;

  proc init(type objType) {
    this.objType = objType;
    this.complete();
    _atomicVar.write(0);
  }
}

proc main {
  type Obj = int;

  var a : LocalAtomicObject(Obj);
}
