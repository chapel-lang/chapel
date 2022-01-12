## EFA Libfabric Send/Receive/Completion Paths

### Overview

The EFA provider supports two different endpoint types, `FI_EP_RDM` and
`FI_EP_DGRAM`. This document covers `FI_EP_RDM` as it implements a wire
protocol and software support for some of the Libfabric API such as tag
matching, send after send ordering guarantees, segmentation and reassembly for
large messages, emulation for RMA and atomics, and more.

There are a couple key data structures that are used to implement these
software-level features. The wire protocol that we implement is covered in a
separate document.

### Relevant data structures and functions

`rxr_ep` contains device information and structures for the endpoint including
the device/shm endpoints and completion queues and their state, the packet
pools for recv/send, outstanding app receives to be matched, outstanding sends
in progress, sends and receives queued due to resource exhaustion, unexpected
messages, and structures to track out of order packets and remote peer
capabilities and status.

`rxr_tx_entry` contains information and structures for a send posted either
directly by the app or indirectly such as an emulated read/write. When the send
is completed a send completion will be written and the tx_entry will be
released.

`rxr_rx_entry` contains information and structures for a receive posted by the
app. This structure is used for tag matching, to queue unexpected messages to
be matched later, and to keep track of whether long message receives are
complete. Just like the tx_entry, when done a receive completion is written to
the app and the rx_entry is freed.

`rxr_ep_progress` is the progress handler we register when the completion queue
is created and is called via the util completion queue functions. While the EFA
device will progress sends and receives posted to it, the Libfabric provider
has to process those device completions, potentially copy data out of a bounce
buffer into the application buffer, and write the application completions. This
all happens in this function. The progress handler also progresses long
messages and queued messages.

### Dealing with device resource exhaustion

The EFA device has fixed send and receive queue sizes which the Libfabric
provider has to manage. In general, we try to write an error to the app when
resources are exhausted as the app can manage resource exhaustion better than
the provider. However, there are some cases where we have to queue packets or
store state about a send or receive to be acted on later.

The first case is control messages that have to be queued, for example, we may
send parts of a message and then hit the device limit when sending a segmented,
medium message, or fail to send a control packet containing information that
can't be reconstructed in the future. `rxr_pkt_post_ctrl_or_queue` handles
those cases.

We also may queue an rx/tx entry if we're unable to continue sending segments
or if we fail to post a control message for that entry. You'll find the lists
where those are queued and progressed in `rxr_ep_progress_internal`.

### Dealing with receiver not ready errors (RNR)

Note: this functionality is currently turned off. We configure the device to do
infinite retries as there are known bugs in the queuing/RNR logic that need to
be resolved first.

Finally, the EFA device may write an error completion for RNR, meaning there is
no receive buffer available for the device to place the payload. This can
happen when the application is not posting receive buffers fast enough, but for
the `FI_EP_RDM` receive buffers are pre posted as packets are processed. When
we get RNR in that case, this means that a peer is overloaded.  This can happen
for any control or data packet we post, so to handle this we queue these
packets to be sent later after we backoff for the remote peer.

The occasional RNR is expected so we configure the device to retransmit a
handful of times without writing an error to the host. This is to avoid the
latency penalty of the device writing an error completion, the provider
processing that completion, and trying the send again. However, once the
Libfabric provider receives an RNR for the same packet that we already tried to
retransmit we start random exponential backoff for that peer. We stop sending
to that peer until the peer exits backoff, meaning we either received a
successful send completion for that peer or the backoff timer expires.

See `rxr_cq_queue_pkt` for where the packets are queued and backoff timers are
set, and see `rxr_ep_check_peer_backoff_timer` for where those timers are
checked and we allow sends to that remote peer again.
