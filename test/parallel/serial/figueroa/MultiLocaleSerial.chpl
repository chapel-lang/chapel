use Time;

var syncvar: sync int = 3,
    singlevar1: single int,
    singlevar2: single int;

begin {sleep(singlevar1.readFF():uint); singlevar2.writeEF(syncvar.readFE() + 10);}

on Locales(1) do
serial do
on Locales(0) do
cobegin {
  {singlevar1.writeEF(5); syncvar.writeEF(singlevar2.readFF());}
  writeln ("syncvar is ", syncvar.readXX());
  syncvar.writeXF(17);
}

writeln ("syncvar is now ", syncvar.readFF());
