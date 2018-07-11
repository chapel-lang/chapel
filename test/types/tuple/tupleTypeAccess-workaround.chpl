class Student {
}

class BadStudent: Student {
}

class GoodStudent: Student {
}

class ExcellentStudent: Student {
}

class AdvancedBasketWeaving {
  var acceptableStudentTypes = (new GoodStudent(), new ExcellentStudent());

  proc accept(student: Student) {
    for param i in 1..acceptableStudentTypes.size {
      if student: acceptableStudentTypes(i).type != nil then return "YES!";
    }
    return "Computer says 'No'";
  }

  proc deinit() {
    for param i in 1..acceptableStudentTypes.size do
      delete acceptableStudentTypes(i);
  }
}

var course = new owned AdvancedBasketWeaving();
writeln(course.accept(new owned Student()));
writeln(course.accept(new owned BadStudent()));
writeln(course.accept(new owned GoodStudent()));
writeln(course.accept(new owned ExcellentStudent()));
