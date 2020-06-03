import Module;

proc main() {
  writeln(Module.v);
  writeln(Module.f());

  writeln(Module.SubTwo.subTwoVar);
  writeln(Module.SubTwo.subTwoFn());

  writeln(Module.SubModule.subVar);
  writeln(Module.SubModule.subFn());

  writeln(Module.SubModule.SubSubModule.subSubVar);
  writeln(Module.SubModule.SubSubModule.subSubFn());
}
