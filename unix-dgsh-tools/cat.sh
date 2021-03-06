#!/usr/bin/env bash
#
# Implementation of POSIX tee through dgsh-tee
#

# Process flags
args=$(getopt u "$@")
if [ $? -ne 0 ]; then
   echo 'Usage: cat [-u] [file ...]'
   exit 2
fi

for i in $args; do
   case "$1" in
   -u)
	   shift
	   ;;
   --)
	   shift; break
	   ;;
   esac
done

declare -a opts

# Process file arguments
for i; do
    opts+=('-i' "$i")
    shift
done

exec @libexecdir@/dgsh/dgsh-tee "${opts[@]}"
