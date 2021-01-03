
# Shall be predefined:
#
# LIBDIR - dir with library binaries
# INCDIR - dir with library include headers
#
# ROOTDIR - root dir for app itself
# NAME
# DEFINES
# LIBS


# Basic defines

SRCDIR := $(ROOTDIR)/Src
DATADIR := $(ROOTDIR)/Data

OBJDIR := $(ROOTDIR)/Obj
BINDIR := $(ROOTDIR)/Bin

DIRS += $(OBJDIR)/
DIRS += $(BINDIR)/


# Basic functions

atd-collect-files = $(foreach wrd,$(1),$(shell \
	recls(){\
		for f in $$(ls $$1);do\
			if [ "$$(stat -c%F $$1/$$f)" = "directory" ];\
				then recls $$1/$$f;\
				else echo "$$1/$$f";\
			fi;\
		done;\
	};\
	echo $$(recls $(wrd))\
))

__COMMA := ,


# Secondary defines

SOURCES := $(filter %.cpp,$(call atd-collect-files,$(SRCDIR)))
HEADERS := $(filter %.hpp,$(call atd-collect-files,$(SRCDIR)))
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SOURCES))
SRCDATA := $(call atd-collect-files,$(DATADIR))
BINDATA := $(patsubst $(DATADIR)/%,$(BINDIR)/%,$(SRCDATA))

DIRS += $(sort $(dir $(OBJECTS)))
DIRS += $(sort $(dir $(BINDATA)))
DIRS := $(sort $(DIRS))

BINARY := $(BINDIR)/$(NAME)
CURDIR := $(shell pwd)

CXX := g++
CFLAGS += -std=c++11
CFLAGS += -Wall
CFLAGS += -Wno-format-zero-length
CFLAGS += -g
CFLAGS += -O2
DEFFLAGS += $(addprefix -D,$(DEFINES))
INCFLAGS += $(addprefix -isystem ,$(INCDIR))
LDPREFLAGS += -L$(CURDIR)/$(LIBDIR)
LDPREFLAGS += -Wl$(__COMMA)-rpath$(__COMMA)$(CURDIR)/$(LIBDIR)
LDPREFLAGS += -Wall
LDFLAGS += $(addprefix -l,$(LIBS))


# Targets

.PHONY: all clean

.SECONDEXPANSION:

all: $(BINARY) $(BINDATA)

clean:
	rm -Rf $(BINDIR)
	rm -Rf $(OBJDIR)

$(BINARY): $(OBJECTS) | $$(dir $$@)
	$(CXX) $(CFLAGS) $(LDPREFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.cpp.o: $(SRCDIR)/%.cpp $(HEADERS) | $$(dir $$@)
	$(CXX) -c $(CFLAGS) $(INCFLAGS) $(DEFFLAGS) -o $@ $<

$(BINDATA): $$(patsubst $$(BINDIR)/%,$$(DATADIR)/%,$$@) | $$(dir $$@)
	cp $(patsubst $(BINDIR)/%,$(DATADIR)/%,$@) $@

$(DIRS):
	mkdir -pv $@


