#!/bin/bash
cd /home/luca/code/eink-frame
busybox httpd -p 80 -h . > /dev/null 2>&1 &
/home/luca/code/eink-frame/epd -2.27 0 &