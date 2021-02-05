record TypelessField {
  var field;
  proc init() {
    field = 1.0;
    writeln("init()");
  }
  proc init(field) {
    this.field = field;
    writeln("init(field)");
  }
}

type TypelessFieldInt = TypelessField(int);

// since default initialization in this case runs init()
// we would expect a type mismatch error.
var b: TypelessFieldInt;
writeln(b.field, ": ", b.field.type:string);
