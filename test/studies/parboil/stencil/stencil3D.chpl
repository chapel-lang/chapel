use IO;

config const outfileName = "output.bin";

config const displayInput = false;
config const displayOutput = false;
config const printTimers = 0;

proc main(args: [] string) {
  use Time;

  if args.size != 5 then
    halt("Usage: a.out nx ny nz iterations");

  const nx = args[1]:int;
  const ny = args[2]:int;
  const nz = args[3]:int;
  const iterations = args[4]:int;
  const c0 = 1.0:real(32) / 6.0:real(32);
  const c1 = 1.0:real(32) / 6.0:real(32) / 6.0:real(32);

  var IOTimer, computeTimer, totalTimer: Timer;

  if nx < 1 || ny < 1 || nz < 1 then
    halt("bad problem size");
  if iterations < 1 then
    halt("need at least one iteration");

  const infileName = nx:string + "x" + ny:string + "x" + nz:string + ".bin";

  totalTimer.start();

  var D = {0..#nx, 0..#ny, 0..#nz};

  var A: [D] real(32);
  var ANext: [D] real(32);

  IOTimer.start();
  readData(infileName, A, nx, ny, nz);
  IOTimer.stop();
  if displayInput {
    for a in A do
      writeln(a);
    writeln("Done with input");
  }
  computeTimer.start();

  ANext = A;

  for i in 1..iterations {
    stencil(c0, c1, A, ANext, nx, ny, nz);
    A <=> ANext; // Reference version uses a pointer swap.
                 // var tmp = A._value.data;
                 // A._value.data = ANext._value.data;
                 // ANext._value.data = tmp;
  }

  A <=> ANext;
  computeTimer.stop();

  if displayOutput {
    for a in ANext do
      writeln(a);
  }
  IOTimer.start();
  outputData(outfileName, ANext, nx, ny, nz);
  IOTimer.stop();
  totalTimer.stop();
  if printTimers >= 3 then writeln("IO: ", IOTimer.elapsed());
  if printTimers >= 2 then writeln("Compute: ", computeTimer.elapsed());
  if printTimers >= 1 then writeln("Total: ", totalTimer.elapsed());
}

proc readData(infileName:string, A: [] real(32), nx: int,ny: int, nz: int) {
  var f = open(infileName, iomode.r);
  var r = f.reader(kind=ionative);
  r.read(A);
  r.close();
  f.close();
}

proc outputData(outfileName: string, ANext: [] real(32),
                nx: int, ny: int, nz: int) {
  var f = open(outfileName, iomode.cw);
  var w = f.writer(kind=ionative);
  var size = (nx*ny*nz):int(32);
  w.write(size);
  w.write(ANext);
  w.close();
  f.close();
}

proc stencil(c0: real(32), c1: real(32), A: [] real(32), ANext: [] real(32),
             nx: int, ny: int, nz: int) {
  // aak! the nz and nx indices are swapped here!
  // This is also walking the leftmost dimension fastest!
  forall (i,j,k) in A.domain[1..nx-2, 1..ny-2, 1..nz-2] {
    ANext[k,j,i] = (A[k+1, j, i] + A[k-1, j, i] +
                    A[k, j+1, i] + A[k, j-1, i] +
                    A[k, j, i+1] + A[k, j, i-1])*c1 - A[k, j, i]*c0;
  }
}

