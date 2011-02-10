module barrier {

  class barrier_data {
    var   tasks_to_finish$         : sync int;
    var   first_barrier_unlocked$  : sync bool;
    var   second_barrier_unlocked$ : sync bool;
    var   use_first_barrier        : bool;
    var   processor_count          : int;

    // initialization should be to set the number of tasks to be completed
    // in each iteration, set the first lock on (empty) and set the second
    // lock off (full).  The middle assignment should be unnecessary.

    proc barrier_data ( n_processors: int ) {
      processor_count  = n_processors;
      tasks_to_finish$ = n_processors;
      if  first_barrier_unlocked$.isFull then
	first_barrier_unlocked$.readFE ();
      second_barrier_unlocked$ = true;
      use_first_barrier        = true;
    }
  }

  proc barrier ( locks ) {

    // =========================================================================
    // Barrier function based on sync variables.  
    // =========================================================================

    // acquire number of tasks yet to finish

    var save_tasks_to_finish = locks.tasks_to_finish$ . readFE ();

    if save_tasks_to_finish == 1 then {

      // last task to finish reloads the counter for the next phase,
      // locks the barrier for the next phase and unlocks the barrier
      // for this phase

      locks.tasks_to_finish$ . writeEF ( locks.processor_count );
      if locks.use_first_barrier then {
	locks.use_first_barrier = false;
      	locks.second_barrier_unlocked$ . readFE ();
      	locks.first_barrier_unlocked$ . writeEF (true);
      } else {
	locks.use_first_barrier = true;
      	locks.first_barrier_unlocked$ . readFE ();
      	locks.second_barrier_unlocked$ . writeEF (true);
      }
    }
    else {

      // if not the last task, decrement the task counter, release its lock
      // and stall waiting for the barrier for this phase to be unlocked

      locks.tasks_to_finish$ . writeEF ( save_tasks_to_finish - 1 ); 
      if locks.use_first_barrier then 
      	locks.first_barrier_unlocked$ . readFF ();
      else
      	locks.second_barrier_unlocked$ . readFF ();
    }
  }

}

