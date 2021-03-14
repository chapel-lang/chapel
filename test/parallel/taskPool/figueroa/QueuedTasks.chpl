var b1, b2, b3: single bool;

begin b2.writeEF(b1.readFF());

begin b3.writeEF(b1.readFF());

writeln(here.queuedTasks(), " tasks are queued up at the moment.");
b1.writeEF(true);
