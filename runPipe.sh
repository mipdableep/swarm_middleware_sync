#!/bin/bash

MY_PIPE="/home/pi/swarm_middleware_sync/PIPE"

# Create the named pipe if it doesn't already exist
if [ ! -p "$MY_PIPE" ]; then
    mkfifo "$MY_PIPE"
fi

# Continuously evaluate the contents of the named pipe
while true; do
    eval "$(cat "$MY_PIPE")"
done

# one liner to output PIPE to log.txt
# while true; read -r line < ./PIPE; do echo "$line" | tee -a ./log.txt; done&