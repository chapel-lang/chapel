use CommDiagnostics;

const x = (2, 3);

startVerboseComm();

extern proc printf(fmt: c_string, x...);

on Locales(1) {
  printf("%s\n", (x(0):string + x(1):string):c_ptrConst(c_char):c_string);
}

printf("%s\n", (x(0):string + x(1):string):c_ptrConst(c_char):c_string);

stopVerboseComm();
