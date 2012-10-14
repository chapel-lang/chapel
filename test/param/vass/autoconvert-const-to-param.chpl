config param testParam = true;

config const x = 1; // not known at compile time

proc test(param useParam) {
  const y = if useParam then -1 else x;
  if y > 0 then
    compilerWarning("here-1");
  else
    compilerWarning("here-2");
}

test(testParam);
compilerError("done"); // skip C compilation - for faster testing
