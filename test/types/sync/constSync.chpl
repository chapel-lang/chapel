config param testNum = 1;
const foo: sync int = 1;

select testNum {
  when 1 {
    foo.writeEF(2);
  }
  when 2 {
    foo.writeFF(2);
  }
  when 3 {
    foo.writeXF(2);
  }
  when 4 {
    foo.reset();
  }
  otherwise halt("Invalid test number");
}
