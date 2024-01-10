#!/bin/sh
#!launcher.sh
#!navigate to home directory, then to here
#!launch simple python server
#!open chrome at localhost:8000

cd /
cd /home/luca/code/eink-frame
python -m SimpleHTTPServer &
cd /