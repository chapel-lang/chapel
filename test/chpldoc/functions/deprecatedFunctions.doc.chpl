// Checks basic deprecated declarations of functions
@deprecated proc foo() { }
@deprecated(notes="bar is deprecated, use baz instead") proc bar() { }
proc baz() { }

// Checks interaction with documentation
/* There was documentation of this symbol */
@deprecated proc alpha() { }
/* There was documentation of this symbol */
@deprecated(notes="beta is deprecated, use gamma instead") proc beta() { }
/* There was documentation of this symbol */
proc gamma() { }

// Checks interaction when documentation mentions deprecation in some form
/* This symbol is deprecated */
@deprecated proc fee() { }
/* This symbol is also deprecated, please use F instead */
@deprecated(notes="fie is deprecated, use foe instead") proc fie() { }
/* This symbol is not deprecated, but it shouldn't affect anything */
proc foe() { }

// Ensures deprecation doesn't cause "no doc" symbols to turn up in
// documentation
@chpldoc.nodoc
@deprecated proc uno() { }
@chpldoc.nodoc
@deprecated(notes="Two can be as bad as one, it's the loneliest number since the #1")
proc dos() { }
