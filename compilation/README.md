# The makefile used to compile a bluetooth-demo for BLE-mini

This makefile was meant to use instead of the IAR-compiler, and try to port it over to the sdcc-compiler, which is free and open-source.

I used: https://github.com/RedBearLab/Biscuit
as my demo-code. However i never succeded to compile the entire program, however i successfully compiled some files. This project needs improvement if wanting to compile for the BLE-mini.

## Other information useful for replication.

changed: hal_board_config in the ''hal'' folder
changed: ./Components/hal/target/CC2540EB/hal_types.h:

`` #error "ERROR: Unknown compiler." ``

to

`` #define ASM_NOP __asm nop __endasm ``

ses the sdcc manual for more info.

changed: ./Components/hal/target/CC2540EB/hal_mcu.h:

`` #error "ERROR: Unknown compiler." ``

to

``
	#define HAL_MCU_LITTLE_ENDIAN()   0
	#define HAL_ISR_FUNC_DECLARATION(f,v)   void f(void) interrupt v
	#define HAL_ISR_FUNC_PROTOTYPE(f,v)     void f(void)
	#define HAL_ISR_FUNCTION(f,v)           HAL_ISR_FUNC_PROTOTYPE(f,v); HAL_ISR_FUNC_DECLARATION(f,v)
``

initially used: https://github.com/lee-wei/CC2540 and https://github.com/aanon4/BlueBasic
as the data for the library, however its just fine to use the real one from ti.
