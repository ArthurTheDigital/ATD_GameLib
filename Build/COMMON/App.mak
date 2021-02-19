
# Shall be predefined:

# ROOTDIR - root dir for app itself
# BUILDDIR - build dir for the app
# NAME
# LIBDIR - dir with library binaries
# INCDIR - dir with library include headers

# DEFINES
# LIBS


# Primary vars:

DATADIR := $(ROOTDIR)/Data
SRCDIR := $(ROOTDIR)/Src
FILEDIRS := $(SRCDIR) $(INCDIR)

OBJDIR := $(ROOTDIR)/Obj
BINDIR := $(ROOTDIR)/Bin

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

# List files if exist
list-existing-files = $(foreach wrd,$(1),$(shell \
	if [ -f $(wrd) ];\
		then echo "$(wrd)";\
	fi;\
))


# Secondary vars:

ATDLIBFILES := $(patsubst %,$(LIBDIR)/lib%.so,$(filter atd-%,$(LIBS)))
LIBFILES := $(call list-existing-files,$(ATDLIBS))

FILES := $(call collect-files,$(FILEDIRS))
SOURCES := $(filter $(SRCDIR)/%.cpp,$(FILES))

OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SOURCES))
BINARY := $(BINDIR)/$(NAME)

SRCDATA := $(call collect-files,$(DATADIR))
BINDATA := $(patsubst $(DATADIR)/%,$(BINDIR)/%,$(SRCDATA))

DIRS += $(patsubst %/,%,$(dir $(OBJECTS)))
DIRS += $(patsubst %/,%,$(dir $(BINDATA)))
DIRS := $(sort $(DIRS))

CURDIR := $(shell pwd)

TRACKDIR := $(OBJDIR)/TRACKER
TRACKMASTER := $(TRACKDIR)/track.master
TRACKDIRSRC := $(TRACKDIR)/$(notdir $(SRCDIR))


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


# Targets

.PHONY: bin tracker clean
.SECONDEXPANSION:


# Bin:

bin: $(TRACKMASTER) $(BINARY) $(BINDATA)

$(BINARY): $(OBJECTS) \
	$(LIBFILES) \
	| $$(@D)
	$(CXX) $(CFLAGS) $(LDPREFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.cpp.o: $(SRCDIR)/%.cpp \
	$$(call list-existing-files,$$(TRACKDIRSRC)/%.cpp.tracker) \
	| $$(@D)
	$(CXX) -c $(CFLAGS) $(INCFLAGS) $(DEFFLAGS) -o $@ $<

$(BINDATA): $$(patsubst $$(BINDIR)/%,$$(DATADIR)/%,$$@) \
	| $$(@D)
	cp $(patsubst $(BINDIR)/%,$(DATADIR)/%,$@) $@


# Tracker:

tracker: $(TRACKMASTER)

$(TRACKMASTER): $(FILES)
	chmod ugo+x ./$(BUILDDIR)/SCRIPTS/UpdateTrackers.sh
	./$(BUILDDIR)/SCRIPTS/UpdateTrackers.sh \
		--source-dir $(SRCDIR) \
		--tracker-dir $(TRACKDIR) \
		--source-files $(call cast-to-comma-list,$(SOURCES)) \
		--sysinclude-dirs $(INCDIR)


# Clean:

clean:
	rm -Rf $(BINDIR)
	rm -Rf $(OBJDIR)


# Dirs:

$(DIRS):
	mkdir -pv $@


