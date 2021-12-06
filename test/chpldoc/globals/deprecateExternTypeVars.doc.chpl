// Checks basic deprecated declarations of extern types
deprecated extern type Blah1;
deprecated "Blah2 is deprecated, use Blah3 instead" extern type Blah2;
extern type Blah3;

// Checks interaction with documentation
/* There was documentation of this symbol */
deprecated extern type Blah4;
/* This symbol also was documented */
deprecated "Blah5 is deprecated, use Blah6 instead" extern type Blah5;
extern type Blah6;

// Checks interaction when documentation mentions deprecation in some form
/* This symbol is deprecated */
deprecated extern type Blah7;
/* This symbol is also deprecated, please use f instead */
deprecated "Blah8 is deprecated, use Blah9 instead" extern type Blah8;
extern type Blah9;

// Ensures deprecation doesn't cause "no doc" symbols to turn up in
// documentation
pragma "no doc"
deprecated extern type Blah10;
pragma "no doc"
deprecated "Blah11 is deprecated" extern type Blah11;
