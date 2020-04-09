/* This module has some global scoped params. */
module hasGlobalParams {
  // This param has a value but no associated comment
  param first = 16;
  // This param has a type and a value but no associated comment
  param second: bool = false;
  /* This param has an associated comment and a value */
  param third = 16;
  /* This param has an associated comment, a type, and a value */
  param fourth: bool = false;
}
