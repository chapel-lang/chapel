var aaa: [1..1] int;
var bbb: DefaultRectangularArr(int, 1, int, false);

proc storecache(ref cache) {
  on here {
    ref slice = aaa;
    cache = slice._value;
  }
}

storecache(bbb);
