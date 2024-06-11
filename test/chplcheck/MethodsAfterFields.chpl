module MethodsAfterFields {
  record emptyRecord {   }

  record methodField {
    proc method1() {
      return 12;
    }
    var field1: int;
  }

  record fieldMethodField {
    var field1: int;
    proc method1() {
      return field1;
    }
    var field2: int;
  }

  record fieldFieldMethod {
    var field1: int;
    const field2: real;

    proc method1() {
      return field1;
    }
  }

  class EmptyClass {   
  }

  class MethodField {
    proc method1() {
      return 12;
    }
    var field1: int;
  }

  class FieldMethodField {
    var field1: int;
    proc method1() {
      return field1;
    }
    var field2: int;
  }

  class FieldFieldMethod {
    var field1: int;
    const field2: real;

    proc method1() {
        return field1;
    }
  }
}
