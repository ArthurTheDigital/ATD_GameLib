
# Basic defines

SRCDIR := $(ROOTDIR)/Src
INCDIR := $(ROOTDIR)/Include
DEPDIR := $(ROOTDIR)/Deps
HDRDIRS := $(SRCDIR) $(INCDIR)

OBJDIR := $(ROOTDIR)/Obj/Lib
LIBDIR := $(ROOTDIR)/Lib

DIRS += $(OBJDIR)
DIRS += $(LIBDIR)


# Basic functions

atd-lowercase = $(foreach wrd,$(1),$(shell \
	echo "$(wrd)" | sed -e 's@\(.*\)@\L\1@'\
))
atd-first-uppercase = $(foreach wrd,$(1),$(shell \
	echo "$(wrd)" | sed -e 's@\(.\)@\U\1@'\
))

atd-comp-bin-so = $(patsubst %,$(LIBDIR)/libatd-%.so,$(call atd-lowercase,$(1)))
atd-comp-name = $(call atd-first-uppercase,$(patsubst $(LIBDIR)/libatd-%.so,%,$(1)))

atd-filter-comp-objects = $(filter $(OBJDIR)/$(call atd-comp-name,$(1))/%,$(2))

atd-collect-files = $(foreach wrd,$(1),$(shell \
	RecLs(){\
		for f in $$(ls $$1); do\
			if [ "$$(stat -c%F $$1/$$f)" = "directory" ];\
				then RecLs $$1/$$f;\
				else echo "$$1/$$f";\
			fi;\
		done;\
	};\
	echo $$(RecLs $(wrd))\
))

atd-resolve-headers = $(sort $(foreach wrd,$(1),$(shell \
	RecSysInc(){\
		hdrs=$$(cat $$1 | sed -rn -e '\_\#include\ <[^>]*>_ { s_(\#include <)([^>]*)(>)_\2_; s_ATD/.*_&_p}');\
		dirs=$$(echo "$$@" | cut -d ' ' -f 1 --complement);\
		for h in $$hdrs; do\
			for d in $$dirs; do\
				if [ -f "$$d/$$h" ];\
					then echo "$$d/$$h $$(RecSysInc $$d/$$h $$dirs)";\
				fi;\
			done;\
		done;\
	};\
	echo "$$(RecSysInc $(wrd) $(2))"\
)))


# Secondary defines

__COMMA := ,

SOURCES := $(filter %.cpp,$(call atd-collect-files,$(SRCDIR)))
HEADERS := $(call atd-collect-files,$(INCDIR))
HEADERS += $(filter %.hpp,$(call atd-collect-files,$(SRCDIR)))
OBJECTS := $(patsubst $(SRCDIR)/ATD/%,$(OBJDIR)/%.o,$(SOURCES))

DIRS += $(sort $(dir $(OBJECTS)))
CURDIR := $(shell pwd)

BINARY := $(call atd-comp-bin-so,$(NAME))

CXX := g++
CFLAGS += -std=c++11
CFLAGS += -fPIC
CFLAGS += -shared
CFLAGS += -Wall
CFLAGS += -Wno-format-zero-length
CFLAGS += -g
CFLAGS += -O2
INCFLAGS += $(addprefix -isystem ,$(INCDIR))
INCFLAGS += $(addprefix -isystem ,$(SRCDIR))
INCFLAGS += $(addprefix -isystem ,$(DEPDIR))
LDPREFLAGS += -L.
#LDPREFLAGS += -Wl$(__COMMA)-rpath-link$(__COMMA).
LDPREFLAGS += -Wl$(__COMMA)-rpath$(__COMMA)\$$ORIGIN$(__COMMA)--enable-new-dtags
LDFLAGS += $(addprefix -l,$(LIBS))
LDFLAGS += $(addprefix -latd-,$(ATDLIBS))


# Targets

.PHONY: lib clean

.SECONDEXPANSION:

lib: $(BINARY)

clean:
	rm -f $(BINARY)
	rm -Rf $(OBJDIR)

$(BINARY): $$(call atd-filter-comp-objects,$$@,$$(OBJECTS)) | $$(dir $$@)
	(cd $(LIBDIR);\
		$(CXX) $(CFLAGS) $(LDPREFLAGS) $(call atd-comp-flag,$@) \
		-o $(notdir $@) $(patsubst $(OBJDIR)/%,../Obj/Lib/%,$^) $(LDFLAGS))

$(OBJDIR)/%.cpp.o: $(SRCDIR)/ATD/%.cpp \
		$$(call atd-resolve-headers,$$(SRCDIR)/ATD/%.cpp,$$(HDRDIRS)) | $$(dir $$@)
	$(CXX) -c $(CFLAGS) $(INCFLAGS) -o $@ $<

$(DIRS):
	mkdir -pv $@


