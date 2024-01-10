#!/bin/bash
cd /home/luca/code/eink-frame
/usr/bin/python3 runWebserverImages.py > /dev/null &
/home/luca/code/eink-frame/epd -2.27 0 &
