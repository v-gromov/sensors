/** \file init.c
  * \brief This file is used for initialisation of STM32F105
  */

void Reset_Handler(void);
static void HardFault_Handler(void);
void Default_Handler(void);
extern void STM32_PCD_OTG_ISR_Handler(void) __attribute__((weak));

/**
 * The entry point for the application.
 */
extern void main(void);
const unsigned long _estack = 0x20010000;

/** \brief Stack space for program
  */

/**
 * The minimal vector table for a Cortex M3.  Note that the proper constructs
 * must be placed on this to ensure that it ends up at physical address
 * 0x0000.0000.
 */
__attribute__ ((section(".isr_vector")))
#ifdef _LINK_RAM_
void (* const g_ramVectors[])(void) =
#else
void (* const g_pfnVectors[])(void) =
#endif
{
    (void (* const )(void))(0x20010000),
                                            // The initial stack pointer
    Reset_Handler,                               // The reset handler
    Default_Handler,	                            // Reserved
    HardFault_Handler,                               // The hard fault handler
    Default_Handler,                      // The MPU fault handler
    Default_Handler,                      // The bus fault handler
    Default_Handler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    Default_Handler,                      // SVCall handler
    Default_Handler,                      // Debug monitor handler
    0,                                      // Reserved
    Default_Handler,                      // The PendSV handler
    Default_Handler,                      // The SysTick handler
    Default_Handler,                      // Supply Controller
    Default_Handler,                      // Reset Controller
    Default_Handler,                      // Real Time Clock
    Default_Handler,                      // Real Time Timer
    Default_Handler,                      // Watchdog Timer
    Default_Handler,                      // Power Management Controller
    Default_Handler,                      // Enhanced Embedded Flash Controller 0
    Default_Handler,                      // Enhanced Embedded Flash Controller 1
    Default_Handler,                      // UART
    Default_Handler,                      // Static Memory Controller
    Default_Handler,                      // PIOA
    Default_Handler,                      // PIOB
    Default_Handler,                      // PIOC
    Default_Handler,                      // USART0
    Default_Handler,                     // USART1
    Default_Handler,	    // USART2
    Default_Handler,                      // USART3
    Default_Handler,                      // HS/MCI3
Default_Handler,                      // TWI 0
Default_Handler,                      // TWI 1
Default_Handler,                      // SPI
Default_Handler,                      // Syncronous Serial Controller
Default_Handler,                      // Timer Counter 0
Default_Handler,                      // Timer Counter 1
Default_Handler,                      // Timer Counter 2
Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler
    ,  Default_Handler  
  ,  0
  ,  0
  ,  0
  ,  0
  ,  0
  ,  0
  ,  0
    ,  Default_Handler            
    ,  Default_Handler            
    ,  Default_Handler           
    ,  Default_Handler           
    ,  Default_Handler           
    ,  Default_Handler            
  , Default_Handler   
  , Default_Handler   
  , Default_Handler   
  , Default_Handler  
  , Default_Handler   
    ,  Default_Handler            
    ,  Default_Handler        
    ,  Default_Handler         
    ,  Default_Handler        
    ,  Default_Handler        
    ,  Default_Handler        
    ,  STM32_PCD_OTG_ISR_Handler
//  , BootRAM     /* @0x1E0. This is for boot in RAM mode for 
//		            STM32F10x Connectivity line Devices. */
  ,0
};


/** \brief Stack space for program
  */
#ifndef _LINK_RAM_
__attribute__((section("vtable")))
void (* g_ramVectors[sizeof(g_pfnVectors)/sizeof(g_pfnVectors[0])])(void);
#endif

/** \brief start of initialisation data for RAM (is located in FLASH)
  */
extern unsigned long _etext;
/** \brief start of initialised data segment in RAM
  */
extern unsigned long _data;
/** \brief end of initialised data segment in RAM
  */
extern unsigned long _edata;
/** \brief start address of BSS segment
  */
extern unsigned long _bss;
/** \brief end address of BSS segment
  */
extern unsigned long _ebss;

/** \brief This is a code which starts executing right after reset
  *
  * This is the code that gets called when the processor first starts execution
  * following a reset event.  Only the absolutely necessary set is performed,
  * after which the application supplied main() routine is called.  Any fancy
  * actions (such as making decisions based on the reset cause register, and
  * resetting the bits in that register) are left solely in the hands of the
  * application.
  */
void
Reset_Handler(void)
{
    unsigned long *pulSrc, *pulDest;
    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.  This is done with inline assembly since this
    // will clear the value of pulDest if it is not kept in a register.
    //

    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");

#ifndef _LINK_RAM_
    pulSrc=g_pfnVectors;
    for(pulDest=g_ramVectors;pulDest < g_ramVectors+sizeof(g_pfnVectors)/sizeof(g_pfnVectors[0]);){
	    *pulDest++ = *pulSrc++;
    };
#endif

        /*
         * Jump to main.
         */
    __asm("		ldr     r0, =main\n"
         "		bx      r0");
}

/** \brief This handler is executed when fault interrupt occurs
  *
  * This is the code that gets called when the processor receives a fault
  * interrupt.  This simply enters an infinite loop, preserving the system state
  * for examination by a debugger.
  * \todo write code for cause detection
  */
static void
HardFault_Handler(void)
{
    //
    // Enter an infinite loop.
    //
    for (;;){
    }
}

/** \brief This is a spurious interrupt handler
 *
 *  This is the code that gets called when the processor receives an unexpected
 * interrupt.  This simply enters an infinite loop, preserving the system state
 * for examination by a debugger.
 */
void
Default_Handler(void)
{
    //
    // Go into an infinite loop.
    //
    for (;;){
    }
}

