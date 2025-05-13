#include"PNG.h"
#include"math.h"
#include "openmv.h"
#include "JY61P.h"
#include"bsp_dwt.h"
#include "servo_motor.h"
#include"controller.h"
static double theta_x = 0;
static double theta_y = 0;

double target_x_angle_last = 0;
double target_y_angle_last = 0;

double target_x_angle = 0;
double target_y_angle = 0;

double target_x_speed = 0;
double target_y_speed = 0;

double ax = 0;
double ay = 0;

float q = 0;

float K_theta =0;
float K_cmd = 0;
uint32_t last_count = 0;

double delta_time = 0;

float servo_1_angle = 0;
float servo_2_angle = 0;
float servo_3_angle = 0;
float servo_4_angle = 0;

float servo_x_angle = 0;
float servo_y_angle = 0;
float servo_z_angle = 0;

float servo_x_angle_imu = 0;
float servo_y_angle_imu = 0;
float servo_z_angle_imu = 0;

PID_Init_Config_s PNG_PID_Config_X;
PID_Init_Config_s PNG_PID_Config_Y;
PID_Init_Config_s PNG_PID_Config_Z;

PIDInstance PNG_PID_X;
PIDInstance PNG_PID_Y;
PIDInstance PNG_PID_Z;

void calculate_angles() {
	theta_x = atan((target_x - u0) / f) * 180 / M_PI;
	theta_y = atan((target_y - v0) / f) * 180 / M_PI;
}

void PNG_Init() {
    PNG_PID_Config_X.Kp = 0.5;
    PNG_PID_Config_X.Ki = 0.01;
    PNG_PID_Config_X.Kd = 0.1;
    PNG_PID_Config_X.MaxOut = 100;

    PNG_PID_Config_Y.Kp = 0.5;
    PNG_PID_Config_Y.Ki = 0.01;
    PNG_PID_Config_Y.Kd = 0.1;
    PNG_PID_Config_Y.MaxOut = 100;

    PNG_PID_Config_Z.Kp = 0.5;
    PNG_PID_Config_Z.Ki = 0.01;
    PNG_PID_Config_Z.Kd = 0.1;
    PNG_PID_Config_Z.MaxOut = 100;

    PIDInit(&PNG_PID_X, &PNG_PID_Config_X);
    PIDInit(&PNG_PID_Y, &PNG_PID_Config_Y);
    PIDInit(&PNG_PID_Z, &PNG_PID_Config_Z);
}

void PNG_TASK(){
    calculate_angles();
    target_x_angle_last = theta_x;
    target_y_angle_last = theta_y;
    delta_time = DWT_GetDeltaT64(&last_count);
    calculate_angles();
    target_x_angle = theta_x;
    target_y_angle = theta_y;
    delta_time = DWT_GetDeltaT64(&last_count);
    target_x_speed = (target_x_angle - target_x_angle_last) / delta_time-gyro_x;
    target_y_speed = (target_y_angle - target_y_angle_last) / delta_time-gyro_y;
    q = 0.5*1.225*pow(dart_speed,2);
    ax = N*dart_speed*target_x_speed;
    ay = N*dart_speed*target_y_speed;
    K_theta =q*S*Cl*l;
    K_cmd = (m*l)/K_theta;
    servo_x_angle = K_cmd*ax;
    servo_y_angle = K_cmd*ay;
    servo_x_angle_imu = K_cmd*IMU_ay;
    servo_y_angle_imu = K_cmd*IMU_az;
    servo_x_angle = PIDCalculate(&PNG_PID_X, servo_x_angle_imu, servo_x_angle);
    servo_y_angle = PIDCalculate(&PNG_PID_Y, servo_y_angle_imu, servo_y_angle);
    servo_1_angle = -1*(-0.25*servo_x_angle+0.25*servo_y_angle);
    servo_2_angle = -1*(0.25*servo_x_angle+0.25*servo_y_angle);
    servo_3_angle = -1*(0.25*servo_x_angle-0.25*servo_y_angle);
    servo_4_angle = -1*(-0.25*servo_x_angle-0.25*servo_y_angle);
    Servo_Motor_Task(servo_1_angle, servo_2_angle, servo_3_angle, servo_4_angle);
}