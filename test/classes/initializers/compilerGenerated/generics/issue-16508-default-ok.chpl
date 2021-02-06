record TypelessField {
  var field;
}
type TypelessFieldInt = TypelessField(int);
var b: TypelessFieldInt;
writeln(b.field, ": ", b.field.type:string);
