#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <stdlib.h>

/*
 * SOME INITIAL NOTES
 *
 * Right now, this has only been tested on port B and on pins PB6/7 and PB8/9. The functions have only been written
 * for I2C1, but if you really want to make it usable for I2C2 and I2C3 before I finish it up, the it should be as easy
 * as copy and pasting the existing functions, then changing the registers used from I2C1 to the ones wanted.
 *
 * Also, for having one read and one write on after the other...
 * Reads seem to do meh. May be the logic analyzer that's screwing up. Can't find any issues with the software, and I'm inclined
 * to say that it's the slave chip (MPU 9500 or whatever) that's not liking the repeated reads.
 * Writes do better. Two consecutive writes work good, but on the third it gets stuck after the start bit is set.
 * To overcome these issues, it may be a good idea to have a short delay. May only need to be 500us or so.
 *
 */

// Used to tell if the I2C channel is running for not. Don't want to accidently get stuck
// in a loop if one the peripheral is disabled.
// With the use of interrupts these may not be needed.
uint8_t I2C1_started;
uint8_t I2C2_started;
uint8_t I2C3_started;

// Define the callback functions as a type so they can be easily called
typedef void (*callback)(void);

/*
 * Initializes the I2C module
 *
 * Input parameters
 *     freq - Defines the clock frequency. There is a minimum of of 20kHz and a maximum of 400kHz. If the input
 *            is outside this range, then it will be set to the maximum or minimum.
 *     port - The GPIO port that the I2C is on. For example if using PB6 and PB7, then this input should be 'B' or 'b'
 *     pin_set - Describes the pin pair of the port used. These go in numerical order. For example, if using PB6 and PB7, then
 *               this will be set to 0. If using PB8 and PB9, this will be set to 1, and so on.
 *     channel - Says if you're using I2C1, I2C2, or I2C3. So the only available values will be 1, 2, or 3.
 *     max_bytes - The maximum number of bytes that being planned to send. Used for allocating memory for the I2C struct.
 *
 * No output parameters
 */
void init_I2C(uint32_t freq, char port, uint8_t pin_set, uint8_t channel, uint8_t max_bytes);

/*
 * Reads a defined number of bytes and puts them into the data location.
 *
 * Input parameters
 *     slave_addr - The address of the slave that's being read.
 *     dest_addr - The address of the register that's being read from the slave.
 *     num_bytes - The number of bytes being read.
 *     *data - Points to the address of the data.
 *
 * No output parameters. The data output is handled by writing using the address of the data.
 */
void I2C1_read(uint8_t slave_addr, uint8_t dest_addr, uint8_t num_bytes, uint8_t *data);

/*
 * Writes data to the slave.
 *
 * Input parameters
 *     slave_addr - The address of the slave that's being read.
 *     dest_addr - The address of the register that's being read from the slave.
 *     num_bytes - The number of bytes being read.
 *     *data - The address of the data that's being written.
 */
void I2C1_write(uint8_t slave_addr, uint8_t dest_addr, uint8_t num_bytes, uint8_t *data);

/*
 * Reads a defined number of bytes and puts them into the data location. Once the transfer is complete, a function is
 * performed. The idea is that if there is something that needs to be done with the data after it's read in, this helps
 * to handle that since this file uses interrupts.
 *
 * Input parameters
 *     slave_addr - The address of the slave that's being read.
 *     dest_addr - The address of the register that's being read from the slave.
 *     num_bytes - The number of bytes being read.
 *     *data - Points to the address of the data.
 *     callback_func - Points to the address of the function to be performed after all bytes are read.
 *
 * No output parameters. The data output is handled by writing using the address of the data.
 */
void I2C1_read_cb(uint8_t slave_addr, uint8_t dest_addr, uint8_t num_bytes, uint8_t *data,callback callback_func);

/*
 * Writes data to the slave. Once the transfer is complete, a function is
 * performed. The idea is that if there is something that needs to be done with the data after it's read in, this helps
 * to handle that since this file uses interrupts.
 *
 * Input parameters
 *     slave_addr - The address of the slave that's being read.
 *     dest_addr - The address of the register that's being read from the slave.
 *     num_bytes - The number of bytes being read.
 *     callback_func - Points to the address of the function to be performed after all bytes are read.
 */
void I2C1_write_cb(uint8_t slave_addr, uint8_t dest_addr, uint8_t num_bytes, uint8_t *data, callback callback_func);

/** TODO
 *
 * Create a destruction function so that the memory for the I2C data struct can be released.
 *
 */

/** TODO
 *
 * During initialization make sure that the bus isn't locked up by clocking the SCL 8 times or so.
 *
 */

/** TODO
 *
 * Possibly add in some sort of watch dog timer compatability. That way if the bus gets locked up, then it can be reset and be used
 * again down the line.
 *
 */

/** TODO
 *
 * Add in some sort of buffer so that if the user sends one message (or reads) after another, then a wait state isn't needed. Tried to
 * implement this already, but it's proving to be a pain in the ass...
 *
 */





