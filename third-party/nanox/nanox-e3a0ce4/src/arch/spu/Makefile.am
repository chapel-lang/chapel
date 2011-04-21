#####################################################################################
#      Copyright 2009 Barcelona Supercomputing Center                               #
#                                                                                   #
#      This file is part of the NANOS++ library.                                    #
#                                                                                   #
#      NANOS++ is free software: you can redistribute it and/or modify              #
#      it under the terms of the GNU Lesser General Public License as published by  #
#      the Free Software Foundation, either version 3 of the License, or            #
#      (at your option) any later version.                                          #
#                                                                                   #
#      NANOS++ is distributed in the hope that it will be useful,                   #
#      but WITHOUT ANY WARRANTY; without even the implied warranty of               #
#      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
#      GNU Lesser General Public License for more details.                          #
#                                                                                   #
#      You should have received a copy of the GNU Lesser General Public License     #
#      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             #
#####################################################################################

SUBDIRS = performance debug instrumentation instrumentation-debug

include $(top_srcdir)/src/common.am

EXTRAD_DIST= spu_idle.spu

SPUCCFLAGS=-I @CELLSDK_INC@

.spu.o: $<
	$(SPUCC) $(SPUCCFLAGS) -x c -o $@ $<

.libs/libspu_embed.a : spu_idle.o
	$(PPUEMBED) -m32 spu_idle $< null.o
	$(AR) cru $@ null.o
	$(RM) -rf null.o
	$(RANLIB) $@

