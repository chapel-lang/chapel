// TaskErrors uses an ordering among the errors it contains
// to determine the first and the last error to show in its message().
// Currently the ordering is based on message() of individual errors.
//
// This test requests the ordering to be based on chpl_describe_error()
// of the individual errors. Rationale: those are the strings reflected
// in TaskErrors.message(). For example, consider a TaskErrors that contains
// three errors: Error1, Error2, Error1, in this order, each with an empty
// message(). Currently TaskErrors.message() contains "Error1: ... Error1:".
// Instead, it makes more sense if it contained "Error1: ... Error2:".

var te1: chpl_TaskErrors;
te1.append(new unmanaged Error());
te1.append(new unmanaged IllegalArgumentError());
te1.append(new unmanaged Error());
var TE1 = new TaskErrors(te1);
writeln(TE1);

// Even when individual errors contain messages, the sorted order
// that reflects the contents of TaskErrors.message() makes more sense.
var te2: chpl_TaskErrors;
te2.append(new unmanaged Error("aaa"));
te2.append(new unmanaged IllegalArgumentError("bbb"));
te2.append(new unmanaged Error("ccc"));
var TE2 = new TaskErrors(te2);
writeln(TE2);
