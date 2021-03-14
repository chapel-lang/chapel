// align.chpl
//
// Tests representation limits for the align operator.

const minS = min(int(32));
const maxS = max(int(32));
const maxS1 = maxS:int(64) + 1; // Note 1.
const maxU = max(uint(32));
const nI = ((-2):int(32))**53;
const pI = 2:uint(64)**63;

writeln(1:int.. by 10 align minS);
writeln(1:int.. by 10 align maxS);
writeln(1:int.. by 10 align (maxS1));
// writeln(1:int.. by 10 align maxU);
writeln(1:int.. by 10 align nI);
// writeln(1:int.. by 10 align pI);
//writeln(1:int(8).. by 10 align minS);
//writeln(1:int(8).. by 10 align maxS);
//writeln(1:int(8).. by 10 align (maxS+1));
// writeln(1:int(8).. by 10 align maxU);
//writeln(1:int(8).. by 10 align nI);
// writeln(1:int(8).. by 10 align pI);
//writeln(1:int(16).. by 10 align minS);
//writeln(1:int(16).. by 10 align maxS);
//writeln(1:int(16).. by 10 align (maxS+1));
// writeln(1:int(16).. by 10 align maxU);
//writeln(1:int(16).. by 10 align nI);
// writeln(1:int(16).. by 10 align pI);
writeln(1:int(32).. by 10 align minS);
writeln(1:int(32).. by 10 align maxS);
//writeln(1:int(32).. by 10 align (maxS1));
// writeln(1:int(32).. by 10 align maxU);
writeln(1:int(32).. by 10 align nI);
// writeln(1:int(32).. by 10 align pI);
writeln(1:int(64).. by 10 align minS);
writeln(1:int(64).. by 10 align maxS);
//writeln(1:int(64).. by 10 align (maxS+1));
writeln(1:int(64).. by 10 align maxU);
writeln(1:int(64).. by 10 align nI);
// writeln(1:int(64).. by 10 align pI);
// writeln(1:uint.. by 10 align minS);
// writeln(1:uint.. by 10 align maxS);
// writeln(1:uint.. by 10 align (maxS+1));
writeln(1:uint.. by 10 align maxU);
// writeln(1:uint.. by 10 align nI);
// writeln(1:uint.. by 10 align pI);
//writeln(1:uint(8).. by 10 align minS);
//writeln(1:uint(8).. by 10 align maxS);
//writeln(1:uint(8).. by 10 align (maxS+1));
// writeln(1:uint(8).. by 10 align maxU);
//writeln(1:uint(8).. by 10 align nI);
// writeln(1:uint(8).. by 10 align pI);
//writeln(1:uint(16).. by 10 align minS);
//writeln(1:uint(16).. by 10 align maxS);
//writeln(1:uint(16).. by 10 align (maxS+1));
// writeln(1:uint(16).. by 10 align maxU);
//writeln(1:uint(16).. by 10 align nI);
// writeln(1:uint(16).. by 10 align pI);
// writeln(1:uint(32).. by 10 align minS);
// writeln(1:uint(32).. by 10 align maxS);
// writeln(1:uint(32).. by 10 align (maxS+1));
writeln(1:uint(32).. by 10 align maxU);
// writeln(1:uint(32).. by 10 align nI);
// writeln(1:uint(32).. by 10 align pI);
// writeln(1:uint(64).. by 10 align minS);
// writeln(1:uint(64).. by 10 align maxS);
// writeln(1:uint(64).. by 10 align (maxS+1));
writeln(1:uint(64).. by 10 align maxU);
// writeln(1:uint(64).. by 10 align nI);
writeln(1:uint(64).. by 10 align pI);

// Note 1. maxS1 is supposed to test the first integer beyond the representable
// range for int(32).  
// If maxS+1 is used, then it is coerced back into an int(32) result, which is
// not what we want.

