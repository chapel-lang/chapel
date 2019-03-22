// This is a simplification of the current code in test/studies/hpcc/RA/diten/,
// with a range shape 0..#numLocs replaced by a domain shape LocaleSpace.
// Any domain/array can be used instead of LocaleSpace/Locales.

module m1 {
  use m2;

  proc main() {
    const totalMem = + reduce physicalMemory3(Locales);
    var myBuckets = new Buckets();
  }

  proc physicalMemory3(arg:locale) return 0;
}

module m2 {
  class Buckets {
    var BucketArray = [idx in LocaleSpace] 777;
  }
}
