
# Shall be predefined:

# ROOTDIR - root dir for library and tests
# BUILDDIR - dir with build scripts
# NAME - the name of the test

# DEFINES
# LIBS


# Primary vars:

DATADIR := $(ROOTDIR)/Test/$(NAME)/Data
SRCDIR := $(ROOTDIR)/Test/$(NAME)/Src
INCDIR := $(ROOTDIR)/Include
LIBDIR := $(ROOTDIR)/Lib
FILEDIRS := $(SRCDIR) $(INCDIR)

OBJDIR := $(ROOTDIR)/ObjBin/$(NAME)
BINDIR := $(ROOTDIR)/Bin/$(NAME)

DIRS += $(OBJDIR)
DIRS += $(BINDIR)


# Functions:

# Syntax macros.
NULL := 
SPACE := $(NULL) #
COMMA := ,

# Collect file paths of all the files in the given directories.
collect-files = $(foreach directory,$(1),$(shell \
	chmod ugo+x $(BUILDDIR)/SCRIPTS/CollectFiles.sh;\
	./$(BUILDDIR)/SCRIPTS/CollectFiles.sh $(directory);\
))

# Cast a list to comma-separated list.
cast-to-comma-list = $(subst $(SPACE),$(COMMA),$(strip $(1)))

# List only files that exist from a given list
list-existing-files=$(foreach wrd,$(1),$(shell \
	if [ -f $(wrd) ]; then echo "$(wrd)"; fi;\
))


# Secondary vars:

FILES := $(call collect-files,$(FILEDIRS))
SOURCES := $(filter $(SRCDIR)/%.cpp,$(FILES))

OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SOURCES))
BINARY := $(BINDIR)/$(NAME)

SRCDATA := $(call collect-files,$(DATADIR))
BINDATA := $(patsubst $(DATADIR)/%,$(BINDIR)/%,$(SRCDATA))

CURDIR := $(shell pwd)

DIRS += $(patsubst %/,%,$(dir $(OBJECTS)))
DIRS += $(patsubst %/,%,$(dir $(BINDATA)))
DIRS := $(sort $(DIRS))

TRACKMASTERNAME := track.master
TRACKDIR := $(OBJDIR)/TRACKER
TRACKMASTER := $(TRACKDIR)/$(TRACKMASTERNAME)
TRACKDIRSRC := $(TRACKDIR)/$(notdir $(SRCDIR))

# Unconditionally update all the trackers.
TRACKRESULT := $(shell \
	chmod ugo+x ./$(BUILDDIR)/SCRIPTS/UpdateTrackers.sh; \
	./$(BUILDDIR)/SCRIPTS/UpdateTrackers.sh \
		--source-dir $(SRCDIR) \
		--tracker-dir $(TRACKDIR) \
		--source-files $(call cast-to-comma-list,$(SOURCES)) \
		--sysinclude-dirs $(SRCDIR),$(INCDIR) \
		--master-name $(TRACKMASTERNAME))


# Compilation vars:

CXX := g++
CFLAGS += -std=c++11
CFLAGS += -Wall
CFLAGS += -Wno-format-zero-length
CFLAGS += -g
CFLAGS += -O2
DEFFLAGS += $(addprefix -D,$(DEFINES))
INCFLAGS += $(addprefix -isystem ,$(INCDIR))
LDPREFLAGS += -L$(CURDIR)/$(LIBDIR)
LDPREFLAGS += -Wl$(COMMA)-rpath$(COMMA)$(CURDIR)/$(LIBDIR)
LDPREFLAGS += -Wall
LDFLAGS += $(addprefix -l,$(LIBS))


# Targets:

.PHONY: bin clean
.SECONDEXPANSION:


# Bin:

bin: $(BINARY) $(BINDATA)

$(BINARY): $(OBJECTS) \
	| $$(@D)
	$(CXX) $(CFLAGS) $(LDPREFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.cpp.o: $(SRCDIR)/%.cpp \
	$$(call list-existing-files,$$(TRACKDIRSRC)/%.cpp.tracker) \
	| $$(@D)
	$(CXX) -c $(CFLAGS) $(INCFLAGS) $(DEFFLAGS) -o $@ $<

$(BINDATA): $$(patsubst $$(BINDIR)/%,$$(DATADIR)/%,$$@) \
	| $$(@D)
	cp $(patsubst $(BINDIR)/%,$(DATADIR)/%,$@) $@


# Clean:

clean:
	rm -Rf $(BINDIR)
	rm -Rf $(OBJDIR)


# Dirs:

$(DIRS):
	mkdir -pv $@


