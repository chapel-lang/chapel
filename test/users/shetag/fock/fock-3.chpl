use taskpool;

config const natom = 5;
const n = (natom/2)*10 + ((natom+1)/2)*5;
const bas_info : [(i,j) in [1..natom, 1..2]] int = 5*(i/2) + 10*((i-1)/2) + if (j==1) then 1 else 10/(i%2+1);

type elemType = real(64);
const matD : domain(2) distributed(Block) = [1..n, 1..n]; 
const dmat : [(i,j) in matD] elemType = 1.0/(i+j); 
var jmat2, kmat2, jmat2T, kmat2T : [matD] elemType; 
var oneAtATime: sync bool = true;

config const numLocs = 100;
config const poolSize = 1;
const t = new taskpool(poolSize);

def buildjk() {
  cobegin {
    coforall loc in 1..numLocs do
      consumer();

    producer();
  }

  cobegin {
    [(i,j) in matD] jmat2T(i,j) = jmat2(j,i);
    [(i,j) in matD] kmat2T(i,j) = kmat2(j,i);
  }

  cobegin {
    jmat2 = (jmat2 + jmat2T) * 2;
    kmat2 += kmat2T;
  }

  writeln("\n1st row of coulomb matrix:-\n", jmat2(1..1,1..n));
  writeln("\n1st col of coulomb matrix:-\n", jmat2(1..n,1..1));
  writeln("\n1st row of exchange matrix:-\n", kmat2(1..1,1..n));
  writeln("\n1st col of exchange matrix:-\n", kmat2(1..n,1..1));
}

def consumer() {
  var bI, copyofbI : blockIndices;
  bI = t.remove();
  while (bI.ilo != 0) {
    copyofbI = bI;
    cobegin {
      buildjk_atom4(copyofbI);
      bI = t.remove();
    }
  }
}

def producer() {
  forall bI in genBlocks() do
    t.add(bI);
}

def genBlocks() {
  forall iat in 1..natom {
    forall jat in 1..iat {
      forall kat in 1..iat {
        const lattop = if (kat==iat) then jat else kat;
        forall lat in 1..lattop {
          yield new blockIndices(bas_info(iat,1), bas_info(iat,2), bas_info(jat,1), bas_info(jat,2), bas_info(kat,1), bas_info(kat,2), bas_info(lat,1), bas_info(lat,2));
        }
      }
    }
  }
  forall loc in 1..numLocs do
    yield new blockIndices(0,0,0,0,0,0,0,0);
}

def buildjk_atom4(bI) {
  const (ilo,ihi,jlo,jhi,klo,khi,llo,lhi) = (bI.ilo,bI.ihi,bI.jlo,bI.jhi,bI.klo,bI.khi,bI.llo,bI.lhi);

  const (ijD,ikD,ilD,jkD,jlD,klD) = ([ilo..ihi,jlo..jhi],[ilo..ihi,klo..khi],[ilo..ihi,llo..lhi],[jlo..jhi,klo..khi],[jlo..jhi,llo..lhi],[klo..khi,llo..lhi]);

  const dij = dmat(ijD);
  const dik = dmat(ikD);
  const dil = dmat(ilD);
  const djk = dmat(jkD);
  const djl = dmat(jlD);
  const dkl = dmat(klD);

  var jij : [ijD] elemType;
  var jkl : [klD] elemType;
  var kik : [ikD] elemType;
  var kil : [ilD] elemType;
  var kjk : [jkD] elemType;
  var kjl : [jlD] elemType;

  var jtop, ktop, ltop : int;
  var facij, facijkl, gijkl: elemType;

  for i in ilo..ihi {
    if (ilo==jlo) then jtop = i; else jtop = jhi;
    for j in jlo..jtop {
      if (i==j) then facij = 0.5; else facij = 1.0;
      if (ilo==klo && jlo==llo) then ktop = i; else ktop = khi;
      for k in klo..ktop {
        ltop = lhi;
        if (klo==llo) then ltop = k;
        if (ilo==klo && jlo==llo && k==i) then ltop = j;
        for l in llo..ltop {
          facijkl = facij;
          if (k==l) then facijkl *= 0.5;
          if (i==k && j==l) then facijkl *= 0.5;
          gijkl = g(i,j,k,l)*facijkl;
          jij(i,j) += dkl(k,l)*gijkl;
          jkl(k,l) += dij(i,j)*gijkl;
          kik(i,k) += djl(j,l)*gijkl;
          kil(i,l) += djk(j,k)*gijkl;
          kjk(j,k) += dil(i,l)*gijkl;
          kjl(j,l) += dik(i,k)*gijkl;
        }
      }
    }
  }

  var tmp = oneAtATime;
  atomic jmat2(ijD) += jij;
  atomic jmat2(klD) += jkl;
  atomic kmat2(ikD) += kik;
  atomic kmat2(ilD) += kil;
  atomic kmat2(jkD) += kjk;
  atomic kmat2(jlD) += kjl;
  oneAtATime = tmp;
}

def g(i,j,k,l) {
  return 1.0/(i*j + k*l);
}

def main() {
  buildjk();
}
