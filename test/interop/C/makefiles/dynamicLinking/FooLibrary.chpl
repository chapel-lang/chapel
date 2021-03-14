// Expose one function so we can put the dynamic linker through its paces.

proc hiddenDragon(): string {
  return "The possession of anything begins in the mind.";
}

proc crouchingTiger(): string {
  return hiddenDragon();
}

export proc wisdom() {
  writeln(crouchingTiger());
}
