/*
 * SPI.h
 *
 *  Created on: 4. maj 2021
 *      Author: ella
 */

#ifndef SPI_H_
#define SPI_H_


void SPI_tx_task(INT8U my_id, INT8U my_state, INT8U data);
void SPI_rx_task(INT8U my_id, INT8U my_state, INT8U data);


#endif /* SPI_H_ */
