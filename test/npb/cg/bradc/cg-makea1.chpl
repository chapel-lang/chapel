use Random;

enum classVals {S, W, A, B, C, D, O};


config const probClass = S;

const Class: domain(classVals);
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


type elemType = real(64);


def main() {
  writeln("Chapel sparsity pattern is:");
  for ((r,c), v) in makea() {
    writeln(" ", r, " ", c, "  ", v);
  }
}


iterator makea() {
  var v: [1..nonzer+1] real,    // BLC: insert domains? or grow as necessary?
      iv: [1..nonzer+1] int;
  
  var size = 1.0;
  const ratio = rcond ** (1.0 / n);

  var randStr = RandomStream(314159265);

  for iouter in 1..n {
    var nzv = nonzer;

    sprnvc(n, nzv, v, iv, randStr);
    vecset(v, iv, nzv, iouter, 0.50);

    // BLC: replace with zippered loop over iv or iv(1..nzv)?
    for ivelt in 1..nzv {
      const jcol = iv(ivelt),
            scale = size * v(ivelt);

      // BLC: replace with zippered loop over iv or iv(1..nzv)?
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


def sprnvc(n, nz, v, iv, randStr) {
  var nn1 = 1;
  while (nn1 < n) do nn1 *= 2;

  var indices: domain(int);

  for nzv in 1..nz {
    var vecelt: real, i: int;
    do {
      vecelt = randStr.getNext();
      var vecloc = randStr.getNext(); 
      i = (vecloc * nn1):int + 1;
    } while (i > n || indices.member?(i));
    indices += i;
    v(nzv) = vecelt;
    iv(nzv) = i;
  }
}


def vecset(v, iv, inout nzv, i, val) {
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
