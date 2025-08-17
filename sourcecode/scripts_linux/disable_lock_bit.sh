#!/bin/bash

set -e
function cleanup {
	rc=$?
	# If error and shell is child level 1 then stay in shell
	if [ $rc -ne 0 ] && [ $SHLVL -eq 1 ]; then exec $SHELL; else exit $rc; fi
}
trap cleanup EXIT

source env_linux.sh
$APP path=$SERIALPATH baud=9600 lock_off id_addr=0x0FFFDF id=00000000000000
if [ $SHLVL -eq 1 ]; then read -n 1 -s -r -p "Press any key to continue"; fi
