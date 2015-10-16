#include <Sparki.h>
/*
 * Basic control for sparki robot given shortest path
 */


float current_x = 0;
float current_y = 0;
float heading = 0;

int solution_size;

int goal_x[solution_size];
int goal_y[solution_size];

goal_node = 1;

float error_d = 0;
float error_a = 0;

float p_gain_d = .01;
float p_gain_a = .01;


void findError() {
    float error_x = goal_x[goal_node]*5 - current_x;
    float error_y = goal_y[goal_node]*5 - current_y;
    error_d = math.sqrt(error_x*error_x + error_y*error_y);
    goal_heading = math.atan2(error_y/error_x)
    error_a = goal_heading - heading;
}

void loop() {
    float right_v = 0;
    float left_v = 0;
    findError();
    if(error_d > 2 || abs(error_a) > 5) {
        float v = error_d * p_gain_d;
        right_v = v;
        left_v = v;

        if(error_a > 0) {
            right_v += error_d*p_gain_a;
            left_v -= error_d*p_gain_a;
        } else {
            left_v += error_d*p_gain_a;
            right_v -= error_d*p_gain_a;
        }
    } else {
        goal_node++;
    }
    sparki.motorRotate(MOROT_LEFT,DIR_CCW,left_v);
    sparki.motorRotate(MOTOR_RIGHT,DIR_CW,right_v);
    delay(50);
}
