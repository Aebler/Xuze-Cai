/*
 * SPI.c
 *
 *  Created on: 4. maj 2021
 *      Author: ella
 */

#include "FreeRTOS.h"
#include "emp_type.h"
#include "tm4c123gh6pm.h"
#include "glob_def.h"


void SPI_task(void *pvParameters)
{
    INT8U SPI_state = 0;
}

while(1)
{

    switch(SPI_state)
    {
    case 0:
        key = get_keyboard()
        if (key = 1)
        {
            SPI_state = 1;
        }
        break;
    case 1: //loading
        ss = 0;
        //UI loading pos og info til at sende til FPGA
        break;
    case 2: //Transmit
        //Sclock
        SSI_DATA_R;
        while ( (SSI2_SR_R & (1<<0)) == 0); //SR reg bit 0 gives 1 when empty
        SPI_state = 3;
        break;
    case 3: //delay

      if  (SSI2_SR_R & (1<<0)) == 0); //SR reg bit 0 gives 1 when empty
      {
          SPI_state = 4;
      }

        break;
    case 4: //receive
        SSI_DATA_R;
        while ((SSI2_SR_R & (0<<2)) == 1); // SR reg bit 2 gives 0 when empty.

        SPI_state = 5;
        break;
    case 5: //done

        break;

    }
}
