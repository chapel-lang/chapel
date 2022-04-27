// #14148 - code from the first comment
// slimmed down to a small reproducer

class Row { }

iter fetchAll(): Row {
  yield new Row();
}

for idx in fetchAll() {
  compilerError(idx.type:string);
}
