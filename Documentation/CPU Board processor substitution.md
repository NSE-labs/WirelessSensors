# CPU Board Processor Substitution

It is possible to substitute pin-compatible versions of the STM32 processor family on the CPU board by simply specifying a different part when ordering the PC board.

Here are some examples:

feature| STM32L151C8T6 | STM32L071CBT6 | STM32F410CBT6
---|---|---|---
Verified | Yes | Yes | No
Tested | Yes | Yes | No
Core | Arm Cortex M3 | Arm Cortex M0+ | ARM Cortex M4
Clock | 32MHz | 32MHz | 100MHz
Flash | 64K bytes | 128K bytes | 128K bytes
RAM | 10K bytes | 20K bytes | 32K bytes
USB | Yes | No | No
I2C ports | 2 | 3 | 3
JLCPCB Cost | $2.65 | $1.65 | $6.20



