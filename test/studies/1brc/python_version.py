# Simple, Python native implementation

import time, csv, sys

filename = 'measurements.txt'
reportTime = False

start_time = time.time()

mins   = {}
maxes  = {}
sums   = {}
counts = {}

with open(filename, newline='') as csvfile:
  reader = csv.reader(csvfile, delimiter=';')
  for row in reader:
    city_name = row[0]
    temperature = float(row[1])

    if not city_name in mins:
      mins [city_name]  = sys.float_info.max
      maxes[city_name]  = sys.float_info.min
      sums [city_name]  = 0
      counts[city_name] = 0

    mins [city_name]  =  min(temperature, mins[city_name])
    maxes[city_name]  =  max(temperature, maxes[city_name])
    sums [city_name]  += temperature
    counts[city_name] += 1

elapsed = time.time() - start_time

for city in sorted(mins.keys()):
  print("%20s: %7.1f %7.1f %7.1f" %
        (city, mins[city], sums[city] / counts[city], maxes[city]))

if reportTime:
  print(f"Total time: {elapsed:.2f} sec")
