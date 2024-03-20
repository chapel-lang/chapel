// When declaring the type of a 'ref' to an array, it should be an error if
// the domain of that type differs from the domain of the referenced array.

// This is a lighter / more obvious variant of type-decls-arrays-1.future
// Here not only the identities, but also the sizes of the two domains differ.

var a1: [1..3] int;
ref r1: [1..44] int = a1;  // error: domain mismatch between r1 and a1
writeln(r1);
