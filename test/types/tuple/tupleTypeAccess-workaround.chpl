class Student {
}

class BadStudent: Student {
}

class GoodStudent: Student {
}

class ExcellentStudent: Student {
}

class AdvancedBasketWeaving {
  var acceptableStudentTypes = (new borrowed GoodStudent(), new borrowed ExcellentStudent());

  proc accept(student: borrowed Student) {
    for param i in 0..acceptableStudentTypes.size-1 {
      if student: acceptableStudentTypes(i).type? != nil then return "YES!";
    }
    return "Computer says 'No'";
  }
}

var course = new owned AdvancedBasketWeaving();
writeln(course.accept(new owned Student()));
writeln(course.accept(new owned BadStudent()));
writeln(course.accept(new owned GoodStudent()));
writeln(course.accept(new owned ExcellentStudent()));
