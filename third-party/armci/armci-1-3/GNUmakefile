all:
	(cd ./src; $(MAKE) )

clean:
	(cd ./src; $(MAKE) clean )


HAS_TAR_EXCLUDE = SUN LINUX LINUX64 SOLARIS SOLARIS64 CYGNUS

# we might have GNU tar available on any platform (requires setting GNUTAR)
ifdef GNUTAR
  HAS_TAR_EXCLUDE += $(TARGET)
  TAR = gtar
else
  TAR = tar
endif

TOPDIR = armci
EXCLUDEF = /tmp/exclude.$(USER)

%.tar:
ifeq ($(TARGET),$(findstring $(TARGET),$(HAS_TAR_EXCLUDE)))
	(cd ..;\
	rm -f $(EXCLUDEF);\
	echo '' > $(EXCLUDEF);\
	find ./$(TOPDIR) -name "*" -type d -print |grep CVS | \
	        sed  -e 's/\.\///' >> $(EXCLUDEF);\
	find ./$(TOPDIR) -name "*.[oxpa]" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	find ./$(TOPDIR) -name "*core" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	find ./$(TOPDIR) -name "lib*" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	find ./$(TOPDIR) -name "fujitsu*.h" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	find ./$(TOPDIR) -name "include*" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	find ./$(TOPDIR) -name "*obj" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	find ./$(TOPDIR) -name "*exe" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	find ./$(TOPDIR) -name "*stamp" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	find ./$(TOPDIR) -name "include/?*" -print|sed -e 's/\.\///' >>$(EXCLUDEF);\
	 $(TAR) cvfX $@ $(EXCLUDEF) $(TOPDIR))
	rm -f $(EXCLUDEF)
else
	(cd ..; $(TAR) cvf $@ $(TOPDIR))
endif

