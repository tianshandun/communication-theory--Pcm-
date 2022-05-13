//vs17���Ǳ������Խ���Ԥ����
#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//		para[8]={ 0, 1/128,		1/64,	  1/32,	   1/16,   1/8,   1/4,  1/2 };
//				   000		001		 010		011		100		101	  110   111
float para[8] = { 0, 0.0078125, 0.015625, 0.03125, 0.0625, 0.125, 0.25, 0.5 };
//�����룬�����룬�����룬����PCM�����Ϊȫ�ֱ���
char polarCode[1] = "0";
char paraCode[3] = "000";
char QuanCode[4] = "0000";
char code[8] = "";

//��������  getPolarCode
//���ܣ�    ��ȡ��ǰ����ֵ������c0
//���������value:����ֵ��float��
//����ֵ��  ��
void getPolarCode(float value)
{
	if (value > 0) {
		polarCode[0] = '1';
	}
}

//��������  getParaCode
//���ܣ�    ��ȡ��ǰ����ֵ������c123
//���������value:����ֵ��float��
//����ֵ��  ��
void getParaCode(float value)
{
	/*ÿһ����ı߽��洢��para��һȫ�ֱ�����
		��ͨ���Ƚϻ�ó���ֵ��������*/
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

//��������  getQuanCode
//���ܣ�    ��ȡ��ǰ����ֵ������c4567
//���������value:����ֵ��float��
//����ֵ��  ��
int getQuanCode(float value)
{
	int lowerIndex = 0, codeNum = 0;
	/*ÿһ����ı߽��洢��para��һȫ�ֱ�����
		��ͨ���Ƚϻ�ó���ֵ����������lowerIndex = 0,1,2....7*/
	for (int i = 0; i <= 7; i++){
		if (value < para[i]){
			lowerIndex = i - 1;
			break;
		}
	}
	float delta, value_parai;
	/*��֪�����ŵ�����£�
		ͨ���Ƚϻ�����������������delta*/
	if (lowerIndex == 0) {
		delta = 0.03125;//  1/32
		value_parai = value - 0.5;
	}
	else {
		delta = (para[lowerIndex + 1] - para[lowerIndex]) / 16.0;
		value_parai = value - para[lowerIndex];
	}
	/*ͨ���Ƚϻ�ó���ֵ����ʮ�����е���һ��codeNum = 0,1,2,3...15*/
	for (int i = 1; i <= 16; i++) {
		if (value_parai < delta * i)
		{
			codeNum = i - 1;
			break;
		}
	}
	/*��codeNum��һʮ�������ת��Ϊ�����ƶ�����*/
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
	// ��ʾ����ȡֵ��Χ�߽�ͳ�����
	printf("��֪ģ���ź�������źŷ�Χ[-V, V]");
	printf("������V = ");
	scanf_s("%d", &v);
	printf("���������ֵ��");
	scanf("%f", &sig);
	
	sig = sig / (float)v;//��һ������ֵ

	if (sig > 1.0 || sig < -1.0) {
		printf("error");
	}//�������ֵ�����ź�ȡֵ��Χ��ô����

	getPolarCode(sig);//���㼫����

	sig = fabs(sig);//���ź�ȡ����ֵ
	getParaCode(sig);//���������
	getQuanCode(sig);//���������

	//�ַ���ƴ�ӣ�������+������+������
	strcat(code, polarCode);
	strcat(code, paraCode);
	strcat(code, QuanCode);
	printf("PCM��������");
	puts(code);//��ʾ���
}