#ifndef _RADIO_PROTO_H_
#define _RADIO_PROTO_H_

// D-to-C: Device to Controller
// C-to-D: Controller to Device

enum CommModel1Command
{
  CM1C_STATUS,                // D-to-C only: mandatory, as a first command
                              //  +2 bytes: uint16_t status bit mask
  CM1C_NOP,                   // C-to-D only: kind of heartbeat
  CM1C_PING,                  // C-to-D only: ping request with id
  CM1C_PONG,                  // D-to-C only: pong with id (as a ping response)
  CM1C_RESET,                 // C-to-D only: 
                              //  +1 byte:
                              //    0: disable
                              //    1: now
                              //    n: delayed in loop cycles (8x ~seconds)

  CM1C_FIRMWARE = 254,        // reserved for radio bootloader
  CM1C_RESERVED255 = 255      // reserved for protocol extension
};

enum CommModel1Status
{
  CM1S_ERROR,                 // +2 bytes: error bit mask
  CM1S_RUNTIME,               // +4 bytes: uint32_t millis
                              // +4 bytes: uint32_t loop_counter
  CM1S_BAT_VOLTAGE,           // +2 bytes: uint16_t in mV
  CM1S_LOW_BATTERY,           // flag only
  CM1S_HT_DATA,               // +2 bytes uint16_t humidity in deci-percents,
                              // +2 bytes int16_t temperature in Celsius deci-grads
  CM1S_APHT_DATA              // +1 byte: BSEC status,
                              // +1 byte: IAQ accuracy,
                              // +1 byte uint8_t IAQ (2x value),
                              // +1 byte uint8_t IAQ static (2x value),
                              // +2 bytes int16_t pressure (100000 + 2x value) in Pa,
                              // +1 byte uint8_t humidity in percents,
                              // +1 byte int8_t temperature in Celsius grads.
};

enum CommModel1Errors
{
  CM1E_SMALL_PACKET,          // fatal
  CM1E_PROTO_VERSION,         // fatal
  CM1E_DEVICE_ID,             // fatal
  CM1E_WRONG_COUNTER,
  CM1E_UNKNOWN_COMMAND,       // fatal
  CM1E_UNEXPECTED_COMMAND,
  CM1E_CANT_DELIVER,
  CM1E_COMM_DEPTH,            // fatal
  CM1E_HT_READ,

  CM1E_INT_1,                 // Fatal internal error 1: send_packet() unknown command.
  CM1E_INT_2,                 // Fatal internal error 1: send_packet() too big payload.
};

enum CommModel2Command
{
  CM2C_STATUS,                // D-to-C only: mandatory, as a first command
                              //  +2 bytes: uint16_t status bit mask
  CM2C_NOP,                   // C-to-D only: kind of heartbeat
  CM2C_PING,                  // C-to-D only: ping request with id
  CM2C_PONG,                  // D-to-C only: pong with id (as a ping response)
  CM2C_RESET,                 // C-to-D only: 
                              //  +1 byte:
                              //    0: disable
                              //    1: now
                              //    n: delayed in loop cycles (8x ~seconds)
  CM2C_LED,                   // TODO
  CM2C_LAMP,                  // TODO

  CM2C_FIRMWARE = 254,        // reserved for radio bootloader
  CM2C_RESERVED255 = 255      // reserved for protocol extension
};

enum CommModel2Status
{
  CM2S_ERROR,                 // +2 bytes: error bit mask
  CM2S_RUNTIME,               // +4 bytes: uint32_t millis
                              // +4 bytes: uint32_t loop_counter
  CM2S_LED_MODE,              // TODO
  CM2S_CORE_TEMP,             // +? bytes: TODO
  CM2S_LAMP_MODE,             // TODO
};

enum CommModel2Errors
{
  CM2E_SMALL_PACKET,          // fatal
  CM2E_PROTO_VERSION,         // fatal
  CM2E_DEVICE_ID,             // fatal
  CM2E_WRONG_COUNTER,
  CM2E_UNKNOWN_COMMAND,       // fatal
  CM2E_UNEXPECTED_COMMAND,
  CM2E_CANT_DELIVER,
  CM2E_COMM_DEPTH,            // fatal

  CM2E_INT_1,                 // Fatal internal error 1: send_packet() unknown command.
  CM2E_INT_2,                 // Fatal internal error 1: send_packet() too big payload.
};

#endif
