// According to the spec, it is legal to define an enum type within a class.
// However, we seem to have issues with accessing its constant values in
// practice.

proc main() {
  var sampleBug = new unmanaged Sample();
  writeln(sampleBug.tag);
  delete sampleBug;
}


class Sample {


  enum classTag { field1,
                  field2};

  // give it a value.. still error
  var tag: classTag = classTag.field2;
}
