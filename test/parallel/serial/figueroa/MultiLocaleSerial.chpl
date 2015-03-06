use Time;

var syncvar: sync int = 3,
    singlevar1: single int,
    singlevar2: single int;

begin {sleep(singlevar1:uint); singlevar2 = syncvar + 10;}

on Locales(1) do
serial do
on Locales(0) do
cobegin {
  {singlevar1 = 5; syncvar = singlevar2;}
  writeln ("syncvar is ", syncvar.readXX());
  syncvar.writeXF(17);
}

writeln ("syncvar is now ", syncvar.readFF());
