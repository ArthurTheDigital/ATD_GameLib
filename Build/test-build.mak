


atd-resolve-headers = $(sort $(foreach wrd,$(1),$(shell \
	RecSysInc(){\
		hdrs=$$(cat $$1 | sed -rn 's|(\#include <)([^>]*)(>)|\2|p' | sed -rn 's|ATD/*|&|p');\
		dirs=$$(echo "$$@" | cut -d ' ' -f 1 --complement);\
		for h in $$hdrs; do\
			for d in $$dirs; do\
				if [ -f "$$d/$$h" ];\
					then\
					RecSysInc $$d/$$h $$d;\
					echo "$$d/$$h";\
				fi;\
			done;\
		done;\
	};\
	echo "$$(RecSysInc $(wrd) $(2))"\
)))



DIRS += ../Src
DIRS += ../Include

FILE := ../Src/ATD/Window/Window.cpp

.PHONY: default

default:
	echo "$(call atd-resolve-headers,$(FILE),$(DIRS))"
	



