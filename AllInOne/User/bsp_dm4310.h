#ifndef __BSP_DM4310_H
#define __BSP_DM4310_H

#include "can.h"

#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -30.0f
#define V_MAX 30.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -10.0f
#define T_MAX 10.0f
#define Master_ID 0


typedef __packed struct
{
	int id;
	int state;
	int p_int;
	int v_int;
	int t_int;
	int kp_int;
	int kd_int;
	float pos;
	float vel;
	float toq;
	float Kp;
	float Kd;
	float Tmos;
	float Tcoil;
}Motor_Inf;

typedef __packed struct
{
	float p_int;
	float v_int;
	float kp_int;
	float kd_int;
    float t_int;
}Motor_MIT_Data_t;


int float_to_uint(float x, float x_min, float x_max, int bits);
    
float uint_to_float(int x_int, float x_min, float x_max, int bits);



void ctrl_motor(CAN_HandleTypeDef* hcan,uint16_t id, Motor_MIT_Data_t* _dm43_mit_t);
void ctrl_motor2(CAN_HandleTypeDef* hcan,uint16_t id, float _pos, float _vel);
void ctrl_motor3(CAN_HandleTypeDef* hcan,uint16_t id, float _vel);
void start_motor(CAN_HandleTypeDef* hcan,uint16_t id);
void can_filter_init(void);

#endif
