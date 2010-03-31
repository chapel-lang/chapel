use BlockDist, Time, Types, Random, HPCCProblemSize;

const numVectors = 3;
type elemType = real(64);

config const m = computeProblemSize(numVectors, elemType), alpha = 3.0;

config const numTrials = 10, epsilon = 0.0, seed = 314159265;

def main() {
  const BlockDist = new dist(new Block(boundingBox=[1..m]));
  const ProblemSpace: domain(1, int(64)) distributed BlockDist = [1..m];
  var A, B, C: [ProblemSpace] elemType;

  writeln("BEGIN INITIALIZATION");
  chpl__testParStart();
  initVectors(B, C);
  chpl__testParStop();
  writeln("END INITIALIZATION");

  chpl__testParStart();
  for trial in 1..numTrials {
    writeln("BEGIN TRIAL ", trial);
    forall (a, b, c) in (A, B, C) do
      a = b + alpha * c;
    writeln("END TRIAL ", trial);
  }
  chpl__testParStop();
}

//
// Initialize vectors B and C using a random stream of values and
// optionally print them to the console
//
def initVectors(B, C) {
  var randlist = new RandomStream(seed);
  randlist.fillRandom(B);
  randlist.fillRandom(C);
  delete randlist;
}
