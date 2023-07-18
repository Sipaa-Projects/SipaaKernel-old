# SipaaKernel
The kernel behind SipaaOS.

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
  * Serial
  * PIT
* Frameworks :
  * KMem memory allocator
  * LibASG graphics library
  * RMUSR (a really small lib for entering to userspace, who is broken)
* Standard Library (only strcat not working as expected sometimes)
      
