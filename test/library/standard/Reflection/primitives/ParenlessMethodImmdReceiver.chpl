
proc int.asdf {}

proc main {
  compilerError(__primitive("resolves", 5.asdf()):string);
}

