use BlockDist;
use CPtr;

const OnePerLocale = LocaleSpace dmapped Block(LocaleSpace);

class BasisStates {
  var size : int;
  var representatives : [OnePerLocale] [0 ..# size] uint(64);
}

proc foo(const ref basisStates : BasisStates)
{
  coforall loc in Locales do on loc {
    writeln(basisStates.representatives[loc.id][0 ..# 1]);
  }
}

proc main()
{
  var representatives : [OnePerLocale] [0 ..# 13] uint(64);

  for subarr in representatives {
    subarr = [31, 47, 55, 87, 91, 93, 103, 107, 155, 171, 173, 179, 341]:uint;
  }

  var states = new BasisStates(13, representatives);
  foo(states);
}
