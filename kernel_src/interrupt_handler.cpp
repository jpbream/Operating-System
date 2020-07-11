#include "interrupt_handler.h"
#include "print.h"
#include "exceptions.h"

extern "C" void __cxa_pure_virtual() 
{ 
	printf("You called a pure virtual function.");
	while ( 1 ); 
}

void DefaultExceptionHandler::Handle(uint8_t interruptNumber)
{
	printf("EXCEPTION: ");
	printf(EXCEPTIONS[interruptNumber]);
	printChar('\n');

	if ( HAS_ERROR_CODE(interruptNumber) ) {

		if ( IS_SELECTOR_EXTERNAL(INTERRUPT_ERROR) ) {
			printf("External Exception ");
		}
		else {
			printf("Internal Exception ");
		}

		switch ( SELECTOR_SOURCE(INTERRUPT_ERROR) ) {

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
		printLong(SELECTOR_INDEX(INTERRUPT_ERROR));
		printChar('\n');
	}

	// page faults have a special error code
	if ( interruptNumber == PAGE_FAULT ) {

		if ( INTERRUPT_ERROR & 0x01 ) {
			printf("Caused by Page-Protection Violation ");
		}
		else {
			printf("Caused by Non-Present Page ");
		}

		if ( INTERRUPT_ERROR & 0x02 ) {
			printf("during a write access ");
		}
		else {
			printf("during a read access ");
		}

		if ( INTERRUPT_ERROR & 0x04 ) {
			printf("in privilege level 3. ");
		}
		else {
			printf("in unknown privilege. ");
		}

		if ( INTERRUPT_ERROR & 0x08 ) {
			printf("Page entry contained reserved bits. ");
		}
		else {
			printf("Page entry contained no reserved bits. ");
		}

		if ( INTERRUPT_ERROR & 0x10 ) {
			printf("Fault was caused by instruction fetch. ");
		}
		else {
			printf("Fault was not caused by instruction fetch. ");
		}
	}

	printf("At address ");
	printHex(FAULT_ADDRESS);
	printf(" in segment ");
	printLong(FAULT_SEGMENT);
	printf("\n");
	printf("Terminating Operating System");

	while ( true );

}

void DefaultInterruptHandler::Handle(uint8_t interruptNumber)
{
	if (interruptNumber != 0x20) {
		printf("INTERRUPT: ");
		printLong(interruptNumber);
		printChar('\n');
	}
}