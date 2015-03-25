use CommDiagnostics;

const x = (2, 3);

startVerboseComm();

extern proc printf(fmt: c_string, x...);

on Locales(1) {
  printf("%s\n", (x(1):string + x(2)).c_str());
}

printf("%s\n", (x(1):string + x(2)).c_str());

stopVerboseComm();
