#!/bin/bash
/usr/bin/python3 /home/luca/code/eink-frame/runWebserverImages.py > /dev/null &
/home/luca/code/eink-frame/epd -2.27 0 &