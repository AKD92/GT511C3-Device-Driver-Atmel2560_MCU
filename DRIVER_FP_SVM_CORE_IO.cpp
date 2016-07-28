

/************************************************************************************
    Implementation of GT-511C3 Fingerprint Sensor Core I/O Hardware Routines
    Designed specially for SVM Project
    Author:             Ashis Kumar Das
    Email:              akd.bracu@gmail.com
    GitHub:             https://github.com/AKD92
*************************************************************************************/






#include "DRIVER_FP_SVM.h"

extern "C" {
    #include <string.h>
}





void fpsvm_writeToBuffer(BYTE *buffer, CommandPacket *pktCmd) {
    
    WORD chksum;
    BYTE i;
    
    *(buffer + 0) = pktCmd->head1;
    *(buffer + 1) = pktCmd->head2;
    *(buffer + 2) = (pktCmd->devID & 0x00FF);                       // Low Order Byte
    *(buffer + 3) = (pktCmd->devID & 0xFF00) >> 8;                  // Hight Order Byte
    *(buffer + 4) = (pktCmd->param & 0x000000FF);
    *(buffer + 5) = (pktCmd->param & 0x0000FF00) >> 8;
    *(buffer + 6) = (pktCmd->param & 0x00FF0000) >> 16;
    *(buffer + 7) = (pktCmd->param & 0xFF000000) >> 24;
    *(buffer + 8) = (pktCmd->command & 0x00FF);
    *(buffer + 9) = (pktCmd->command & 0xFF00) >> 8;
    
    chksum = 0;
    i = 0;
    while (i <= 9) {                                                // Calculating checksum value
        chksum = chksum + *(buffer + i);
        i = i + 1;
    }
    
    pktCmd->checksum = chksum;
    *(buffer + 10) = (pktCmd->checksum & 0x00FF);
    *(buffer + 11) = (pktCmd->checksum & 0xFF00) >> 8;
    
    return;
}


void fpsvm_readFromBuffer(ResponsePacket *pktRes, BYTE *buffer) {
    
    
    memset((void *) pktRes, 0, sizeof(ResponsePacket));
    
    pktRes->head1 = *(buffer + 0);
    pktRes->head2 = *(buffer + 1);
    
    pktRes->devID = pktRes->devID << 8;
    pktRes->devID = pktRes->devID | *(buffer + 3);
    pktRes->devID = pktRes->devID << 8;
    pktRes->devID = pktRes->devID | *(buffer + 2);
    
    pktRes->param = pktRes->param << 8;
    pktRes->param = pktRes->param | *(buffer + 7);
    pktRes->param = pktRes->param << 8;
    pktRes->param = pktRes->param | *(buffer + 6);
    pktRes->param = pktRes->param << 8;
    pktRes->param = pktRes->param | *(buffer + 5);
    pktRes->param = pktRes->param << 8;
    pktRes->param = pktRes->param | *(buffer + 4);
    
    pktRes->response = pktRes->response << 8;
    pktRes->response = pktRes->response | *(buffer + 9);
    pktRes->response = pktRes->response << 8;
    pktRes->response = pktRes->response | *(buffer + 8);
    
    pktRes->checksum = pktRes->checksum << 8;
    pktRes->checksum = pktRes->checksum | *(buffer + 11);
    pktRes->checksum = pktRes->checksum << 8;
    pktRes->checksum = pktRes->checksum | *(buffer + 10);
    
    return;
}


void fpsvm_writePacketToSerial(HardwareSerial *serial, BYTE *buffer) {
    
    serial->write(buffer, 12);
    serial->flush();
    return;
}


void fpsvm_readPacketFromSerial(BYTE *buffer, HardwareSerial *serial) {
    
    BYTE i;
    BYTE firstbyte;
    
    REPEAT:                                         /* Repeat Until loop for Header detection */
    firstbyte = (BYTE) serial->read();
    if (firstbyte != STX1) {
        delay(5);
        goto REPEAT;
    }
    
    *(buffer + 0) = firstbyte;                      /* Set header value on top of the buffer */
    for (i = 1; i <= 11; i++) {                     /* Continue reading rest of the bytes */
        
        while (serial->available() == 0) {
            delay(5);
        }
        *(buffer + i) = (BYTE) serial->read();
    }
    return;
}


void fpsvm_writeTemplateToSerial(HardwareSerial *serial, BYTE *bufStorage) {
    
    WORD i;
    WORD checksum;
    
    checksum = 0;
    checksum += STX3;
    checksum += STX4;
    checksum += 0x01;
    checksum += 0x00;
    
    serial->write(STX3);
    serial->write(STX4);
    serial->write(0x01);
    serial->write(0x00);
    
    for (i = 0; i < 498; i++) {
        serial->write(*(bufStorage + i));              /* Write the value of (i - 1)th index of the buffer */
        checksum += *(bufStorage + i);                 /* Continue calculating checksum value */
    }
    
    serial->write(checksum & 0x00FF);
    serial->write((checksum & 0xFF00) >> 8);
    serial->flush();
    return;
}


void fpsvm_readTemplateFromSerial(BYTE *bufStorage, HardwareSerial *serial) {
    
    WORD i;
    BYTE firstbyte;
    
    REPEAT:
    firstbyte = (BYTE) serial->read();
    if (firstbyte != STX3) {
        delay(5);
        goto REPEAT;
    }
    
    while (serial->available() < 3) {
        delay(10);
    }
    serial->read();
    serial->read();
    serial->read();                                         // Reading headers complete
    
    for (i = 0; i < 498; i++) {                             // Reading original template bytes
        
        while (serial->available() == 0) {
            delay(5);
        }
        *(bufStorage + i) = (BYTE) serial->read();
    }
    
    while (serial->available() < 2) {                       // Reading 2 byte checksum
        delay(5);
    }
    serial->read();
    serial->read();
    return;
}



