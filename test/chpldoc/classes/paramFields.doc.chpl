class hasParams {
  param typeless;
  /* Typed param with a comment */
  param docTyped: int;
  param hasType: int;
  param hasDefaultValue = 14;
  /* Param with default value, declared type, and comment */
  param doctypedDefaultVal: string = "blah blah blah";

  /* Typeless param with a comment */
  param docTypeless;
  /* Param with default value but no type, and comment */
  param docDefaultValue = false;
  param typedDefaultVal: string = "blah blah blah";
}
