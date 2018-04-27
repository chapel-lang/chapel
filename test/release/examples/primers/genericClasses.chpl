// Generic Classes

/*
   This primer covers generic class types.
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

class UntypedField {
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
var taf  = new unmanaged TypeAliasField(real, 1.0, 2.0);
var taf2 = new unmanaged TypeAliasField(int, 3, 4);
writeln("taf = ", taf, ", taf2 = ", taf2);

var pf  = new unmanaged ParamField(3);
var pf2 = new unmanaged ParamField(2);
writeln("pf = ", pf, ", pf2 = ", pf2);

var uf  = new unmanaged UntypedField(3.14 + 2.72i);
var uf2 = new unmanaged UntypedField(new unmanaged ParamField(2));
writeln("uf = ", uf, ", uf2 = ", uf2);

//
// To specify a generic class type (without creating an instance),
// don't use the ``new`` keyword and just specify the generic arguments.
// For fields that have no types, specify a type for that field,
// instead of a value.
//
var taf3: TypeAliasField(real);
var pf3: ParamField(3);
var uf3: UntypedField(complex);

taf3 = taf;
pf3 = pf;
uf3 = uf;

writeln("taf3 = ", taf3);
writeln("pf3 = ", pf3);
writeln("uf3 = ", uf3);

delete uf2.a;
delete uf2;
delete uf;

delete pf2;
delete pf;

delete taf2;
delete taf;
