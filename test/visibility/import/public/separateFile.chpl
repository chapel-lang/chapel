module separateFile {
  public import separateFileSrc.Inner;

  proc main() {
    writeln(Inner.innerX);
  }
}
