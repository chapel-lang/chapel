import ImplicitModule;

proc main() {
  writeln(ImplicitModule.v);
  writeln(ImplicitModule.f());

  writeln(ImplicitModule.SubModule.subVar);
  writeln(ImplicitModule.SubModule.subFn());

  writeln(ImplicitModule.SubModule.SubSubModule.subSubVar);
  writeln(ImplicitModule.SubModule.SubSubModule.subSubFn());
}
