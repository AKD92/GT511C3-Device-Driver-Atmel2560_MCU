

/************************************************************************************
    Implementation of GT-511C3 Fingerprint Sensor Device Driver Software
    Designed specially for SVM Project
    Author:             Ashis Kumar Das
    Email:              akd.bracu@gmail.com
    GitHub:             https://github.com/AKD92
*************************************************************************************/






#include "DRIVER_FP_SVM.h"







// Global input command memory

BYTE bHostCommand;




// Function prototypes

void setup(void);
void loop(void);








// Function definitions

void setup(void) {
  
        Serial.begin(9600);                         // Host serial connection
        Serial1.begin(9600);                        // GT-511C3 Fingerprint device serial connection
        fpsvm_init(&Serial, &Serial1);
        fpsvm_DeleteDatabase();
        return;
}



void loop(void) {
  
        while (Serial.available() == 0) {
            delay(10);
        }
        
        bHostCommand = (BYTE) Serial.read();
        
        
        if (bHostCommand == CMD_ENROLL) {
            fpsvm_Enroll();
        }
        else if (bHostCommand == CMD_GETENROLLED) {
            fpsvm_GetEnrolledTemplate();
        }
        else if (bHostCommand == CMD_SCANVOTER) {
            fpsvm_ScanLiveTemplate();
        }
        else if (bHostCommand == CMD_SETTEMPLATE) {
            fpsvm_ReadTemplateFromHost();           // First read 498 byte data (from host)
            fpsvm_SetTemplate();                    // Then read iPosition (a byte, 0 - 199) and execute
        }
        else if (bHostCommand == CMD_IDENTIFY) {
            fpsvm_IdentifyTemplate();
            fpsvm_DeleteDatabase();
        }
}



