// Checks basic deprecated declarations of variables
@deprecated var x: int;
@deprecated(notes="y is deprecated, use z instead") var y: int;
var z: int;

// Checks interaction with documentation
/* There was documentation of this symbol */
@deprecated var a: string;
/* This symbol also was documented */
@deprecated(notes="b is deprecated, use c instead") var b: string;
var c: string;

// Checks interaction when documentation mentions deprecation in some form
/* This symbol is deprecated */
@deprecated var d: real;
/* This symbol is also deprecated, please use f instead */
@deprecated(notes="e is deprecated, use f instead") var e: real;
var f: real;

// Ensures deprecation doesn't cause "no doc" symbols to turn up in
// documentation
@chpldoc.nodoc
@deprecated var g: int;
@chpldoc.nodoc
@deprecated(notes="h is deprecated, use z instead") var h: int;


// Ensures that modules respond appropriately to deprecated
@deprecated
module DeprecatedMod1 {
}

@deprecated("This module is deprecated")
module DeprecatedMod2 {
}

/* Including modules with documentation */
@deprecated("This module is deprecated")
module DeprecatedMod3 {
  /* Just a regular symbol to show how it interacts */
  var x: int;
}
