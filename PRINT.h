#ifndef __PT_SAMPLE_H__
#define __PT_SAMPLE_H__

#define LAPIN_BAT_LOW			1
#define LAPIN_BUFF_FULL			2
#define LAPIN_LOW_FULL			3
#define LAPIN_PRINTING			4
#define LAPIN_STANDBY			5
#define LAPIN_EDIT			6

#define LAPIN_OK			 0
#define LAPIN_ERR_CLOSE			-10
#define LAPIN_ERR_INIT			-12
#define LAPIN_ERR_OPEN			-13
#define LAPIN_ERR_CONNECT		-14
#define LAPIN_ERR_GETSTATUS		-15
#define LAPIN_ERR_BUFF			-16
#define LAPIN_ERR_PAPER			-17
#define LAPIN_ERR_BAT			-18
#define LAPIN_ERR_SENSOR		-19
#define LAPIN_ERR_HEAD			-20
#define LAPIN_ERR_COVER			-21
#define LAPIN_ERR_OTHER			-22
#define LAPIN_ERR_COMM			-23
#define LAPIN_ERR_OFFLINE		-24
#define LAPIN_ERR_COMMAND		-25

#define COMMAND_MODE_SBPL		0	// SBPL?申R?申}?申?申?申h?申?申?申[?申h
#define COMMAND_MODE_COMPAT		1	// ?申��鐃�?申R?申}?申?申?申h?申?申?申[?申h

/* ?申?申?申?申R?申[?申h ---------------------------*/
#define STX		0x02
#define ETX		0x03
#define ACK		0x06
#define NAK		0x15
#define ESC		0x1B
#define ENQ		0x05
#define CAN		0x18


/*SBPL?申?申?申[?申h*/
/* ?申X?申e?申[?申^?申X ---------------------------*/
#define OFF_NOERR				'0'
#define OFF_BAT_LOW				'1'
#define OFF_BUFF_FULL				'2'
#define OFF_LOW_FULL				'3'
#define ON_WAIT_NOERR				'A'
#define ON_WAIT_BAT_LOW				'B'
#define ON_WAIT_BUFF_FULL			'C'
#define ON_WAIT_LOW_FULL			'D'
#define ON_PRT_NOERR				'G'
#define ON_PRT_BAT_LOW				'H'
#define ON_PRT_BUFF_FULL			'I'
#define ON_PRT_LOW_FULL				'J'
#define ON_STANDBY_NOERR			'M'
#define ON_STANDBY_BAT_LOW			'N'
#define ON_STANDBY_BUFF_FULL			'O'
#define ON_STANDBY_LOW_FULL			'P'
#define ON_EDIT_NOERR				'S'
#define ON_EDIT_BAT_LOW				'T'
#define ON_EDIT_BUFF_FULL			'U'
#define ON_EDIT_LOW_FULL			'V'
#define ERR_BUFF				'a'
#define ERR_PAPER				'c'
#define ERR_BAT					'd'
#define ERR_SENSOR				'f'
#define ERR_HEAD				'g'
#define ERR_COVER				'h'
#define ERR_OTHER				'k'

/*?申v?申`?申?申?申p?申?申?申��鐃�?申?申?申[?申h */
#define CMD_ERR_NOERR				0x00	/*?申?申?申?申渊丹?申?申 ?申?申?申?申I?申?申*/
#define CMD_ERR					0x01	/*?申?申?申?申渊丹?申?申 ?申R?申}?申?申?申h?申w?申?申G?申?申?申[*/
#define CMD_ERR_PARAM				0x02	/*?申?申?申?申渊丹?申?申 ?申p?申?申?申?申?申[?申^?申w?申?申G?申?申?申[*/
#define CMD_ERR_POS				0x03	/*?申?申?申?申渊丹?申?申 ?申?����u?申w?申?申G?申?申?申[*/
#define CMD_ERR_BCR				0x04	/*?申?申?申?申渊丹?申?申 ?申o?申[?申R?申[?申h?申?����鐃�I?申[?申o?申[?申G?申?申?申[*/
#define CMD_ERR_PDF				0x05	/*?申?申?申?申渊丹?申?申 PDF417?申w?申?申G?申?申?申[*/
#define CMD_ERR_QR				0x06	/*?申?申?申?申渊丹?申?申 QR?申R?申[?申h?申w?申?申G?申?申?申[*/
#define CMD_ERR_CRC				0x10	/*?申?申?申?申渊丹?申?申 CRC?申G?申?申?申[*/

#define STS_ERR_NOERR				0x00	/*?申丹?申?申?申��鐃� ?申?申?申?申I?申?申?申A?申?申M?申��鐃�*/
#define STS_ERR_EDIT				0x01	/*?申丹?申?申?申��鐃� ?申?申��E?申��W?申?申?申A?申n?申N?申?申?申��鐃�*/
#define STS_ERR_OFFLINE				0x02	/*?申丹?申?申?申��鐃� ?申I?申t?申?申?申C?申?申*/
#define STS_ERR_COVER				0x03	/*?申丹?申?申?申��鐃� ?申J?申o?申[?申I?申[?申v?申?申*/
#define STS_ERR_PAPER				0x04	/*?申丹?申?申?申��鐃� ?申y?申[?申p?申[?申G?申?申?申h*/
#define STS_ERR_SENSOR				0x06	/*?申丹?申?申?申��鐃� ?申Z?申?申?申T?申G?申?申?申[*/
#define STS_ERR_HEAD				0x07	/*?申丹?申?申?申��鐃� ?申w?申b?申h?申G?申?申?申[*/
#define STS_ERR_BAT				0x10	/*?申丹?申?申?申��鐃� ?申o?申b?申e?申?申?申G?申?申?申[*/
#define STS_ERR_OTHER				0x11	/*?申丹?申?申?申��鐃� ?申?申?申��鐃�?申G?申?申?申[*/


#define RETRY_CNT				50


// * ---------------------------
// *    NEX-M230 ��巡擦
// * --------------------------- 

// * Romaji
extern const char bRomaji[2];

// * Quadruple Japanese Kanji
extern const char bJPx4Release[3];
extern const char bJPx4Set[3];

// * Japanese Kanji
extern const char bJP[5] ;
extern const char bDoulbeStrike[3] ;
extern const char bNonDoulbeStrik[3] ;

// * Japanese Release
extern const char bJPRelease[2];

// *  Alignment
extern const char bAlignLeft[3] ;
extern const char bAlignCenter[3] ;
extern const char bAlignRight[3] ;

// *  Size
extern const char  bSizeNor[3] ;
extern const char  bSizeDbl[3] ;
extern const char  bSizeTri[3] ;
extern const char  bSizeHDb[3] ;

// *  Feed 4 lines
extern const char  nFeedLine[3] ;

// *  Cut Paper
extern const char  bCut[2] ;

// *  Underline
extern const char  bUnderline0[3] ;
extern const char  bUnderline1[3] ;
extern const char  bUnderline2[3] ;

// *  Register Image
extern const char  startRegisterImage0[3] ;
extern const char  startRegisterImage1[3] ;
extern const char  startRegisterImage2[3] ;
extern const char  finishRegisterImage[3] ;

// *  Print Register Image
extern const char  printRegisterImage0[3] ;
extern const char  printRegisterImage1[3] ;
extern const char  printRegisterImage2[3] ;

// *  HRI Character
extern const char  hriNon[3] ;
extern const char  hriAbove[3] ;
extern const char  hriBelow[3] ;
extern const char  hriBoth[3] ;

// *  Print Barcode
extern const char  printJAN13[3] ;


extern int LapinBTOpen(char* btAddr, bool CRC_Check, int commandMode);
extern int LapinBTGetStat(void);
extern int LapinBTSend(char* data, int length);
extern int LapinBTDisconnect(void);
extern int PrintMain(char* addr, bool crc, int mode , short Flag );

#endif // __PT_SAMPLE_H__