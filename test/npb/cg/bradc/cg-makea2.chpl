use Random;

enum classVals {S, W, A, B, C, D, O};


config const probClass = S;


const Class: domain(1) = [S..O];
const probSizes:   [Class] int = (/ 1400, 7000, 14000, 75000, 150000, 150000, 1400 /),
      nonZeroes:   [Class] int = (/ 7, 8, 11, 13, 15, 21, 7 /),
      shifts:      [Class] int = (/ 10, 12, 20, 60, 110, 500, 10 /),
      numIters:    [Class] int = (/ 15, 15, 15, 75, 75, 100, 15 /),
      verifyZetas: [Class] real = (/ 8.5971775078648, 10.362595087124,
                                    17.130235054029, 22.712745482631,
                                    28.973605592845, 52.5145321058, 0.0 /);

config const n = probSizes(probClass),
             rcond = 0.1,
             nonzer = nonZeroes(probClass),
             shift = shifts(probClass);


const nonzer_max = 16,
      naMax = 150000;


type elemType = real(64);


def main() {
  makea();
//  for res in makea() {
//    writeln("res is: ", res);
//  }
}


def makea() {
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

        writeln("want to yield: ", irow, " ", jcol, " ", v(ivelt1)*scale);
      }
    }
    size *= ratio;
  }

  for i in 1..n {
    writeln("want to yield: ", i, " ", i, " ", rcond - shift);
  }
}


def sprnvc(n, nz, v, iv, nzloc, mark) {
  var randStr = RandomStream(314159265);

  const zeta = randStr.getFirst();

  const nn1 = log2(n);

  var nzv = 0,
      nzrow = 0;

  while (nzv < nz) {
    do {
      const vecelt = randStr.getNext(),
            i = (randStr.getNext() * nn1):int + 1;
      if (i <= n && mark(i) == 0) {
        mark(i) = 1;
        nzrow += 1;
        nzloc(nzrow) = i;
        nzv += 1;
        v(nzv) = vecelt;
        iv(nzv) = i;
      }
    } while (!(i <= n));
  }
  for i in 1..nzrow do
    mark(nzloc(i)) = 0;
}


def vecset(n, v, iv, inout nzv, i, val) {
  var set = false;
  for k in 1..nzv {
    if (iv(k) == i) {
      v(k) = val;
      set = true;
    }
  }
  if (!set) {
    nzv += 1;
    v(nzv) = val;
    iv(nzv) = i;
  }
}
