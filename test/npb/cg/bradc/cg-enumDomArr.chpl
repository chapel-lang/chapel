enum classVals {S, W, A, B, C, D, O};


config var probClass = classVals.S;


const Class = {classVals.S..classVals.O};
const probSizes:   [Class] int = [ 1400, 7000, 14000, 75000, 150000, 150000, 1400 ],
      nonZeroes:   [Class] int = [ 7, 8, 11, 13, 15, 21, 7 ],
      shifts:      [Class] int = [ 10, 12, 20, 60, 110, 500, 10 ],
      numIters:    [Class] int = [ 15, 15, 15, 75, 75, 100, 15 ],
      verifyZetas: [Class] real = [ 8.5971775078648, 10.362595087124,
                                    17.130235054029, 22.712745482631,
                                    28.973605592845, 52.5145321058, 0.0 ];


type elemType = real(64);


proc main() {
  const n = probSizes(probClass),
        nonzer = nonZeroes(probClass),
        shift = shifts(probClass),
        niter = numIters(probClass),
        rcond = 1.0,
        zetaVerifyValue = verifyZetas(probClass);

  writeln("For Problem Class ", probClass, ":");
  writeln("  n = ", n);
  writeln("  nonzer = ", nonzer);
  writeln("  shift = ", shift);
  writeln("  niter = ", niter);
  writeln("  zetaVerifyValue = ", zetaVerifyValue);
}
