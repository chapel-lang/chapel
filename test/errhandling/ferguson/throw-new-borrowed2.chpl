proc main() throws {
  var x = new Error(); // aka new owned Error
  throw x;
}
