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
