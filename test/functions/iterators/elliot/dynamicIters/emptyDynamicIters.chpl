use DynamicIters;

proc testIters(iterand) {
  for    i in iterand                              do writeln(i);
  forall i in iterand                              do writeln(i);
  forall (i, j) in zip(iterand, iterand)           do writeln(i);

  for    i in dynamic(iterand)                     do writeln(i);
  forall i in dynamic(iterand)                     do writeln(i);
  forall (i, j) in zip(dynamic(iterand), iterand)  do writeln(i);

  for    i in guided(iterand)                      do writeln(i);
  forall i in guided(iterand)                      do writeln(i);
  forall (i, j) in zip(guided(iterand), iterand)   do writeln(i);

  for    i in adaptive(iterand)                    do writeln(i);
  forall i in adaptive(iterand)                    do writeln(i);
  forall (i, j) in zip(adaptive(iterand), iterand) do writeln(i);
}

// test iteration over empty ranges and domains
testIters(1..0);
testIters({1..0});
