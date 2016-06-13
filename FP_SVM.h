

/************************************************************************************
    Program Interface of GT-511C3 Fingerprint Sensor, specialized for SVM Project
    Author:             Ashis Kumar Das
    Email:              akd.bracu@gmail.com
    GitHub:             https://github.com/AKD92
*************************************************************************************/






#ifndef FP_SVM						/* Include Guard */
#define FP_SVM



#include <Arduino.h>
#include <stdint.h>					/* Provides fixed-length integer types */
#include <HardwareSerial.h>



typedef uint8_t BYTE;				/* A BYTE is 8bit Unsigned Integer */
typedef uint16_t WORD;				/* A WORD is 16bit Unsigned Integer */
typedef uint32_t DWORD;				/* A DWORD (Double Word) is 32bit Unsigned Integer */



struct CmdPacket {
	
	BYTE head1;						/* 1st header of Command Packet */
	BYTE head2;						/* 2nd header of Command Packet */
	WORD devID;						/* Device id (0x0001) */
	DWORD param;					/* Operation parameter */
	WORD command;					/* Command value */
	WORD checksum;					/* Checksum value */
};

struct ResPacket {
	
	BYTE head1;
	BYTE head2;
	WORD devID;
	DWORD param;
	WORD response;
	WORD checksum;
};

typedef struct CmdPacket CommandPacket;
typedef struct ResPacket ResponsePacket;




#define CMD_ENROLL						0xBB
#define CMD_GETENROLLED					0x55
#define CMD_SETTEMPLATE					0xBA
#define CMD_SCANVOTER					0xAB
#define CMD_IDENTIFY					0x77
#define RESP_OK							0xB5
#define RESP_ERROR						0x5B


/* Header Values for any Packet */
#define		STX1						0x55
#define		STX2						0xAA
#define		STX3						0x5A
#define		STX4						0xA5


/* Command Values for Command Packet */
#define		ACK							0x30
#define		NACK						0x31
#define		OPEN						0x01
#define		CLOSE						0x02
#define		USBINTERNALCHECK			0x03
#define		CHANGEBAUDRATE				0x04
#define		CMOSLED						0x12
#define		GETENROLLCOUNT				0x20
#define		CHECKENROLLED				0x21
#define		ENROLLSTART					0x22
#define		ENROLL1						0x23
#define		ENROLL2						0x24
#define		ENROLL3						0x25
#define		ISPRESSFINGER				0x26
#define		DELETE						0x40
#define		DELETEALL					0x41
#define		VERIFYTEMPLATE				0x52
#define		IDENTIFYTEMPLATE			0x53
#define		CAPTUREFINGER				0x60
#define		MAKETEMPLATE				0x61
#define		GETTEMPLATE					0x70
#define		SETTEMPLATE					0x71



/* Error Codes for Response Packet */
#define		NACK_TIMEOUT				0x1001
#define		NACK_INVALID_BAUD			0x1002
#define		NACK_INVALD_POS				0x1003
#define		NACK_IS_NOT_USED			0x1004
#define		NACK_IS_ALREADY_USED		0x1005
#define		NACK_COMM_ERROR				0x1006
#define		NACK_VERIFY_FAILED			0x1007
#define		NACK_IDENTIFY_FAILED		0x1008
#define		NACK_DB_IS_FULL				0x1009
#define		NACK_DB_IS_EMPTY			0x100A
#define		NACK_TURN_ERROR				0x100B
#define		NACK_BAD_FINGER				0x100C
#define		NACK_ENROLL_FAILED			0x100D
#define		NACK_IS_NOT_SUPPORTED		0x100E
#define		NACK_DEV_ERROR				0x100F
#define		NACK_CAPTURE_CANCELED		0x1010
#define		NACK_INVALID_PARAM			0x1011
#define		NACK_FINGER_NOT_PRESSED		0x1012




void fpsvm_init(HardwareSerial *host, HardwareSerial *fp);


void fpsvm_writeToBuffer(BYTE *buffer, CommandPacket *pktCmd);

void fpsvm_readFromBuffer(ResponsePacket *pktRes, BYTE *buffer);


void fpsvm_writePacketToSerial(HardwareSerial *serial, BYTE *buffer);

void fpsvm_readPacketFromSerial(BYTE *buffer, HardwareSerial *serial);


void fpsvm_writeTemplateToSerial(HardwareSerial *serial, BYTE *bufTemplate);

void fpsvm_readTemplateFromSerial(BYTE *bufTemplate, HardwareSerial *serial);




void fpsvm_enroll(void);

void fpsvm_getenrolledtemplate(void);

void fpsvm_deleteall(void);

void fpsvm_settemplate(void);

void fpsvm_scanvoter(void);

void fpsvm_identify(void);


#endif