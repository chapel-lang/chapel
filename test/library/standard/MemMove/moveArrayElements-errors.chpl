
use MemMove;

enum modes {
  dstNotRect,
  srcNotRect,
  notDomOrRange,
  srcRegionNotRect,
  dstRegionNotRect,
  range2D,
  srcIdxType,
  dstIdxType,
  eltType,
  sizeMismatch,
  dstBadInds,
  srcBadInds,
  unboundRange,
}

config param mode : modes = modes.dstNotRect;

proc indsWrapper(dst, dstRegion, src, srcRegion) {
  try {
    moveArrayElements(dst, dstRegion, src, srcRegion);
  } catch e: IllegalArgumentError {
    writeln("IllegalArgumentError: ", e.message());
  } catch {
    writeln("Unknown error!");
  }
}

proc main() {
  var dstRectDom = {1..100};
  var srcRectDom = {1..100};
  var dstRect : [dstRectDom] int;
  var srcRect : [srcRectDom] int;

  var dstAssocDom = {1, 4, 8, 42};
  var srcAssocDom = {1, 4, 8, 42};
  var dstAssoc : [dstAssocDom] int;
  var srcAssoc : [srcAssocDom] int;

  var inds = [i in 1..100 by 2] i;

  var dst2D : [1..10, 1..10] int;

  var uintIdx = {1:uint..100:uint};

  var srcString : [srcRectDom] string;

  var half = {1..50};

  use modes;
  select mode {
    when dstNotRect do moveArrayElements(dstAssoc, srcRect);
    when srcNotRect do moveArrayElements(dstRect, srcAssoc);
    when notDomOrRange do moveArrayElements(dstRect, dstRectDom,
                                            srcRect, inds);
    when srcRegionNotRect do moveArrayElements(dstRect, dstRectDom,
                                               srcRect, srcAssocDom);
    when dstRegionNotRect do moveArrayElements(dstRect, dstAssocDom,
                                               srcRect, srcRectDom);
    when range2D do moveArrayElements(dst2D, 1..10, dst2D, 21..30);
    when srcIdxType do moveArrayElements(dstRect, dstRectDom,
                                         srcRect, uintIdx);
    when dstIdxType do moveArrayElements(dstRect, uintIdx,
                                         srcRect, srcRectDom);
    when eltType do moveArrayElements(dstRect, dstRectDom,
                                      srcString, srcRectDom);
    when sizeMismatch do moveArrayElements(dstRect, 1..5, srcRect, 1..6);
    when dstBadInds {
      indsWrapper(dstRect, 0..#50, srcRect, half);
      indsWrapper(dstRect, {..101 # -50}, srcRect, half);
    }
    when srcBadInds { 
      indsWrapper(dstRect, half, srcRect, 0..#50);
      indsWrapper(dstRect, half, srcRect, {..101 # -50});
    } when unboundRange {
      moveArrayElements(dstRect, 1..100, srcRect, 1..);
    }
  }
}
