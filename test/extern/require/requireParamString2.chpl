config param filestem = "foo";

param filename = filestem + ".h";

require filename;

extern proc foo();

proc main() {
  foo();
}

