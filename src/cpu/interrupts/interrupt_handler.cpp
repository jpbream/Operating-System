#include "interrupt_handler.h"
#include "print.h"
#include "exceptions.h"

extern "C" void __cxa_pure_virtual() 
{ 
	printf("You called a pure virtual function.");
	while ( 1 ); 
}

CPUState* DefaultExceptionHandler::Handle(uint8_t interruptNumber, CPUState* regs)
{
	printf("EXCEPTION: ");
	printf("%s", EXCEPTIONS[interruptNumber]);
	putchar('\n');

	uint32_t error = regs->error;
	uint32_t address = regs->eip; // get address of the fault
	uint32_t segment = regs->cs; // get code segment of the fault

	if ( HAS_ERROR_CODE(interruptNumber) ) {

		if ( IS_SELECTOR_EXTERNAL(error) ) {
			printf("External Exception ");
		}
		else {
			printf("Internal Exception ");
		}

		switch ( SELECTOR_SOURCE(error) ) {

		case SELECTOR_SOURCE_GDT:
			printf(" Originated from GDT ");
			break;
		case SELECTOR_SOURCE_IDT:
			printf(" Originated from IDT ");
			break;
		case SELECTOR_SOURCE_LDT:
			printf(" Originated from LDT ");
			break;
		}
		printf("at index ");
		printf("%d", SELECTOR_INDEX(error));
		putchar('\n');
	}

	// page faults have a special error code
	if ( interruptNumber == PAGE_FAULT ) {

		if ( error & 0x01 ) {
			printf("Caused by Page-Protection Violation ");
		}
		else {
			printf("Caused by Non-Present Page ");
		}

		if ( error & 0x02 ) {
			printf("during a write access ");
		}
		else {
			printf("during a read access ");
		}

		if ( error & 0x04 ) {
			printf("in privilege level 3. ");
		}
		else {
			printf("in unknown privilege. ");
		}

		if ( error & 0x08 ) {
			printf("Page entry contained reserved bits. ");
		}
		else {
			printf("Page entry contained no reserved bits. ");
		}

		if ( error & 0x10 ) {
			printf("Fault was caused by instruction fetch. ");
		}
		else {
			printf("Fault was not caused by instruction fetch. ");
		}
	}

	printf("At address ");
	printf("%x", address);
	printf(" in segment ");
	printf("%d", segment);
	printf("\n");
	printf("Terminating Operating System");

	while ( true );

}

CPUState* DefaultInterruptHandler::Handle(uint8_t interruptNumber, CPUState* regs)
{
	if (interruptNumber != 0x20) {
		printf("INTERRUPT: ");
		printf("%d", interruptNumber);
		putchar('\n');
	}

	return regs;
}