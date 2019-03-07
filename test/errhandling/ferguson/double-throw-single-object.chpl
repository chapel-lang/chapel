proc main() {
  try {
    var ex = new owned Error();
    forall i in 0..1 {
      throw ex;
    }
  }
  catch e {
    writeln(e);
  }
}
