proc main() {
  var order: memoryOrder = memoryOrder.seqCst;
  writeln(order == memoryOrder.seqCst);
  writeln(order != memoryOrder.seqCst);
  writeln(order);

  order = memoryOrder.relaxed;
  writeln(order == memoryOrder.relaxed);
  writeln(order != memoryOrder.seqCst);
  writeln(order);
  
  order = memoryOrder.acquire;
  writeln(order == memoryOrder.acquire);
  writeln(order != memoryOrder.seqCst);
  writeln(order);

  order = memoryOrder.release;
  writeln(order == memoryOrder.release);
  writeln(order != memoryOrder.seqCst);
  writeln(order);

  order = memoryOrder.acqRel;
  writeln(order == memoryOrder.acqRel);
  writeln(order != memoryOrder.seqCst);
  writeln(order);

  order = memoryOrder.seqCst;
  writeln(order == memoryOrder.seqCst);
  writeln(order != memoryOrder.seqCst);
  writeln(order);
}
