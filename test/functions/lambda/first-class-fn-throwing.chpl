proc raiseError() throws {
  throw new owned IllegalArgumentError();
}

var temp = raiseError;
try { temp(); }
catch { writeln('Caught an error'); }

