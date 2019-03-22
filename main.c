/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

// Function to get the current value of the pins 

uint8 GetTemp( void ){
    // Get the state of every pin
    
    uint8 FW = Forward_Pin_Read();
    uint8 RV = Reverse_Pin_Read();
    uint8 R  = Right_Pin_Read();
    uint8 L  = Left_Pin_Read();
    
    if( (~FW) && (~RV) && (~R) && (~L) ){
        return( 0 );
        
    }else if( (FW) && (~RV) && (~R) && (~L) ){
        return ( 1 );
        
    }else if( (FW) && (~RV) && (R) && (~L) ){
        return ( 2 );
        
    }else if( (FW) && (~RV) && (~R) && (L) ){
        return ( 3 );
        
    }else if( (~FW) && (RV) && (~R) && (~L) ){
        return ( 4 );
        
    }else if( (~FW) && (RV) && (R) && (~L) ){
        return ( 5 );
        
    }else if( (~FW) && (RV) && (~R) && (L) ){
        return ( 6 );
        
    }else{
        return 0;
    }
}

// Function to update the pin state in the GATT database

void updateState( void ){
    
    // If the BLE is not connected, don't update the LED.
    
    if( CyBle_GetState() != CYBLE_STATE_CONNECTED ){
        return;
    }
    
    // Value of state
    
    uint8 Val = GetTemp();
    
    // Make a temp value for drive
    
    CYBLE_GATTS_HANDLE_VALUE_NTF_T tempDrive;
    
    tempDrive.attrHandle = CYBLE_RC_CAR_DRIVE_CHAR_HANDLE;   
    tempDrive.value.val = ( uint8* ) &Val;       
    tempDrive.value.len = 1;
    
    // Write temp values to GATT database
    
    CyBle_GattsWriteAttributeValue( &tempDrive, 0, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED );
    
}

// Write Pins function for ble

void WritePins( uint8 Val ){

    switch( Val ){
    
        case '0':
            Forward_Pin_Write(0);
            Reverse_Pin_Write(0);
            Right_Pin_Write(0);
            Left_Pin_Write(0);
            break;
        
        case '1':
            Forward_Pin_Write(1);
            /*Reverse_Pin_Write(0);
            Right_Pin_Write(0);
            Left_Pin_Write(0);*/
            break;
            
        case '2':
            Forward_Pin_Write(1);
            Reverse_Pin_Write(0);
            Right_Pin_Write(1);
            Left_Pin_Write(0);
            break;
            
        case '3':
            Forward_Pin_Write(1);
            Reverse_Pin_Write(0);
            Right_Pin_Write(0);
            Left_Pin_Write(1);
            break;
            
        case '4':
            Forward_Pin_Write(0);
            Reverse_Pin_Write(1);
            Right_Pin_Write(0);
            Left_Pin_Write(0);
            break;
            
        case '5':
            Forward_Pin_Write(0);
            Reverse_Pin_Write(1);
            Right_Pin_Write(1);
            Left_Pin_Write(0);
            break;
            
        case '6':
            Forward_Pin_Write(0);
            Reverse_Pin_Write(1);
            Right_Pin_Write(0);
            Left_Pin_Write(1);
            break;
            
        default: 
            break;
    }
}
// BLE Event Handler

void BleCallBack( uint32 event, void* eventParam ){
    
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    
    switch( event ){
    
        // If stack is turned on or there is a disconnect, start advertising an blink blue LED
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            CyBle_GappStartAdvertisement( CYBLE_ADVERTISING_FAST );
            PWM_Start();
            break;
            
        // If device is connected, turn off blue led and update states
        case CYBLE_EVT_GATT_CONNECT_IND:
            PWM_Stop();
            updateState();
            break;
            
        // If there is a write request
        case CYBLE_EVT_GATTS_WRITE_REQ:
            
            //Determine which characteristic to user wants to write to
            wrReqParam = ( CYBLE_GATTS_WRITE_REQ_PARAM_T * ) eventParam;
            
            // Drive
            if( wrReqParam->handleValPair.attrHandle == CYBLE_RC_CAR_DRIVE_CHAR_HANDLE ){
                
                // Only update the value and write the response if the request is allowed
                if( (CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue( &wrReqParam->handleValPair, 0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED )) ){
                    
                    //uint8 WriteValue = (uint8) ((wrReqParam->handleValPair.value.val) - 30;
                    WritePins( *wrReqParam->handleValPair.value.val );
                    CyBle_GattsWriteRsp(cyBle_connHandle);
                }
            }
            
            break;
            
        default:
            break;
    }
    
    
}

int main( void ){
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    CyBle_Start( BleCallBack );

    for( ; ; ){
        /* Place your application code here. */
        
        CyBle_ProcessEvents();
        CyBle_EnterLPM( CYBLE_BLESS_DEEPSLEEP );
    }
}

/* [] END OF FILE */
