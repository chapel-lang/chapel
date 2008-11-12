/* $Source: /var/local/cvs/gasnet/gm-conduit/gasnet_core_conf.c,v $
 * $Date: 2006/08/19 10:48:56 $
 * $Revision: 1.24 $
 * Description: GASNet GM conduit Implementation
 * Copyright 2002, Christian Bell <csbell@cs.berkeley.edu>
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
/* Much of the code below is taken from Myricom's MPICH bootstrap code */

#include <gasnet_core_internal.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>

#define RETURN_ERR(err)	do {	\
		printf err;			\
		printf ("\n");			\
		return GASNET_ERR_RESOURCE;	\
	} while (0)


#define GASNETC_HOST_PATH	"/usr/bin/host"
#define GASNETC_INIT_TIMEOUT	(3*60*1000)	/* 3 minutes */
#define GASNETC_SOCKET_BUFSIZ	(128*1024)

#define GASNETC_SPAWNERR(msg) \
   gasneti_fatalerror("spawning job on node %d (%s): %s", gasneti_mynode, gasnett_gethostname(), msg)

#define GASNETC_GETENV(env, ptr, err) do {				\
		char *ptrenv = getenv(env);				\
		if (ptrenv == NULL || *ptrenv == '\0') {		\
                        GASNETC_SPAWNERR(err);                          \
		}							\
		else {							\
			ptr = ptrenv;					\
		}							\
	} while (0)


#if 1
/*
 * Disable the gethostbyname() hack, as it's too fragile as is and hinders our
 * ability to reliably support other GASNet spawners.
 */
#include <netdb.h>

uint32_t
gasnetc_parse_addr(char *hostaddr)
{
    struct hostent  *he;
    uint32_t	     ip, a2, a3, a4;

    /*
     * Use gethostbyname IFF we can't find an ip in the input address
     * 'hostaddr'
     */
    if (sscanf(hostaddr, "%d.%d.%d.%d",&ip,&a2,&a3,&a4) != 4) {
    	he = gethostbyname(hostaddr);

    	if (he == NULL || he->h_length != 4)
		return 0;
    	else {
		memcpy(&ip, he->h_addr, he->h_length);
		return ip;
    	}
    }
    else {
	if (inet_pton(AF_INET, hostaddr, &ip) < 1)
	    return 0;
	else
	    return ip;
    }
}
#else
/* 
 * Replace gethostby... functionality
 *
 * These are unfortunately located in shared object libraries on linux, so we
 * have to emulate them here in case they don't exist on certain systems that
 * fail to install shared libraries correctly.
 *
 * This command parses hostaddr and returns the 32-bit ip address as an
 * integer in network-byte order.  If fatal, or the address could not be parsed,
 * the function returns 0.  This, of course, assumes that the caller will never
 * want to use 0.0.0.0 as an address, which is reasonable for what we use this
 * function for.
 *
 * In order, the command first tries to parse using perl, then /usr/bin/host.
 */
#include <ctype.h> /* isdigit */
uint32_t
gasnetc_parse_addr(char *hostaddr)
{
    uint32_t ip, a2, a3, a4;
    char    cmd[256], line[128];
    char    ipaddr[32];
    char    *p, *ip_ptr = NULL;
    int	    i = 0;
    FILE    *fd;

    ip_ptr = hostaddr;
    
    if (sscanf(ip_ptr, "%d.%d.%d.%d",&ip,&a2,&a3,&a4) != 4) {

	/* First try using perl, assuming it's in users path */
	snprintf(cmd, 256, "perl -e 'print join(\".\", unpack(\"C*\", "
			   "(gethostbyname(\"%s\"))[4])), \"\\n\"'", hostaddr);

	fd = popen(cmd, "r");
	if (fd != NULL && (p = fgets(line,128,fd)) != NULL &&
	   sscanf(p, "%d.%d.%d.%d", &ip,&a2,&a3,&a4) == 4) {
	    int iplen = strspn(p, "0123456789.");
	    *(p + iplen) = '\0';
	    pclose(fd);
	    if (inet_pton(AF_INET, p, &ip) < 1)
		return 0;
	    else
		return ip;
	}

	/* If perl failed, try parsing /usr/bin/host, although this may be with
	 * limited success since /usr/bin/host does not parse /etc/hosts */
	snprintf(cmd, 256, "%s -t a %s", GASNETC_HOST_PATH, hostaddr);

	fd = popen(cmd, "r");
	if (fd == NULL) {
	    fprintf(stderr, "Couldn't open %s\n", cmd);
	    return 0;
	}

	while ((p = fgets(line, 128, fd)) != NULL) {
	    /* Return the first matching ip -- wherever it is */
	    while (*p != '\0') {
		if (isdigit(*p) &&
		   sscanf(p, "%d.%d.%d.%d", &ip,&a2,&a3,&a4) == 4) {
		    int iplen = strspn(p, "0123456789.");
		    *(p + iplen) = '\0';
		    pclose(fd);
		    if (inet_pton(AF_INET, p, &ip) < 1)
			return 0;
		    else
			return ip;
		}
		p++;
	    }
	}
	/* is EOF, or error, or no match at all */
	pclose(fd);
	return 0;
    }
    else {
	if (inet_pton(AF_INET, ip_ptr, &ip) < 1)
	    return 0;
	else
	    return ip;
    }
}
#endif

static
char *
gasnetc_gexec_ip(char *buf, size_t buflen, gasnet_node_t nodeid)
{
	char	*svrs, *rank, *c, *e, *hostname;
	int	id, procs, i, len;
	size_t	s;

	svrs = getenv("GEXEC_SVRS");
	if (svrs == NULL || *svrs == '\0')
		return NULL;

	c = svrs;
	if (*c == ' ')
		c += strspn(svrs, " ");
	if (*c == '\0')
		return NULL;

	for (i = 0; i < nodeid; i++) {
		c += strcspn(c, " \t");
		if (*c == '\0') 
			return NULL;
		c += strspn(c, " ");
		if (*c == '\0') 
			return NULL;
	}

	len = strcspn(c, " \t");

	memcpy(buf, c, len);
	buf[len] = '\0';

	return buf;
}

static
void
gasnetc_get_nodeids(gasnet_node_t *myid, gasnet_node_t *numnodes)
{
	char    	*nprocs, *rank;
	gasnet_node_t	id, nodes;

	nprocs = getenv("GEXEC_NPROCS");

	if (nprocs != NULL && *nprocs != '\0') {
		nodes = (gasnet_node_t) atoi(nprocs);

		rank = getenv("GEXEC_MY_VNN");
		if (rank == NULL || *rank == '\0')
			GASNETC_SPAWNERR("Can't obtain the number of GASNet/GM processes");
		id = (gasnet_node_t) atoi(rank);
	}
	else {
		nprocs = getenv("GMPI_NP");
		if (nprocs == NULL || *nprocs == '\0')
			GASNETC_SPAWNERR("Can't obtain the number of GASNet/GM processes");
		nodes = (gasnet_node_t) atoi(nprocs);

		rank = getenv("GMPI_ID");
		if (rank == NULL || *rank == '\0')
			GASNETC_SPAWNERR("Can't obtain the number of GASNet/GM processes");
		id = (gasnet_node_t) atoi(rank);
	}

	if (nodes < 1) 
		gasneti_fatalerror("Bad number of processes: %d", nodes);
	
	if (id >= nodes)
		gasneti_fatalerror("Bad id %d out of %d processes", id, nodes);

	*numnodes = nodes;
	*myid = id;

	return;
}

void
gasnetc_getconf_mpiexec()
{
	char	*magic, *master, *port;
	char	*np, *board;
	char	buffer[GASNETC_SOCKET_BUFSIZ];
	char	*temp;
	int	sockfd, sockfd2;

	unsigned int	slave_port, master_port;

	unsigned int 	count, magic_number;
	unsigned int	board_id, port_id, temp_id, temp_local_id;
	unsigned int	i, j;

	struct sockaddr_in	sa;
		
	/* gasnetrun with sockets */
	GASNETC_GETENV ("GMPI_MAGIC", magic, "the job magic number");
	GASNETC_GETENV ("GMPI_MASTER", master, "the master's hostname");
	GASNETC_GETENV ("GMPI_PORT", port, "the master's port number");
	GASNETC_GETENV ("GMPI_BOARD", board, "the specified board");

	/* 
	 * Check for mpich 1.2.4..8, which we do _not_ support.
	 *
	 * XXX This might break if Myricom decides to revive PORT1 in a new
	 *     mpirun revision.  
	 */
	{
		char *t1, *t2;
		t1 = getenv("GMPI_PORT1");
		t2 = getenv("MPIEXEC_STDOUT_PORT");

		if (t1 != NULL && *t1 != '\0' &&
		    (t2 == NULL || *t2 == '\0'))
			gasneti_fatalerror(
		    "Bootstrap doesn't support spawning from MPICH 1.2.4..8");
	}

	if (sscanf (magic, "%ud", &magic_number) != 1)
		gasneti_fatalerror("(%s) Bootstrap: Bad magic number %s", gasnett_gethostname(), magic);
	_gmc.job_magic = magic_number;
		
	if (sscanf (port, "%ud", &master_port) != 1) 
		gasneti_fatalerror(
		    "(%s) Bootstrap: Bad master port 1 '%s', out of %d processes", 
		    gasnett_gethostname(), port, gasneti_nodes);
	_gmc.master_port = master_port;

	if (sscanf (board, "%ud", &board_id) != 1)
		gasneti_fatalerror("(%s) Bootstrap: Bad magic number: %s", gasnett_gethostname(), magic);
	_gmc.my_board = board_id;

	gasnetc_get_nodeids(&gasneti_mynode, &gasneti_nodes);

	port_id = 0;
	if (!gasnetc_gmport_allocate((int*)&board_id, (int*)&port_id))
		GASNETC_SPAWNERR("Can't obtain GM port");

	_gmc.my_port = port_id;

	/* get the GM node id */
	if (gm_get_node_id (_gmc.port, &_gmc.my_id) != GM_SUCCESS)
		GASNETC_SPAWNERR("Can't get local GM node id");

	#ifdef GASNETC_GM_2
	temp_id = _gmc.my_id;

	/* GM2 only stores local node ids, so a global has to be obtained */
	if (gm_node_id_to_global_id(_gmc.port, temp_id, &(_gmc.my_id)) 
	    != GM_SUCCESS)
		GASNETC_SPAWNERR("Couldn't get GM global node id");
	#endif

	/* allocate space for node mapping */
	if (!gasnetc_alloc_nodemap(gasneti_nodes))
		GASNETC_SPAWNERR("Can't allocate node mapping");

	/*
	 * Set up a socket for the master to connect o.
	 * 
	 */
	sockfd = socket (AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		GASNETC_SPAWNERR("Can't open first socket");
	else {
		char	buf[256];
		char	*slave;
		int	hostlen;
		uint32_t slave_n;

		slave = getenv("GMPI_SLAVE");
		if (slave == NULL || *slave == '\0') {
			slave = gasnetc_gexec_ip(buf, 256, gasneti_mynode);

			if (slave == NULL)
				GASNETC_SPAWNERR("Can't identify local hostname");
		}

		slave_n = gasnetc_parse_addr(slave);
		if (!slave_n)
		    gasneti_fatalerror("(%s) Couldn't get ip for %s", gasnett_gethostname(), slave);

		memset(&_gmc.slave_addr, 0, sizeof(struct sockaddr_in));
		_gmc.slave_addr.sin_family = AF_INET;
		_gmc.slave_addr.sin_addr.s_addr = slave_n;

		for (slave_port = 8000; slave_port < 20000; slave_port++) {
			_gmc.slave_addr.sin_port = htons(slave_port);
			if (bind(sockfd, (struct sockaddr *) &(_gmc.slave_addr),
			    sizeof(_gmc.slave_addr)) == 0)
				break;
		}
		if (slave_port >= 20000)
			GASNETC_SPAWNERR("Couldn't find a port to bind slave socket");
	}

	/*
	 * Listen for the master
	 */
	if (listen(sockfd, 1) != 0) GASNETC_SPAWNERR("can't listen() on socket");

	/*
	 * Get a second socket to connect to the master
	 *
	 */
	sockfd2 = socket (AF_INET, SOCK_STREAM, 0);
	if (sockfd2 < 0) GASNETC_SPAWNERR("Can't open second socket");
	else {
		gm_u64_t    start_time, stop_time;
		ssize_t	    b;
		int	    junk;
		uint32_t    master_n;

		master_n = gasnetc_parse_addr(master);
		if (!master_n)
		    gasneti_fatalerror("(%s) Couldn't get ip for %s", gasnett_gethostname(), master);

		memset(&_gmc.master_addr, 0, sizeof(struct sockaddr));
		_gmc.master_addr.sin_family = AF_INET;
		_gmc.master_addr.sin_addr.s_addr = master_n;
		_gmc.master_addr.sin_port = htons(_gmc.master_port);

		start_time = gm_ticks(_gmc.port);

		while (connect(sockfd2, (struct sockaddr *) 
		    &_gmc.master_addr, sizeof(_gmc.master_addr)) < 0) {

			stop_time = gm_ticks(_gmc.port);
			
			if ((stop_time - start_time) > 
			    (2000 * GASNETC_INIT_TIMEOUT))
                                GASNETC_SPAWNERR("Unable to connect to master");
		}

		/* 
		 * Send the magic:ID:port:board:node:numanode:pid::remoteport
		 * used to the master 
		 */

		count = 0;
		sprintf (buffer, 
			"<<<%d:%d:%d:%d:%u:%d:%d::%d>>>\n", 
			magic_number, gasneti_mynode, port_id, board_id, 
			(unsigned) _gmc.my_id, 0, (int) getpid(), slave_port);

		while (count < strlen (buffer)) {
			b = write(sockfd2, &buffer[count], 
				  strlen (buffer) - count);
			if (b < 0) GASNETC_SPAWNERR("can't write to socket");
			count += b;
		}
		close (sockfd2);
		
		/*
		 * Wait for the master to send the mapping
		 */
		sockfd2 = accept(sockfd, 0, 0);
		if (sockfd2 < 0) GASNETC_SPAWNERR("can't accept on socket");

		count = 0;
		memset(buffer, '0', GASNETC_SOCKET_BUFSIZ * sizeof(char));

		/*
		 * Read the entire mapping
		 *
		 */
		while (strstr (buffer, "]]]") == NULL) {
			b = read(sockfd2, &buffer[count], 
				 GASNETC_SOCKET_BUFSIZ-count);
			if (b < 0) GASNETC_SPAWNERR("can't read from socket");
			count += b;
		}

		close(sockfd2);


		/*
		 * Find where the mapping actually starts
		 */
		j = 0;
		if (strncmp (buffer, "[[[", 3) != 0) GASNETC_SPAWNERR("bad format in node data from master");

		/*
		 * Decrypt the mapping
		 *
		 * We ignore the board id (field 2) and numa_nodes (field 4).
		 */
		j += 3;
		for (i = 0; i < gasneti_nodes; i++) {
			if (sscanf (&buffer[j], "<%hu:%*d:%u:%*d>",
		    	    (unsigned short *) &_gmc.gm_nodes[i].port, &temp_id) != 2)
				GASNETC_SPAWNERR("can't decode node mapping");
			
			_gmc.gm_nodes_rev[i].port = _gmc.gm_nodes[i].port;
			_gmc.gm_nodes_rev[i].node = i;

			#ifdef GASNETC_GM_2
			temp_local_id = 0;
			if (gm_global_id_to_node_id(_gmc.port, temp_id,
			    &temp_local_id) != GM_SUCCESS)
				gasneti_fatalerror("%d(%s)> couldn't translate "
				    "GM global node id (%u) for gasnet "
				    "node id %d", gasneti_mynode, gasnett_gethostname(),
				    (unsigned) temp_id, i);

			_gmc.gm_nodes_rev[i].id = (uint16_t) temp_local_id;
			_gmc.gm_nodes[i].id = (uint16_t) temp_local_id;
			#else
			_gmc.gm_nodes[i].id = temp_id;
			_gmc.gm_nodes_rev[i].id = temp_id;
			#endif

			temp = strchr(&buffer[j], '>');
			if (temp == NULL) GASNETC_SPAWNERR("malformed node map");
			j += (size_t) (temp - &buffer[j] + 1);
		}

		/* 
		 * Skip the local mapping .
		 *
		 * The global->local ampping is separated by |||
		 */

		if (strncmp (&buffer[j], "]]]", 3) != 0 &&
		    strncmp (&buffer[j], "|||", 3) != 0) {
			char toto[96];
			snprintf(toto, 96, &buffer[j]);
			gasneti_fatalerror(
			    "%d(%s): can't decode node mapping (end marker): %s",
			    gasneti_mynode, gasnett_gethostname(), toto);
		}

		/* check consistency */
		if (_gmc.gm_nodes[gasneti_mynode].port != _gmc.my_port)
                  GASNETC_SPAWNERR("inconsistency in data collected from master");

		qsort(_gmc.gm_nodes_rev, gasneti_nodes,
				sizeof(gasnetc_gm_nodes_rev_t),
				gasnetc_gm_nodes_compare);
	}

	return;
}

#ifdef GASNETC_GM_MPI_COMPAT
#include <gasnet_bootstrap_internal.h>
void
gasnetc_getconf_bootmpi(int *argc, char ***argv)
{
    unsigned int    port_id = 0, board_id = 0;
    unsigned int    temp_id;
    unsigned int    my_idport[2];
    unsigned int    *global_idport;
    int		    i;

    gasneti_bootstrapInit_mpi(argc, argv, &gasneti_nodes, &gasneti_mynode);

    if (!gasnetc_gmport_allocate((int*)&board_id, (int*)&port_id))
	GASNETC_SPAWNERR("Can't obtain GM port");

    _gmc.my_port = port_id;

    /* get the GM node id */
    if (gm_get_node_id (_gmc.port, &_gmc.my_id) != GM_SUCCESS)
        GASNETC_SPAWNERR("Can't get local GM node id");

    #ifdef GASNETC_GM_2
    /* GM2 only stores local node ids, so a global has to be obtained */
    if (gm_node_id_to_global_id(_gmc.port, _gmc.my_id, &(_gmc.my_id)) != GM_SUCCESS)
	GASNETC_SPAWNERR("Couldn't get GM global node id");
    #endif

    /* allocate space for node mapping */
    if (!gasnetc_alloc_nodemap(gasneti_nodes)) GASNETC_SPAWNERR("Can't allocate node mapping");

    if ((global_idport = 
       gasneti_malloc(sizeof(unsigned int)*2*gasneti_nodes)) == NULL)
	GASNETC_SPAWNERR("Can't allocate node mapping");

    /* exchange port, id information */
    my_idport[0] = _gmc.my_id;
    my_idport[1] = _gmc.my_port;
    gasneti_bootstrapExchange_mpi(&my_idport, sizeof(unsigned int)*2, global_idport);

    for (i = 0; i < gasneti_nodes; i++) {

	temp_id = global_idport[i*2];
	_gmc.gm_nodes[i].port = 
	    _gmc.gm_nodes_rev[i].port = (uint16_t) global_idport[i*2+1];
	_gmc.gm_nodes_rev[i].node = i;

	#ifdef GASNETC_GM_2
	if (gm_global_id_to_node_id(_gmc.port, temp_id, &temp_id) != GM_SUCCESS)
	    gasneti_fatalerror(
	      "%d (%s)> couldn't translate GM global node id (%u) for gasnet node id %d", 
	      gasneti_mynode, gasnett_gethostname(), (unsigned) temp_id, i);
	#endif

	_gmc.gm_nodes[i].id = _gmc.gm_nodes_rev[i].id = (uint16_t) temp_id;
    }
    /* check consistency */
    if (_gmc.gm_nodes[gasneti_mynode].port != _gmc.my_port)
      GASNETC_SPAWNERR("inconsistency in data collected from MPI spawner");

    qsort(_gmc.gm_nodes_rev, gasneti_nodes, sizeof(gasnetc_gm_nodes_rev_t), 
	  gasnetc_gm_nodes_compare);

    gasneti_free(global_idport);
    return;
}
#endif

extern void
gasnetc_getconf (int *argc, char ***argv)
{
    setbuf (stdout, NULL);
    setbuf (stderr, NULL);

    #ifdef GASNETC_GM_MPI_COMPAT
	if (getenv("GASNET_GASNETRUN_GM"))
	    gasnetc_getconf_mpiexec();
	else
	    gasnetc_getconf_bootmpi(argc, argv);
    #else
	if (getenv("GMPI_MAGIC"))
	    gasnetc_getconf_mpiexec();
	else
	    gasnetc_getconf_conffile();
    #endif

    return;
}
