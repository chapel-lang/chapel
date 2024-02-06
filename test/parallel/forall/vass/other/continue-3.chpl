
forall i in -5..5 {
  if i != 0 then continue;
  defer { writeln("deferred i = ", i); }
}
