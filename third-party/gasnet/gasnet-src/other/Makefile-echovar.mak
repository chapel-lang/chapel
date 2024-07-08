include $(FRAGMENT)

.EXPORT_ALL_VARIABLES:

# echovar:
#  output a VAR=VAL string for provided $(VAR)
#  if the variable is unset, the result is an empty VAL

echovar:
	@echo $(VAR)=$($(VAR))

# Iterative form accepts variable names in $(VARS)
echovars:
	@for var in $(VARS) ; do  \
	   eval echo $$var=\$$$$var ; \
	 done

# echovar-preserve-unset:
#  output a VAR=VAL string for provided $(VAR)
#  if the variable is unset, the result is no output

echovar-preserve-unset:
	@if ! test -z $${$(VAR)+set} ; then \
	    echo $(VAR)=$($(VAR)) ; \
	 fi 

# Iterative form accepts variable names in $(VARS)
echovars-preserve-unset:
	@for var in $(VARS) ; do  \
	   if ! eval test -z \$${$${var}+set} ; then \
	     eval echo $$var=\$$$$var ; \
	   fi ; \
	 done


.PHONY: echovar echovars echovar-preserve-unset echovars-preserve-unset

