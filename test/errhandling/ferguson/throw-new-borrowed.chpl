proc main() throws {
  throw (new owned Error()).borrow();
}
