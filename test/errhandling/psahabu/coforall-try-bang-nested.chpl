module Test {
  proc throwme() throws {
    throw new owned Error();
  }

  proc test() {
    coforall i in 1..10 {
      try! {
        try {
          throwme();
        }
      }
    }
  }

  test();
}
