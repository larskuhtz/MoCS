# *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
#
# This copyright notice is auto-generated by ./add-copyright-notice.
# Additional copyright notices must be added below the last line of this notice.
#
# MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "c-bindings/Makefile".
# The content of this file is copyright of Saarland University -
# Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz.
#
# This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
#
# License: three-clause BSD style license.
# The license text can be found in the file LICENSE.
#
# *** MOCS-COPYRIGHT-NOTICE-END ***

ROOT=..

include $(ROOT)/Makefile.pre

MODULEDEPS=libaaut
LIBTARGETS=libaautc.a

LIBAAUTSCRS=$(wildcard $(ROOT)/libaaut/*.hh) $(wildcard $(ROOT)/libaaut/*.tcc) $(wildcard $(ROOT)/libaaut/*.cc) $(ROOT)/libaaut/config.h

# deps
aa_c.o : aa_c.cc aa.h expr.h context.h $(LIBAAUTSCRS)
expr_c.o: expr_c.cc expr.h context.h   $(LIBAAUTSCRS)
context_c.o: context_c.cc context.h    $(LIBAAUTSCRS)
sm_c.o: sm_c.cc aa.h expr.h context.h  $(LIBAAUTSCRS)

include $(ROOT)/Makefile.post
