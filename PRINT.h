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

#define COMMAND_MODE_SBPL		0	// SBPLコマンドモード
#define COMMAND_MODE_COMPAT		1	// 互換コマンドモード

/* 制御コード ---------------------------*/
#define STX		0x02
#define ETX		0x03
#define ACK		0x06
#define NAK		0x15
#define ESC		0x1B
#define ENQ		0x05
#define CAN		0x18


/*SBPLモード*/
/* ステータス ---------------------------*/
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

/*プチラパン互換モード */
#define CMD_ERR_NOERR				0x00	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ 正常終了*/
#define CMD_ERR					0x01	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ コマンド指定エラー*/
#define CMD_ERR_PARAM				0x02	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ パラメータ指定エラー*/
#define CMD_ERR_POS				0x03	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ 印字位置指定エラー*/
#define CMD_ERR_BCR				0x04	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ バーコード印字領域オーバーエラー*/
#define CMD_ERR_PDF				0x05	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ PDF417指定エラー*/
#define CMD_ERR_QR				0x06	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ QRコード指定エラー*/
#define CMD_ERR_CRC				0x10	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ CRCエラー*/

#define STS_ERR_NOERR				0x00	/*ｽﾃｰﾀｽ詳細 正常終了、受信待ち*/
#define STS_ERR_EDIT				0x01	/*ｽﾃｰﾀｽ詳細 解析・編集中、ハクリ待ち*/
#define STS_ERR_OFFLINE				0x02	/*ｽﾃｰﾀｽ詳細 オフライン*/
#define STS_ERR_COVER				0x03	/*ｽﾃｰﾀｽ詳細 カバーオープン*/
#define STS_ERR_PAPER				0x04	/*ｽﾃｰﾀｽ詳細 ペーパーエンド*/
#define STS_ERR_SENSOR				0x06	/*ｽﾃｰﾀｽ詳細 センサエラー*/
#define STS_ERR_HEAD				0x07	/*ｽﾃｰﾀｽ詳細 ヘッドエラー*/
#define STS_ERR_BAT				0x10	/*ｽﾃｰﾀｽ詳細 バッテリエラー*/
#define STS_ERR_OTHER				0x11	/*ｽﾃｰﾀｽ詳細 その他エラー*/


#define RETRY_CNT				50


extern int LapinBTOpen(char* btAddr, bool CRC_Check, int commandMode);
extern int LapinBTGetStat(void);
extern int LapinBTSend(char* data, int length);
extern int LapinBTDisconnect(void);
extern int PrintMain(char* addr, bool crc, int mode , short Flag );

#endif // __PT_SAMPLE_H__