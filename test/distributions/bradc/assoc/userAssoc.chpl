use UserMapAssoc;

class myMapper : AbstractMapper {
  def ind2locInd(ind: real, numlocs: int) {
    const indAsInt = ind: int;
    return indAsInt % numlocs;
  }

  def ind2slot(ind: real, numslots: int) {
    const indAsInt = ind: int;
    ind -= indAsInt;
    ind *= 100;
    return ind % numslots;
  }
}

var newDist = new UserMapAssoc(idxType=real, initMapper=new myMapper());

var D: domain(real) distributed newDist;

D += 1.3;
D += 2.4;
D += 3.5;

writeln("D is: ", D);

