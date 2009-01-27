/*
 * Generic Classes Example
 *
 * This example demonstrates generic class types.
 *
 */

//
// A class is generic if it contains a type alias, contains a field that is
// a parameter, or contains a field with no type and no initialization
// expression. The following three classes are each generic in one of
// these ways.
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
// To create objects from generic classes, the classes must be instantiated.
// This is accomplished by passing the type or parameter value of the generic
// fields as an argument to the default constructor. In the class with the
// untyped field, the class is instantiated using the types of the arguments
// representing the generic fields in the default constructor.
//
var taf  = new TypeAliasField(real, 1.0, 2.0);
var taf2 = new TypeAliasField(int, 3, 4);

var pf  = new ParamField(3);
var pf2 = new ParamField(2);

var uf  = new UntypedField(3.14 + 2.72i);
var uf2 = new UntypedField(new ParamField(2));

writeln((taf, taf2));
writeln((pf, pf2));
writeln((uf, uf2));

//
// To give a variable a type using a generic class without constructing an
// instance of the class, the type specifier is a call to the default
// constructor, but the 'new' keyword is omitted and only with arguments
// corresponding to the generic types are used. All generic types in the class
// must be fully specified.
//
var taf3: TypeAliasField(real);
writeln("This wasn't constructed and should be nil: ", taf3);

