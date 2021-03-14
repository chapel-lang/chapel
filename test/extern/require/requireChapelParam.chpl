config param filestem = "fooM";

param filename = filestem + ".chpl";

require filename;

proc main() {
  foo();
}
