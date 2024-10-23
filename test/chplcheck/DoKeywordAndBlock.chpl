module DoKeywordAndBlock {
  for i in 1..10 do {
    writeln(i);
  }

  for i in 1..10 do writeln(i);

  for i in 1..10 {
    writeln(i);
  }

  var A: [1..10] int;

  @chplcheck.ignore("UnusedLoopIndex")
  forall i in 1..10 with (ref A) do { A; }

  @chplcheck.ignore("UnusedLoopIndex")
  forall i in 1..10 with (ref A) do
  { A; }

  @chplcheck.ignore("UnusedLoopIndex")
  forall i in 1..10 with (ref A) do {
    A;
  }

  @chplcheck.ignore("UnusedLoopIndex")
  for i in 1..10 do {}

  @chplcheck.ignore("UnusedLoopIndex")
  for i in 1..10 do
  {}

  @chplcheck.ignore("UnusedLoopIndex")
  for i in 1..10  do   {

  }


  var mydo: int;
  // make sure `mydo` does not become `do`
  forall 1..10 with (ref mydo ) do {
    mydo = 1;
  }

  for 1..10 do{}
  for 1..10
  do{}
  for 1..10
    do {}
  for 1..10 do{
  }
  forall 1..10 with (ref A)do{ A; }
  forall 1..10 with (ref A)do { A; }

  on Locales[0] do {}

  var a = 1;
  select a {
    when 1 do {}
    when 2 do {}
    when 3 do {}
  }

  local do {}
  serial do {}
  while true do {}

  record R: contextManager {
    proc enterContext() {}
    @chplcheck.ignore("UnusedFormal")
    proc exitContext(in err) {}
  }
  manage new R() do {}
}
