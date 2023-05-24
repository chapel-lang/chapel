use CommDiagnostics, CTypes;

const x = (2, 3);

startVerboseComm();

extern proc printf(fmt: c_string, x...);

on Locales(1) {
  printf("%s\n", c_ptrToConst_helper(x(0):string + x(1):string):c_string);
}

printf("%s\n", c_ptrToConst_helper(x(0):string + x(1):string):c_string);

stopVerboseComm();
