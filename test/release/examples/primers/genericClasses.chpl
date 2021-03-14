// Generic Classes and Records

/*
   This primer covers generic classes and records.
*/

//
// A class is generic if it contains a type alias, contains a field
// that is a parameter, or contains a field with no type and no
// initialization expression.  The following three classes are each
// generic in one of these ways.
//
class TypeAliasField {
  type t;
  var a, b: t;
}

class ParamField {
  param p: int;
  var tup: p*int;
}

record UntypedField {
  var a;
}

//
// To create objects from generic classes, the classes must be
// instantiated.  This is accomplished by passing the type or
// parameter value of the generic fields as an argument to the default
// constructor.  In the class with the untyped field, the class is
// instantiated using the types of the arguments representing the
// generic fields in the default constructor.
//
var taf  = new TypeAliasField(real, 1.0, 2.0);
var taf2 = new TypeAliasField(int, 3, 4);
writeln("taf = ", taf, ", taf2 = ", taf2);

var pf  = new ParamField(3);
var pf2 = new ParamField(2);
writeln("pf = ", pf, ", pf2 = ", pf2);

var uf  = new UntypedField(3.14 + 2.72i);
var uf2 = new UntypedField(new ParamField(2));
writeln("uf = ", uf, ", uf2 = ", uf2);

//
// To specify a generic class type (without creating an instance),
// don't use the ``new`` keyword and just specify the generic arguments.
// For fields that have no types, specify a type for that field,
// instead of a value.
//
var taf3: borrowed TypeAliasField(real)?;
var pf3: borrowed ParamField(3)?;
var uf3: UntypedField(complex);

taf3 = taf;
pf3 = pf;
uf3 = uf;

writeln("taf3 = ", taf3);
writeln("pf3 = ", pf3);
writeln("uf3 = ", uf3);
