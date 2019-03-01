proc main() throws {
  var x = new Error(); // aka new borrowed Error
  throw x;
}
