class Student {
}

class BadStudent: Student {
}

class GoodStudent: Student {
}

class ExcellentStudent: Student {
}

class AdvancedBasketWeaving {
  type acceptableStudentTypes = (GoodStudent, ExcellentStudent);

  proc accept(student: Student) {
    for param i in 1..acceptableStudentTypes.size do
      if student: acceptableStudentTypes(i) != nil then
        return "YES!";
    return "Computer says 'No'";
  }
}

var course = new owned AdvancedBasketWeaving();
writeln(course.accept(new owned Student()));
writeln(course.accept(new owned BadStudent()));
writeln(course.accept(new owned GoodStudent()));
writeln(course.accept(new owned ExcellentStudent()));
