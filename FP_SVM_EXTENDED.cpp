

/************************************************************************************
    Implementation of GT-511C3 Fingerprint Sensor Extended Functionalites,
    Specialized for SVM Project
    Author:             Ashis Kumar Das
    Email:              akd.bracu@gmail.com
    GitHub:             https://github.com/AKD92
*************************************************************************************/






#include "FP_SVM.h"

extern "C" {
    #include "string.h"
}





BYTE bufCommand[12];
BYTE bufResponse[12];
BYTE bufTemplate[498];
BYTE bufLive[498];

HardwareSerial *srlHost;
HardwareSerial *srlFp;




void fpsvm_init(HardwareSerial *host, HardwareSerial *fp) {
    
    CommandPacket pkc;
    ResponsePacket pkr;
    
    srlHost = host;
    srlFp = fp;
    
    pkc.head1 = STX1;
    pkc.head2 = STX2;
    pkc.devID = 0x0001;
    
    // Intialize the fingerprint device
    
    pkc.command = OPEN;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    
    if (pkr.response == ACK) {
        
        pkc.command = CMOSLED;
        pkc.param = 0;
        fpsvm_writeToBuffer(bufCommand, &pkc);
        fpsvm_writePacketToSerial(srlFp, bufCommand);
        fpsvm_readPacketFromSerial(bufResponse, srlFp);
        // fpsvm_readFromBuffer(&pkr, bufResponse);
        
        delay(2);
        
        pkc.command = CMOSLED;
        pkc.param = 1;
        fpsvm_writeToBuffer(bufCommand, &pkc);
        fpsvm_writePacketToSerial(srlFp, bufCommand);
        fpsvm_readPacketFromSerial(bufResponse, srlFp);
        // fpsvm_readFromBuffer(&pkr, bufResponse);
        
        delay(150);
        
        pkc.command = CMOSLED;
        pkc.param = 0;
        fpsvm_writeToBuffer(bufCommand, &pkc);
        fpsvm_writePacketToSerial(srlFp, bufCommand);
        fpsvm_readPacketFromSerial(bufResponse, srlFp);
        // fpsvm_readFromBuffer(&pkr, bufResponse);
        
        delay(120);
        
        pkc.command = CMOSLED;
        pkc.param = 1;
        fpsvm_writeToBuffer(bufCommand, &pkc);
        fpsvm_writePacketToSerial(srlFp, bufCommand);
        fpsvm_readPacketFromSerial(bufResponse, srlFp);
        // fpsvm_readFromBuffer(&pkr, bufResponse);
        
        delay(400);
        
        pkc.command = CMOSLED;
        pkc.param = 0;
        fpsvm_writeToBuffer(bufCommand, &pkc);
        fpsvm_writePacketToSerial(srlFp, bufCommand);
        fpsvm_readPacketFromSerial(bufResponse, srlFp);
        // fpsvm_readFromBuffer(&pkr, bufResponse);
        
        // srlHost->write(RESP_OK);
    }
    // else {
        // srlHost->write(RESP_ERROR);
    // }
    
    return;
}



void fpsvm_enroll(void) {
    
    CommandPacket pkc;
    ResponsePacket pkr;
    
    pkc.head1 = STX1;
    pkc.head2 = STX2;
    pkc.devID = 0x0001;
    
    pkc.command = CMOSLED;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    delay(80);
    
    pkc.command = CMOSLED;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    delay(80);
    
    pkc.command = CMOSLED;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    delay(80);
    
    pkc.command = CMOSLED;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    delay(100);
    
    pkc.command = CMOSLED;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    // Enroll Start
    
    pkc.command = ENROLLSTART;
    pkc.param = -1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == ACK) {
        srlHost->write(RESP_OK);
        srlHost->flush();
    }
    else {
        srlHost->write(RESP_ERROR);
        srlHost->flush();
        return;
    }
    
    // Check for pressed finger
    
    REPEAT_1:
    pkc.command = ISPRESSFINGER;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.param != 0) {
        delay(100);
        goto REPEAT_1;
    }
    
    // Capture Finger
    
    pkc.command = CAPTUREFINGER;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == NACK) {
        delay(100);
        goto REPEAT_1;
    }
    
    // Enroll 1
    
    pkc.command = ENROLL1;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == ACK) {
        srlHost->write(RESP_OK);
        srlHost->flush();
    }
    else {
        srlHost->write(RESP_ERROR);
        srlHost->flush();
        return;
    }
    
    // Remove Finger
    
    REPEAT_2:
    pkc.command = ISPRESSFINGER;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.param == 0) {
        delay(100);
        goto REPEAT_2;
    }
    
    // Turn off CMOS led for 2 seconds
    
    pkc.command = CMOSLED;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    delay(1000);
    pkc.command = CMOSLED;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    // check for finger pressed
    
    REPEAT_3:
    pkc.command = ISPRESSFINGER;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.param != 0) {
        delay(100);
        goto REPEAT_3;
    }
    
    // Capture finger
    
    pkc.command = CAPTUREFINGER;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == NACK) {
        delay(100);
        goto REPEAT_3;
    }
    
    // Enroll 2
    
    pkc.command = ENROLL2;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == ACK) {
        srlHost->write(RESP_OK);
        srlHost->flush();
    }
    else {
        srlHost->write(RESP_ERROR);
        srlHost->flush();
        return;
    }
    
    // Remove Finger
    
    REPEAT_4:
    pkc.command = ISPRESSFINGER;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.param == 0) {
        delay(100);
        goto REPEAT_4;
    }
    
    // Turn off CMOS led for 2 seconds
    
    pkc.command = CMOSLED;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    delay(1000);
    pkc.command = CMOSLED;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    // Check for finger pressed
    
    REPEAT_5:
    pkc.command = ISPRESSFINGER;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.param != 0) {
        delay(100);
        goto REPEAT_5;
    }
    
    // Capture finger
    
    pkc.command = CAPTUREFINGER;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == NACK) {
        delay(100);
        goto REPEAT_5;
    }
    
    // Enroll 3
    
    pkc.command = ENROLL3;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == NACK) {
        srlHost->write(RESP_ERROR);
        srlHost->flush();
        return;
    }
    
    // Read enrolled fingerprint template to buffer
    fpsvm_readTemplateFromSerial(bufTemplate, srlFp);
    
    // Turn off CMOS led
    
    pkc.command = CMOSLED;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    
    srlHost->write(RESP_OK);
    srlHost->flush();
    
    return;
}



void fpsvm_getenrolledtemplate(void) {
    
    WORD index;
    BYTE bValue;
    
    index = 0;
    while (index < 498) {
        bValue = *(bufTemplate + index);
        srlHost->write(bValue);
        index = index + 1;
    }
    srlHost->flush();
    
    return;
}


void fpsvm_deleteall(void) {
    
    CommandPacket pkc;
    ResponsePacket pkr;
    
    pkc.head1 = STX1;
    pkc.head2 = STX2;
    pkc.devID = 0x0001;
    pkc.command = DELETEALL;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    
    if (pkr.response == ACK) {
        srlHost->write(RESP_OK);
    }
    else {
        srlHost->write(RESP_ERROR);
    }
    
    return;
}



void fpsvm_settemplate(void) {
    
    CommandPacket pkc;
    ResponsePacket pkr;
    BYTE iPos;
    
    pkc.head1 = STX1;
    pkc.head2 = STX2;
    pkc.devID = 0x0001;
    
    while (srlHost->available() == 0) {
        delay(5);
    }
    iPos = (BYTE) srlHost->read();
    
    if (iPos > 199) {
        srlHost->write(RESP_ERROR);
        return;
    }
    
    // Read the template from the host
    
    fpsvm_readTemplateFromSerial(bufTemplate, srlHost);
    
    // Make a room for storing the template in the device
    
    pkc.command = CHECKENROLLED;
    pkc.param = iPos;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    
    if (pkr.response == ACK) {
        pkc.command = DELETE;
        pkc.param = iPos;
        fpsvm_writeToBuffer(bufCommand, &pkc);
        fpsvm_writePacketToSerial(srlFp, bufCommand);
        fpsvm_readPacketFromSerial(bufResponse, srlFp);
        fpsvm_readFromBuffer(&pkr, bufResponse);
    }
    
    pkc.command = SETTEMPLATE;
    pkc.param = iPos;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    
    // Send template data to the device
    
    fpsvm_writeTemplateToSerial(srlFp, bufTemplate);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    
    if (pkr.response == ACK) {
        srlHost->write(RESP_OK);
    }
    else {
        srlHost->write(RESP_ERROR);
    }
    srlHost->flush();
    
    return;
}



void fpsvm_scanvoter(void) {
    
    CommandPacket pkc;
    ResponsePacket pkr;
    
    pkc.head1 = STX1;
    pkc.head2 = STX2;
    pkc.devID = 0x0001;
    
    pkc.command = CMOSLED;
    pkc.param = 1;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    REPEAT_1:
    pkc.command = ISPRESSFINGER;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.param != 0) {
        delay(100);
        goto REPEAT_1;
    }
    
    pkc.command = CAPTUREFINGER;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == NACK) {
        delay(100);
        goto REPEAT_1;
    }
    
    pkc.command = MAKETEMPLATE;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == NACK) {
        srlHost->write(RESP_ERROR);
        srlHost->flush();
        return;
    }
    
    fpsvm_readTemplateFromSerial(bufLive, srlFp);
    
    pkc.command = CMOSLED;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    // fpsvm_readFromBuffer(&pkr, bufResponse);
    
    srlHost->write(RESP_OK);
    srlHost->flush();
    
    return;
}



void fpsvm_identify(void) {
    
    CommandPacket pkc;
    ResponsePacket pkr;
    BYTE iPos;
    
    pkc.head1 = STX1;
    pkc.head2 = STX2;
    pkc.devID = 0x0001;
    pkc.command = IDENTIFYTEMPLATE;
    pkc.param = 0;
    fpsvm_writeToBuffer(bufCommand, &pkc);
    fpsvm_writePacketToSerial(srlFp, bufCommand);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    if (pkr.response == NACK) {
        srlHost->write(RESP_ERROR);
        srlHost->flush();
        return;
    }
    
    fpsvm_writeTemplateToSerial(srlFp, bufLive);
    fpsvm_readPacketFromSerial(bufResponse, srlFp);
    fpsvm_readFromBuffer(&pkr, bufResponse);
    
    if (pkr.response == ACK) {
        iPos = (BYTE) (pkr.param & 0x000000FF);
        srlHost->write(RESP_OK);
        srlHost->write(iPos);
    }
    else {
        srlHost->write(RESP_ERROR);
    }
    srlHost->flush();
    
    return;
}