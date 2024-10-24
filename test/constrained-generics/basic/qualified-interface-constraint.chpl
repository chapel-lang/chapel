module Other {
  class CC {}
  interface II {}
}

module M {
  import Other;
  class C: Other.CC {}
  record R: Other.II {}

  proc main() {}
}
