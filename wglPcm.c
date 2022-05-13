//vs17总是报错，所以进行预处理
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//		para[8]={ 0, 1/128,		1/64,	  1/32,	   1/16,   1/8,   1/4,  1/2 };
//				   000		001		 010		011		100		101	  110   111
float para[8] = { 0, 0.0078125, 0.015625, 0.03125, 0.0625, 0.125, 0.25, 0.5 };
//极性码，段落码，段内码，完整PCM编码均为全局变量
char polarCode[1] = "0";
char paraCode[3] = "000";
char QuanCode[4] = "0000";
char code[8] = "";

//函数名：  getPolarCode
//功能：    获取当前抽样值极性码c0
//输入参数：value:抽样值（float）
//返回值：  无
void getPolarCode(float value)
{
	if (value > 0) {
		polarCode[0] = '1';
	}
}

//函数名：  getParaCode
//功能：    获取当前抽样值段落码c123
//输入参数：value:抽样值（float）
//返回值：  无
void getParaCode(float value)
{
	/*每一段落的边界点存储在para这一全局变量中
		，通过比较获得抽样值所在区间*/
	if (value >= para[4]){
		paraCode[0] = '1';
	}
	if ((value >= para[6]) 
		|| (para[2] <= value && value < para[4])){
		paraCode[1] = '1';
	}
	if ((para[1] <= value && value < para[2])
		|| (para[3] <= value && value < para[4])
		|| (para[5] <= value && value < para[6])
		|| (value >= para[7])){
		paraCode[2] = '1';
	}
}

//函数名：  getQuanCode
//功能：    获取当前抽样值段内码c4567
//输入参数：value:抽样值（float）
//返回值：  无
int getQuanCode(float value)
{
	int lowerIndex = 0, codeNum = 0;
	/*每一段落的边界点存储在para这一全局变量中
		，通过比较获得抽样值所在区间编号lowerIndex = 0,1,2....7*/
	for (int i = 0; i <= 7; i++){
		if (value < para[i]){
			lowerIndex = i - 1;
			break;
		}
	}
	float delta, value_parai;
	/*已知段落编号的情况下，
		通过比较获得所在区间量化间隔delta*/
	if (lowerIndex == 0) {
		delta = 0.03125;//  1/32
		value_parai = value - 0.5;
	}
	else {
		delta = (para[lowerIndex + 1] - para[lowerIndex]) / 16.0;
		value_parai = value - para[lowerIndex];
	}
	/*通过比较获得抽样值处于十六段中的哪一个codeNum = 0,1,2,3...15*/
	for (int i = 1; i <= 16; i++) {
		if (value_parai < delta * i)
		{
			codeNum = i - 1;
			break;
		}
	}
	/*将codeNum这一十六段序号转化为二进制段内码*/
	if (codeNum / 8)
		QuanCode[0] = '1';
	if (codeNum % 8 / 4)
		QuanCode[1] = '1';
	if (codeNum % 8 % 4 / 2)
		QuanCode[2] = '1';
	if (codeNum % 8 % 4 % 2)
		QuanCode[3] = '1';
}

void main()
{
	float sig;
	int v;
	// 提示输入取值范围边界和抽样点
	printf("已知模拟信号输出的信号范围[-V, V]");
	printf("请输入V = ");
	scanf_s("%d", &v);
	printf("请输入抽样值：");
	scanf("%f", &sig);
	
	sig = sig / (float)v;//归一化抽样值

	if (sig > 1.0 || sig < -1.0) {
		printf("error");
	}//如果抽样值超过信号取值范围那么报错

	getPolarCode(sig);//计算极性码

	sig = fabs(sig);//对信号取绝对值
	getParaCode(sig);//计算段落码
	getQuanCode(sig);//计算段内码

	//字符串拼接：极性码+段落码+段内码
	strcat(code, polarCode);
	strcat(code, paraCode);
	strcat(code, QuanCode);
	printf("PCM编码结果：");
	puts(code);//显示结果
}