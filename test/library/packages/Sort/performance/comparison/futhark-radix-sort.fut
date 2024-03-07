-- nix-shell -p futhark
-- futhark pkg add github.com/diku-dk/cpprandom
-- futhark pkg add github.com/diku-dk/sorts
-- futhark pkg sync
-- futhark bench futark-radix-sort.fut

import "lib/github.com/diku-dk/sorts/bitonic_sort"
import "lib/github.com/diku-dk/sorts/merge_sort"
import "lib/github.com/diku-dk/sorts/radix_sort"

-- 64-bit keys
-- ==
-- entry: merge_sort_i64 radix_sort_i64
-- random input { [100000]i64 }
-- random input { [134217728]i64 }
entry merge_sort_i64 = merge_sort (i64.<=)
entry radix_sort_i64 = radix_sort 64 i64.get_bit
