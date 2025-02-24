/*
    This tests the CommDiagnostics counters for following remote cache prefetch counters:

    cache_num_prefetches: how many cache pages were prefetched via chpl_comm_remote_prefetch 
    cache_prefetch_unused: how many cache pages that were prefetched were evicted before being accessed once
    cache_prefetch_waited: how many cache pages that were prefetches had not arrived by the time we tried to access it.

    Given the nature of this test, it must be executed with at least 2 locales.
*/

use CommDiagnostics;
use IO;
use CTypes;

// This is a wrapper around the function that we currently use to
// issue a prefetch. Prefetches via this function are what the CommDiagnostics
// counters target.
inline proc prefetch(ref x, len:int) {
  __primitive("chpl_comm_remote_prefetch", x.locale.id, x, len);
}

// Helper function we use in this test to invalidate pages we prefetched
// into the remote cache
pragma "insert line file info"
extern proc chpl_cache_invalidate(node:c_int, raddr:c_ptr(void),
                                  size: c_size_t);

// Constants for the tests
config const numElems= 128*1024;// number of elements in the test array
const elemSize = 8;             // always assuming 8-byte elements in the array
assert(elemSize == c_sizeof(int));
const pageSize = 1024;          // cache page size assumed to be 1024 bytes
const alignment = 1024*pageSize;  // align to this amount
const elemsPerPage = pageSize/elemSize; // how many elements in the array would
                                        // fall into a cache page
config const numPrefetches = 32;
const prefetchStride = 3*pageSize/elemSize; // prefetch stride (in elements)
// make sure there is room for doing a prefetch, a few unused pages,
// and then a prefetch
assert((numPrefetches+1)*prefetchStride < numElems);

config const numBusyElems = 16; //32*1024; // number of elements in the busy work array

// Main procedure called to perform the test. Different IDs (1, 2, etc.) are
// passed in to run a specific test case
proc testCounters(id: int)
{
    writef("Running test %i numPrefetches=%i\n", id, numPrefetches);

    // The array that is allocated on locale 0. We'll use this as the target of
    // the prefetches that are issued from locale 1.

    // Allocate an array where the initial element is aligned to 'alignment'.
    // We want it to be aligned to a multiple of pageSize so that
    // there is no instability in the test.
    const locPtr = allocate(int, numElems, alignment=alignment, clear=true);
    defer {
      // deallocate the buffer when exiting this function
      deallocate(locPtr);
    }
    // note: the array created in this way does not free the buffer
    var A = makeArrayFromPtr(locPtr, numElems);

    var StoreVar: int = 0;

    //#########################################################################
    //
    // Test 1: Perform prefetches and do not access anything. We should see 0
    // prefetches that were waited on and all prefetches should be marked as
    // unused. To get the unused count to work "correctly", we need to evict the
    // cache pages that were prefetched (a current issue/feature is that when
    // the commDiagnostics stop, we don't account for anything left in the
    // remote cache).  Furthermore, we insert a loop that does some "busy work"
    // so that the prefetches complete. That ensures that the waited counter is
    // 0.
    //
    // Because the comms layer might not be able to know if a prefetch request
    // is done until some requests are waited for, wait for an unrelated
    // store (PUT). That way, the counting for the prefetches will indicate
    // that nothing was waited for.
    //
    //#########################################################################
    if id == 1 {
        // ensure prefetches are 1024 bytes apart (i.e., a cache page).
        resetCommDiagnostics();
        //startVerboseComm();
        startCommDiagnostics();
        on Locales[1] {
            for i in 1..#numPrefetches {
                const idx = i*prefetchStride;
                assert(0 <= idx && idx < numElems);
                prefetch(A[idx], 1);
            }

            // do a unrelated store and wait for it
            // (both to give time for the prefetches to complete
            //  and to allow the runtime to realize that they are complete)
            StoreVar = 1;
            atomicFence(memoryOrder.release); // wait for the store

            // force evictions of the pages we prefetched to
            // ensure that the prefetch counts are updated.
            for i in 1..#numPrefetches {
                const idx = i*prefetchStride;
                const eltPtr:c_ptr(void) = c_ptrTo(A[idx]):c_ptr(void);
                chpl_cache_invalidate(0:c_int, eltPtr, 1:c_size_t);
            }
        }
        stopCommDiagnostics();
        //stopVerboseComm();
        var res = getCommDiagnostics();
        var cache_num_prefetches = res[1].cache_num_prefetches;
        var cache_prefetch_unused = res[1].cache_prefetch_unused;
        var cache_prefetch_waited = res[1].cache_prefetch_waited;
        writef("\tcache_num_prefetches: %i\n", cache_num_prefetches);
        writef("\tcache_prefetch_unused: %i\n", cache_prefetch_unused);
        writef("\tcache_prefetch_waited: %i\n", cache_prefetch_waited);
    } /* end of test 1 */

    //#########################################################################
    //
    // Test 2: Perform same number of prefetches as test 1, but access the data
    // associated with "even" number prefetches. This tests that the unused
    // counter is working correctly. We should see that half of the prefetches
    // we issued went unused before being evicted. Again, we should see 0
    // prefetches that were waited on.
    //
    //#########################################################################
    if id == 2 {
        for i in 0..<numElems do A[i] = 1;
        var numAccessed = 0;
        resetCommDiagnostics();
        startCommDiagnostics();
        on Locales[1] {
            for i in 1..#numPrefetches {
                const idx = i*prefetchStride;
                prefetch(A[idx], 1);
            }

            // do a unrelated store and wait for it
            // (both to give time for the prefetches to complete
            //  and to allow the runtime to realize that they are complete)
            StoreVar = 1;
            atomicFence(memoryOrder.release); // wait for the store

            // access "even" prefetches
            var temp = 0;
            for i in 1..#numPrefetches {
                const idx = i*prefetchStride;
                if i % 2 == 0 {
                    temp += A[idx];
                    numAccessed += 1;
                }
            }
            // force evictions of the pages we prefetched to
            // ensure that the prefetch counts are updated.
            for i in 1..#numPrefetches {
                const idx = i*prefetchStride;
                const eltPtr:c_ptr(void) = c_ptrTo(A[idx]):c_ptr(void);
                chpl_cache_invalidate(0:c_int, eltPtr, 1:c_size_t);
            }   
        }
        stopCommDiagnostics();
        var res = getCommDiagnostics();
        var cache_num_prefetches = res[1].cache_num_prefetches;
        var cache_prefetch_unused = res[1].cache_prefetch_unused;
        var cache_prefetch_waited = res[1].cache_prefetch_waited;
        writef("\tcache_num_prefetches: %i\n", cache_num_prefetches);
        writef("\tcache_prefetch_unused: %i\n", cache_prefetch_unused);
        writef("\tcache_prefetch_waited: %i\n", cache_prefetch_waited);          
    } /* end of test 2 */

    //#########################################################################
    //
    // Test 3: Attempt to access the data prefetched immediately after it was
    // prefetched. This should result in all prefetches being counted as
    // "waited". We should see 0 unused prefetches, since we access everything
    // we prefetched.
    //
    //#########################################################################
    if id == 3 { 
        resetCommDiagnostics();
        startCommDiagnostics();
        on Locales[1] {
            var temp = 0;
            for i in 0..#numPrefetches {
                const idx = i*prefetchStride;
                prefetch(A[idx], 1);
                temp += A[idx];
            }
            // force evictions of the pages we prefetched to
            // ensure that the prefetch counts are updated.
            for i in 0..#numPrefetches {
                const idx = i*prefetchStride;
                const eltPtr:c_ptr(void) = c_ptrTo(A[idx]):c_ptr(void);
                chpl_cache_invalidate(0:c_int, eltPtr, 1:c_size_t);
            }
        }
        stopCommDiagnostics();
        var res = getCommDiagnostics();
        var cache_num_prefetches = res[1].cache_num_prefetches;
        var cache_prefetch_unused = res[1].cache_prefetch_unused;
        var cache_prefetch_waited = res[1].cache_prefetch_waited;
        writef("\tcache_num_prefetches: %i\n", cache_num_prefetches);
        writef("\tcache_prefetch_unused: %i\n", cache_prefetch_unused);
        writef("\tcache_prefetch_waited: %i\n", cache_prefetch_waited);
    } /* end of test 3 */  
}

proc main()
{
    testCounters(1);

    writef("Running test 2\n");
    testCounters(2);

    writef("Running test 3\n");
    testCounters(3);
    
}
