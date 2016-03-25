use Queue;

var myQueue: queue;
myQueue = new queue();

myQueue.enqueue(10);
myQueue.displayQueue();		
myQueue.enqueue(9);
myQueue.displayQueue();
myQueue.enqueue(15);
myQueue.displayQueue();

myQueue.dequeue();
myQueue.displayQueue();
