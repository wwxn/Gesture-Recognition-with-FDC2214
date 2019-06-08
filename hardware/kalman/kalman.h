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
extern void kalman_1(struct _1_ekf_filter *ekf,float input);  //һά������


/*******************����Ϊһά�������ĺ�������ʽ***********************/
typedef struct{
	float x; //ϵͳ״̬
	float p; //Э����
	float A; //ϵͳ״̬����
	float H; //����ϵͳ�Ĳ�������
	float q; //��������Э����
	float r; //��������Э����
	float gain; //����������
}kalman1_state;
extern void kalman1_init(kalman1_state *state, float init_x, float init_p);
extern float kalman1_filter(kalman1_state *state, float z_measure);

#endif


