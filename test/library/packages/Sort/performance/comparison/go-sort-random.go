// go run go-sort-random.go 

package main

import (
    "fmt"
    "slices"
    "time"
    "math/rand"
)

func main() {

  n := 128*1024*1024
  var slice = make([]uint64, n)

  // fill in random elements
  start1 := time.Now()
  for i:=0; i<n; i++ {
      slice[i] = rand.Uint64()
  }
  elapsed1 := time.Since(start1).Seconds()
  fmt.Printf("Filling it took %v\n", elapsed1)

  start := time.Now()
  // sort it
  slices.Sort(slice)
  elapsed := time.Since(start).Seconds()
  fmt.Printf("Sorting it took %v\n", elapsed)
  fmt.Printf("%v MiB/s\n", float64(8*n)/elapsed/1024.0/1024.0)
  fmt.Printf("%v million elements per second\n",
             float64(n)/elapsed/1000.0/1000.0)
}
