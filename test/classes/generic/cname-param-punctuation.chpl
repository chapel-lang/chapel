class C {
  param tag;
  type valueType;
  var value: valueType;

  proc show() {
    writeln(tag, ": ", value);
  }
}

proc main() {
  var question = new owned C("question?", int, 42);
  var punctuation = new owned C("slash\\quote\"[]", int, 17);
  param longTag = "aaaaaaaaaaaaaaaaaaaaaaaaa" +
                  "aaaaaaaaaaaaaaaaaaaaaaaaa" +
                  "aaaaaaaaaaaaaaaaaaaaaaaaa" +
                  "aaaaaaaaaaaaaaaaaaaaaaaaa" +
                  "aaaaaaaaaaaaaaaaaaaaaaaaa" + "\\tail";
  var truncated = new owned C(longTag, int, 7);
  question.show();
  punctuation.show();
  writeln(question.type:string);
  writeln(punctuation.type:string);
  writeln(truncated.value);
}
