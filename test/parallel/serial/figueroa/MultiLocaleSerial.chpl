use Time;

var syncvar: sync int = 3;

on Locales(1) do
serial do
on Locales(0) do
cobegin {
  writeln ("syncvar is ", syncvar.readXX());
  syncvar.writeXF(17);
}

writeln ("syncvar is now ", syncvar.readFF());
