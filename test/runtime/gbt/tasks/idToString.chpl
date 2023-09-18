
use CTypes;

type buf_t = c_char;

extern proc chpl_task_idToString(buf: c_ptr(void),
                                 size: c_size_t,
                                 id: chpl_taskID_t): c_ptrConst(c_char);
extern proc chpl_task_getId(): chpl_taskID_t;

config const bufLen = 21;

proc main() {
  proc showMe(what) {
    var id = chpl_task_getId();
    var buf: [1..bufLen] buf_t;
    var idStr = chpl_task_idToString(c_ptrTo(buf), buf.size:c_size_t, id);
    writeln('task ID of ', what, ' is: ',
            if idStr==nil:c_ptrConst(c_char) then '<OVF>'
                                     else string.createCopyingBuffer(idStr));
  }

  showMe('main()');
  sync begin showMe('task body');
}

