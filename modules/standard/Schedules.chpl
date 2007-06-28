def BlockSchedule(D, numBlocks) {
  const lo = D.dim(1)._low;
  const hi = D.dim(1)._high;
  const numelems = hi - lo + 1;
  const nbAsIndexType = numBlocks:D.dim_type;
  forall i in 0..nbAsIndexType-1 {
    const blo = lo + (i*numelems) / nbAsIndexType;
    const bhi = lo + (((i+1)*numelems) / nbAsIndexType) - 1;
    yield [blo..bhi];
  }
}


def FixedBlockSchedule(D, blockSize) {
  const lo = D.dim(1)._low;
  const hi = D.dim(1)._high;
  const numelems = hi - lo + 1;
  forall i in 0..ceil(numelems:real/blockSize):int - 1 {
    const blo = lo + i*blockSize;
    const bhi = min(lo + (i+1)*blockSize-1, hi);
    yield [blo..bhi];
  }
}
