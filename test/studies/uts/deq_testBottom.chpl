use dequeue;

def main() {
var q = DeQueue(int);

for i in [1..10] do
  q.pushBottom(i);

for i in [1..10] do
  writeln(q.popTop(), ", size=", q.size);

writeln("--");

for i in [1..10] do
  q.pushBottom(i);

for i in [1..10] do
  writeln(q.popBottom(), ", size=", q.size);

writeln("--");

for i in [1..10] do
  q.pushBottom(i);

var q2 = q.split(5);

for i in [1..5] do
  writeln(q.popTop(), " ", q2.popTop());
  }
