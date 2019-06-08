#ifndef _KALMAN_H
#define _KALMAN_H

struct _1_ekf_filter
{
	float LastP;
	float Now_P;
	float out;
	float Kg;
	float Q;
	float R;	
};
extern void kalman_1(struct _1_ekf_filter *ekf,float input);  //一维卡尔曼


/*******************以下为一维卡尔曼的函数库形式***********************/
typedef struct{
	float x; //系统状态
	float p; //协方差
	float A; //系统状态矩阵
	float H; //测量系统的参数矩阵
	float q; //过程噪声协方差
	float r; //测量噪声协方差
	float gain; //卡尔曼增益
}kalman1_state;
extern void kalman1_init(kalman1_state *state, float init_x, float init_p);
extern float kalman1_filter(kalman1_state *state, float z_measure);

#endif


