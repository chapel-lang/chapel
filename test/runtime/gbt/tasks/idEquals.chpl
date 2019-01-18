extern proc chpl_task_getId(): chpl_taskID_t;
extern proc chpl_task_idEquals(id1: chpl_taskID_t, id2: chpl_taskID_t): bool;

proc main() {
  var mainId = chpl_task_getId();
  sync begin writeln(if chpl_task_idEquals(chpl_task_getId(), mainId)
                     then 'FAIL'
                     else 'PASS');
}

