/* This is the only valid doc string in this modules. */
module main {

  begin {
    /* This should not be documented */
    record beginR {
      /* And neither should this. */
      var beginX: int;
    }
  }

  cobegin {
    /* This should not be documented */
    record cobeginR {
      /* And neither should this. */
      var cobeginX: int;
    }
    new cobeginR(1);
    new cobeginR(2);
  }

  while false {
    /* This should not be documented */
    record whileR {
      /* And neither should this. */
      var whileX: int;
    }
  }

  for i in 1..10 {
    /* This should not be documented */
    record forR {
      /* And neither should this. */
      var forX: int;
    }
  }

  forall i in 1..10 {
    /* This should not be documented */
    record forallR {
      /* And neither should this. */
      var forallX: int;
    }
  }

  coforall i in 1..10 {
    /* This should not be documented */
    record coforallR {
      /* And neither should this. */
      var forallX: int;
    }
  }


  proc foo() {
    /* This should not be documented */
    record procR {
      /* And neither should this. */
      var procX: int;
    }
  }

  iter bar() {
    /* This should not be documented */
    record procR {
      /* And neither should this. */
      var procX: int;
    }
    yield 0;
  }

}
