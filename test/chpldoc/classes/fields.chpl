class fullOfFields {
  var thing1;
  var thing2;
  var typedField: int;
  var alsoTyped: string;
  /* This field has a doc attached, does that work? */
  var commented;
  /* This field has a doc and an explicit type */
  var typeComment: int;
  var initialized = "bleah";
  var initializedType: int = 3;
}

record fullOfFieldsToo {
  var thing1;
  var thing2;
  var typedField: int;
  var alsoTyped: string;
  /* This field has a doc attached, does that work? */
  var commented;
  /* This field has a doc and an explicit type */
  var typeComment: int;

}