#!/bin/bash

TRK_MASTER_NAME="track.master"

SRC_DIR=""
TRK_DIR=""

SRC_FILES=""
INCLUDE_DIRS=""
SYSINCLUDE_DIRS=""

# Arguments:
# $1 - root dir (either $SRC_DIR or one of $(INCLUDE_DIRS) $(SYSINCLUDE_DIRS))
# $2 - source file path
fn_UPD_INCLUDE_TRACKERS_RC(){
	local root_dir=$1;
	local src_file=$2;

	local root_base=$(basename "$root_dir")

	local trk_file=$(echo "$src_file" | \
		sed -rn -e "s@($root_dir/)(.*)@$TRK_DIR/$root_base/\\2.tracker@p");

	local trk_marker=$(echo "$trk_file" | \
		sed -rn -e "s@(.*)(\\.tracker\$)@\\1.marker@p");

	# echo "DBG: root_dir $root_dir";
	# echo "DBG: src_file $src_file";
	# echo "DBG: root_base $root_base";
	# echo "DBG: trk_file $trk_file";
	# echo "DBG: trk_marker $trk_marker";
	# echo "DBG:";

	# Update current tracker file, if it is older, than the source.
	if [ "$trk_file" -ot "$src_file" ]; then
		mkdir -p $(dirname "$trk_file");
		rm -f "$trk_file";
		touch "$trk_file";

		# Get system includes:
		local src_sysheaders=$(cat $src_file | \
			sed -rn -e "s@(^\\#include <)([^>]*)(>)@\\2@p");
		# echo "DBG: src_sysheaders $src_sysheaders";
		# echo "DBG:";

		for src_header in $src_sysheaders; do
			for inc_dir in $SYSINCLUDE_DIRS; do
				local hdr_file=$inc_dir/$src_header;
				if [ -f "$hdr_file" ]; then
					echo "$inc_dir:$hdr_file" >> "$trk_file";
					break;
				fi;
			done;
		done;

		# Get regular includes:
		local src_regheaders=$(cat "$src_file" | \
			sed -rn -e "s@(^\\#include \")([^\"]*)(\")@\\2@p");
		# echo "DBG: src_regheaders $src_regheaders";
		# echo "DBG:";

		for src_header in $src_regheaders; do
			local hdr_file=$(dirname "$src_file")/"$src_header";
			if [ -f "$hdr_file" ]; then
				echo "$root_dir:$hdr_file" >> "$trk_file";
			else
				for inc_dir in $INCLUDE_DIRS; do
					hdr_file="$inc_dir"/"$src_header";
					if [ -f "$hdr_file" ]; then
						echo "$inc_dir:$hdr_file" >> "$trk_file";
						break;
					fi;
				done;
			fi;
		done;
	fi;

	# Now trk_file exists and is full of fresh paths to all the includes.
	# Time to apply the recursion!
	local src_hdr_files=$(cat "$trk_file");
	for child_paths_str in $src_hdr_files; do
		local child_paths=(${child_paths_str//:/ });

		local child_root_dir=${child_paths[0]};
		local child_src_file=${child_paths[1]};

		local child_root_base=$(basename "$child_root_dir");

		local child_trk_file=$(echo "$child_src_file" | \
			sed -rn -e \
			"s@($child_root_dir/)(.*)@$TRK_DIR/$child_root_base/\\2.tracker@p");

		local child_trk_marker=$(echo "$child_trk_file" | \
			sed -rn -e "s@(.*)(.tracker\$)@\\1.marker@p");

		# echo "DBG: child_root_dir $child_root_dir";
		# echo "DBG: child_src_file $child_src_file";
		# echo "DBG: child_root_base $child_root_base";
		# echo "DBG: child_trk_file $child_trk_file";
		# echo "DBG: child_trk_marker $child_trk_marker";
		# echo "DBG:";

		if [ "$child_trk_marker" -ot "$TRK_DIR/$TRK_MASTER_NAME" ]; then
			# echo "DBG: fn_UPD_INCLUDE_TRACKERS_RC args: $child_root_dir";
			# echo "DBG: fn_UPD_INCLUDE_TRACKERS_RC args: $child_src_file";
			# echo "DBG:";
			fn_UPD_INCLUDE_TRACKERS_RC $child_root_dir $child_src_file;
			if [ "$trk_file" -ot "$child_trk_file" ]; then
				touch "$trk_file";
			fi;
		fi;
	done;

	# Now all the subtree of the tracker is fresh.
	# Time to mark it, so other trackers will not re-check it!
	touch "$trk_marker";
	# echo "DBG: mark $trk_marker";
	# echo "DBG:";
};

# Main.
# Arguments:
# $@ - command-line oprions in any order.
# No dirname shall be ending with '..'
# '--source-dir' and '--tracker-dir' must be set, otherwise the script breaks.
SPIT_BACK=();
while [[ $# -gt 0 ]]; do
	OPT=$1;
	case $OPT in
		--source-dir) # A single directory, containing source files
			SRC_DIR=$2;
			shift; # swallow $1
			shift; # swallow $2
			;;
		--tracker-dir) # A directory, where tracker can place it's files
			TRK_DIR=$2
			shift;
			shift;
			;;
		--source-files) # Comma-separated list of source files to track (paths)
			FILES_TMP=$2;
			SRC_FILES=${FILES_TMP//,/ };
			shift;
			shift;
			;;
		--include-dirs) # Comma-separated list of include directories
			DIRS_TMP=$2;
			INCLUDE_DIRS=${DIRS_TMP//,/ };
			shift;
			shift;
			;;
		--sysinclude-dirs) # Comma-separated list of sysinclude directories
			DIRS_TMP=$2;
			SYSINCLUDE_DIRS=${DIRS_TMP//,/ };
			shift;
			shift;
			;;
		*)
			SPIT_BACK+=$1; # remember to spit $1 back
			shift; # but for now swallow it
			;;
	esac;
done;
set -- "${SPIT_BACK[@]}"; # spit back everything, that is not ours


# echo "DBG: SRC_DIR $SRC_DIR";
# echo "DBG: TRK_DIR $TRK_DIR";
# echo "DBG: SRC_FILES $SRC_FILES";
# echo "DBG: INCLUDE_DIRS $INCLUDE_DIRS";
# echo "DBG: SYSINCLUDE_DIRS $SYSINCLUDE_DIRS";
# echo "DBG:";

mkdir -p $TRK_DIR;
touch $TRK_DIR/$TRK_MASTER_NAME;
for file in $SRC_FILES; do
	if [ -n "$(echo "$file" | grep '.*\.cpp')" ]; then
		# echo "DBG: fn_UPD_INCLUDE_TRACKERS_RC args: $SRC_DIR $file";
		# echo "DBG:";
		fn_UPD_INCLUDE_TRACKERS_RC $SRC_DIR $file;
	fi;
done;


