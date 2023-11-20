// Checks basic unstable declarations of variables
@unstable var a: int;
@unstable("b is unstable, don't use it") var b: int;

// Checks interaction with documentation
/* There was documentation of this symbol */
@unstable var c: string;
/* This symbol was also documented */
@unstable("d is unstable, don't use it") var d: string;

// Checks interaction when documentation mentions stability in some form
/* This symbol is unstable */
@unstable var e: real;
/* This symbol is also unstable */
@unstable("f is unstable, don't use it") var f: real;
/* This symbol is not considered stable */
@unstable var g: bool;
/* This symbol is also not considered stable */
@unstable("h is unstable, don't use it") var h: bool;

// Ensures instability doesn't cause "no doc" symbols to turn up in
// documentation
@chpldoc.nodoc
@unstable var i: int;
@chpldoc.nodoc
@unstable("j is unstable, don't use it") var j: int;

// Ensures that modules respond appropriately to unstable
@unstable
module UnstableMod1 {
}

@unstable("This module is unstable")
module UnstableMod2 {
}

/* Including modules with documentation */
@unstable("This module is unstable")
module UnstableMod3 {
  /* Just a regular symbol to show how it interacts */
  var x: int;
}
