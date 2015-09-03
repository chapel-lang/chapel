use BlockDist, CyclicDist, BlockCycDist, ReplicatedDist;
use DimensionalDist2D, ReplicatedDim, BlockCycDim;

const Space = {1..8, 1..8};

const OneLocOnly: [0..0, 0..0] locale = Locales[0];

const B1 = Space dmapped Block(boundingBox=Space);
const B2 = Space dmapped Block(boundingBox={1..4, 1..4});
const B3 = Space dmapped Block(boundingBox=Space, targetLocales=OneLocOnly);

writeln("Block comparisons:");

writeln(B1.dist == B1.dist);
writeln(B1.dist == B2.dist);
writeln(B1.dist == B3.dist);

writeln(B2.dist == B2.dist);
writeln(B2.dist == B3.dist);

writeln(B3.dist == B3.dist);

writeln(B1.dist == (Space dmapped Block(boundingBox=Space)).dist);

writeln();


const C1 = Space dmapped Cyclic(startIdx=Space.low);
const C2 = Space dmapped Cyclic(startIdx=(3,3));
const C3 = Space dmapped Cyclic(startIdx=(4,4));
const C4 = Space dmapped Cyclic(startIdx=Space.low, targetLocales=OneLocOnly);

writeln("Cyclic comparisons:");

writeln(C1.dist == C1.dist);
writeln(C1.dist == C2.dist); // tricky, but equal on 2x2 locales since 3==1, mod 2

writeln(C1.dist == C3.dist);
writeln(C1.dist == C4.dist);

writeln(C2.dist == C2.dist);
writeln(C2.dist == C3.dist);
writeln(C2.dist == C4.dist);

writeln(C3.dist == C3.dist);
writeln(C3.dist == C4.dist);

writeln(C4.dist == C4.dist);

writeln(C1.dist == (Space dmapped Cyclic(startIdx=Space.low)).dist);

writeln();



const BC1 = Space dmapped BlockCyclic(startIdx=Space.low, 
                                      blocksize=(2, 3));
const BC2 = Space dmapped BlockCyclic(startIdx=(-1,-1),
                                      blocksize=(2,3));
const BC3 = Space dmapped BlockCyclic(startIdx=Space.low,
                                      blocksize=(3,2));
const BC4 = Space dmapped BlockCyclic(startIdx=Space.low,
                                      blocksize=(2,3),
                                      targetLocales=OneLocOnly);

writeln("Block-Cyclic comparisons:");

writeln(BC1.dist == BC1.dist);
writeln(BC1.dist == BC2.dist);
writeln(BC1.dist == BC3.dist);
writeln(BC1.dist == BC4.dist);

writeln(BC2.dist == BC2.dist);
writeln(BC2.dist == BC3.dist);
writeln(BC2.dist == BC4.dist);

writeln(BC3.dist == BC3.dist);
writeln(BC3.dist == BC4.dist);

writeln(BC4.dist == BC4.dist);

writeln(BC1.dist == (Space dmapped BlockCyclic(startIdx=Space.low,
                                          blocksize=(2,3))).dist);

writeln();


const OneLocOnly1D: [0..0] locale = Locales[0];

const R1 = Space dmapped ReplicatedDist();
const R2 = Space dmapped ReplicatedDist(targetLocales=OneLocOnly1D);

writeln("Replicated comparisons:");

writeln(R1.dist == R1.dist);
writeln(R1.dist == R2.dist);

writeln(R2.dist == R2.dist);

writeln(R2.dist == (Space dmapped ReplicatedDist(targetLocales=OneLocOnly1D)).dist);