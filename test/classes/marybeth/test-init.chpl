
class TestRandom {

  const timeofday = 4531;
  const seed = if (timeofday%2) then (timeofday) else (timeofday + 1);

  proc showseed() {
    writeln(seed);
  }
}

var randlist = new owned TestRandom();

randlist.showseed();
