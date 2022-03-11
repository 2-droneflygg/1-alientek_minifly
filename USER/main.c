#include "system.h"	/*ͷ�ļ�����*/

/********************************************************************************
 * ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
 * ALIENTEK MiniFly
 * main.c
 * ����ϵͳ��ʼ���ʹ�������
 * ����ԭ��@ALIENTEK
 * ������̳:www.openedv.com
 * ��������:2017/5/12
 * �汾��V1.3
 * ��Ȩ���У�����ؾ���
 * Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
 * All rights reserved
********************************************************************************/

TaskHandle_t startTaskHandle;
static void startTask(void *arg);

int main()
{
    systemInit();			/*�ײ�Ӳ����ʼ��*/

    xTaskCreate(startTask, "START_TASK", 300, NULL, 2, &startTaskHandle);	/*������ʼ����*/

    vTaskStartScheduler();	/*�����������*/

    while(1) {};
}
/*��������*/
void startTask(void *arg)
{
    taskENTER_CRITICAL();	/*�����ٽ���*/

    xTaskCreate(radiolinkTask, "RADIOLINK", 150, NULL, 5, NULL);		/*����������������*/

    // xTaskCreate(usblinkRxTask, "USBLINK_RX", 150, NULL, 4, NULL);		/*����usb��������*/
    // xTaskCreate(usblinkTxTask, "USBLINK_TX", 150, NULL, 3, NULL);		/*����usb��������*/

    // xTaskCreate(atkpTxTask, "ATKP_TX", 150, NULL, 3, NULL);				/*����atkp������������*/
    xTaskCreate(atkpRxAnlTask, "ATKP_RX_ANL", 300, NULL, 6, NULL);		/*����atkp��������*/ 

    // xTaskCreate(configParamTask, "CONFIG_TASK", 150, NULL, 1, NULL);	/*����������������*/

    // xTaskCreate(pmTask, "PWRMGNT", 150, NULL, 2, NULL);					/*������Դ��������*/

    xTaskCreate(sensorsTask, "SENSORS", 450, NULL, 4, NULL);			/*������������������*/

    xTaskCreate(stabilizerTask, "STABILIZER", 450, NULL, 5, NULL);		/*������̬����*/

    //xTaskCreate(expModuleMgtTask, "EXP_MODULE", 150, NULL, 1, NULL);	/*������չģ���������*/

    printf("Free heap: %d bytes\n", xPortGetFreeHeapSize());			/*��ӡʣ���ջ��С*/

    vTaskDelete(startTaskHandle);										/*ɾ����ʼ����*/

    taskEXIT_CRITICAL();	/*�˳��ٽ���*/
}

void vApplicationIdleHook( void )
{
    static u32 tickWatchdogReset = 0;

    portTickType tickCount = getSysTickCnt();

    if (tickCount - tickWatchdogReset > WATCHDOG_RESET_MS)
    {
        tickWatchdogReset = tickCount;
        watchdogReset();
    }

    __WFI();	/*����͹���ģʽ*/
}


//20220310
// C ->ң������Ϣ����(һ֡����)->atkp���У��������ݣ�����>rcdata(ȫ�־�̬����)����>atkpSendPeriod��������
// ���������У�����DOWN_REMOTER ���id�����ݣ�������ʵ��ң�������ݷ��͵���̬������ȥ
// ң������������ͨ���ȴ���Ϣ���У��ȴ�����2�����ж����淢�������ֽڣ�������һ�����ݣ����͵�atkpRxAnlTask�е���Ϣ����


/*****************************atkpRxAnlTask***************************************/
//�ȴ���Ϣ���У���Ϣ���Դ�radio link�з�����Ҳ������usb link�з�����
//�����msg->id == remote_control ����ң�������� 
//������ң�����ݱ�command.c���ã����洢��ȫ�ֱ�������,����������remoteCache


/*****************************stabilizerTask��***************************************/
//commanderGetSetpoint ������� ���ݵ�ǰ����Ϣ  ���Ŀ�����̬��Ϣ�������н��ٶ� �Ƕ� �� 
//commanderGetSetpoint �����ctrlDataUpdate ������������¿�������

20220311
//����2�ж�����--->ң���������յ�--->������ݷ��͵���Ϣ������
//�������񡪡�>ң���������յ�����>������ͨ������2�ش���ȥ

//atkpRxAnlTask ֻ����ң�����ķ��͵���Ϣ���С���>������ң�����ݡ���>�������ң�е���Ϣ����
                                                        //|     
                                                        //--> ������ظ�usb���Ͷ���


//���������ݴ������� -->�ȴ��ⲿ�ж�-->�жϷ����ź���,����iic����dmaͨ�����ݰ�-->������ݷ��͵���Ϣ�����У�
   //���������ݵ�Դͷ������dma���䣬���ս����ݷŵ��ڲ��ľ�̬ȫ�ֱ����У���Ϣ���У�

//stabilizerTask ����һ����Ƶ��ȥ��ȡ��Ϣ�����л�ȡ���ݣ�����õ�������������





