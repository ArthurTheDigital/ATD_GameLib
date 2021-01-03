
# Shall be predefined

# ROOTDIR
# NAME
# DEFINES
# LIBS


# Basic defines

SRCDIR := $(ROOTDIR)/Test/$(NAME)
INCDIR := $(ROOTDIR)/Include
LIBDIR := $(ROOTDIR)/Lib
DATADIR := $(SRCDIR)/DATA

OBJDIR := $(ROOTDIR)/Obj/Test/$(NAME)
BINDIR := $(ROOTDIR)/Bin/$(NAME)

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
SRCDATA := $(filter $(DATADIR)/%,$(call atd-collect-files,$(SRCDIR)))
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

# NOTE:
# Weird: in order to link libraries I had to use cd and some 
# black magic flags. But in order to link executable I don't 
# have to


