use CTypes;
extern proc printf(fmt:c_ptrConst(c_char)) where true;

proc main() {
  printf("test\n");
}
