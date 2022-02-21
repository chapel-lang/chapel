module M {
  class X {    
    proc Y() {
      if (true) {
        if (true) {
        } else {}
      }

      // Must be `override`!
      override proc Z() {}
    }
  }
}
