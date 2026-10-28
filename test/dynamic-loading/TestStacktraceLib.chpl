
proc shouldHalt() {
  halt("This is a test of the stack trace.");
}

proc anotherLayer() {
  shouldHalt();
}

export proc callMe() {
  anotherLayer();
}
