use Time;

// Print "Hello world!" using a begin.
export proc hello1() {
  begin writeln('Hello world!');
  return;
}

// Test with coforall...
export proc hello2() {
  coforall i in 1..4 do writeln('Hello world!');
  return;
}

// Test with forall...
export proc hello3() {
  forall i in 1..4 do writeln('Hello world!');
  return;
}

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

proc recursiveBegin(level=0) {
  if level >= 4 {
    begin writeln('Hello there!');
    return;
  } else {
    begin recursiveBegin(level+1);
  }
}

export proc hello5() {
  recursiveBegin();
}

