class EitherOr {
  var fieldInit: bool = true;
  var infer: int;

  proc init(param useField: bool) {
    if (useField) {
      this.complete();
    } else {
      this.init(false, 17);
    }
  }

  proc init(useField: bool, otherVal: int) {
    fieldInit = useField;
    infer = otherVal;
  }
}

proc main() {
  var c1: EitherOr = new EitherOr(true);
  writeln(c1);
  delete c1;

  var c2: EitherOr = new EitherOr(false);
  writeln(c2);
  delete c2;
}
