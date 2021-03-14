import MultipleModules;
import OtherModule;

proc main() {
  writeln(MultipleModules.SubModule.subVar);
  writeln(MultipleModules.SubModule.subFn());

  writeln(OtherModule.otherVar);
  writeln(OtherModule.otherFn());
}
