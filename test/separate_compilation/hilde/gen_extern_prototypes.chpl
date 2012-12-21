// gen_extern_prototypes.chpl
//
// Test the function of the --gen-extern-prototypes flag.
//

extern proc putchar(i:int(32)) : int(32);

putchar(68); // 'D'

writeln("one.");
