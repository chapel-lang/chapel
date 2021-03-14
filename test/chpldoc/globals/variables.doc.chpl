/* This module has some global scoped variables. */
module hasGlobalVariables {
  // This variable has a type but no associated comment
  var first: int;
  // This variable has a value but no associated comment
  var second = 16;
  // This variable has a type and a value but no associated comment
  var third: bool = false;
  /* This variable has an associated comment and a type */
  var fourth: int;
  /* This variable has an associated comment and a value */
  var fifth = 16;
  /* This variable has an associated comment, a type, and a value */
  var sixth: bool = false;
}
