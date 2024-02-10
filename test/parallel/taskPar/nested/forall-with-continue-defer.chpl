// An example inspired by #21292
//
// Currently the compiler uses a unified process for placing the bodies of
// defer statements when lowering them and for placing calls to deinitilizers.

forall i in -5..5 {
  if i != 0 then continue;
  defer { writeln("deferred i = ", i); }
}
