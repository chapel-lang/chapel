
proc shouldThrow() throws {
  throw new Error("Encountered an error");
}

export proc callMe() {
  shouldThrow();
}
