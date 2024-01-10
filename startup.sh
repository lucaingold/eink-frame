#!/bin/bash
/usr/bin/python3 -m http.server --directory /home/luca/code/eink-frame & 
/home/luca/code/eink-frame/epd -2.27 0 &