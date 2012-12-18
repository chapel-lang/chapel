use CommDiagnostics;

const x = (2, 3);

startVerboseComm();

extern proc printf(x...);

on Locales(1) {
  printf("%s\n", x(1):string + x(2));
}

printf("%s\n", x(1):string + x(2));

stopVerboseComm();
