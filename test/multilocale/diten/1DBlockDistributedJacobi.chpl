config const size = 10;
config const epsilon = 0.0001;

class DistribArrayNode {
  type arrType;
  var size: int;
  var arr: [0..size-1] arrType;
}

class DistribArray {
  type arrType;
  const arrSize: int;
  const localSize:int = arrSize / numLocales;
  var internalArr: [0..numLocales-1] DistribArrayNode(arrType);

  def initialize() {
    for i in 0..numLocales-1 {
      on Locales(i) {
        if (i != numLocales-1) {
          internalArr(i) = new DistribArrayNode(arrType, localSize);
        } else {
          const numElts = arrSize - localSize*(numLocales-1);
          internalArr(i) = new DistribArrayNode(arrType, numElts);
        }
      }
    }
  }

  def element(indexNum: int) var {
    const localeNum = min(indexNum / localSize, numLocales-1);
    const localIndex = indexNum - (localeNum * localSize);
    return internalArr(localeNum).arr(localIndex);
  }

  def getLocales() {
    // One could imagine not distributing it across all locales
    [i in 0..numLocales-1] yield i;
  }
  
  def getLocalIndices() {
    // Get the indices that live on this locale
    var myID = localeID();
    var size = if myID == numLocales-1
                 then arrSize-localSize*(numLocales-1)
                 else localSize;

    for i in 0..size-1 {
      yield myID*localSize + i;
    }
  }

  def writeThis(w: Writer) {
    on Locales(0) {
      for i in 0..arrSize-1 {
        w.writeln(element(i));
      }
    }
  }

  def copy(b: DistribArray(arrType)) {
    if (b.arrSize != arrSize) then
      halt("Bad sizes in DistribArray.copy");

    forall loc in getLocales() {
      on Locales(loc) {
        // hope that most indices match locale between the arrays
        // With this DistribArray class, they are.
        forall i in getLocalIndices() {
          this.element(i) = b.element(i);
        }
      }
    }
  }
}

def main {
  var A = new DistribArray(real, size);
  var B = new DistribArray(real, size);
  var delta: real;

  A.element(0) = 1.0;

  do {
    delta = 0.0;
    coforall loc in A.getLocales() {
      on Locales(loc) {
        var localDelta: real;
        for i in A.getLocalIndices() {
          if (i == 0) {
            // Pin the low end at 1.0
            B.element(i) = 1.0;
          } else if (i == size-1) {
            B.element(i) = A.element(i-1)/2.0;
          } else {
            B.element(i) = (A.element(i-1) + A.element(i+1))/2.0;
          }
          localDelta = max(localDelta, abs(B.element(i) - A.element(i)));
        }
        delta = max(delta, localDelta);
      }
    }
    
    A.copy(B);
  } while (delta > epsilon);

  write(A);
}

