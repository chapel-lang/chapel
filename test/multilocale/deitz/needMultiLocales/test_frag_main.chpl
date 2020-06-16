use CommDiagnostics;

config const useVerboseComm = false;
config var token = 0;

proc main() {
  coforall loc in Locales do on loc do
    fragmentedMain();
}

proc fragmentedMain() {
  var rank = here.id;
  var size = numLocales;

  if useVerboseComm then startVerboseCommHere();

  if rank == size-1 {
    chpl_send_int(token, 0); 
  }

  chpl_recv_int(token, (rank-1+size)%size);

  if !useVerboseComm then
    writeln("Locale ", rank, " received token ", token);

  chpl_send_int(token, (rank+1)%size);

  if useVerboseComm then stopVerboseCommHere();
}

class node {
  var data: int;
  var next: shared node?;
}

class list {
  var head, tail: shared node?;
  var lock$: sync bool;
  var signal$: sync bool;
}

use PrivateDist;

var buffer: [PrivateSpace] [0..numLocales-1] owned list?;
forall p in PrivateSpace do
  forall l in LocaleSpace do
    buffer[p][l] = new list();

proc chpl_send_int(data: int, loc) {
  var from = here.id;
  on Locales[loc] {
    var b = buffer[here.id][from]!;
    b.lock$ = true;
    b.tail = new node(data, b.tail);
    if b.head == nil then
      b.head = b.tail;
    b.signal$.writeXF(true);
    b.lock$;
  }
}

proc chpl_recv_int(out data: int, loc) {
  var b = buffer[here.id][loc]!;
  b.signal$;
  b.lock$ = true;
  data = b.head!.data;
  var next = b.head!.next;
  b.head = next;
  if b.head == nil then
    b.tail = nil;
  else
    b.signal$.writeXF(true);
  b.lock$;
}
