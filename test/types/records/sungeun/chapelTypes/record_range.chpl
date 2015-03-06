// Make sure that a locally-defined range type overrides the 
// version implicitly used from ChapelRange.
//
// Quoted from the 0.96 version of the spec:
//
// "Symbols made available by a use statement are at an outer scope from those
//  defined directly in the scope where the use statement occurs, but at a
//  nearer scope than symbols defined in the scope containing the scope where
//  the use statement occurs.
//  
// "If used modules themselves use other modules, symbols are scoped according
//  the depth of use statements followed to find them. It is an error for two
//  variables, types, or modules to be defined at the same depth."
//
record range {
  var r: range(int);
}
var r: range;
writeln(r);
