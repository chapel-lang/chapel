use BlockDist, CyclicDist, BlockCycDist, ReplicatedDist;
use DimensionalDist2D, ReplicatedDim, BlockCycDim;

const Space = {1..8, 1..8};

const OneLocOnly: [0..0, 0..0] locale = Locales[0];

const B1 = Space dmapped new blockDist(boundingBox=Space);
const B2 = Space dmapped new blockDist(boundingBox={1..4, 1..4});
const B3 = Space dmapped new blockDist(boundingBox=Space, targetLocales=OneLocOnly);

writeln("Block comparisons:");

writeln(B1.distribution == B1.distribution);
writeln(B1.distribution == B2.distribution);
writeln(B1.distribution == B3.distribution);

writeln(B2.distribution == B2.distribution);
writeln(B2.distribution == B3.distribution);

writeln(B3.distribution == B3.distribution);

writeln(B1.distribution == (Space dmapped new blockDist(boundingBox=Space)).distribution);

writeln();


const C1 = Space dmapped new cyclicDist(startIdx=Space.low);
const C2 = Space dmapped new cyclicDist(startIdx=(3,3));
const C3 = Space dmapped new cyclicDist(startIdx=(4,4));
const C4 = Space dmapped new cyclicDist(startIdx=Space.low, targetLocales=OneLocOnly);

writeln("Cyclic comparisons:");

writeln(C1.distribution == C1.distribution);
writeln(C1.distribution == C2.distribution); // tricky, but equal on 2x2 locales since 3==1, mod 2

writeln(C1.distribution == C3.distribution);
writeln(C1.distribution == C4.distribution);

writeln(C2.distribution == C2.distribution);
writeln(C2.distribution == C3.distribution);
writeln(C2.distribution == C4.distribution);

writeln(C3.distribution == C3.distribution);
writeln(C3.distribution == C4.distribution);

writeln(C4.distribution == C4.distribution);

writeln(C1.distribution == (Space dmapped new cyclicDist(startIdx=Space.low)).distribution);

writeln();



const BC1 = Space dmapped new blockCycDist(startIdx=Space.low, 
                                      blocksize=(2, 3));
const BC2 = Space dmapped new blockCycDist(startIdx=(-1,-1),
                                      blocksize=(2,3));
const BC3 = Space dmapped new blockCycDist(startIdx=Space.low,
                                      blocksize=(3,2));
const BC4 = Space dmapped new blockCycDist(startIdx=Space.low,
                                      blocksize=(2,3),
                                      targetLocales=OneLocOnly);

writeln("Block-Cyclic comparisons:");

writeln(BC1.distribution == BC1.distribution);
writeln(BC1.distribution == BC2.distribution);
writeln(BC1.distribution == BC3.distribution);
writeln(BC1.distribution == BC4.distribution);

writeln(BC2.distribution == BC2.distribution);
writeln(BC2.distribution == BC3.distribution);
writeln(BC2.distribution == BC4.distribution);

writeln(BC3.distribution == BC3.distribution);
writeln(BC3.distribution == BC4.distribution);

writeln(BC4.distribution == BC4.distribution);

writeln(BC1.distribution == (Space dmapped new blockCycDist(startIdx=Space.low,
                                          blocksize=(2,3))).distribution);

writeln();


const OneLocOnly1D: [0..0] locale = Locales[0];

const R1 = Space dmapped new replicatedDist();
const R2 = Space dmapped new replicatedDist(targetLocales=OneLocOnly1D);

writeln("Replicated comparisons:");

writeln(R1.distribution == R1.distribution);
writeln(R1.distribution == R2.distribution);

writeln(R2.distribution == R2.distribution);

writeln(R2.distribution == (Space dmapped new replicatedDist(targetLocales=OneLocOnly1D)).distribution);
