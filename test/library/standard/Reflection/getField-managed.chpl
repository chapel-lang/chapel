use Reflection;
proc test(const ref obj) {
  writeln("===== ", obj.type:string);
  writeln("  numFields       ", numFields(obj.type)            :string);
  writeln("  getFieldName    ", getFieldName(obj.type, 5)             );
  writeln("  getField        ", getField(obj, 5)         :string);
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
test(new unmanaged CCon());
test((new unmanaged CCon()):unmanaged class?);

record RCon {
  var f1, f2, f3, f4, f5: int;
  var f6:int = 17;
}

test(new RCon());
