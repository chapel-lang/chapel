
config const numMessages = 36;
config const dptpl = 4;

const MessageSpace = {1..numMessages};

var taskCount: atomic int;
var taskSlots: [0..(numMessages + 10)] int;

forall msg in MessageSpace
  with (const taskId = taskCount.fetchAdd(1),
        ref r = taskSlots[taskId],
        const ref cr = taskSlots[taskId])
{
  r += 1;
  writef("t1  %i  %i\n", taskId, cr);
}

// Just to exercise leader-followers.
var MessageArray: [MessageSpace] int;
taskCount.write(0);

forall (msg,elm) in zip(MessageSpace,MessageArray)
  with (const taskId = taskCount.fetchAdd(1),
        ref r = taskSlots[taskId],
        const ref cr = taskSlots[taskId])
{
  r -= 1;
  writef("t2  %i  %i\n", taskId, cr);
}
