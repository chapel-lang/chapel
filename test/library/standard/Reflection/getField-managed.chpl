use Reflection;
proc test(in obj) {
  testRef(obj);
  testConstRef(obj);
}

proc testRef(ref obj) {
  writeln("===== ref ", obj.type:string);
  writeln("  getNumFields    ", getNumFields(obj.type)         :string);
  writeln("  getFieldName    ", getFieldName(obj.type, 5)             );
  writeln("  getField        ", getField(obj, 5)               :string);
  writeln("  getField        ", getField(obj, "f6")            :string);
  writeln("  getFieldRef     ", getFieldRef(obj, 5)            :string);
  writeln("  getFieldRef     ", getFieldRef(obj, "f6")         :string);
  writeln("  hasField        ", hasField(obj.type, "f6")       :string);
  writeln("  getFieldIndex   ", getFieldIndex(obj.type, "f6")  :string);
  writeln("  isFieldBound 5  ", isFieldBound(obj.type, 5)      :string);
  writeln("  isFieldBound f6 ", isFieldBound(obj.type, "f6")   :string);
}
proc testConstRef(const ref obj) {
  writeln("===== const ref ", obj.type:string);
  writeln("  getNumFields    ", getNumFields(obj.type)         :string);
  writeln("  getFieldName    ", getFieldName(obj.type, 5)             );
  writeln("  getField        ", getField(obj, 5)               :string);
  writeln("  getField        ", getField(obj, "f6")            :string);
  if !isRecord(obj) {
  writeln("  getFieldRef     ", getFieldRef(obj, 5)            :string);
  writeln("  getFieldRef     ", getFieldRef(obj, "f6")         :string);
  } else {
  writeln("  getFieldRef     ", "N/A (record)"                        );
  writeln("  getFieldRef     ", "N/A (record)"                        );
  }
  writeln("  hasField        ", hasField(obj.type, "f6")       :string);
  writeln("  getFieldIndex   ", getFieldIndex(obj.type, "f6")  :string);
  writeln("  isFieldBound 5  ", isFieldBound(obj.type, 5)      :string);
  writeln("  isFieldBound f6 ", isFieldBound(obj.type, "f6")   :string);
}

class CCon {
  var f1, f2, f3, f4, f5: int;
  var f6:int = 17;
}

test(new owned CCon());
test((new owned CCon()):owned class?);
test(new shared CCon());
test((new shared CCon()):shared class?);
test((new owned CCon()).borrow());
test(((new owned CCon()):owned class?).borrow());
{
  var un = new unmanaged CCon();
  test(un);
  delete un;
}
{
  var un = (new unmanaged CCon()):unmanaged class?;
  test(un);
  delete un;
}
var a = new CCon();
test(a.borrow());
var b = new CCon():class?;
test(b.borrow());

record RCon {
  var f1, f2, f3, f4, f5: int;
  var f6:int = 17;
}

test(new RCon());
