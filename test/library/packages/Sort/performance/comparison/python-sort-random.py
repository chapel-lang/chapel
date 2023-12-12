import random
import time

#n = 1024*1024*16 # for 16MiB
n = 1024*1024*128 # for 128MiB

array = [random.randint(0, 0xffffffffffffffff) for _ in range(n)]
start = time.time()
array.sort()
stop = time.time()

print ("Sorted ", n, " elements in ", stop-start, " seconds")
print (((8*n) / (stop-start))/1024.0/1024.0, " MiB/s")
