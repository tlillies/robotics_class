#include <Sparki.h>
/*
 * Basic control for sparki robot given shortest path
 */
float xi = 0;
float yi = 0;
int xint;
int yint;
float theta = 0;
float r = 2.55; // cm
float d = 8.45; // cm
float motorSpeed = 2.7724; // cm/s
float wheelSpeed = motorSpeed / r; // 1/s
float wheelRight = 0;
float wheelLeft = 0;
unsigned long time;
float deltaTime;

float heading = 0;

int solution_size;

int goal_x[solution_size];
int goal_y[solution_size];

goal_node = 1; // First node is first goal node

float error_d = 0;
float error_a = 0;

/* Multipliers to make reasonable values */
float p_gain_d = .01;
float p_gain_a = .01;


void findError() {
    float error_x = goal_x[goal_node]*5 - xi;
    float error_y = goal_y[goal_node]*5 - yi;
    error_d = math.sqrt(error_x*error_x + error_y*error_y); // Total distance
    goal_heading = math.atan2(error_y/error_x);
    error_a = goal_heading - theta;
}

void loop() {
    float right_v = 0;
    float left_v = 0;

    findError();

    /* If Enough Error Send Control Values */
    if(error_d > 2 || abs(error_a) > 5) {
        float v = error_d * p_gain_d;
        right_v = v;
        left_v = v;

        if(error_a > 0) { // Need to turn right
            right_v += error_d*p_gain_a;
            left_v -= error_d*p_gain_a;
        } else { // Need to turn left
            left_v += error_d*p_gain_a;
            right_v -= error_d*p_gain_a;
        }
    } else { // If error is small go to next node
        goal_node++;
    }
    
    /* Filter Out Bad Values */
    if(left_v > 100) {
        left_v = 100;
    }
    if(left_v < 0) {
        left_v = 0;
    }
    if(right_v > 100) {
        right_v = 100;
    }
    if(right_v < 0) {
        right_v = 0;
    }

    /* Send Motor Speeds */
    sparki.motorRotate(MOROT_LEFT,DIR_CCW,left_v);
    sparki.motorRotate(MOTOR_RIGHT,DIR_CW,right_v);

    /* Calculate Speeds Of Left And Right Motors */
    wheelLeft = (left_v/100) * motorSpeed;
    wheelRight = (right_v/100) * motorSpeed;

    delay(50);

    /* Calculate Odometry */
    deltaTime = float((millis() - time)) / 1000.0;
    xi = xi + cos(theta) * (r * wheelRight / 2 + r * wheelLeft / 2) * deltaTime;
    yi = yi + sin(theta) * (r * wheelRight / 2 + r * wheelLeft / 2) * deltaTime;
    theta = theta + (wheelRight * r / d - wheelLeft * r / d) * deltaTime;
    time = millis();
}
