/******************** (C) COPYRIGHT 2017 FG Tech ***************************
* Author       : FlyGhost
* Filename     : FG_kalman.c
* Description  : 卡尔曼滤波函数
*****************************************************************************/
 
#include "kalman.h"
#include <math.h>
/*****************************************************************************************
 * 卡尔曼
 * @param[in] 
 * @param[out] 
 * @return     
 ******************************************************************************************/
void kalman_1(struct _1_ekf_filter *ekf,float input)  //一维卡尔曼
{
	ekf->Now_P = ekf->LastP + ekf->Q;
	ekf->Kg = ekf->Now_P / (ekf->Now_P + ekf->R);
	ekf->out = ekf->out + ekf->Kg * (input - ekf->out);
	ekf->LastP = (1-ekf->Kg) * ekf->Now_P ;
}
/*********************************************************************************************************************
卡尔曼滤波
**@brief: 线性最优评估滤波
**@param[in]  InputData 滤波前的数据，QR误差
**@param[out] None
**@return 滤波后的数据
**@remark: 通过修改过程噪声和测量噪声R,Q值优化输出量
X(k)=A X(k-1)+B U(k)+W(k)  
Z(k)=H X(k)+V(k)
AB是系统参数
X  K时刻的系统状态
H  测量系统的参数
Z  K时刻的测量量
WV 过程和测量噪声

X(k|k-1)=X(k-1|k-1) 预测下一状态的系统
P(k|k-1)=P(k-1|k-1) +Q  
Kg(k)= P(k|k-1) / (P(k|k-1) + R)   计算Kg卡尔曼增益
X(k|k)= X(k|k-1)+Kg(k) (Z(k)-X(k|k-1))   根据预测值结合估算值得出当前状态值
P(k|k)=(1-Kg(k))P(k|k-1)  更新协方差


(k-1|k-1)  上一个状态的最有评估
(k|k-1)    由上一个状态的最优评估预测当前状态的最优评估
(k|k)      由预测本状态的评估具体实现最优评估

Q ：  系统过程的协方差
R ：  测量的协方差
高斯白噪声 = Q+R
Kg ：  卡尔曼增益
P ：协方差

注:本卡尔曼滤波器针对单模型,单测量H,I均为1,没有控制量U=0,通常对A,B初始值取1
注:X会逐渐收敛,X(0|0)初始测量值状态根据测量前的数值而定
注 :   P (0|0)一般不取0,取0意味在0时候的方差为0,系统认为0时刻的值是最优的。
       然而在实际系统中往往0时刻不是最优的
强调：调整卡尔曼的滤波效果时只需要调整过程噪声和测量噪声即可，初始X可0，P取最小值
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
//卡尔曼滤波属于专门针对高斯白的滤波器，无法应对突变干扰，
//所以加一个除去最大值最小值的华东均值滤波
/********************************************************************************************************/
///*******************以下为一维卡尔曼的函数库形式***********************/

//typedef struct{
//	float x; //系统状态
//	float p; //协方差
//	float A; //系统状态矩阵
//	float H; //测量系统的参数矩阵
//	float q; //过程噪声协方差
//	float r; //测量噪声协方差
//	float gain; //卡尔曼增益
//}kalman1_state;


/*********************************************************************************************************
 * @brief   一维卡尔曼以角度滤波为例，干扰只有一个，即是角度自身的测量误差
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
    state->x = init_x; //系统状态初始化
    state->p = init_p; //协方差初始化，一般不为0，可取较小的适当的一个数
    state->A = 1;     //一维中的系统系数为A = 1   B=0
    state->H = 1;     //测量的误差只有一个维度，例如角度
    state->q = 2e2;//10e-6;  /* predict noise convariance */ //过程噪声初始值
    state->r = 5e2;//10e-5;  /* measure error convariance */ //测量噪声初始值
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
    state->x = state->A * state->x;//预测状态
    state->p = state->A * state->A * state->p + state->q;  /* p(n|n-1)=A^2*p(n-1|n-1)+q */  //预测下一时刻的协方差 

    /* Measurement */
    state->gain = state->p * state->H / (state->p * state->H * state->H + state->r);   //更新卡尔曼增益
    state->x = state->x + state->gain * (z_measure - state->H * state->x);   //更新卡尔曼当前状态，即卡尔曼滤波输出 state->x
    state->p = (1 - state->gain * state->H) * state->p;  //更新协方差

    return state->x;
}


/******************* (C) COPYRIGHT 2017 FG TECH *****END OF FILE************/

