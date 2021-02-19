
# Defines:

ROOTDIR := ..
BUILDDIR := $(ROOTDIR)/Build

OBJDIR := $(ROOTDIR)/ObjLib
LIBDIR := $(ROOTDIR)/Lib

# ============================================= #
# Component order matters a lot! Do not change! #
# ============================================= #

COMPONENTS += Core
COMPONENTS += Ansi
COMPONENTS += Graphics
COMPONENTS += Window

# ============================================= #

COMPBUILDD := $(addprefix $(BUILDDIR)/,$(COMPONENTS))


# Targets:

.PHONY: default clean

default:
	for COMPD in $(COMPBUILDD); do $(MAKE) -C $$COMPD; done

clean:
	for COMPD in $(COMPBUILDD); do $(MAKE) clean -C $$COMPD; done
	rm -Rf $(OBJDIR)
	rm -Rf $(LIBDIR)

