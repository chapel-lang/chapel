proc main() {
  var order: memory_order;
  writeln(order == memory_order_seq_cst);
  writeln(order != memory_order_seq_cst);
  writeln(order);

  order = memory_order_relaxed;
  writeln(order == memory_order_relaxed);
  writeln(order != memory_order_seq_cst);
  writeln(order);
  
  order = memory_order_consume;
  writeln(order == memory_order_consume);
  writeln(order != memory_order_seq_cst);
  writeln(order);

  order = memory_order_acquire;
  writeln(order == memory_order_acquire);
  writeln(order != memory_order_seq_cst);
  writeln(order);

  order = memory_order_release;
  writeln(order == memory_order_release);
  writeln(order != memory_order_seq_cst);
  writeln(order);

  order = memory_order_acq_rel;
  writeln(order == memory_order_acq_rel);
  writeln(order != memory_order_seq_cst);
  writeln(order);

  order = memory_order_seq_cst;
  writeln(order == memory_order_seq_cst);
  writeln(order != memory_order_seq_cst);
  writeln(order);
}
