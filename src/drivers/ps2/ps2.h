#ifndef _PS2_H_
#define _PS2_H_

#include <stdint.h>

// PS2 CONTROLLER COMMANDS //////////////////////////////////////////////////////

// read the controller configuration byte
// response is the cc byte
#define PS2_READ_CC 0x20

// write the parameter to the controller configuration byte
#define PS2_WRITE_CC 0x60

#define CC_ENABLE_INT1 0x01
#define CC_ENABLE_INT12 0x02
#define CC_DISABLE_KBD 0x10
#define CC_DISABLE_MOUSE 0x20

#define PS2_DISABLE_KBD 0xAD
#define PS2_ENABLE_KBD 0xAE

#define PS2_DISABLE_MOUSE 0xA7
#define PS2_ENABLE_MOUSE 0xA8

// perform a mouse test
// response is the test result
#define PS2_TEST_MOUSE 0xA9
#define PS2_TEST_MOUSE_PASSED 0x00
#define PS2_TEST_MOUSE_CLOCK_LOW 0x01
#define PS2_TEST_MOUSE_CLOCK_HIGH 0x02
#define PS2_TEST_MOUSE_DATA_LOW 0x03
#define PS2_TEST_MOUSE_DATA_HIGH 0x04

// perform a keyboard test
// response is the test result
#define PS2_TEST_KBD 0xAB
#define PS2_TEST_KBD_PASSED 0x00
#define PS2_TEST_KBD_CLOCK_LOW 0x01
#define PS2_TEST_KBD_CLOCK_HIGH 0x02
#define PS2_TEST_KBD_DATA_LOW 0x03
#define PS2_TEST_KBD_DATA_HIGH 0x04

// perform a ps2 controller test (resets it)
// response is the test result
#define PS2_TEST_CONTROLLER 0xAA
#define PS2_TEST_PASSED 0x55
#define PS2_TEST_FAILED 0xFC

// read controller output port
#define PS2_READ_CO 0xD0

// write controller output port
#define PS2_WRITE_CO 0xD1

// makes parameter look like it came from keyboard
#define PS2_SPOOF_KBD 0xD2

// makes parameter look like it came from mouse
#define PS2_SPOOF_MOUSE 0xD3

// END PS2 CONTROLLER COMMANDS //////////////////////////////////////////////////////



// KEYBOARD COMMANDS //////////////////////////////////////////////////////

#define KBD_ACK 0xFA
#define KBD_RESEND 0xFE

// responds ack or resend
#define KBD_SET_LED 0xED
#define KBD_SET_LED_SCROLL_LOCK 0x00
#define KBD_SET_LED_NUM_LOCK 0x01
#define KBD_SET_LED_CAPS_LOCK 0x02

// responds 0xEE or resend
#define KBD_ECHO 0xEE

// responds ack or resend, then the code set
#define KBD_SET_CODESET 0xF0
#define KBD_SET_CODESET_GET 0x00
#define KBD_SET_CODESET_1 0x01
#define KBD_SET_CODESET_2 0x02
#define KBD_SET_CODESET_3 0x03

// responds ack, the sequence of bytes
#define KBD_IDENTIFY 0xF2
#define KBD_IDENTIFY_MF2 0xAB83

// responds ack or resend
#define KBD_SET_RATE 0xF3
// a lower number is a higher rate
#define KBD_REPEAT_MASK 0x1F
#define KBD_DELAY_250 0x00
#define KBD_DELAY_500 0x20
#define KBD_DELAY_750 0x40
#define KBD_DELAY_1000 0x60

// responds ack or resend
#define KBD_ENABLE_SCANNING 0xF4

// responds ack or resend
#define KBD_DISABLE_SCANNING 0xF5

// responds ack or resend
#define KBD_SET_DEFAULT_PARAMS 0xF6

// responds ack or resend, then result
#define KBD_RESET 0xFF
#define KBD_RESET_PASSED 0xAA
#define KBD_RESET_FAILED 0xFC

// END KEYBOARD COMMANDS //////////////////////////////////////////////////////



// MOUSE COMMANDS //////////////////////////////////////////////////////

#define MS_ACK 0xFA
#define MS_RESEND 0xFE

// responds ack or resend, then result, then final byte
#define MS_RESET 0xFF
#define MS_RESET_PASSED 0xAA
#define MS_RESET_FAILED 0xFC
#define MS_RESET_FINAL 0x00

// responds ack or resend
#define MS_SET_DEFAULT_PARAMS 0xF6

// responds ack or resend
#define MS_DISABLE_STREAMING 0xF5

// responds ack or resend
#define MS_ENABLE_STREAMING 0xF4

// responds ack or resend
#define MS_SET_SAMPLE_RATE 0xF3
#define MS_SAMPLE_RATE_10 10
#define MS_SAMPLE_RATE_20 20
#define MS_SAMPLE_RATE_40 40
#define MS_SAMPLE_RATE_60 60
#define MS_SAMPLE_RATE_80 80
#define MS_SAMPLE_RATE_100 100
#define MS_SAMPLE_RATE_200 200

// responds ack or resend, then id
#define MS_IDENTIFY 0xF2
#define MS_IDENTIFY_STANDARD 0x00
#define MS_IDENTIFY_SCROLL 0x03
#define MS_IDENTIFY_5BUTTON 0x04

// responds ack or reset
#define MS_SET_REMOTE_MODE 0xF0

// responds ack or reset
#define MS_SET_WRAP_MODE 0xEE

// responds ack or reset
#define MS_RESET_WRAP_MODE 0xEC

// responds ack or reset, then with a data packet
#define MS_READ_DATA 0xEB

// responds ack or resend
#define MS_SET_STREAM_MODE 0xEA

// responds ack or resend, then with a status packet
#define MS_READ_STATUS 0xE9

// responds ack or resend
#define MS_SET_RESOLUTION 0xE8
#define MS_RESOLUTION_1 0x00
#define MS_RESOLUTION_2 0x01
#define MS_RESOLUTION_4 0x02
#define MS_RESOLUTION_8 0x03

// responds ack or resend
#define MS_SET_SCALING_21 0xE7

// responds ack or resend
#define MS_SET_SCALING_11 0xE6

// END MOUSE COMMANDS //////////////////////////////////////////////////////


// if returned from a read data call, the device had no data and timed out
#define PS2_TIMEOUT 0x01 // <- this just needs to be a value that is unused for device returns

namespace PS2 {

	// use for commands to the ps2 controller
	void WriteCommand(uint8_t cmd);

	// use for commands to the keyboard
	void WriteKeyboardCommand(uint8_t cmd);

	// use for commands to the mouse
	void WriteMouseCommand(uint8_t cmd);

	// use for data for the ps2 controller
	void WriteData(uint8_t dt);

	// use for data for the keyboard
	void WriteKeyboardData(uint8_t dt);

	// use for data for the mouse
	void WriteMouseData(uint8_t dt);

	// use when data is expected to come from the ps2 controller
	uint8_t ReadData();

	// use when data is expected to come from the keyboard
	uint8_t ReadKeyboardData();

	// use when data is expected to come from the mouse
	uint8_t ReadMouseData();

	// use to clear all data currently in the keyboard
	// output buffer
	void ClearKeyboardOutput();

	// use to clear all data currently in the mouse output
	// buffer
	void ClearMouseOutput();

}

#endif