enum tm { t1, t2, t3 };

proc tm.start() {
  writeln("starting ", this);
}

const tx = tm.t2;
tx.start();

(tm.t2).start();
