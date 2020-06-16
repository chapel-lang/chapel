
/*
 * /////////////////////////////////////////////////////
 * //   "Speed of Light" Memory Fifo Ping Pong test   //
 * /////////////////////////////////////////////////////
 */


/*
Idea: set up an injection fifo and pre-fill in all of the descriptors
for the number of messages to send in a round For each iteration, only
need to advance the injection fifo then poll on reception fifo. Between
message sizes update all injection fifo descriptors with the new message
size to test.  Ideally, the size of the injection fifo should be the
same as the number of messages to send in each round.

Receive (pong) side polls on the reception fifo and counts the number
of packets received, when the count for a message size is reached it
will advance its injection fifo to send the "pong". The "ping" side
similarly waits on the reception fifo for each "pong" before sending
the next "ping" in the round.

No memory is copied. No rendezvous. Just trying to measure how fast
the network is....
*/


#include "rdma/bgq/fi_bgq_mu.h"



#define NUM_LOOPS   			1024

#define MAX_MESSAGE_SIZE		2097152

#define BYTE_COUNTER_BAT_ID		(0)
#define RECEIVE_BUFFER_BAT_ID		(1)

uint64_t sbuf[MAX_MESSAGE_SIZE/sizeof(uint64_t)];
uint64_t rbuf[MAX_MESSAGE_SIZE/sizeof(uint64_t)];

MUHWI_Descriptor_t desc[NUM_LOOPS] __attribute__((__aligned__(64)));




static inline void bat_allocate (MUSPI_BaseAddressTableSubGroup_t * bat_subgroup)
{
	uint32_t nbatids;
	uint32_t batids[BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP];
	int32_t cnk_rc __attribute__ ((unused));
	cnk_rc = Kernel_QueryBaseAddressTable(0, &nbatids, batids);
	assert(cnk_rc == 0);
	assert(nbatids > 0);

	cnk_rc = Kernel_AllocateBaseAddressTable(0, bat_subgroup, nbatids, &batids[0], 0);
	assert(cnk_rc == 0);
}

static inline void bat_write (MUSPI_BaseAddressTableSubGroup_t * bat_subgroup, uint64_t index, uint64_t offset)
{

	int32_t cnk_rc __attribute__ ((unused));
	cnk_rc = MUSPI_SetBaseAddress(bat_subgroup, index, offset);
	assert(cnk_rc == 0);

}


static inline void init_gi_barrier (MUSPI_GIBarrier_t * GIBarrier)
{
	int rc;
	rc = MUSPI_GIBarrierInit(GIBarrier, 0);
	if (rc) exit(1);
}


static inline void do_gi_barrier (MUSPI_GIBarrier_t * GIBarrier)
{
	int rc;
	uint64_t gi_timeout = 1600000000;	/* about 1 sec at 16 mhz */
	gi_timeout *= 30;
  
	rc = MUSPI_GIBarrierEnter(GIBarrier);
	if (rc) exit(1);

	rc = MUSPI_GIBarrierPollWithTimeout(GIBarrier, gi_timeout);
	if (rc) exit(1);
}


static inline void do_gi_barrier_no_timeout (MUSPI_GIBarrier_t * GIBarrier)
{
	int rc;
	rc = MUSPI_GIBarrierEnter(GIBarrier);
	if (rc) exit(1);

	rc = MUSPI_GIBarrierPoll(GIBarrier);
	if (rc) exit(1);
}


static inline MUSPI_RecFifo_t * allocate_reception_fifo (MUSPI_RecFifoSubGroup_t * rfifo_subgroup)
{

	int rc __attribute__ ((unused));
	uint8_t * memptr;

	size_t nbytes = 8 * 1024 * 1024;
	rc = posix_memalign((void**)&memptr, 32, nbytes);
	assert(0 == rc);

	Kernel_MemoryRegion_t mregion;
	rc = Kernel_CreateMemoryRegion(&mregion, (void*)memptr, nbytes);
	assert(0 == rc);

	uint32_t free_fifo_num;
	uint32_t free_fifo_ids[BGQ_MU_NUM_REC_FIFOS_PER_SUBGROUP];
	rc = Kernel_QueryRecFifos(0, &free_fifo_num, free_fifo_ids);
	assert(0 == rc);
	assert(0 < free_fifo_num);
	assert(0 == free_fifo_ids[0]);

	Kernel_RecFifoAttributes_t attr;
	memset(&attr, 0x00, sizeof(attr));
	rc = Kernel_AllocateRecFifos(0, rfifo_subgroup, 1, free_fifo_ids, &attr);
	assert(0 == rc);

	rc = Kernel_RecFifoInit(rfifo_subgroup, 0, &mregion,
		((uint64_t)memptr) - (uint64_t)mregion.BaseVa,
		nbytes-1);
	assert(0 == rc);

	rc = Kernel_RecFifoEnable(0, 0x08000ull);
	assert(0 == rc);

	assert(rfifo_subgroup->_recfifos[0]._fifo.hwfifo);

	return &rfifo_subgroup->_recfifos[0];
}


static inline void inject (struct fi_bgq_spi_injfifo * ififo, MUHWI_Descriptor_t * model)
{

	MUSPI_InjFifo_t * muspi_injfifo = ififo->muspi_injfifo;
	MUHWI_Descriptor_t * d = fi_bgq_spi_injfifo_tail_wait(ififo);
	*d = *model;

	MUSPI_InjFifoAdvanceDesc(muspi_injfifo);
	return;
}


/* return the number of *chunks* consumed */
static inline uint64_t receive (MUSPI_RecFifo_t * recfifo)
{
	MUSPI_Fifo_t * fifo = (MUSPI_Fifo_t *)recfifo;

	const uintptr_t pa_start = MUSPI_getStartPa(fifo);
	const uintptr_t va_head = (uintptr_t) MUSPI_getHeadVa(fifo);
	const uintptr_t va_start = (uintptr_t) MUSPI_getStartVa(fifo);
	const uintptr_t offset_head = va_head - va_start;

	uintptr_t offset_tail = MUSPI_getHwTail(fifo) - pa_start;

	/*
	 * wait until the head does not equal the tail; this signifies that
	 * a packet has been received
	 */
	while (offset_head == offset_tail) {
		offset_tail =  MUSPI_getHwTail(fifo) - pa_start;
	}

	uint64_t bytes_consumed;
	if (offset_head < offset_tail) {

		MUSPI_setHeadVa(fifo, (void*)(va_start + offset_tail));
		MUSPI_setHwHead(fifo, offset_tail);

		bytes_consumed = offset_tail - offset_head;

	} else {

		MUSPI_setHeadVa(fifo, (void*)(va_start));
		MUSPI_setHwHead(fifo, 0);

		const uintptr_t va_end = (uintptr_t) fifo->va_end;
		bytes_consumed = va_end - va_head;
	}

	return bytes_consumed >> 5;	/* each chunk is 32 bytes */
}




int main (int argc, char **argv)
{
	Personality_t pers;
	Kernel_GetPersonality(&pers, sizeof(pers));

	BG_CoordinateMapping_t local;
	local.a = pers.Network_Config.Acoord;
	local.b = pers.Network_Config.Bcoord;
	local.c = pers.Network_Config.Ccoord;
	local.d = pers.Network_Config.Dcoord;
	local.e = pers.Network_Config.Ecoord;
	local.t = Kernel_PhysicalProcessorID();

	int is_root = 0;
	int is_neighbor = 0;
	if (local.t == 0 && local.a == 0 && local.b == 0 && local.c == 0 && local.d == 0) {

		is_root = (local.e == 0);
		is_neighbor = (local.e == 1);
	}


	MUSPI_BaseAddressTableSubGroup_t bat_subgroup;
	bat_allocate(&bat_subgroup);

	volatile uint64_t byte_counter __attribute__((__aligned__(64)));
	byte_counter = 0;

	uint64_t byte_counter_paddr = 0;
	fi_bgq_cnk_vaddr2paddr((const void *)&byte_counter, sizeof(uint64_t), &byte_counter_paddr);
	uint64_t atomic_byte_counter_paddr = MUSPI_GetAtomicAddress(byte_counter_paddr, MUHWI_ATOMIC_OPCODE_STORE_ADD);
	bat_write(&bat_subgroup, BYTE_COUNTER_BAT_ID, atomic_byte_counter_paddr);

	uint64_t rbuf_paddr = 0;
	fi_bgq_cnk_vaddr2paddr((const void *)&rbuf[0], MAX_MESSAGE_SIZE, &rbuf_paddr);
	bat_write(&bat_subgroup, RECEIVE_BUFFER_BAT_ID, rbuf_paddr);
	

	struct fi_bgq_spi_injfifo rget_ififo;
	MUSPI_InjFifoSubGroup_t rget_ififo_subgroup;
	fi_bgq_spi_injfifo_init(&rget_ififo, &rget_ififo_subgroup, 1, NUM_LOOPS, 0, 1, 0);

	struct fi_bgq_spi_injfifo ififo;
	MUSPI_InjFifoSubGroup_t ififo_subgroup;
	fi_bgq_spi_injfifo_init(&ififo, &ififo_subgroup, 1, NUM_LOOPS, 0, 0, 1);

	MUSPI_RecFifoSubGroup_t rfifo_subgroup;
	MUSPI_RecFifo_t * recfifo = allocate_reception_fifo(&rfifo_subgroup);


	/*
	 * Create the 'memory fifo' descriptor - used for eager-style transfers
	 * and rendezvous-style RTS messages.
	 */
	MUHWI_Descriptor_t fifo_model __attribute__((__aligned__(64)));
	MUSPI_DescriptorZeroOut(&fifo_model);

	fifo_model.Half_Word0.Prefetch_Only = MUHWI_DESCRIPTOR_PRE_FETCH_ONLY_NO;
	fifo_model.Half_Word1.Interrupt = MUHWI_DESCRIPTOR_DO_NOT_INTERRUPT_ON_PACKET_ARRIVAL;
	fi_bgq_cnk_vaddr2paddr((const void *)&sbuf[0], MAX_MESSAGE_SIZE+64, &fifo_model.Pa_Payload);
	fifo_model.Message_Length = 0;
	fifo_model.PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type = MUHWI_PT2PT_DATA_PACKET_TYPE;
	fifo_model.PacketHeader.NetworkHeader.pt2pt.Byte3.Byte3 = MUHWI_PACKET_VIRTUAL_CHANNEL_DETERMINISTIC;
	fifo_model.PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 = MUHWI_PACKET_TYPE_FIFO;
	fifo_model.PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id = 0;
	fifo_model.PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;

	fifo_model.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination = 0;
	fifo_model.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination = 0;
	fifo_model.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination = 0;
	fifo_model.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination = 0;

	if (is_root) {

		fifo_model.Torus_FIFO_Map = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP;
		fifo_model.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination = 1;

	} else if (is_neighbor) {

		fifo_model.Torus_FIFO_Map = MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM;
		fifo_model.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination = 0;
	}


	MUHWI_Descriptor_t dput __attribute__((__aligned__(64)));
	MUSPI_DescriptorZeroOut(&dput);

	dput = fifo_model;
	dput.Torus_FIFO_Map =
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP;
	dput.PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 = MUHWI_PACKET_TYPE_PUT;
	dput.PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing = MUHWI_PACKET_DIRECT_PUT_IS_NOT_PACED;
	MUSPI_SetRecPayloadBaseAddressInfo(&dput, RECEIVE_BUFFER_BAT_ID, 0);
	dput.PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id = BYTE_COUNTER_BAT_ID;
	dput.PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset = 0;

	dput.Message_Length = 0;	/* updated during the test */
	dput.PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_MSB = 0;
	dput.PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_LSB = 0;

	dput.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination = local.a;
	dput.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination = local.b;
	dput.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination = local.c;
	dput.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination = local.d;
	dput.PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination = local.e;
	dput.PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;


	MUHWI_Descriptor_t rget_model __attribute__((__aligned__(64)));
	MUSPI_DescriptorZeroOut(&rget_model);

	rget_model = fifo_model;
	rget_model.Torus_FIFO_Map =
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM |
		MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP;
	rget_model.PacketHeader.NetworkHeader.pt2pt.Byte8.Byte8 = MUHWI_PACKET_TYPE_GET;
	fi_bgq_cnk_vaddr2paddr((const void *)&dput, sizeof(MUHWI_Descriptor_t), &rget_model.Pa_Payload);
	rget_model.Message_Length = sizeof(MUHWI_Descriptor_t);

	rget_model.PacketHeader.NetworkHeader.pt2pt.Byte8.Size = 16;


	/*
	 * Barrier
	 */
	MUSPI_GIBarrier_t GIBarrier;
	init_gi_barrier(&GIBarrier);
	do_gi_barrier(&GIBarrier);


	if (is_root) {

		fprintf(stdout, "#                       eager            rendezvous\n");
		fprintf(stdout, "# %10s %10s %9s %10s %9s\n", "bytes", "cycles", "usec", "cycles", "usec");
		fprintf(stdout, "# ====================================================\n");

	}

	const uint64_t num_loops = NUM_LOOPS;

	uint64_t i = 0;
	uint64_t msg_size = 0;
	while (msg_size <= MAX_MESSAGE_SIZE) {

		fifo_model.Message_Length = msg_size;
		dput.Message_Length = msg_size;

		uint64_t eager_cycles = 0;
		uint64_t rendezvous_cycles = 0;

		/*
		 * each torus chunk is 32 bytes and the first 32 bytes in each
		 * packet is header
		 */
		const uint64_t npackets = msg_size == 0 ? 1 : (msg_size / 512) + (msg_size % 512 != 0);
		const uint64_t nchunks = npackets + (msg_size / 32) + (msg_size % 32 != 0);
		if (is_root) {

			Delay(500000);	/* make sure receiver is ready */

			const unsigned long long t0 = GetTimeBase();
			for (i=0; i<num_loops; ++i) {

				inject(&ififo, &fifo_model);

				unsigned n = nchunks;
				while (n > 0) {
					n -= receive(recfifo);
				}
			}
			const unsigned long long t1 = GetTimeBase();
			eager_cycles = t1 - t0;

		} else if (is_neighbor) {
			for (i=0; i<num_loops; ++i) {

				unsigned n = nchunks;
				while (n > 0) {
					n -= receive(recfifo);
				}

				inject(&ififo, &fifo_model);
			}

			Delay(500000);	/* make sure sender is finished */
		}


		/*
		 * rendezvous
		 */
		if (msg_size > 0) {

			fifo_model.Message_Length = 0;

			if (is_root) {

				Delay(500000);	/* make sure receiver is ready */

				const unsigned long long t0 = GetTimeBase();
				for (i=0; i<num_loops; ++i) {

					/* inject the 'ping' rts */
					inject(&ififo, &fifo_model);

					/* wait for the 'pong' rts */
					unsigned n = 1;
					while (n > 0) {
						unsigned chunks = receive(recfifo);
						n -= chunks;
					}

					/* transfer the 'pong' data */
					byte_counter = msg_size;
					inject(&ififo, &rget_model);

					/* wait until all 'pong' data is delivered */
					while (byte_counter > 0);
				}
				const unsigned long long t1 = GetTimeBase();
				rendezvous_cycles = t1 - t0;

			} else if (is_neighbor) {

				for (i=0; i<num_loops; ++i) {

					/* wait for the 'ping' rts */
					unsigned n = 1;
					while (n > 0) {
						unsigned chunks = receive(recfifo);
						n -= chunks;
					}

					/* transfer the 'ping' data */
					byte_counter = msg_size;
					inject(&ififo, &rget_model);

					/* wait until all 'ping' data is delivered */
					while (byte_counter > 0);

					/* inject the 'pong' rts */
					inject(&ififo, &fifo_model);
				}

				Delay(500000);	/* make sure sender is finished */
			}
		}

		if (is_root) {

			/* report half pingpong */
			fprintf(stdout, "  %10lu %10lu %9.2f %10lu %9.2f\n", msg_size,
				(eager_cycles)/(num_loops*2), (((eager_cycles)*1.0)/1600.0) / (num_loops * 2.0),
				(rendezvous_cycles)/(num_loops*2), (((rendezvous_cycles)*1.0)/1600.0) / (num_loops * 2.0));
		}

		msg_size = msg_size == 0 ? 1 : msg_size*2;
	}

	return 0;
}

