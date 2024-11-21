module Other {
  class CC {}
}
module M {
  import Other;

  class C: Other.CC {}

  proc main() {

  }
}
