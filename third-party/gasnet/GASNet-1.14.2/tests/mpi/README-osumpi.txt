http://nowlab.cis.ohio-state.edu/projects/mpi-iba/benchmarks.html

 This page contains sample MPI-level tests for performance evaluation:

    * Latency, bandwidth, bidirectional bandwidth, and broadcast for MVAPICH
    * Latency, bandwidth, bidirectional bandwidth, one-sided put latency, one-sided put bandwidth, one-sided put bidirectional bandwidth, one-sided get latency, one-sided get bandwidth, and one-sided accumulate latency for MVAPICH2.

These tests are also available under the "osu_benchmarks" directory in the respective (MVAPICH/MVAPICH2) software distributions.
Latency Test

    * The latency tests were carried out in a ping-pong fashion. The sender sends a message with a certain data size to the receiver and waits for a reply from the receiver. The receiver receives the message from the sender and sends back a reply with the same data size. Many iterations of this ping-pong test were carried out and average one-way latency numbers were obtained. Blocking version of MPI functions (MPI_Send and MPI_Recv) were used in the tests. The program for OSU latency 2.0 is available here.

Bandwidth Test

    * The bandwidth tests were carried out by having the sender sending out a fixed number (equal to the window size) of back-to-back messages to the receiver and then waiting for a reply from the receiver. The receiver sends the reply only after receiving all these messages. This process is repeated for several iterations and the bandwidth is calculated based on the elapsed time (from the time sender sends the first message until the time it receives the reply back from the receiver) and the number of bytes sent by the sender. The objective of this bandwidth test is to determine the maximum sustained date rate that can be achieved at the network level. Thus, non-blocking version of MPI functions (MPI_Isend and MPI_Irecv) were used in the test. The program for OSU bandwidth 2.0 is available here.

Bidirectional Bandwidth Test

    * The bidirectional bandwidth test is similar to the bandwidth test, except that both the nodes involved send out a fixed number of back-to-back messages and wait for the reply. This test measures the maximum sustainable aggregrate bandwidth by two nodes. The program for OSU bidirectional bandwidth 2.0 is available here.

Broadcast Latency Test

    * Broadcast Latency Test: The Broadcast latency test was carried out in the following manner. After doing a MPI_Bcast the root node waits for an ack from the last receiver. This ack is in the form of a zero byte message from the receiver to the root. This test is carried out for a large number (1000) of iterations. The Broadcast latency is obtained by subtracting the time taken for the ack from the total time. We compute the ack time initially by doing a ping-pong test. The program for OSU broadcast latency 2.0 is available here.

One-Sided Put Latency Test (only applicable for MVAPICH2)

    * One-Sided Put Latency Test: The sender (origin process) calls MPI_Put (ping) to directly place a message of certain data size in the receiver window. The receiver (target process) calls MPI_Win_wait to make sure the message has been received. Then the receiver initates a MPI_Put (pong) of the same data size to the sender which is now waiting on a synchronization call. Several iterations of this test is carried out and the average put latency numbers is obtained. The program for OSU One-Sided Put latency 1.0 is available here.

One-Sided Get Latency Test (only applicable for MVAPICH2)

    * One-Sided Get Latency Test: The origin process calls MPI_Get (ping) to directly fetch a message of certain data size from the target process window to its local window.It then waits on a synchronization call (MPI_Win_complete) for local completion. After the synchronization call the target and origin process are switched for the pong message. Several iterations of this test are carried out and the average get latency numbers is obtained. The program for OSU One-Sided Get latency 1.0 is available here.

One-Sided Put Bandwidth Test (only applicable for MVAPICH2)

    * One-Sided Put Bandwidth Test: The bandwidth tests were carried out by the origin process calling a fixed number of back to back Puts and then wait on a synchronization call (MPI_Win_complete) for completion. This process is repeated for several iterations and the bandwidth is calculated based on the elapsed time and the number of bytes sent by the origin process. The program for OSU One-Sided Put Bandwidth 1.0 is available here.

One-Sided Get Bandwidth Test (only applicable for MVAPICH2)

    * One-Sided Get Bandwidth Test: The bandwidth tests were carried out by origin process calling a fixed number of back to back Gets and then wait on a synchronization call (MPI_Win_complete) for completion. This process is repeated for several iterations and the bandwidth is calculated based on the elapsed time and the number of bytes sent by the origin process. The program for OSU One-Sided Get Bandwidth 1.0 is available here.

One-Sided Put Bidirectional Bandwidth Test (only applicable for MVAPICH2)

    * One-Sided Put Bidirectional Bandwidth Test: The bidirectional bandwidth test is similar to the bandwidth test,except that both the nodes involved send out a fixed number of back to back put messages and wait for the completion. This test measures the maximum sustainable aggregrate bandwidth by two nodes. The program for OSU One-Sided Put Bidirectional Bandwidth 1.0 is available here.

Accumulate Latency Test (only applicable for MVAPICH2)

    * One-Sided Accumulate Latency Test: The origin process calls MPI_Accumulate to combine the data moved to the target process window with the data that resides at the remote window. The combining operation used in the test is MPI_SUM. It then waits on a synchronization call (MPI_Win_complete) for local completion. After the synchronization call, the target and origin process are switched for the pong message. Several iterations of this test are carried out and the average accumulate latency number is obtained.The program for OSU One-Sided Accumulate Latency 1.0 is available here.

Please note that there are many different ways to measure these performance parameters. For example, the bandwidth test can have different variations wrt the types of MPI calls (blocking vs. non-blocking) being used, total number of back-to-back messages sent in one iteration, number of iterations, etc. Other ways to measure bandwidth may give different numbers. Readers are welcome to use other tests, as appropriate to their application environments.