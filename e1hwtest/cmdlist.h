char* cmdlist = 
"p    Program FPGA from Dataflash\r\n"
"f    Access FPGA registers. Write: 'f<a> <d>'. Read: 'f<a>'\r\n"
"i    Access LIU registers. Format is same as in 'f'\r\n"
"S    Print liu ports short status in human readable format\r\n"
"a    Print pulse shape from FPGA. This is the test of ADC\r\n"
"c    Access Codec registers (write, read) format: same as f\r\n"
"v    Turn on/off power to the board. Format v[0|1]\r\n"
"l    Access leds. Format: l<number> <0|1>\r\n"
"r    Access to load/short resistors. Format: r<l|s><port> [0|1]\r\n"
"D    Dump all registers (fpga, liu, codec)\r\n"
"h    Print help (this screen)\r\n"
;
