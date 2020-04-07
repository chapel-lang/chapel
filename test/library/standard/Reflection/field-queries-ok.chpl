// Check that numFields et al. handles a variety of aggregate types.
// See also: test/param/ferguson/field*
use Reflection;

proc test(type tp) {
  compilerWarning("===== ", tp:string);
  compilerWarning("  numFields       ", numFields(tp)            :string);
  compilerWarning("  getFieldName    ", getFieldName(tp, 5)             );
  compilerWarning("  hasField        ", hasField(tp, "f6")       :string);
  compilerWarning("  getFieldIndex   ", getFieldIndex(tp, "f6")  :string);
  compilerWarning("  isFieldBound 5  ", isFieldBound(tp, 5)      :string);
  compilerWarning("  isFieldBound f6 ", isFieldBound(tp, "f6")   :string);
}


class CCon {
  var f1, f2, f3, f4, f5, f6: int;
}

test(CCon);
test(CCon: owned class);
test(CCon: owned class?);
test(CCon: shared class);
test(CCon: shared class?);
test(CCon: borrowed class);
test(CCon: borrowed class?);
test(CCon: unmanaged class);
test(CCon: unmanaged class?);


class CGen {
  var f1, f2, f3, f4, f5, f6;
}

test(CGen);
test(CGen: owned class);
test(CGen: owned class?);
test(CGen: shared class);
test(CGen: shared class?);
test(CGen: borrowed class);
test(CGen: borrowed class?);
test(CGen: unmanaged class);
test(CGen: unmanaged class?);


record RCon {
  var f1, f2, f3, f4, f5, f6: int;
}

test(RCon);


record RGen {
  var f1, f2, f3, f4, f5, f6;
}

test(RGen);


union UCon {
  var f1, f2, f3, f4, f5, f6: int;
}

test(UCon);


union UGen {
  var f1, f2, f3, f4, f5, f6;
}

test(UGen);


compilerError("=== done ===");
