
# Shall be predefined:

# ROOTDIR - root dir for library and tests
# BUILDDIR - dir with build scripts
# NAME - the name of the lib component

# LIBS - libs, the component depends on
# ATD_LIBS - other ATD components, the current one depends on


# Primary vars:

SRCDIR := $(ROOTDIR)/Src
INCDIR := $(ROOTDIR)/Include
DEPDIR := $(ROOTDIR)/Deps
FILEDIRS := $(SRCDIR) $(INCDIR)

OBJDIR := $(ROOTDIR)/ObjLib
LIBDIR := $(ROOTDIR)/Lib

DIRS += $(OBJDIR)
DIRS += $(LIBDIR)


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

# Cast all words to lowercase.
lowercase = $(foreach wrd,$(1),$(shell \
	echo "$(wrd)" | sed -e 's@\(.*\)@\L\1@'\
))

# Make library .so filename from a component name.
comp-bin-so = $(patsubst %,$(LIBDIR)/libatd-%.so,$(call lowercase,$(1)))

# List only files that exist from a given list
list-existing-files=$(foreach wrd,$(1),$(shell \
	if [ -f $(wrd) ]; then echo "$(wrd)"; fi;\
))


# Secondary vars:

FILES := $(call collect-files,$(FILEDIRS))
SOURCES := $(filter $(SRCDIR)/ATD/$(NAME)/%.cpp,$(FILES))

OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SOURCES))
BINARY := $(call comp-bin-so,$(NAME))

DIRS += $(patsubst %/,%,$(dir $(OBJECTS)))
DIRS := $(sort $(DIRS))

TRACKMASTERNAME := track-$(call lowercase,$(NAME)).master
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
LDPREFLAGS += -Wl$(COMMA)-rpath$(COMMA)\$$ORIGIN$(COMMA)--enable-new-dtags
LDFLAGS += $(addprefix -l,$(LIBS))
LDFLAGS += $(addprefix -latd-,$(ATDLIBS))


# Targets:

.PHONY: lib clean
.SECONDEXPANSION:


# Lib:

# Do not change prerequisites order here!
lib: $(BINARY)

# Black magic with linker requires cd. I don't know exactly, why, but it requires.
#
# I tried liking with '-L$(LIBDIR)' flag set, it links itself successfully, but linking 
# with an executable resulted into error. And with cd and '-L.' flags both linkings  went 
# fine.
# (In both cases executable, of course, was linked with -L<path-to-lib> set.)
$(BINARY): $(OBJECTS) \
	| $$(@D)
	(cd $(LIBDIR);\
		$(CXX) $(CFLAGS) $(LDPREFLAGS) \
		-o $(notdir $@) $(patsubst $(ROOTDIR)/%,../%,$(OBJECTS)) $(LDFLAGS))

$(OBJDIR)/%.cpp.o: $(SRCDIR)/%.cpp \
	$$(call list-existing-files,$$(TRACKDIRSRC)/%.cpp.tracker) \
	| $$(@D)
	$(CXX) -c $(CFLAGS) $(INCFLAGS) -o $@ $<


# Clean:

clean:
	rm -f $(BINARY)
	rm -f $(TRACKMASTER)
	rm -Rf $(OBJDIR)/ATD/$(NAME)


# Directories:

$(DIRS):
	mkdir -pv $@


