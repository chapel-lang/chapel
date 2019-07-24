import random
import time

#n = 1024*1024*16 # for 128MiB
n = 1024*1024*128 # for 1GiB

array = [random.randint(0, 0xffffffffffffffff) for _ in range(n)]
start = time.clock()
array.sort()
stop = time.clock()

print ("Sorted ", n, " elements in ", stop-start, " seconds")
print (((8*n) / (stop-start))/1024.0/1024.0, " MiB/s")
