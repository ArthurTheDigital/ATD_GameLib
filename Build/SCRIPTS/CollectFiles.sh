#!/bin/bash

# Arguments:
# $1 - directory to ls
fn_LS_FILES_RC(){
	local file=$1;
	if [ -d "$file" ]; then
		local files=$(ls $file/);
		for f in $files; do
			fn_LS_FILES_RC $file/$f;
		done;
	elif [ -f "$file" ]; then
		echo "$file";
	fi;
};

echo $(fn_LS_FILES_RC $1);


