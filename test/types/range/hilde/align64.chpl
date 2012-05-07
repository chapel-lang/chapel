// align64.chpl
//
// Tests representation limits for the 64-bit align operator with a 64-bit constant.
//

const maxS = max(int(32));

writeln(1:int(64).. by 10 align (maxS+1));
