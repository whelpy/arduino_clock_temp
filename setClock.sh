#!/bin/bash
stty -F /dev/ttyUSB1 -hupcl
echo "$1" > /dev/ttyUSB1