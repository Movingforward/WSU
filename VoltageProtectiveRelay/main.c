#include "RelayControl.h"

//This function is used to split a 16-bit data into two 8-bit data
void convert(void)
{
    int i = 0;
    
    for(i = 0; i < 144; i++)
    {
        VaRow1[i] = 0xff&(Va16[i] >> 8);
        VaRow2[i] = 0xff&Va16[i];
        VbRow1[i] = 0xff&(Vb16[i] >> 8);
        VbRow2[i] = 0xff&Vb16[i];
        VcRow1[i] = 0xff&(Vc16[i] >> 8);
        VcRow2[i] = 0xff&Vc16[i];
        V1Row1[i] = 0xff&(V116[i] >> 8);
        V1Row2[i] = 0xff&V116[i];
        V2Row1[i] = 0xff&(V216[i] >> 8);
        V2Row2[i] = 0xff&V216[i];
        V3Row1[i] = 0xff&(V316[i] >> 8);
        V3Row2[i] = 0xff&V316[i];
        digitalRow1[i] = 0xff&(digital16[i] >> 8);
        digitalRow2[i] = 0xff&digital16[i];
    }
}

uint16 convert8to16(uint8 a, uint8 b)
{
    uint16 temp = 0x0000;
    temp = temp|a;
    temp = temp|b<<8;
    
    return temp;
}

/***************************************************************
 * Function to update the CapSesnse state in the GATT database
 **************************************************************/
void updateThreshold()
{       
CYBLE_GATTS_HANDLE_VALUE_NTF_T tempHandle;
    uint16 threshold = convert8to16(newOverThresholdVoltage, newUnderThresholdVoltage);
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_THRESHOLD_CHARACTERISTIC_CHAR_HANDLE;
  tempHandle.value.val = (uint8 *)&threshold;
    tempHandle.value.len = 4; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED );
    
    CyBle_GattsNotification(cyBle_connHandle,&tempHandle);
}

void updateFlashAvailable()
{
CYBLE_GATTS_HANDLE_VALUE_NTF_T tempHandle;
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_FLASH_AVAILABLE_CHARACTERISTIC_CHAR_HANDLE;
  tempHandle.value.val = (uint8 *)&newflashAvailable;
    tempHandle.value.len = 2; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED );      
    
    CyBle_GattsNotification(cyBle_connHandle,&tempHandle);
}

void sendBLE(int event)
{
    CYBLE_GATTS_HANDLE_VALUE_NTF_T tempHandle;
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_VAROW1CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_ARow1(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_VAROW2CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_ARow2(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_VBROW1CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_BRow1(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_VBROW2CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_BRow2(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
 
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_VCROW1CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_CRow1(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_VCROW2CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_CRow2(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
        
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_V1ROW1CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_1Row1(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_V1ROW2CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_1Row2(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);
  
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_V2ROW1CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_2Row1(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);    
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_V2ROW2CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_2Row2(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED); 

    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_V3ROW1CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_3Row1(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);  
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_V3ROW2CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_3Row2(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED); 
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_DIGITALROW1CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_DRow1(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED); 
    
    tempHandle.attrHandle = CYBLE_THRESHOLD_CONTROL_SERVICE_DIGITALROW2CHARACTERISTIC_CHAR_HANDLE;
    tempHandle.value.val = (uint8 *)CY_TEST_FLASH_ADDR(CY_TEST_FLASH_ROW_DRow2(event));
    tempHandle.value.len = 144; 
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,CYBLE_GATT_DB_LOCALLY_INITIATED);
    
}


//returns 0 if any of rows are not written into flash
uint16 WriteToFlash(int event_select)
{
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_ARow1(event_select), VaRow1)!=CYRET_SUCCESS)
    {
//        return false;
    }
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_ARow2(event_select), VaRow2)!=CYRET_SUCCESS)
    {
 //       return false;
    }
 
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_BRow1(event_select), VbRow1)!=CYRET_SUCCESS)
    {        
//        return false;
    }
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_BRow2(event_select), VbRow2)!=CYRET_SUCCESS)
    {        
//        return false;
    }    
        
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_CRow1(event_select), VcRow1)!=CYRET_SUCCESS)
    {
//        return false;
    }
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_CRow2(event_select), VcRow2)!=CYRET_SUCCESS)
    {
//        return false;
    }
        
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_1Row1(event_select), V1Row1)!=CYRET_SUCCESS)
    {
//        return false;
    }
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_1Row2(event_select), V1Row2)!=CYRET_SUCCESS)
    {
//        return false;
    }    
        
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_2Row1(event_select), V2Row1)!=CYRET_SUCCESS)
    {
//        return false;
    }    
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_2Row2(event_select), V2Row2)!=CYRET_SUCCESS)
    {
//        return false;
    }
    
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_3Row1(event_select), V3Row1)!=CYRET_SUCCESS)
    {
//        return false;
    }
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_3Row2(event_select), V3Row2)!=CYRET_SUCCESS)
    {
//        return false;
    }
    
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_DRow1(event_select), digitalRow1)!=CYRET_SUCCESS)
    {
//        return false;
    }
    if(CySysFlashWriteRow(CY_TEST_FLASH_ROW_DRow2(event_select), digitalRow2)!=CYRET_SUCCESS)
    {
//        return false;
    }
    return (newflashAvailable|(0x0001<<event_select));
}


//update the voltage value 
void updateFlash(int event_select)
{
    convert();
    newflashAvailable = WriteToFlash(event_select);
    
}

void BleCallBack(uint32 event,void* eventParam)
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    switch(event)
    {
        case CYBLE_EVT_STACK_ON:       
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            flashAvailableNotify = 0;
            thresholdNotify = 0;
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            break;
       
        case CYBLE_EVT_GATT_CONNECT_IND:
            updateThreshold();
            updateFlashAvailable();
                //if(event_select==1)
                //printf("Connected to Client\r\n");     
                    
                
        break;
       
        case CYBLE_EVT_GATTS_WRITE_REQ:
               
                wrReqParam=(CYBLE_GATTS_WRITE_REQ_PARAM_T*) eventParam;
                
                
                if(wrReqParam->handleValPair.attrHandle == CYBLE_THRESHOLD_CONTROL_SERVICE_SELECT_CHARACTERISTIC_CHAR_HANDLE)
                {
                   
                    /* only update the value and write the response if the requested write is allowed */
                    if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                    {
                        sendBLE((int)wrReqParam->handleValPair.value.val[0] - 1);
                        CyBle_GattsWriteRsp(cyBle_connHandle);        
                    }
                   
                    
                }
               
                if(wrReqParam->handleValPair.attrHandle == CYBLE_THRESHOLD_CONTROL_SERVICE_FLASH_AVAILABLE_CHARACTERISTIC_FLASHCCCD_DESC_HANDLE)
                {
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    flashAvailableNotify = wrReqParam->handleValPair.value.val[0] & 0x01;
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                }
               
                if(wrReqParam->handleValPair.attrHandle == CYBLE_THRESHOLD_CONTROL_SERVICE_RECEIVE_CHARACTERISTIC__CHAR_HANDLE)
                {
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    newUnderThresholdVoltage = convert8to16(wrReqParam->handleValPair.value.val[0], wrReqParam->handleValPair.value.val[1]);
                    newOverThresholdVoltage = convert8to16(wrReqParam->handleValPair.value.val[2], wrReqParam->handleValPair.value.val[3]);
                    CyBle_GattsWriteRsp(cyBle_connHandle);            
                }
               
                
                if(wrReqParam->handleValPair.attrHandle == CYBLE_THRESHOLD_CONTROL_SERVICE_THRESHOLD_CHARACTERISTIC_THRESHOLDCCCD_DESC_HANDLE)
                {
                    CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    thresholdNotify = wrReqParam->handleValPair.value.val[0] & 0x01;
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                }
               
             break; 
                
                
        default:
            break;
    }
}


int16_t RTFilter(int32_t *coefficients, int16_t *Ph_in, int16_t *hist_buff, int counter)
{
int16_t output = 0;
int16_t temp[11] = { 0 };
hist_buff[counter] = Ph_in[counter];
//Put sample in history buffer in FIFO

for (int i = 0; i <= counter; i++)              //Coeffcient Multiplication
{
temp[i] = hist_buff[i] * coefficients[i];
}
for (int j = 0; j <= counter; j++)              //Addition
{
output += temp[j];
}

return output + 0x1000;                                  //SCALE FACTOR IS PERFORMED OUTSIDE FUNCTION SCOPE
}

uint16_t RTMagCalc(int16_t *filteredData, int16_t *hist_buff, int counter)
{
int16_t output = 0;
hist_buff[counter] = filteredData[counter]; //Put filtered data in hist_buff
int PhaseIndex = 0;
PhaseIndex = counter - 3; // N-3 Elements from current index (90 degree phase shift)

/* If PhaseIndex < 0 results in Seg Fault
* SOLUTION: Using same value for shifted and new value
*/
if(PhaseIndex < 0)
{
PhaseIndex = counter;
}
    
output = sqrt((double)hist_buff[counter] * (double)hist_buff[counter] + (double)hist_buff[PhaseIndex] *(double)hist_buff[PhaseIndex]);
return  (uint16)output;
}

void negative_seq(int32 *phA, int32 *phB, int32 *phC, int32 *neg) 
{
    int i;
    for (i = 13; i < 40; i++)
    {
        neg[i] = phA[i] -(phB[i] +phC[i])/2.0 + (SQRT3*(phB[i-i]-phC[i-1]))/2.0;
    }
}


CY_ISR(MYIsrHandler)
{   
    /*Store raw ADC output data in buffer for each phase voltage*/
    /*Scaled by 2 due to ADC being Vref/2*/
    rawA[buffer_index] = 2 * ADC_SAR_Seq_1_GetResult16(0); 
    rawB[buffer_index] = 2 * ADC_SAR_Seq_1_GetResult16(1);
    rawC[buffer_index] = -2 * ADC_SAR_Seq_1_GetResult16(2);
    k++;   
    //FIR Filter Data
    PhA[buffer_index] = RTFilter(coefficients,rawA,hist_bufa,counter) * SCALE_FACTOR;
    PhB[buffer_index] = RTFilter(coefficients,rawB,hist_bufb,counter) * SCALE_FACTOR;
    PhC[buffer_index] = RTFilter(coefficients,rawC,hist_bufc,counter) * SCALE_FACTOR;
    
    //Magnitude Calculation - Output magPhX variable
    magPhA[buffer_index] = RTMagCalc(PhA,magBufa,counter);
    magPhB[buffer_index] = RTMagCalc(PhB,magBufb,counter);
    magPhC[buffer_index] = RTMagCalc(PhC,magBufc,counter);
    
    
    /* Thresholds - Overvoltage and Undervoltage 
    *  Digital Buffer that stores flags of voltage conditions
    */
    digitals[buffer_index] = 0;
    if (magPhA[buffer_index] > (newOverThresholdVoltage *newOverThresholdVoltage)) // (1241 = 1V0p)  (2^12) 4096/3.3 
    {
        digitals[buffer_index] = digitals[buffer_index] | (1 << 0); 
    }
    if (magPhB[buffer_index] > (newOverThresholdVoltage * newOverThresholdVoltage)) 
    {
        digitals[buffer_index] = digitals[buffer_index] | (1 << 1); 
    }
    if (magPhC[buffer_index] > (newOverThresholdVoltage * newOverThresholdVoltage))
    {
        digitals[buffer_index] = digitals[buffer_index] | (1 << 2); 
    }
    if (magPhA[buffer_index] <=  (newUnderThresholdVoltage * newUnderThresholdVoltage))
    {
        digitals[buffer_index] = digitals[buffer_index] | (1 << 3); 
    }
    if (magPhB[buffer_index] <= (newUnderThresholdVoltage * newUnderThresholdVoltage))
    {
        digitals[buffer_index] = digitals[buffer_index] | (1 << 4); 
    }
    if (magPhC[buffer_index] <= (newUnderThresholdVoltage * newUnderThresholdVoltage))
    {
        digitals[buffer_index] = digitals[buffer_index] | (1 << 5); 
    }
//(digitals[buffer_index] & 0x3F) != 0)&&
    if (event_written < 10&& k > 144) // while a voltage flag has been detected
    {
        
        /*Copy Filtered Data into New Buffer 
        * Due to flash memory being slow it will overwrite the data
        */
        
        //Copy Raw Data from ADC
        
        for( int z = 0; z<144;z++)
        {
            Va16[z] = rawA[z];
            Vb16[z] = rawB[z];
            Vc16[z] = rawC[z];
            
            //Copy Filtered Data
            V116[z] = (uint16)PhA[z];
            V216[z] = (uint16)PhB[z];
            V316[z] = (uint16)PhC[z];
        }
        k = 0;
        updateFlash(event_written++);
        Relay_Write(0u);
    }
    Relay_Write(1u);

    //Ring Buffer - Resets Index to beginning of Array
    if (buffer_index == EVENT_LENGTH - 1)
    {
        buffer_index = 0;
    }
    else //Otherwise continues incrementing index
    {
        buffer_index++;
    }

    if(counter == 11)
    {
        counter = 0;
    }
    else{
        counter++; //Needed for Calculations
    }
    
}

void init()
{
    CyGlobalIntEnable;
    Clock_1_Start();
    Counter_1_Start();
    ADC_SAR_Seq_1_Start();
    CyBle_Start(BleCallBack); /*MUST BE ABOVE ISR START*/
    isr_1_StartEx(MYIsrHandler);
}


int main()
{
    init(); 
    
    for(;;)
    {
       if(flashAvailableNotify)
        {
         updateFlashAvailable();       
        }
       if(thresholdNotify)
        {
            updateThreshold();
        }
            
        CyBle_ProcessEvents();
    }        
}    
