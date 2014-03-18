pragma "no use ChapelStandard"
module ChapelDynDispHack {
  //
  // This is an incredibly lame hack.  If the test:
  //
  // test/classes/bradc/arrayInClass/arrayDomInClassRecord-simpler.chpl
  //
  // works with the following line removed then it sounds like we've
  // moved to a better world than the one in which I committed this,
  // and we can remove this check.  See the comments in that test for
  // more details.
  //
  assert (here.id >= 0);
}