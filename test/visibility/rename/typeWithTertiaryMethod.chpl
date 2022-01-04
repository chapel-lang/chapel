module TypeSrc {
  record Rec {
    var x: int;
  }
}
module TertiaryTypeMethodSrc {
  public use TypeSrc;

  proc type Rec.typeMethod() {
    writeln("In Rec.typeMethod()");
  }
}

module MainMod {
  use TertiaryTypeMethodSrc only Rec as MyRec;

  proc main() {
    MyRec.typeMethod();
  }
}
