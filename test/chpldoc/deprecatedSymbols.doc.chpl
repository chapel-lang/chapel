// Checks basic deprecated declarations of variables
deprecated var x: int;
deprecated "y is deprecated, use z instead" var y: int;
var z: int;

// Checks interaction with documentation
/* There was documentation of this symbol */
deprecated var a: string;
/* This symbol also was documented */
deprecated "b is deprecated, use c instead" var b: string;
var c: string;

// Checks interaction when documentation mentions deprecation in some form
/* This symbol is deprecated */
deprecated var d: real;
/* This symbol is also deprecated, please use f instead */
deprecated "e is deprecated, use f instead" var e: real;
var f: real;
