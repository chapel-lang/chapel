import ImplicitModule;

proc main() {
  writeln(ImplicitModule.v);
  writeln(ImplicitModule.f());

  writeln(ImplicitModule.SubTwo.subTwoVar);
  writeln(ImplicitModule.SubTwo.subTwoFn());

  writeln(ImplicitModule.SubModule.subVar);
  writeln(ImplicitModule.SubModule.subFn());

  writeln(ImplicitModule.SubModule.SubSubModule.subSubVar);
  writeln(ImplicitModule.SubModule.SubSubModule.subSubFn());
}
