use CPtr;
use SysCTypes;

type buf_t = c_char;

extern proc chpl_task_idToString(buf: c_void_ptr,
                                 size: size_t,
                                 id: chpl_taskID_t): c_string;
extern proc chpl_task_getId(): chpl_taskID_t;

config const bufLen = 21;

proc main() {
  proc showMe(what) {
    var id = chpl_task_getId();
    var buf: [1..bufLen] buf_t;
    var idStr = chpl_task_idToString(c_ptrTo(buf), buf.size:size_t, id);
    writeln('task ID of ', what, ' is: ',
            if idStr==c_nil:c_string then '<OVF>'
                                     else createStringWithNewBuffer(idStr));
  }

  showMe('main()');
  sync begin showMe('task body');
}

