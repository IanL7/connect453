#ifndef IOEXPANDER_H_
#define IOEXPANDER_H_

#include <stdint.h>
#include <stdbool.h>

#define IOEXPANDER_SUBORDINATE_ADDR         0x20


/** Read the value of the input port
 *
 * @param reg The reg address to read
 *
 */
uint8_t io_expander_get_input_port(void);

/** Write the value of the output port
 *
 * @param value The value written
 *
 */
void io_expander_set_output_port(uint8_t value);

/** Write the polarity inversion register
 *
 * @param value The value written
 *
 */
void io_expander_set_polarity_inversion(uint8_t value);

/** Write the value of the configuration Register
 *
 * @param value The value written
 *
 */
void io_expander_set_configuration(uint8_t value);

#endif

