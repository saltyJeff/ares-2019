ARES-TELEMETRY-2019
This project is an Arduino sketch that takes telemetry data from:
* pressure transducers
* IMU
* altimeter
and outputs it to:
* SD card
* XBee radio

In 'firmware/', is the actual C++ code that is compiled and deployed to the device

In 'modules/', is the modularized form of the code. Each file ending .module.h is preprocessed with a Kotlin script into
a corresonding file in 'firmware/'.
