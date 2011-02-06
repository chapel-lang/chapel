use UserMapAssoc;

class myMapper : AbstractMapper {
  proc idxToLocaleInd(ind: real, numlocs: int) {
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }
}

var newDist = new dmap(new UserMapAssoc(idxType=real, initMapper=new myMapper()));

var D: domain(real) dmapped newDist;

D += 1.3;
D += 2.4;
D += 3.5;

writeln("D is: ", D);

