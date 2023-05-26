/*
 * movement.c
 *
 *  Created on: Sep 9, 2021
 *      Author: mdreeves
 */
#include "Timer.h"
#include "lcd.h"
#include "string.h"
#include "open_interface.h"
#include "movement.h"

   // void sendString(char data[]);

    void straight(oi_t *sensor_data, double dist) { //Function to move forwards
        double sum = 0;
        oi_setWheels(100, 100); // move forward; full speed
        while (sum < dist) {
            oi_update(sensor_data);
            sum += sensor_data->distance;
        }
        oi_setWheels(0, 0); // stop
    }

    void back(oi_t *sensor_data, double dist) { //Function to move backwards
            double sum = 0;
            oi_setWheels(-100, -100); // move back; full speed
            while (sum > -dist) {
                oi_update(sensor_data);
                sum += sensor_data->distance;
            }
            oi_setWheels(0, 0); // stop
        }

    /*Function that deals with the sensors of the CyBot.
     * Handles when the bump sensors detect a collision of if the cliff sensors detect a cliff.
     */
    void moveWithObstacle(oi_t *sensor_data, double totalDist) {

        double distMoved = 0;
        double distToMove = totalDist;

        while(distMoved < distToMove) {

            oi_update(sensor_data);

            if (sensor_data->bumpLeft == 1) {
                sendString(" Ran into bump left. ");
                distMoved -= 20;

                oi_update(sensor_data);

                back(sensor_data, 20);
                break;

            }
            else if (sensor_data->bumpRight == 1) {
                sendString(" Ran into bump right. ");
                distMoved -= 20;

                oi_update(sensor_data);

                back(sensor_data, 20);
                break;

            }
            else if (sensor_data-> cliffFrontLeft == 1) {
                sendString(" Ran into cliff front left. ");
                distMoved -= 100;

                oi_update(sensor_data);

                back(sensor_data, 100);
                break;

            }
            else if (sensor_data->cliffFrontRight == 1) {
                sendString(" Ran into cliff front right. ");
                distMoved -= 100;

                oi_update(sensor_data);

                back(sensor_data, 100);
                break;

            }
            else if (sensor_data-> cliffLeft == 1) {
                sendString(" Ran into cliff far left. ");
                distMoved -= 100;

                oi_update(sensor_data);

                back(sensor_data, 100);
                break;

            }
            else if (sensor_data->cliffRight == 1) {
                sendString(" Ran into cliff far right. ");
                distMoved -= 100;

                oi_update(sensor_data);

                back(sensor_data, 100);
                break;

            }
            else if (sensor_data->cliffFrontLeftSignal > 2700 || sensor_data->cliffFrontRightSignal > 2700){
                sendString(" Ran into line. ");
                distMoved -= 100;

                oi_update(sensor_data);

                back(sensor_data, 100);
                break;
            }


            oi_setWheels(100, 100);
            distMoved += sensor_data->distance;
         }

        oi_setWheels(0, 0);

    }
/*
 * Function that will handle turning right
 */
    void turn_right(oi_t *sensor_data, double deg) {
        double turn_angle = 0;
        while (turn_angle > -deg) {

            oi_setWheels(-100, 100);
            oi_update(sensor_data);
            turn_angle += sensor_data->angle;
            lcd_printf("%f", turn_angle);

        }

        oi_setWheels(0, 0); // stop

    }
/*
 * Function that will handle turning left
 */
    void turn_left(oi_t *sensor_data, double deg) {
            double turn_angle = 0;
            while (turn_angle < deg) {

                oi_setWheels(100, -100);
                oi_update(sensor_data);
                turn_angle += sensor_data->angle;
                lcd_printf("%f", turn_angle);

            }

            oi_setWheels(0, 0); // stop

        }





