// Use non-prototype module to ensure strict nilability checking.
module w {

  class CC {
    type tField;;
  }

  type RT = Locales.type;

  type tbb = borrowed CC(RT);
  // it would work if 'tbb' were defined as: var tbb = new borrowed CC(RT);
  var abb: tbb.tField;
  assert(abb.dom == Locales.dom);

  // Also do the above for the other management + nilability
  // variants of CC(RT), ex.: type tbq = borrowed CC(RT)?; ...
}
