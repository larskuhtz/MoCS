# *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
#
# This copyright notice is auto-generated by ./add-copyright-notice.
# Additional copyright notices must be added below the last line of this notice.
#
# MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "Makefile".
# The content of this file is copyright of Saarland University -
# Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
#
# This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
#
# License: three-clause BSD style license.
# The license text can be found in the file LICENSE.
#
# *** MOCS-COPYRIGHT-NOTICE-END ***

ROOT=.

include $(ROOT)/Makefile.pre

.PHONY: archive brelease hsdoc all

MODULEDEPS=libaaut c-bindings haskell-bindings test haskell-frontend # cppfrontend

MODULEDEPSTARGET=$(MAKECMDGOALS)

HSSOURCES:=$(wildcard haskell-bindings/*.hs haskell-frontend/*.hs test/*.hs)
hsdoc: $(HSSOURCES)
	mkdir -p haskell-docs/src
	HsColour -print-css > haskell-docs/src/hscolour.css
	for i in $^ ; do echo haskell-docs/src/`basename $$i .hs`.html; HsColour -css -anchor $$i > haskell-docs/src/`basename $$i .hs`.html ; done
	haddock --html --title="psl-monitor-synth" --odir=haskell-docs   \
	        --source-module='src/%M.html' --source-entity='src/%M.html#%N' $^

archive:
	hg archive -p mocs -t tbz2 mocs.tar.bz2 -X '\.hg[a-z]*' -X 'add-copyright-notice'

brelease: MODULEDEPSTARGET=
brelease: haskell-frontend
	mkdir -p mocs
	cp LICENSE README haskell-frontend/Main mocs
	mv mocs/Main mocs/MoCS
	tar czf "mocs-$(VERSION)-$(shell hg id -i).tar.gz" mocs
	rm -rf mocs

include $(ROOT)/Makefile.post
