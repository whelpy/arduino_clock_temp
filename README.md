Simple arduino clock with thermometer.
Used parts:
* Arduino Nano (cheap Chinese fake pcb)
* DHT22 (connected to D8)
* MAX7919 7-segment module with 8 digits (CLK=D7, CS=D6, DIN=D5)
* DS1302 RTC (CLK=D4, DAT=D3, RST=D2)
* AC/DC adapter, 5v 700ma

Setting time.
Write hours and minutes without a delimeter into the serial port.
For example, 13:33:55 should be written as 1333. It will be saved into the RTC module.
It could be done using Arduino IDE port monitor, or just use included bash script: "setClock.sh 1333", just set proper port number in the script.

Temperature uses last 2 digits, and might also use one more digit for "minus" symbol. Measurement unit is Celsius.

Brightness.
Between 8pm and 8am it's dimmed.
