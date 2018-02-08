#include <stdio.h>
#include <project.h>
#include <math.h>
#include <stdbool.h>
#include <CyFlash.h>
    
#define PI 3.14159265
#define LED_ON   0u
#define LED_OFF  1u
#define TRUE 0x01
#define FALSE 0x00
#define SQRT3 1.73205
#define SCALE_FACTOR .1294
#define TAPS 12
#define EVENT_LENGTH 144
#define EVENT 10


/* Defines second ROW from the last ROW */
#define CY_TEST_FLASH_ROW_ARow1(event)       (CY_FLASH_NUMBER_ROWS - 2u - 14u * event)
#define CY_TEST_FLASH_ROW_ARow2(event)       (CY_FLASH_NUMBER_ROWS - 3u - 14u * event)
#define CY_TEST_FLASH_ROW_BRow1(event)       (CY_FLASH_NUMBER_ROWS - 4u - 14u * event)
#define CY_TEST_FLASH_ROW_BRow2(event)       (CY_FLASH_NUMBER_ROWS - 5u - 14u * event)
#define CY_TEST_FLASH_ROW_CRow1(event)       (CY_FLASH_NUMBER_ROWS - 6u - 14u * event)
#define CY_TEST_FLASH_ROW_CRow2(event)       (CY_FLASH_NUMBER_ROWS - 7u - 14u * event)
#define CY_TEST_FLASH_ROW_1Row1(event)       (CY_FLASH_NUMBER_ROWS - 8u - 14u * event)
#define CY_TEST_FLASH_ROW_1Row2(event)       (CY_FLASH_NUMBER_ROWS - 9u - 14u * event)
#define CY_TEST_FLASH_ROW_2Row1(event)       (CY_FLASH_NUMBER_ROWS - 10u - 14u * event)
#define CY_TEST_FLASH_ROW_2Row2(event)       (CY_FLASH_NUMBER_ROWS - 11u - 14u * event)
#define CY_TEST_FLASH_ROW_3Row1(event)       (CY_FLASH_NUMBER_ROWS - 12u - 14u * event)
#define CY_TEST_FLASH_ROW_3Row2(event)       (CY_FLASH_NUMBER_ROWS - 13u - 14u * event)
#define CY_TEST_FLASH_ROW_DRow1(event)       (CY_FLASH_NUMBER_ROWS - 14u - 14u * event)
#define CY_TEST_FLASH_ROW_DRow2(event)       (CY_FLASH_NUMBER_ROWS - 15u - 14u * event)

/* Defines absolute address of ROW */
#define CY_TEST_FLASH_ADDR(row)      (row * CY_FLASH_SIZEOF_ROW)

uint16 oldThresholdVoltage = 0x0000;
uint16 newThresholdVoltage = 0x0000;
uint16 newOverThresholdVoltage = 0x1000;
uint16 newUnderThresholdVoltage = 0x0000;

int flashAvailableNotify = 0;
int thresholdNotify = 0;
uint16 oldflashAvailable = 0x0000;
uint16 newflashAvailable = 0x0000;

int event_written= 0;

uint16 rawA[EVENT_LENGTH];
uint16 rawB[EVENT_LENGTH];
uint16 rawC[EVENT_LENGTH];

int16 PhA[EVENT_LENGTH];
int16 PhB[EVENT_LENGTH];
int16 PhC[EVENT_LENGTH];
uint16 magPhA[EVENT_LENGTH];
uint16 magPhB[EVENT_LENGTH];
uint16 magPhC[EVENT_LENGTH];
int16 digitals[EVENT_LENGTH];

//NEW STUFF
//row1 contains the first 8-bit data and row2 contains the rest 8-bit data
uint8 VaRow1[EVENT_LENGTH];
uint8 VbRow1[EVENT_LENGTH];
uint8 VcRow1[EVENT_LENGTH];
uint8 V1Row1[EVENT_LENGTH];
uint8 V2Row1[EVENT_LENGTH];
uint8 V3Row1[EVENT_LENGTH];
uint8 digitalRow1[EVENT_LENGTH];

uint8 VaRow2[EVENT_LENGTH];
uint8 VbRow2[EVENT_LENGTH];
uint8 VcRow2[EVENT_LENGTH];
uint8 V1Row2[EVENT_LENGTH];
uint8 V2Row2[EVENT_LENGTH];
uint8 V3Row2[EVENT_LENGTH];
uint8 digitalRow2[EVENT_LENGTH];


//16-bit BLE data 
uint16 Va16[EVENT_LENGTH];
uint16 Vb16[EVENT_LENGTH];
uint16 Vc16[EVENT_LENGTH];
uint16 V116[EVENT_LENGTH];
uint16 V216[EVENT_LENGTH];
uint16 V316[EVENT_LENGTH];
uint16 digital16[EVENT_LENGTH];
int k = 0; //temp 
cystatus returnValue = CYRET_SUCCESS;

int over_voltage_threshold = 0;
int under_voltage_threshold = 0;

int over_voltage_a=0;
int under_voltage_a=0;
int over_voltage_b=0;
int under_voltage_b=0;
int over_voltage_c=0;
int under_voltage_c=0;
int positive_sequence_over=0;
int positive_sequence_under=0;
int negative_sequence_over=0;
int negative_sequence_under=0;
int zero_sequence_over=0;
int zero_sequence_under=0;

size_t buffer_index = 0;

/*History Buffer for FIR Filter*/
int16_t hist_bufa[11] = { 0 };
int16_t hist_bufb[11] = { 0 };
int16_t hist_bufc[11] = { 0 };

/*History Buffer for Magnitude Calculator*/
int16_t magBufa[11] = { 0 };
int16_t magBufb[11] = { 0 };
int16_t magBufc[11] = { 0 };


/*Counter for FIR Filter*/
int counter = 0;
 
int32 coefficients[] = {1, 1, 1, -1, -1, -1, -1, -1, -1, 1, 1, 1};

int32 negSeq[100];

/*Function Prototypes*/
void BleCallBack(uint32 event,void * eventParam);
bool WriteToFlashBLE(int event);
void sendBLE(int event);
void convert(void);
CY_ISR_PROTO(MYIsrHandler);
void negative_seq(int32 *phA, int32 *phB, int32 *phC, int32 *neg) ;
void magnitudeCalc(int16 *filteredData, int16 *magCh, int buffer_index);
void myFilter (int32 *coefficients, int taps, int16 *Ph_in, int16 *output, int buffer_index);
int16_t RTFilter(int32_t *coefficients, int16_t *Ph_in, int16_t *hist_buff, int counter);
uint16_t RTMagCalc(int16_t *filteredData, int16_t *hist_buff, int counter);

/* [] END OF FILE */
