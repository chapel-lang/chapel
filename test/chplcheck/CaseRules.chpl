module CaseRules {
  var camelVar: int;
  const PascalVar: real;
  var snake_var: string;
  var AVar: bool;

  record camelRecord {
    proc camelCaseMethod() {return 0;}
    proc PascalCaseMethod() {return 0;}
    proc snake_case_method() {return 0;}
  }
  record PascalRecord {}
  record snake_record {
    var camelField, anotherCamelField: int;
    const camelField2, PascalField: real;
    var camelField3, snake_field: string;
  }

  proc camelProc() {return 0;}
  proc PascalProc() {return 0;}
  proc snake_proc() {
    var camelVar: int;
    const PascalVar: real;
    var snake_var: string;
    return 0;
  }

  proc foo(camelArg: int, PascalArg: string, snake_arg: real) {
    return camelArg + snake_arg : int + PascalArg.size();
  }

  module camelModule {}
  module PascalModule {}
  module snake_module {}
  module AModule {}
  
  class camelClass {}
  class PascalClass {}
  class snake_class {}
  class AClass {}

  var snake_CapVar: real;
  const snake_Var: string;

  record R {}

  var _privateVar: int;

  extern proc proc_from_another_lang();
  extern proc Proc_with_Bad_consistenty();
  extern var c_var: int(32);
  extern const c_const: int(32);

  var justOneCapitalLetterAtTheE: string;
  const justO: real;

  operator +(a: int, b: string) {
    // assign the variables to a temporary to make them not unused anymore.
    var temp1 = a,
        temp2 = b;
  }

  record testRecord {
    proc init=(other: testRecord) {
      var temp = other;
    }
  }

  class ParentClassWithBadMethod {
    proc badly_capitalized() {}
  }

  class ChildThatOverridesBadMethod : ParentClassWithBadMethod {
    // shouldn't warn: override procs are exempt from capitalization rules,
    // because the parent controls the name.
    override proc badly_capitalized() {}
  }
}
