/*
 * Description:  A robot seeking for light while beeing able to dodge obstacles, 
 * using the Braitenberg method.
 * The Brantenberg part is taken from the default khepera 2 controller sample
 *
 * The robot is using the Subsumption architecture, where :
 * - Avoid obstacle is priority 1
 * - Seeking for light (following light) is priority 0
 *
 * In case of obstacle, "avoid obstacle" will be choosen 
 * because it has a greater priority than "seeking for light".
 *
 * Code has been written following the given sensors positions: 
 * https://www.researchgate.net/profile/Prof-Dr-H-Behera/publication/287360228/figure/fig7/AS:308569191796739@1450580036116/Position-of-sensors-and-internal-structure-of-Khepera-II.png
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <webots/robot.h>
#include <webots/light_sensor.h>

#define MAX_SENSOR 8
#define RANGE (1024 / 2)
#define BOUND(x, a, b) (((x) < (a)) ? (a) : ((x) > (b)) ? (b) : (x))
#define TIME_STEP 64
#define MAXSPEED 3
#define THRESHOLD 0.01

static WbDeviceTag sensors[MAX_SENSOR], left_motor, right_motor, light_sensors[MAX_SENSOR];
static double matrix[MAX_SENSOR][2];
static int num_sensors;
static double range;
static int time_step = 0;
static double max_speed = 0.0;
static double speed_unit = 1.0;

void move(left_motor, right_motor, left_speed, right_speed) {
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
}

int max_index(double tab[], int len) {
    int max = 0;
    for (int i = 0; i < len; i++) {
        if (tab[i] > tab[max]) {
            max = i;
        }
    }
    return max;
}


static void initialize() {
    /* necessary to initialize Webots */
    wb_robot_init();

    time_step = wb_robot_get_basic_time_step();

    const char *robot_name = wb_robot_get_name();

    const char khepera_name[] = "ds0";

    char sensors_name[5];
    const double(*temp_matrix)[2];

    range = RANGE;
    const double khepera_matrix[8][2] = {{-2, 4}, {-3, 5}, {-7, 7}, {7, -6}, {5, -4}, {4, -2}, {-0.5, -0.5}, {-0.5, -0.5}};
    const double khepera_max_speed = MAXSPEED;
    const double khepera_speed_unit = 1.0;

    num_sensors = 8;
    sprintf(sensors_name, "%s", khepera_name);
    temp_matrix = khepera_matrix;
    max_speed = khepera_max_speed;
    speed_unit = khepera_speed_unit;

    char name[8];
    for (int i = 0; i < 8; i++) {
        sprintf(name, "ls%d", i);
        light_sensors[i] = wb_robot_get_device(name);
        wb_light_sensor_enable(light_sensors[i], TIME_STEP);
    }

    for (int i = 0; i < num_sensors; i++) {
        sensors[i] = wb_robot_get_device(sensors_name);
        wb_distance_sensor_enable(sensors[i], time_step);

    if ((i + 1) >= 10) {
        sensors_name[2] = '1';
        sensors_name[3]++;

        if ((i + 1) == 10) {
            sensors_name[3] = '0';
            sensors_name[4] = '\0';
        }
    } else {
        sensors_name[2]++;
    }

    for (int j = 0; j < 2; j++)
        matrix[i][j] = temp_matrix[i][j];
    }

    left_motor = wb_robot_get_device("left wheel motor");
    right_motor = wb_robot_get_device("right wheel motor");
    wb_motor_set_position(left_motor, INFINITY);
    wb_motor_set_position(right_motor, INFINITY);
    move(left_motor, right_motor, 0, 0);

    printf("The %s robot is initialized, it uses %d distance sensors\n", robot_name, num_sensors);
}

int main() {
  initialize();

    while (wb_robot_step(time_step) != -1) {
        double speed[2];
        double sensors_value[MAX_SENSOR];
        int obstacle = 0;

        for (int i = 0; i < num_sensors; i++) {
            sensors_value[i] = wb_distance_sensor_get_value(sensors[i]);
            if (sensors_value[i] > 0) { // Obstacle soon, we will need to avoid it
                obstacle = 1;
            }
        }

        // Subsumption architecture
        if (obstacle == 1) { // Avoiding obstacle
            printf("=== AVOIDING OBSTACLE ===\n");
            for (int i = 0; i < 2; i++) {
                speed[i] = 0.0;
                for (int j = 0; j < num_sensors; j++) {
                    speed[i] += speed_unit * matrix[j][i] * (1.0 - (sensors_value[j] / range));
                }
                speed[i] = BOUND(speed[i], -max_speed, max_speed);
            }

            /* Set the motor speeds */
            move(left_motor, right_motor, speed[0], speed[1]);
        } else { // Seeking for light
            printf("=== SEEKING FOR LIGHT ===\n");
            double left_light[3];
            double right_light[3];
            double rear_light[2];
            for (int i = 0; i < 3; i++)
                left_light[i] = wb_light_sensor_get_value(light_sensors[i]);
            for (int i = 3; i < 6; i++)
                right_light[i-3] = wb_light_sensor_get_value(light_sensors[i]);
            for (int i = 6; i < 8; i++)
                rear_light[i-6] = wb_light_sensor_get_value(light_sensors[i]);
        
            int maxl = max_index(left_light, 3);
            int maxr = max_index(right_light, 3);
            int maxrear = max_index(rear_light, 2);
        
            if (left_light[maxl] > right_light[maxr]) { // Light stronger at left
                if (rear_light[maxrear] > left_light[maxl]) { // Light stronger at rear left
                    printf("Arrière gauche\n");
                    move(left_motor, right_motor, 0, -MAXSPEED);
                } else { // Light stronger at front left
                    if (maxl == 0) {
                        printf("Gauche 90\n");
                        move(left_motor, right_motor, 0, MAXSPEED);
                    } else if (maxl == 1) {
                        printf("Gauche 45\n");
                        move(left_motor, right_motor, MAXSPEED/2, MAXSPEED);
                    } else {
                        printf("Gauche 10\n");
                        move(left_motor, right_motor, MAXSPEED-0.2, MAXSPEED);
                    }
                }  
            } else { // Light stronger at right
                if (rear_light[maxrear] > right_light[maxl]) { // Light stronger at rear right
                    printf("Arrière droit\n");
                    move(left_motor, right_motor, -MAXSPEED, 0);
                } else { // Light stronger at front right
                    if (maxr == 3) {
                        printf("Droit 10\n");
                        move(left_motor, right_motor, MAXSPEED, MAXSPEED-0.2);
                    } else if (maxr == 4) {
                        printf("Droit 45\n");
                        move(left_motor, right_motor, MAXSPEED, MAXSPEED/2);
                    } else {
                        printf("Droit 90\n");
                        move(left_motor, right_motor, MAXSPEED, 0);
                    }
                }  
            }
        }
    }
    wb_robot_cleanup();
    return 0;
}