class fullOfFields {
  /* This is a generic type. */
  type eltType;
  var thing1;
  param thing2;
  var typedField: int;
  var alsoTyped: string;
  /* This field has a doc attached, does that work? */
  var commented;
  /* This field has a doc and an explicit type */
  var typeComment: int;
  const initialized = "bleah";
  var initializedType: int = 3;
  /* This is a type alias. */
  type yards = uint(16);
}

record fullOfFieldsToo {
  /* This is a generic type. */
  type eltType;
  var thing1;
  param thing2;
  var typedField: int;
  var alsoTyped: string;
  /* This field has a doc attached, does that work? */
  var commented;
  /* This field has a doc and an explicit type */
  var typeComment: int;
  /* This is a const. */
  const myConst = 10;
  /* This is a type alias. */
  type yards = int;
}