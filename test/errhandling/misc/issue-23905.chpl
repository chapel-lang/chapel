proc main() {
  func();
}

proc func() throws {
  coforall loc in Locales {
    on loc {
      try {
        forall i in 0..8 {
          throw new Error("in forall");
        }
      } catch e {
        throw e;
      }
    }
  }
}
