#include "Timer.h"
#include "lcd.h"
#include "ping_template.h"
#include "adc.h"
#include "servo.h"
#include "movement.h"
#include "open_interface.h"
#include "uart.h"
#include "resetSimulation.h"


void sendString(char data[]); //Function that will print to Putty.

//#warning "Possible unimplemented functions"
#define REPLACEME 0

int main(void)
{
/**
 * Variables that will hold the values to be used to receive input from putty and variables tha w will hold tthe
 * values of the degrees and angles of the objects detected.
 */
    char currInput = ' ';
    char newInput = ' ';
    double val;
    double avg;

    int i = 0;
    int objCounter = 0;
    int distAtObjStart = 0;
    int distToSmallestObj = 0;
    int objAngle = 0;
    int anglePointAtObj = 0;
    int smallestObjAngle;
    float objWidth = 0;
    int objWidthUse = 0;
    int smallestObjWidth = 1000;
    int objWidthRad = 0;
    char preRun[27];
    char data[50];
    float count = 0;
    float finalDist = 0;
    float angleCount = 0;
    float distCount = 0;

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();
    timer_init();
    uart_init(115200);
    servo_init();
    adc_init();

    while (1) //Infinite loop that will receive input from Putty and converts it into instructions for the bot's movement and scan.
    {


        currInput = uart_receive();//Waits for input from putty.

        while (currInput == 'w')//Move forward
        {
            sendString("w");
            moveWithObstacle(sensor_data, 50);
            currInput = uart_receive();
        }
        while (currInput == 'd') //Turn right
        {
            sendString("d");
            turn_right(sensor_data, 86);
            currInput = uart_receive();
        }
        while (currInput == 'a') //Turn left
        {
            sendString("a");
            turn_left(sensor_data, 86);
            currInput = uart_receive();
        }
        while (currInput == 'c') //Slight turn to the right, around 15 degrees to the right.
        {
            sendString("c");
            turn_right(sensor_data, 14);
            currInput = uart_receive();
        }
        while (currInput == 'z') //Slight turn to the left, around 15 degrees to the left.
        {
            sendString("z");
            turn_left(sensor_data, 14);
            currInput = uart_receive();
        }
        while (currInput == 's') //Move backwards
        {
            sendString("s");
            back(sensor_data, 25);
            currInput = uart_receive();
        }

        while (currInput == 'f') //Scan 180 degrees
        {
            sendString("f");
            int j = 0;
            while (j < 180) //While loop that will go up to 180, this will help move the sensor 180 degrees in front of the bot.
            {

                servo_move(j); //Function that will make the servo on the PING sensor move to a specific degree.

                avg = 0;
                for (i = 0; i < 16; i++)
                {
                    avg += adc_read();
                }

                avg = avg / 16;
                val = 97812 * pow(avg, -1.142); //Calibration for the servo, to avoid that it rotates above 180 degrees.


                sprintf(data, "%f ", val); //this call to sprintf is to save the distances of objects found into a char array called data.
                sprintf(preRun, "%d", j); //This call to srpintf is to save the degrees of the objects found into a char array called preRun.

                if (val < (double) 40) /**If statement to ensure that the objects that will be printed into putty are the ones that are 40 cm in front of the bot,
                                            this way we would avoid any collisions **/
                {

                        sendString(data);
                        sendString("\n\r");
                        sendString(preRun);
                        sendString("\n\r");
                        sendString("\n");


                }
                angleCount = j;
                j++;

                timer_waitMillis(100); //Wait 100 ms before scanning again
            }

            currInput = uart_receive(); //Wait for next input from putty.

        }

    }
    oi_free(sensor_data);
}



void sendString(char data[])
{
    int i = 0;

    while (data[i] != '\0')
    {
        cyBot_sendByte(data[i]);
        i++;

    }

}
