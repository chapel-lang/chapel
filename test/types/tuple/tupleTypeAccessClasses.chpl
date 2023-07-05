class Student { }
class BadStudent: Student { }
class GoodStudent: Student { }
class ExcellentStudent: Student { }

type acceptableStudentTypes =
  (borrowed GoodStudent, borrowed ExcellentStudent);
  // currently has to be concrete, see #10172

class AdvancedBasketWeaving {
  proc accept(student: borrowed Student) {
    for param i in 0..acceptableStudentTypes.size-1 {
      if student: acceptableStudentTypes(i)? != nil {
        return "YES!";
      }
    }
    return "Computer says 'No'";
  }
}

var course = new owned AdvancedBasketWeaving();
writeln(course.accept(new owned Student()));
writeln(course.accept(new owned BadStudent()));
writeln(course.accept(new owned GoodStudent()));
writeln(course.accept(new owned ExcellentStudent()));
