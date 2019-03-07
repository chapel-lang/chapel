proc emptyNoDocNoArg() throws {

}

proc emptyNoDocArg(x: int) throws {

}

proc noDocNoArg() throws {
  throw new owned Error();
}

proc noDocArg(b: bool) throws {
  throw new owned Error();
}

proc noDocReturn(): int throws {

}

/* This function has no body or argument, and throws */
proc emptyNoArg() throws {

}

/* This function has no body, has an argument, and throws */
proc emptyWithArg(x: int) throws {

}

/* This function has a body, no argument, and throws */
proc noArg() throws {
  throw new owned Error();
}

/* This function has a body, an argument, and throws */
proc hasArg(b: bool) throws {
  writeln("I don't actually throw.  But I could!");
}

/* This function declares a return and throws */
proc hasDocReturn(): string throws {

}

module Inner {
  proc emptyNoDocNoArg() throws {

  }

  proc emptyNoDocArg(x: int) throws {

  }

  proc noDocNoArg() throws {
    throw new owned Error();
  }

  proc noDocArg(b: bool) throws {
    throw new owned Error();
  }

  proc noDocReturn(): int throws {

  }

  /* This function has no body or argument, and throws */
  proc emptyNoArg() throws {

  }

  /* This function has no body, has an argument, and throws */
  proc emptyWithArg(x: int) throws {

  }

  /* This function has a body, no argument, and throws */
  proc noArg() throws {
    throw new owned Error();
  }

  /* This function has a body, an argument, and throws */
  proc hasArg(b: bool) throws {
    writeln("I don't actually throw.  But I could!");
  }

  /* This function declares a return and throws */
  proc hasDocReturn(): string throws {

  }
}
