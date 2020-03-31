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
  var internalArr: [LocaleSpace] unmanaged DistribArrayNode(arrType)?;
  var isLocalize = false;

  proc postinit() {
    if isLocalize then return;

    for loc in LocaleSpace {
      on Locales(loc) {
        if (loc != numLocales-1) {
          internalArr(loc) = new unmanaged DistribArrayNode(arrType, localSize);
        } else {
          const numElts = arrSize - localSize*(numLocales-1);
          internalArr(loc) = new unmanaged DistribArrayNode(arrType, numElts);
        }
      }
    }
  }

  proc deinit() {
    if isLocalize then return;
    for loc in LocaleSpace {
      on Locales(loc) {
        delete internalArr(loc);
      }
    }
  }

  proc localize() {
    var localArrays: [LocaleSpace] unmanaged DistribArray(arrType)?;
    for loc in LocaleSpace {
      on Locales(loc) {
        var x = new unmanaged DistribArray(arrType, arrSize, localSize, isLocalize=true);
        [i in LocaleSpace] x.internalArr(i) = internalArr(i);
        localArrays(loc) = x;
      }
    }
    return localArrays!;
  }

  proc element(indexNum: int) ref {
    const localeNum = min(indexNum / localSize, numLocales-1);
    const localIndex = indexNum - (localeNum * localSize);
    return internalArr(localeNum)!.arr(localIndex);
  }

  iter getLocales() {
    // One could imagine not distributing it across all locales
    for i in LocaleSpace do yield i;
  }
  
  iter getLocalIndices() {
    // Get the indices that live on this locale
    var myID = here.id;
    var size = if myID == numLocales-1
                 then arrSize-localSize*(numLocales-1)
                 else localSize;

    for i in 0..size-1 {
      yield myID*localSize + i;
    }
  }

  proc writeThis(w) throws {
    on Locales(0) {
      for i in 0..arrSize-1 {
        w.writeln(element(i));
      }
    }
  }

  proc copy(b: unmanaged DistribArray(arrType)) {
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

proc main {
  var delta: sync real;
  var distA = new unmanaged DistribArray(real, size);
  var distB = new unmanaged DistribArray(real, size);
  ref localAs = distA.localize();
  ref localBs = distB.localize();
  localAs(0).element(0) = 1.0;

  do {
    delta = 0.0;
    coforall loc in localAs(0).getLocales() {
      on Locales(loc) {
        var localDelta: real;
        var A = localAs(loc);
        var B = localBs(loc);
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
        var deltaTmp = delta;
        delta = max(deltaTmp, localDelta);
      }
    }
    localAs <=> localBs;
  } while (delta > epsilon);

  write(localAs(0));

  delete distA, distB;
  for a in localAs do delete a;
  for b in localBs do delete b;
}

