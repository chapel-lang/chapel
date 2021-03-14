var x: int(64);

//
// This should generate an error.  At present we intentionally don't
// support sizeof() within Chapel because (a) it's not entirely clear
// how it should be defined for things like arrays that typically have
// a descriptor in combination with some stack allocated memory and
// (b) the user doesn't have much call to use it since they're
// typically not doing things that relate directly to something's
// memory layout (and advanced users can create an extern sizeof()
// declaration if they need it).
//
// We could revisit this decision down the road, but an advantage
// of not supporting it, at least by this name, is that it won't
// result in confusion w.r.t. the C function of the same name
// (which is easier to define due to the simpler types).
//
writeln("sizeof(x) = ", sizeof(x));
