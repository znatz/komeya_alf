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

#define COMMAND_MODE_SBPL		0	// SBPL?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ[?øΩh
#define COMMAND_MODE_COMPAT		1	// ?øΩ›äÔøΩ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩ?øΩ?øΩ[?øΩh

/* ?øΩ?øΩ?øΩ?øΩR?øΩ[?øΩh ---------------------------*/
#define STX		0x02
#define ETX		0x03
#define ACK		0x06
#define NAK		0x15
#define ESC		0x1B
#define ENQ		0x05
#define CAN		0x18


/*SBPL?øΩ?øΩ?øΩ[?øΩh*/
/* ?øΩX?øΩe?øΩ[?øΩ^?øΩX ---------------------------*/
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

/*?øΩv?øΩ`?øΩ?øΩ?øΩp?øΩ?øΩ?øΩ›äÔøΩ?øΩ?øΩ?øΩ[?øΩh */
#define CMD_ERR_NOERR				0x00	/*?øΩ?øΩ?øΩ?øΩﬁΩ√∞?øΩ?øΩ ?øΩ?øΩ?øΩ?øΩI?øΩ?øΩ*/
#define CMD_ERR					0x01	/*?øΩ?øΩ?øΩ?øΩﬁΩ√∞?øΩ?øΩ ?øΩR?øΩ}?øΩ?øΩ?øΩh?øΩw?øΩ?øΩG?øΩ?øΩ?øΩ[*/
#define CMD_ERR_PARAM				0x02	/*?øΩ?øΩ?øΩ?øΩﬁΩ√∞?øΩ?øΩ ?øΩp?øΩ?øΩ?øΩ?øΩ?øΩ[?øΩ^?øΩw?øΩ?øΩG?øΩ?øΩ?øΩ[*/
#define CMD_ERR_POS				0x03	/*?øΩ?øΩ?øΩ?øΩﬁΩ√∞?øΩ?øΩ ?øΩ?öà íu?øΩw?øΩ?øΩG?øΩ?øΩ?øΩ[*/
#define CMD_ERR_BCR				0x04	/*?øΩ?øΩ?øΩ?øΩﬁΩ√∞?øΩ?øΩ ?øΩo?øΩ[?øΩR?øΩ[?øΩh?øΩ?öóÃàÔøΩI?øΩ[?øΩo?øΩ[?øΩG?øΩ?øΩ?øΩ[*/
#define CMD_ERR_PDF				0x05	/*?øΩ?øΩ?øΩ?øΩﬁΩ√∞?øΩ?øΩ PDF417?øΩw?øΩ?øΩG?øΩ?øΩ?øΩ[*/
#define CMD_ERR_QR				0x06	/*?øΩ?øΩ?øΩ?øΩﬁΩ√∞?øΩ?øΩ QR?øΩR?øΩ[?øΩh?øΩw?øΩ?øΩG?øΩ?øΩ?øΩ[*/
#define CMD_ERR_CRC				0x10	/*?øΩ?øΩ?øΩ?øΩﬁΩ√∞?øΩ?øΩ CRC?øΩG?øΩ?øΩ?øΩ[*/

#define STS_ERR_NOERR				0x00	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩ?øΩ?øΩ?øΩI?øΩ?øΩ?øΩA?øΩ?øΩM?øΩ“ÇÔøΩ*/
#define STS_ERR_EDIT				0x01	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩ?øΩÕÅE?øΩ“èW?øΩ?øΩ?øΩA?øΩn?øΩN?øΩ?øΩ?øΩ“ÇÔøΩ*/
#define STS_ERR_OFFLINE				0x02	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩI?øΩt?øΩ?øΩ?øΩC?øΩ?øΩ*/
#define STS_ERR_COVER				0x03	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩJ?øΩo?øΩ[?øΩI?øΩ[?øΩv?øΩ?øΩ*/
#define STS_ERR_PAPER				0x04	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩy?øΩ[?øΩp?øΩ[?øΩG?øΩ?øΩ?øΩh*/
#define STS_ERR_SENSOR				0x06	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩZ?øΩ?øΩ?øΩT?øΩG?øΩ?øΩ?øΩ[*/
#define STS_ERR_HEAD				0x07	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩw?øΩb?øΩh?øΩG?øΩ?øΩ?øΩ[*/
#define STS_ERR_BAT				0x10	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩo?øΩb?øΩe?øΩ?øΩ?øΩG?øΩ?øΩ?øΩ[*/
#define STS_ERR_OTHER				0x11	/*?øΩ√∞?øΩ?øΩ?øΩ⁄çÔøΩ ?øΩ?øΩ?øΩÃëÔøΩ?øΩG?øΩ?øΩ?øΩ[*/


#define RETRY_CNT				50


// * ---------------------------
// *    NEX-M230 ÂëΩ‰ª§
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