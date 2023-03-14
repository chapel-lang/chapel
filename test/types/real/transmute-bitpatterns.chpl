proc testTransmute(x: uint(?w)) {
  const val = x;
  write(val, ": ", val.type:string, ": ");
  const r = val.transmute(real(w));
  const ui = r.transmute(uint(w));
  if ui == val {
    writeln("OK");
  } else {
    writeln("Uh-oh: ", ui, " != ", val,  " via real ", r);
  }
}

testTransmute(0:uint(32));
testTransmute(0xffffffff:uint(32));
testTransmute(0xAAAAAAAA:uint(32));
testTransmute(0x55555555:uint(32));

testTransmute(0:uint(64));
testTransmute(0xffffffffffffffff:uint(64));
testTransmute(0xAAAAAAAAAAAAAAAA:uint(64));
testTransmute(0x5555555555555555:uint(64));
