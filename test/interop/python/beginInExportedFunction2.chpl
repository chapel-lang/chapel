use Time;

proc sleepTest() {
  sleep(1);
  return;
}

// Test with sleep...
export proc hello4() {
  writeln('Hello...');
  begin sleepTest();
  writeln('...World!');
  return;
}
