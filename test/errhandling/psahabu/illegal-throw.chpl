proc illegalThrow() {
  throw new owned Error();
}

illegalThrow();
