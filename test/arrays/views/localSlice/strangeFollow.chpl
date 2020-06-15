
use BlockDist;

proc main() {
  var D = {1..10, 1..10};
  var S = D dmapped Block(D);
  var A : [S] int;

  assert(numLocales >= 2, "test requires at least two locales");

  for loc in Locales do on loc {
    writeln("Testing ", here, "...");
    // This doesn't work at the moment because the leader expression will yield
    // a 'followThis' value that has the same shape as the local subdomain but
    // is densified and translated to the origin (.low is zeroes).
    //
    // The "A[local]" slice currently assumes that whatever indices it recieves
    // can be accessed locally, and will attempt to do so. This may work for
    // the first locale, but subsequent locales will fail because the
    // "A[local]" follower will attempt to access elements on the first locale.
    forall (x, y) in zip(A[A.localSubdomain()], A[local]) {
      assert(x == y);
    }
    writeln(here, ": SUCCESS");
  }
}
