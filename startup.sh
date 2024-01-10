#!/bin/bash
cd /home/luca/code/eink-frame
/usr/bin/python3 -m http.server 80 > /dev/null &
/home/luca/code/eink-frame/epd -2.27 0 &