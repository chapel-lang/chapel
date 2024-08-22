// init, deinit, and postinit are keywords that should only be used for special
// methods.  This test shows the proper and inproper use of these keywords.


// Redefining these methods inside a class should be fine
record C {
  proc init()     { }
  proc deinit()   { }
  proc postinit() { }
}

class CC {
  proc init()     { }
  proc deinit()   { }
  proc postinit() { }
}

// Even as secondary methods:
record C2 { }
proc C2.init() { }
proc C2.deinit() { }
proc C2.postinit() { }

// deinit can also be used to deinitialize the module
proc deinit() { }

// postinit can be explicitly called, but not init and deinit
var obj = new C();
obj.init();         // [error: explicit calls to init() on anything other than 'this' or 'super' are not allowed]
obj.postinit();
obj.deinit();       // [error: explicit calls to deinit() are not allowed]

// Using these reserved words as variables names is not OK:
var init = 42;      // [error: attempt to redefine reserved word 'init']
var deinit = 52;    // [error: attempt to redefine reserved word 'deinit']
var postinit = 62;  // [error: attempt to redefine reserved word 'postinit']

// Can not use 'init' and 'postinit' as free functions:
proc init() { }       // [error: attempt to redefine reserved word 'init']
proc postinit() { }   // [error: attempt to redefine reserved word 'postinit']

// We don't want to allow deinit or postinit to have arguments
// postinit we handle later in 'passes/initializerRules.cpp'
record C3 {
  proc postinit(x : int) { }  // [*]
  proc deinit(x : int) { }    // [*2]
}
// [*] in this particular test you won't see the error for this since it happens
//     in 'passes/initializerRules.cpp' as we stop before then. Run it in
//     isolation and you'll get: 'postinit must have zero arguments'.
// [*2] Similiar to [*] this will error out in the initializerRules
//    pass with: 'destructors must not have arguments'

// we don't want to support 'deinit' or 'postinit' having an explicit return
// type:
record C4 {
  proc deinit() : int { }   // [*3]
  proc postinit() : int { } // [*4]
}
// [*3] will error out in the initializerRules pass with 'destructors may not
//      declare a return type'
// [*4] this does not error out in initializerRules but will error out
//      with 'control reaches end of function that returns a value'
//      and if you add a 'return' statement you're in the next use case


// or an implicit return type
record C5 {
  proc deinit() { return 1; }   // [error: 'return' statements with values are not allowed in special methods]
  proc postinit() { return 1; } // [error: 'return' statements with values are not allowed in special methods]
}
