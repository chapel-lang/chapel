use CTypes;

extern type char = c_char;
if c_sizeof(char) != c_sizeof(c_long) {
  warning("C sizes don't match in size (as expected)");
}
