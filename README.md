# Content Summary

## lines_controler

Connect to n led strips lines (vertical horizontal of rings does not matter).
Receive commands from the 1-wire PJON network and show them on the strip.
See Lines protocol for network commands.

## network_writer

ESP demo code that writes commands on the PJON network.
Will be transformed into a Raspberry Pi C++ code soon.

# Lines protocol

## 1 led control

Set 1 led to a RGB value.

6 Bytes:
* Command: 'L' char (1 Byte)
* Line coordinate (1 Byte)
* Led coordinate (1 Byte)
* Colors \[R, G, B\] (3 Bytes)
