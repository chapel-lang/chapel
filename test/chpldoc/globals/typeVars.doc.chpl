/* This module has some global scoped types. */
module hasGlobalTypes {
  // This type has a value but no associated comment
  type first = bool;
  /* This type has an associated comment and a value */
  type second = int(64);
  /* This is an opaque type from external source. */
  extern type MyT;
  /* This is a non-opaque type from external source. */
  extern type myFloats = 2*real(64);
}
