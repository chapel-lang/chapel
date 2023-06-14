var aaa: [1..1] int;
var bbb: unmanaged DefaultRectangularArr(eltType=int, rank=1, idxType=int, strides=strideKind.one)?;

proc storecache(ref cache) {
  on here {
    ref slice = aaa;
    cache = slice._value;
  }
}

storecache(bbb);
