# SipaaKernel
The kernel behind your favorite OS.

## Features
* Architecture :
  * x86_32 :
      * GDT
      * IDT :
        * Exception Handlers
        * IRQs
      * PIC
      * RTC (looks somewhat broken)
      * Really basic TSS
  * Input / Output
* Devices :
  * PS2 :
    * Mouse
    * Keyboard
  * Serial
  * PIT
* Frameworks :
  * KMem memory allocator
  * LibASG graphics library
  * System library
  * SysTime (not implemented)
  * RMUSR (a really small lib for entering to userspace)
* Standard Library (only strcat not working as expected sometimes)
      
