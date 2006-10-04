const NumUpdates:uint(64) = 64;
const NumStreams:int = 1 << 2;
const BigStep:int = (NumUpdates/NumStreams);

writeln(BigStep);
