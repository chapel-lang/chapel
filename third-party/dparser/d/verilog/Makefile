#
# Makefile
#
# $Revision$
#

DPARSERDIR= ..
GRAMMAR= verilog.g
DPARSEFLAGS= -l -v

TARGET= v
CSRCS= main.c vparse.c ambig.c
OBJS= $(CSRCS:.c=.o) $(GRAMMAR).d_parser.o
INCDIRSS= -I$(DPARSERDIR)
CFLAGS= -g $(INCDIRSS)

#########

target: $(TARGET)

##########

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) \
		-L$(DPARSERDIR) -ldparse

$(GRAMMAR).d_parser.c: $(GRAMMAR)
	$(DPARSERDIR)/make_dparser $(DPARSEFLAGS) $(GRAMMAR)

lint:
	lint $(CFLAGS) $(CSRCS)

test:	$(TARGET)
	./verilog_tests
	grep succeeded log.txt | wc -l
	grep failed log.txt | wc -l

clean:
	rm -f $(GRAMMAR).d_parser.c $(TARGET) $(OBJS) log.txt
