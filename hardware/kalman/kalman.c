/******************** (C) COPYRIGHT 2017 FG Tech ***************************
* Author       : FlyGhost
* Filename     : FG_kalman.c
* Description  : �������˲�����
*****************************************************************************/
 
#include "kalman.h"
#include <math.h>
/*****************************************************************************************
 * ������
 * @param[in] 
 * @param[out] 
 * @return     
 ******************************************************************************************/
void kalman_1(struct _1_ekf_filter *ekf,float input)  //һά������
{
	ekf->Now_P = ekf->LastP + ekf->Q;
	ekf->Kg = ekf->Now_P / (ekf->Now_P + ekf->R);
	ekf->out = ekf->out + ekf->Kg * (input - ekf->out);
	ekf->LastP = (1-ekf->Kg) * ekf->Now_P ;
}
/*********************************************************************************************************************
�������˲�
**@brief: �������������˲�
**@param[in]  InputData �˲�ǰ�����ݣ�QR���
**@param[out] None
**@return �˲��������
**@remark: ͨ���޸Ĺ��������Ͳ�������R,Qֵ�Ż������
X(k)=A X(k-1)+B U(k)+W(k)  
Z(k)=H X(k)+V(k)
AB��ϵͳ����
X  Kʱ�̵�ϵͳ״̬
H  ����ϵͳ�Ĳ���
Z  Kʱ�̵Ĳ�����
WV ���̺Ͳ�������

X(k|k-1)=X(k-1|k-1) Ԥ����һ״̬��ϵͳ
P(k|k-1)=P(k-1|k-1) +Q  
Kg(k)= P(k|k-1) / (P(k|k-1) + R)   ����Kg����������
X(k|k)= X(k|k-1)+Kg(k) (Z(k)-X(k|k-1))   ����Ԥ��ֵ��Ϲ���ֵ�ó���ǰ״ֵ̬
P(k|k)=(1-Kg(k))P(k|k-1)  ����Э����


(k-1|k-1)  ��һ��״̬����������
(k|k-1)    ����һ��״̬����������Ԥ�⵱ǰ״̬����������
(k|k)      ��Ԥ�Ȿ״̬����������ʵ����������

Q ��  ϵͳ���̵�Э����
R ��  ������Э����
��˹������ = Q+R
Kg ��  ����������
P ��Э����

ע:���������˲�����Ե�ģ��,������H,I��Ϊ1,û�п�����U=0,ͨ����A,B��ʼֵȡ1
ע:X��������,X(0|0)��ʼ����ֵ״̬���ݲ���ǰ����ֵ����
ע :   P (0|0)һ�㲻ȡ0,ȡ0��ζ��0ʱ��ķ���Ϊ0,ϵͳ��Ϊ0ʱ�̵�ֵ�����ŵġ�
       Ȼ����ʵ��ϵͳ������0ʱ�̲������ŵ�
ǿ�����������������˲�Ч��ʱֻ��Ҫ�������������Ͳ����������ɣ���ʼX��0��Pȡ��Сֵ
 **********************************************************************************************************************/
//		const float Q = 0.018;//0.01; 
//		const float R = 0.543;//0.9;			
//float kalman_1(float InputData,float Q,float R)  //?????
//{
//	struct Kalman{
//		float K_1_K_1;//(k-1|k-1)
//		float K_K_1;  //(k|k-1)
//		float K_K;    //(k|k)
//	};
//	static struct Kalman P={0};
//	static struct Kalman X={0};
//	float Kg;
//	
//	X.K_K_1 = X.K_1_K_1;
//	P.K_K_1 = P.K_1_K_1 + Q;
//	Kg = P.K_K_1 / (P.K_K_1 + R);
//	X.K_K = X.K_K_1 + Kg * (InputData - X.K_K_1);
//	P.K_K = (1-Kg) * P.K_K_1 ;
//	
//	X.K_1_K_1 = X.K_K;
//	P.K_1_K_1 = P.K_K;
//	return X.K_K;
//}
/********************************************************************************************************/
//�������˲�����ר����Ը�˹�׵��˲������޷�Ӧ��ͻ����ţ�
//���Լ�һ����ȥ���ֵ��Сֵ�Ļ�����ֵ�˲�
/********************************************************************************************************/
///*******************����Ϊһά�������ĺ�������ʽ***********************/

//typedef struct{
//	float x; //ϵͳ״̬
//	float p; //Э����
//	float A; //ϵͳ״̬����
//	float H; //����ϵͳ�Ĳ�������
//	float q; //��������Э����
//	float r; //��������Э����
//	float gain; //����������
//}kalman1_state;


/*********************************************************************************************************
 * @brief   һά�������ԽǶ��˲�Ϊ��������ֻ��һ�������ǽǶ�����Ĳ������
 *   Init fields of structure @kalman1_state.
 *   I make some defaults in this init function:
 *     A = 1;
 *     H = 1; 
 *   and @q,@r are valued after prior tests.
 *
 *   NOTES: Please change A,H,q,r according to your application.
 *
 * @inputs  
 *   state - Klaman filter structure
 *   init_x - initial x state value   
 *   init_p - initial estimated error convariance
 * @outputs 
 * @retval  
 */
void kalman1_init(kalman1_state *state, float init_x, float init_p)
{
    state->x = init_x; //ϵͳ״̬��ʼ��
    state->p = init_p; //Э�����ʼ����һ�㲻Ϊ0����ȡ��С���ʵ���һ����
    state->A = 1;     //һά�е�ϵͳϵ��ΪA = 1   B=0
    state->H = 1;     //���������ֻ��һ��ά�ȣ�����Ƕ�
    state->q = 2e2;//10e-6;  /* predict noise convariance */ //����������ʼֵ
    state->r = 5e2;//10e-5;  /* measure error convariance */ //����������ʼֵ
}
/*********************************************************************************************************
 * @brief   
 *   1 Dimension Kalman filter
 * @inputs  
 *   state - Klaman filter structure
 *   z_measure - Measure value
 * @outputs 
 * @retval  
 *   Estimated result
 */
float kalman1_filter(kalman1_state *state, float z_measure)
{
    /* Predict */
    state->x = state->A * state->x;//Ԥ��״̬
    state->p = state->A * state->A * state->p + state->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */  //Ԥ����һʱ�̵�Э���� 

    /* Measurement */
    state->gain = state->p * state->H / (state->p * state->H * state->H + state->r);   //���¿���������
    state->x = state->x + state->gain * (z_measure - state->H * state->x);   //���¿�������ǰ״̬�����������˲���� state->x
    state->p = (1 - state->gain * state->H) * state->p;  //����Э����

    return state->x;
}


/******************* (C) COPYRIGHT 2017 FG TECH *****END OF FILE************/

