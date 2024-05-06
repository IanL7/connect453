#include "ioexpander.h"
#include "i2c.h"

/** Read the value of the input port
 *
 * @param reg The reg address to read
 *
 */
uint8_t io_expander_get_input_port(void){
    uint8_t write_buffer[1];
	uint8_t * read_buffer;

	write_buffer[0] = 0x00;

    if ( CY_RSLT_SUCCESS == cyhal_i2c_master_write(
							&i2c_master_obj,
							IOEXPANDER_SUBORDINATE_ADDR,	// I2C Address
							write_buffer, 					// Array of data to write
							1, 								// Number of bytes to write
							0, 								// Block until completed
							false))							// Do NOT generate Stop Condition
	{
        if ( CY_RSLT_SUCCESS == cyhal_i2c_master_read(
								&i2c_master_obj, 			    // I2C Object
								IOEXPANDER_SUBORDINATE_ADDR,	// I2C Address
								read_buffer, 					// Read Buffer
								1 , 							// Number of bytes to read
								0, 								// Block until completed
								true)) 							// Generate Stop Condition
		{
            return *read_buffer;
        }
    }
	return 0;
}

/** Write the value of the output port
 *
 * @param value The value written
 *
 */
void io_expander_set_output_port(uint8_t value){
    uint8_t write_buffer[2];

	/* ADD CODE */
	write_buffer[0] = 0x01;
	write_buffer[1] = value;

	/* ADD CODE */
	/* Use cyhal_i2c_master_write to write the required data to the device. */
	cyhal_i2c_master_write(
							&i2c_master_obj, 				// I2C Object
							IOEXPANDER_SUBORDINATE_ADDR,		// I2C Address
							write_buffer, 					// Array of data to write
							2, 								// Number of bytes to write
							0, 								// Block until completed
							true);							// Generate Stop Condition

}

/** Write the polarity inversion register
 *
 * @param value The value written
 *
 */
void io_expander_set_polarity_inversion(uint8_t value){
    uint8_t write_buffer[2];

	/* ADD CODE */
	write_buffer[0] = 0x02;
	write_buffer[1] = value;

	/* ADD CODE */
	/* Use cyhal_i2c_master_write to write the required data to the device. */
	cyhal_i2c_master_write(
							&i2c_master_obj, 				// I2C Object
							IOEXPANDER_SUBORDINATE_ADDR,		// I2C Address
							write_buffer, 					// Array of data to write
							2, 								// Number of bytes to write
							0, 								// Block until completed
							true);	

}

/** Write the value of the configuration Register
 *
 * @param value The value written
 *
 */
void io_expander_set_configuration(uint8_t value){
    uint8_t write_buffer[2];

	/* ADD CODE */
	write_buffer[0] = 0x03;
	write_buffer[1] = value;

	/* ADD CODE */
	/* Use cyhal_i2c_master_write to write the required data to the device. */
	cyhal_i2c_master_write(
							&i2c_master_obj, 				// I2C Object
							IOEXPANDER_SUBORDINATE_ADDR,	// I2C Address
							write_buffer, 					// Array of data to write
							2, 								// Number of bytes to write
							0, 								// Block until completed
							true);	

}