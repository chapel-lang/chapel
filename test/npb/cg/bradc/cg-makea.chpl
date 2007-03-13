enum classVals {S, W, A, B, C, D, O};


config const probClass = S;


const Class: domain(classVals) = [S..O];
const probSizes:   [Class] int = (/ 1400, 7000, 14000, 75000, 150000, 150000, 1400 /),
      nonZeroes:   [Class] int = (/ 7, 8, 11, 13, 15, 21, 7 /),
      shifts:      [Class] int = (/ 10, 12, 20, 60, 110, 500, 10 /),
      numIters:    [Class] int = (/ 15, 15, 15, 75, 75, 100, 15 /),
      verifyZetas: [Class] real = (/ 8.5971775078648, 10.362595087124,
                                    17.130235054029, 22.712745482631,
                                    28.973605592845, 52.5145321058, 0.0 /);

config const n = probSizes(probClass),
             rcond = 0.1,
             nonzer = nonZeroes(probClass);


const nonzer_max = 16,
      naMax = 150000;


type elemType = real(64);


def main() {
  for res in makea() {
    writeln("res is: ", res);
  }
}


iterator makea() {
  var v: [1..nonzer_max] real,
      iv: [1..nonzer_max] int,
      nzloc: [1..nonzer_max] int;
  var mark: [1..naMax] uint(8);
  
  var size = 1.0,
      ratio = rcond ** (1.0 / n),
      nnza = 0;

  for iouter in 1..n {
    const nzv = nonzer;

    sprnvc(n, nzv, v, iv, nzloc, mark);
    vecset(n, v, iv, nzv, iouter, 0.50);

    for ivelt in 1..nzv {
      const jcol = iv(ivelt),
            scale = size * v(ivelt);

      for ivelt1 in 1..nzv {
        const irow = iv(ivelt1);

        yield ((irow, jcol), v(ivelt1)*scale);
      }
    }
    size *= ratio;
  }

  for i in 1..n {
    yield ((i, i), rcond - shift);
  }
}


def sprnvc(n, nz, v, iv, nzloc, mark) {
  const tran = 314159265.0,
        amult = 1220703125.0,
        zeta = randlc(tran, amult);

  const nn1 = lg2(n);

  var nzv = 0,
      nzrow = 0;

  while (nzv < nz) {
    do {
      const vecelt = randlc(tran, amult),
            i = icnvrt(randlc(tran, amult), nn1) + 1;
      if (i <= n && mark(i - 1) == 0) {
        mark(i - 1) = 1;
        nzrow += 1;
        nzloc(nzrow = 1) = i;
        nzv += 1;
        v(nzv - 1) = vecelt;
        iv(nzv - 1) = i;
      }
    } while (!(i <= n));
  }
  for i in 1..nzrow do
    mark(nzloc(i-1) - 1) = 0;
}


def vecset(n, v, iv, nzv, i, val) {
  var set = false;
  for k in 1..nzv {
    if (iv(k - 1) == i) {
      v(k - 1) = val;
      set = true;
    }
  }
  if (!set) {
    nzv += 1;
    v(nzv - 1) = val;
    iv(nzv - 1) = i;
  }
}
