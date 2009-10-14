const x = (2, 3);

startVerboseComm();

on Locales(1) {
  _debugWriteln(x(1), x(2));
}

_debugWriteln(x(1), x(2));

stopVerboseComm();
