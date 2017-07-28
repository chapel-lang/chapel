include $(FRAGMENT)

echovar:
	@echo $(VAR)=$($(VAR))

#export $(VARS)
.EXPORT_ALL_VARIABLES:

echovars:
	@for var in $(VARS) ; do  \
	   eval echo $$var=\$$$$var ; \
	 done

.PHONY: echovar echovars

