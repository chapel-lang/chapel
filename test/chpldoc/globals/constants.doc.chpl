/* This module has some global scoped constants. */
module hasGlobalConstants {
  // This constant has a type but no associated comment
  const first: int;
  // This constant has a value but no associated comment
  const second = 16;
  // This constant has a type and a value but no associated comment
  const third: bool = false;
  /* This constant has an associated comment and a type */
  const fourth: int;
  /* This constant has an associated comment and a value */
  const fifth = 16;
  /* This constant has an associated comment, a type, and a value */
  const sixth: bool = false;
}
