# CoreN2G
This is a hardware abstraction layer for SAMD5x, SAME5x and SAMC21 processors. It performs roughly the same functions as project CoreNG does for SAM3X, SAM4E, SAM4S and SAME7 processors. However, it has been written from scratch (with help from ASF4 code) and the API is different. Here are the main differences:

1. This project provides a very limited API to C clients. C client files should #include <Core.h> which declares the available functions. C++ client files should #include <CoreIO.h> instead.

2. This project includes the C/C++ startup code, but not the main() function. The client must provide main(), which must set up the clocks and flash wait states and call CoreInit(). However, this project assumes standard configurations for the clocks. GCLK0 is always the main processor clock. On the SAME5x, GCLK1 is a slow clock (because only GCLK1 allows a large clock divider), GCLK2 is a raw clock from the main crystal (because the Duet 3 Mini needs that on the GCLK2 output pin which feeds the Ethernet PHY), GCLK3 runs at half the processor clock speed so that it can be used for SERCOMs, TCs and TCCs; and GCLK4 is a 48MHz clock for USB and CAN. On the SAMC21, most peripherals run at 48MHz so they can use GCLK0. In future we may add generic clock setup code to this project.

3. CoreInit() only initialises functions that are typically needed by all clients, such as the DMA manager. This is to avoid pulling in functions that the client may not need. In particular, the client must call AnalogIn::Init() and AnalogOut::Init() if it uses those modules.

4. Serial I/O devices and USB devices are not declared or defined in this project. The client must do that if it needs them and must declare the corresponding interrupt service routines.

5. Tables of pin functions are application-dependent; therefore they live in the application. The client application should derive the pin table entry struct from struct PinDescriptionBase provided by this project. The client must provide function GetPinDescription(Pin p) so that functions in CoreN2G can retrieve the details of a pin. It should return nullptr if the pin is out of range or otherwise not available.

6. Analog input is managed by a separate task, which must be created by the client. Currently, analog input is not available in non-RTOS builds.

The SAME5x builds are also suitable for use on SAMD processors, provided that the client does not try to use any features that are not available on the target processor, such as Ethernet or CAN.

DC, 2020-07-11
