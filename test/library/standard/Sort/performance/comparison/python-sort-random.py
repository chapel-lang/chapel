import random
import time

n = 128*1024*1024;

t1 = time.time()
array = [random.randint(0, 0xffffffffffffffff) for _ in range(n)]
t2 = time.time()
print ("Generated ", n, " elements in ", t2-t1, " seconds")
print (((8*n) / (t2-t1))/1024.0/1024.0, " MiB/s")

start = time.time()
array.sort()
stop = time.time()

print ("Sorted ", n, " elements in ", stop-start, " seconds")
print (((8*n) / (stop-start))/1024.0/1024.0, " MiB/s")
print ((n / (stop-start))/1000.0/1000.0, " million elements sorted per second")
