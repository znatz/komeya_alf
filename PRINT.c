#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"
#include "CTRL.H"
#include "SUB.H"
#include "PRINT.h"
#include <Math.h>

static int commMode;
static bool isCrc;
void getStatus(void);

static long calculateTax(long lngPrice, short rate)
{
	float t = (float) rate;
	float result = round(lngPrice * rate / 100.0);
	// TODO
//	if ( ctrl.TaxType[0] == '2') result = lngPrice * rate / 100.0;
	return (long) result;
}

/********************************************************
 * CRC計算
 ********************************************************/
unsigned short getcrc16(unsigned char *str, short len)
{
	unsigned short	crc, p, b, r0, bcc, dat;
	unsigned short	sv_bcc, sv_dat, i, j;

	crc = 0xa001;
	bcc = 0;
	for (i = 0; i < len; i++)
	{
		dat = (short)str[i];
		for (j = 0; j < 8; j++)
		{
			sv_dat = dat;
			dat >>= 1;
			sv_bcc = bcc;
			bcc &= 0x7fff;
			bcc >>= 1;
			if (sv_bcc & 0x8000)
			{
				bcc |= 0x4000;
			}
			r0 = (short)(sv_bcc & 0x0001);
			b = (short)(sv_dat & 0x0001);
			p = (r0 ^ b);
			if (p == 0x0001)
			{
				bcc ^= crc;
			}
		}
	}

	return (bcc);
}

/********************************************************
 * Bluetooth取得
 ********************************************************/
int BtGet(unsigned char* buf, short len)
{
	int ret;
	unsigned short shRet;
	short recvLen = 0;
	short length;

	while (recvLen < len) {
		shRet = rgets(PORT_BLUETOOTH, &buf[recvLen], len - recvLen , &length);
		recvLen += length;

		if (shRet & 0xF800) {
			if (shRet & RSTAT_BIT_TIMEOUT) {
				ret = ERROR_TIMEOUT;
			} else if (shRet & RSTAT_BIT_BUFOVER) {
				ret = LAPIN_ERR_BUFF;
			} else if (shRet & RSTAT_BIT_NOT_CONNECT) {
				ret = LAPIN_ERR_CONNECT;
			} else if (shRet & RSTAT_BIT_BTERR) {
				ret = LAPIN_ERR_OTHER;
			} else {
				ret = LAPIN_ERR_OTHER;
			}
			break;
		} else {
			ret = LAPIN_OK;
		}
	}

	return ret;
}

/********************************************************
 * Bluetooth送信
 ********************************************************/
int BtPut(unsigned char* buf, short len)
{
	int ret;
	unsigned short shRet;

	shRet = rputs(PORT_BLUETOOTH, buf, len);

	if (shRet & 0xF800) {
		if (shRet & RSTAT_BIT_TIMEOUT) {
			ret = ERROR_TIMEOUT;
		} else if (shRet & RSTAT_BIT_BUFOVER) {
			ret = LAPIN_ERR_BUFF;
		} else if (shRet & RSTAT_BIT_NOT_CONNECT) {
			ret = LAPIN_ERR_CONNECT;
		} else if (shRet & RSTAT_BIT_BTERR) {
			ret = LAPIN_ERR_OTHER;
		}
	} else {
		ret = LAPIN_OK;
	}

	return ret;
}

/********************************************************
 * プリンタオープン
 ********************************************************/
int LapinBTOpen(char* btAddr, bool CRC_Check, int commandMode)
{
	int i;
	int ret;
	char bt_addr[20];

	/* initialize */
	ret = LAPIN_OK;

	//isCrc = CRC_Check;

	commMode = commandMode;
	if (commMode != COMMAND_MODE_SBPL && commMode != COMMAND_MODE_COMPAT) {
		ret = LAPIN_ERR_INIT;
		return ret;
	}

	memset(bt_addr, 0x00, sizeof(bt_addr));
	memcpy(bt_addr, btAddr, 12);

	if (strlen(bt_addr) != 12) {
		ret = LAPIN_ERR_INIT;
		return ret;
	}

	for (i = 0; i < 3; i++) {
		if (i > 0) {
			goto exit;
		}


		ropen( PORT_BLUETOOTH );
		//ret = BtConnect((unsigned char*)bt_addr, UUID_SPP);
		ret = EzBtConnect((unsigned char*)bt_addr, UUID_SPP);
		if (ret == BT_AT_NO_ANSWER) {
			continue;
		} else if (ret != 0) {
			goto exit;
		} else {
			break;
		}
	}

	rsettime(PORT_BLUETOOTH, 5);

	delay(50);

exit:
	if (ret != LAPIN_OK) {
		rclose(PORT_BLUETOOTH);
	}

	return ret;
}

/********************************************************
 * ステータス取得
 ********************************************************/
int LapinBTGetStat(void)
{
	static unsigned char	buff[16];
	unsigned short		crc;
	int 			ret = LAPIN_OK;

	/* ステータス要求コマンドを送信 */
	memset(buff, 0x00, sizeof(buff));


	/*SBPLモード*/ /*** ======================================================================= ***/
	if (commMode == COMMAND_MODE_SBPL) {
		strcpy( ( char* )buff, "\x02\x05\x03" ); /* STX ENQ ETX */

		if (isCrc) {
			crc = getcrc16(&buff[1], 2);
			buff[3] = ((0xFF00 & crc) >> 8);
			buff[4] = (0x00FF & crc);
			ret = BtPut((unsigned char *)buff, 5);
		} else {
			ret = BtPut((unsigned char *)buff, 3);
		}

		if (ret != LAPIN_OK) {
			ret = LAPIN_ERR_GETSTATUS;
			goto end;
		}

		/* ステータスを受信 */
		memset(buff, 0x00, sizeof(buff));
		ret = BtGet(buff, 11);

		if (ret != LAPIN_OK) {
			ret = LAPIN_ERR_GETSTATUS;
			goto end;
		}

		switch (buff[ 3 ]) {
		case OFF_NOERR:
		case OFF_BAT_LOW:
		case OFF_LOW_FULL:
		case OFF_BUFF_FULL:
			ret = LAPIN_ERR_OFFLINE;
			break;

		case ON_EDIT_NOERR:
		case ON_PRT_NOERR:
		case ON_STANDBY_NOERR:
		case ON_WAIT_NOERR:
			ret = LAPIN_OK;
			break;

		case ON_WAIT_BAT_LOW:
			ret = LAPIN_BAT_LOW;
			break;
		case ON_WAIT_BUFF_FULL:
			ret = LAPIN_BUFF_FULL;
			break;
		case ON_WAIT_LOW_FULL:
			ret = LAPIN_LOW_FULL;
			break;

		case ON_PRT_BAT_LOW:
		case ON_PRT_BUFF_FULL:
		case ON_PRT_LOW_FULL:
			ret = LAPIN_PRINTING;
			break;

		case ON_STANDBY_BAT_LOW:
		case ON_STANDBY_BUFF_FULL:
		case ON_STANDBY_LOW_FULL:
			ret = LAPIN_STANDBY;
			break;


		case ON_EDIT_BAT_LOW:
		case ON_EDIT_BUFF_FULL:
		case ON_EDIT_LOW_FULL:
			ret = LAPIN_EDIT;
			break;

		case ERR_BUFF:
			ret = LAPIN_ERR_BUFF;
			break;

		case ERR_PAPER:
			ret = LAPIN_ERR_PAPER;
			break;

		case ERR_BAT:
			ret = LAPIN_ERR_BAT;
			break;

		case ERR_SENSOR:
			ret = LAPIN_ERR_SENSOR;
			break;

		case ERR_HEAD:
			ret = LAPIN_ERR_HEAD;
			break;

		case ERR_COVER:
			ret = LAPIN_ERR_COVER;
			break;

		case ERR_OTHER:
		default:
			ret = LAPIN_ERR_OTHER;
			break;
		}

		/*互換モード*/ /*** ======================================================================= ***/
	} else {
		strcpy((char*)buff, "\x02\x1b\x41\x1b\x45\x51\x1b\x5A\x03"); /* STX ESC A ESC E Q ESC Z ETX */

		if (isCrc) {
			memcpy(&buff[9], "\xC5\xC5", 2); /*このCRCはC5C5*/
			ret = BtPut(buff, 11);

		} else {
			ret = BtPut(buff, 9);
		}

		if (ret != LAPIN_OK) {
			ret = LAPIN_ERR_GETSTATUS;
			goto end;
		}

		/* ステータスを受信 */
		memset(buff, 0x00, sizeof(buff));
		ret = BtGet(buff, 8);
		if (ret != LAPIN_OK) {
			ret = LAPIN_ERR_GETSTATUS;
			goto end;
		}

		if (buff[0] == NAK) {
			ret = LAPIN_ERR_COMM;
			goto end;
		}

		switch (buff[1]) { /*コマンドステータス*/
		case CMD_ERR_NOERR:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ 正常終了*/
			ret = LAPIN_OK;
			break;
		case CMD_ERR:		/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ コマンド指定エラー*/
		case CMD_ERR_PARAM:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ パラメータ指定エラー*/
		case CMD_ERR_POS:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ 印字位置指定エラー*/
		case CMD_ERR_BCR:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ バーコード印字領域オーバーエラー*/
		case CMD_ERR_PDF:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ PDF417指定エラー*/
		case CMD_ERR_QR:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ QRコード指定エラー*/
		case CMD_ERR_CRC:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ CRCエラー*/
			ret = LAPIN_ERR_COMMAND;
			break;
		default:
			ret = LAPIN_ERR_OTHER;
			break;
		}

		if (ret != LAPIN_OK) {
			goto end;
		}

		switch (buff[5]) { /*プリンタステータス*/
		case STS_ERR_NOERR:		/*ｽﾃｰﾀｽ詳細 正常終了、受信待ち*/
			ret = LAPIN_OK;
			break;
		case STS_ERR_EDIT:		/*ｽﾃｰﾀｽ詳細 解析・編集中、ハクリ待ち*/
			ret = LAPIN_EDIT;
			break;
		case STS_ERR_OFFLINE:	/*ｽﾃｰﾀｽ詳細 オフライン*/
			ret = LAPIN_ERR_OFFLINE;
			break;
		case STS_ERR_COVER:		/*ｽﾃｰﾀｽ詳細 カバーオープン*/
			ret = LAPIN_ERR_COVER;
			break;
		case STS_ERR_PAPER:		/*ｽﾃｰﾀｽ詳細 ペーパーエンド*/
			ret = LAPIN_ERR_PAPER;
			break;
		case STS_ERR_SENSOR:	/*ｽﾃｰﾀｽ詳細 センサエラー*/
			ret = LAPIN_ERR_SENSOR;
			break;
		case STS_ERR_HEAD:		/*ｽﾃｰﾀｽ詳細 ヘッドエラー*/
			ret = LAPIN_ERR_HEAD;
			break;
		case STS_ERR_BAT:		/*ｽﾃｰﾀｽ詳細 バッテリエラー*/
			ret = LAPIN_ERR_BAT;
			break;
		case STS_ERR_OTHER:		/*ｽﾃｰﾀｽ詳細 その他エラー*/
		default:
			ret = LAPIN_ERR_OTHER;
			break;
		}
	} /*互換モード*/

end:
	/* 受信バッファをクリア */
	rxbclr(PORT_BLUETOOTH);
	delay(10);

	return ret;
}

/********************************************************
 * コマンド送信
 ********************************************************/
int LapinBTSend(char* data, int length)
{
	unsigned char	buff[2048];
	short		retry_cnt;
	short		state_cnt;
	unsigned short	crc;
	int		ret;
	char printflg = 0;


	/* 互換モード */
	if (commMode == COMMAND_MODE_COMPAT) {
		if (strstr(data, "\x1BQ") != NULL) {
			printflg = 1;
		} else {
			printflg = 0;
		}
	}

	if (length > sizeof(buff) + 4) { /* data size + STX + ETX + CRC */
		return LAPIN_ERR_BUFF;
	}

	/* ステータス確認 */
	retry_cnt = 0;
	state_cnt = 0;
	while (1) {
		ret = LapinBTGetStat();

		/* 印刷中、解析中 */
		if (ret == LAPIN_PRINTING || ret == LAPIN_EDIT) {
			delay(10);
			retry_cnt++;
			if ( retry_cnt >= RETRY_CNT )
			{
				break;
			}
			continue;
		}
		if (ret == LAPIN_ERR_GETSTATUS) {
			delay(10);
			state_cnt++;
			if (state_cnt >= 3) {
				break;
			}
			continue;
		}

		/* 印刷成功系 */
		if (ret == LAPIN_OK || ret == LAPIN_BAT_LOW ||
		        ret == LAPIN_BUFF_FULL || ret == LAPIN_LOW_FULL || ret == LAPIN_STANDBY ) {

			ret = LAPIN_OK;
			break;
		}

		break;
	}

	if (ret != LAPIN_OK) {
		return ret;
	}

	retry_cnt = 0;

	/* STX ETXを付加 */
	memset(buff, 0x00, sizeof(buff));
	buff[0] = STX;
	memcpy(&buff[1], data, length);
	buff[1 + length] = ETX;							/* STX + length 				*/

	if (isCrc) {
		crc = getcrc16(&buff[1], length + 1);		/* data~ETXまでが計算範囲 	*/
		buff[length + 2] = ((0xFF00 & crc) >> 8);
		buff[length + 3] = (0x00FF & crc);

		ret = BtPut(buff, length + 4) ;				/* STX+ETX+CRC+length 		*/
	} else {
		ret = BtPut(buff, length + 2) ;				/* STX+ETX+length 		*/
	}

	if (ret != LAPIN_OK) {
		ret = LAPIN_ERR_COMM;
		goto end;
	}

	memset(buff, 0x00, sizeof(buff));

	/*SBPLモード*/
	if (commMode == COMMAND_MODE_SBPL) {
		ret = BtGet(buff, 1);

		if (ret == LAPIN_OK) {
			if (buff[0] == ACK) {
				ret = LAPIN_OK;

			} else if (buff[0] == NAK) {
				ret = LAPIN_ERR_COMM;

			} else {
				ret = LAPIN_ERR_COMM;
			}
		}

	} else {
		ret = BtGet( buff, 2);
		if (ret == LAPIN_OK) {
			if (buff[0] == ACK) {
				ret = LAPIN_OK;

			} else if (buff[0] == NAK) {
				ret = LAPIN_ERR_COMM;

			} else {
				ret = LAPIN_ERR_COMM;
			}
		} else {
			ret = LAPIN_ERR_COMM;
		}

		if (ret != LAPIN_OK) {
			goto end;
		}

		switch (buff[1]) { /*コマンドステータス*/
		case CMD_ERR_NOERR:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ 正常終了*/
			ret = LAPIN_OK;
			break;
		case CMD_ERR:		/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ コマンド指定エラー*/
		case CMD_ERR_PARAM:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ パラメータ指定エラー*/
		case CMD_ERR_POS:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ 印字位置指定エラー*/
		case CMD_ERR_BCR:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ バーコード印字領域オーバーエラー*/
		case CMD_ERR_PDF:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ PDF417指定エラー*/
		case CMD_ERR_QR:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ QRコード指定エラー*/
		case CMD_ERR_CRC:	/*ｺﾏﾝﾄﾞｽﾃｰﾀｽ CRCエラー*/
			ret = LAPIN_ERR_COMMAND;
			ret = buff[0] + 100;
			break;
		default:
			ret = LAPIN_ERR_OTHER;
			break;
		}

		if (ret != LAPIN_OK) {
			goto end;
		}

		if (printflg) {
			memset(buff, 0x00, sizeof(buff));
			ret = BtGet(buff, 6);

			if (ret != LAPIN_OK) {
				goto end;
			}
			switch (buff[3]) { /*プリンタステータス*/
			case STS_ERR_NOERR:		/*ｽﾃｰﾀｽ詳細 正常終了、受信待ち*/
				ret = LAPIN_OK;
				break;
			case STS_ERR_EDIT:		/*ｽﾃｰﾀｽ詳細 解析・編集中、ハクリ待ち*/
				ret = LAPIN_EDIT;
				break;
			case STS_ERR_OFFLINE:	/*ｽﾃｰﾀｽ詳細 オフライン*/
				ret = LAPIN_ERR_OFFLINE;
				break;
			case STS_ERR_COVER:		/*ｽﾃｰﾀｽ詳細 カバーオープン*/
				ret = LAPIN_ERR_COVER;
				break;
			case STS_ERR_PAPER:		/*ｽﾃｰﾀｽ詳細 ペーパーエンド*/
				ret = LAPIN_ERR_PAPER;
				break;
			case STS_ERR_SENSOR:	/*ｽﾃｰﾀｽ詳細 センサエラー*/
				ret = LAPIN_ERR_SENSOR;
				break;
			case STS_ERR_HEAD:		/*ｽﾃｰﾀｽ詳細 ヘッドエラー*/
				ret = LAPIN_ERR_HEAD;
				break;
			case STS_ERR_BAT:		/*ｽﾃｰﾀｽ詳細 バッテリエラー*/
				ret = LAPIN_ERR_BAT;
				break;
			case STS_ERR_OTHER:		/*ｽﾃｰﾀｽ詳細 その他エラー*/
			default:
				ret = LAPIN_ERR_OTHER;
			}

		}

	}

end:

	/* 受信バッファをクリア */
	rxbclr(PORT_BLUETOOTH);
	delay(10);

	return ret;
}

/********************************************************
 * 切断
 ********************************************************/
int LapinBTDisconnect(void)
{
	int ret = LAPIN_OK;
	int retry_cnt;

	if (commMode == COMMAND_MODE_SBPL) {
		/* ステータス確認 */
		retry_cnt = 0;
		while (1) {
			ret = LapinBTGetStat();

			/* 印刷中、解析中 */
			if (ret == LAPIN_PRINTING || ret == LAPIN_EDIT) {
				delay(10);
				retry_cnt++;
				if (retry_cnt >= RETRY_CNT) {
					break;
				}
				continue;
			}

			/* 印刷成功 */
			if (ret == LAPIN_OK || ret == LAPIN_BAT_LOW ||
			        ret == LAPIN_BUFF_FULL || ret == LAPIN_LOW_FULL || ret == LAPIN_STANDBY ) {

				ret = LAPIN_OK;
				break;
			}
			break;
		}
	}

	BtDisconnect();
	delay(100);
	rclose(PORT_BLUETOOTH);
	return ret;

}

/********************************************************
 * プリンタデータ送信
 ********************************************************/
int PrintMain(char* addr, bool crc, int mode , short Flag )
{
	int ret;
	char buf [2048];
	short len;
	int i;
	int intMsg;
	char now[14], Year[4], Month[2], Day[2];
	char Hour[2], Min[2];
	char Name[20];
	char RecNo[6];
	char strKingaku[MAXLEN];
	long lngNum = 0, lngBaika = 0, lngTensuu = 0;
	long lngKingaku = 0 , lngZei = 0 , lngZeiGoukei = 0;
	double dblZei = 0 , dblTaxRate = 0 , dblNum = 0 , dblNo = 0 ;

	short taxrate = atoin( ctrl.TaxRate, sizeof(ctrl.TaxRate)) + 100.0;
	long baika;

	progress_start(LCD_PROGRESS_USE_ANIMATION, "印刷中");
	progress_update(NULL, NULL, 0);

	//if( Flag == 0 ){//#01 日計表
	ret = LapinBTOpen(addr, crc, mode);
	if (ret != LAPIN_OK) {
		goto end;
	}
	//}

	// 初期化コマンドはプリンタコマンド表を元に適宜設定すること。
	if ( mode == COMMAND_MODE_SBPL ) {

		memset(buf, 0x00, sizeof(buf));
		len = 0;
		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// データ送出開始
		memcpy( &buf[ len ], "A", 1 );					len += 1;

		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// プリンタ動作設定
		memcpy( &buf[ len ], "PG", 2);					len += 2;
		memcpy( &buf[ len ], "3", 1);					len += 1;	// 印字速度 50mm/sec
		memcpy( &buf[ len ], "2", 1);					len += 1;	// 印字濃度レベル 中薄
		memcpy( &buf[ len ], "A", 1);					len += 1;	// 印字濃度 A
		memcpy( &buf[ len ], "3", 1);					len += 1;	// バックフィード無し
		memcpy( &buf[ len ], "0", 1);					len += 1;	// ゼロスラッシュ無効
		memcpy( &buf[ len ], "1", 1);					len += 1;	// 漢字コード シフトJIS
		memcpy( &buf[ len ], "0", 1);					len += 1;	// 初期フィード無効
		memcpy( &buf[ len ], "0", 1);					len += 1;	// プロポーショナルピッチ解除
		memcpy( &buf[ len ], "2400", 4);				len += 4;	// 縦ラベルサイズ
		memcpy( &buf[ len ], "0832", 4);				len += 4;	// 横ラベルサイズ
		memcpy( &buf[ len ], "+000", 4);				len += 4;	// 縦基点補正値
		memcpy( &buf[ len ], "+000", 4);				len += 4;	// 横基点補正値
		memcpy( &buf[ len ], "+00", 3);					len += 3;	// 用紙ピッチオフセット
		memcpy( &buf[ len ], "+00", 3);					len += 3;	// ティアオフオフセット
		memcpy( &buf[ len ], "+00", 3);					len += 3;	// ハクリオフセット
		memcpy( &buf[ len ], "000", 3);					len += 3;	// オートパワーオフ無し
		memcpy( &buf[ len ], "1", 1);					len += 1;	// ジャーナル発行モード
		memcpy( &buf[ len ], "1", 1);					len += 1;	// 動作モード 連続
		memcpy( &buf[ len ], "1", 1);					len += 1;	// センサ種指定 透過式センサ

		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// データ送出終了
		memcpy( &buf[ len ], "Z", 1 );					len += 1;

	} else {
		/*プリンタ設定*/
		memset(buf, 0x00, sizeof(buf));
		/*データ送信開始指定*/
		len = 0;
		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	/* データ送出開始				*/
		memcpy( &buf[ len ], "A", 1 );					len += 1;
		/*自動改行設定*/
		/*文字フィールド指定*/
		buf[len++] = 0x1b;
		//abcdefghiiiijjjjkkkkllllmmmnnnooo
		memcpy( &buf[len], "PG43A3010012800384+000+000+00+00+00", 35 );
		len += 35;
		memcpy( &buf[len], "000", 3);
		len += 3;
		memcpy( &buf[len], "111", 3 );					//	ジャーナル発行モード
		len += 3;

		/*データ送信終了指定*/
		buf[len++] = 0x1b;
		buf[len++] = 'Z';
	}

	// 初期化コマンド送信

	ret = LapinBTSend(buf, len);
	if (ret != LAPIN_OK) {
		goto end;
	}

	//ロゴ印字部分
	memset(buf, 0x00, sizeof(buf));
	len = 0;
	buf[len++] = 0x1b;
	buf[len++] = 'A';
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "V0", 4 ); len += 4;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "H0", 4 ); len += 4;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "L0101", 5 ); len += 5;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "RF010002,", 9 ); len += 9;
	buf[len++] = 0x6b;
	buf[len++] = 0x82;
	buf[len++] = 0x1b;
	buf[len++] = 'Z';
	ret = LapinBTSend(buf, len);

	// 印字コマンドは印字内容によって適宜設定すること
	if (mode == COMMAND_MODE_SBPL) {
		memset(buf, 0x00, sizeof(buf));
		len = 0;
		buf[len++] = 0x1b;
		buf[len++] = 'A';
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E2", 2 );					len += 2;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "K9B", 3 );				len += 3;

	} else {
		memset(buf, 0x00, sizeof(buf));
		len = 0;
		buf[len++] = 0x1b;
		buf[len++] = 'A';
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E001", 4 );				len += 4;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E2", 2 );					len += 2;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "L0001000108111000", 17 );	len += 17;
		buf[len++] = 0x1b;
		buf[len++] = 'D';
	}


	if ( Flag == 0 ) { //日計表
		memcpy( &buf[ len ], "     ＜＜ 日 計 表 ＞＞\r", 24 ); len += 24;
		getrtc4( now );
		memcpy( Year,  &now[0], 4 );
		memcpy( Month, &now[4], 2 );
		memcpy( Day,   &now[6], 2 );
		memcpy( Hour,  &now[8], 2 );
		memcpy( Min,   &now[10], 2 );

		sprintf(&buf[len], "%s", Year ); len += sizeof( Year );
		memcpy( &buf[ len ], "/", 1 ); len += 1;
		sprintf(&buf[len], "%s", Month); len += sizeof( Month );
		memcpy( &buf[ len ], "/", 1 ); len += 1;
		sprintf(&buf[len], "%s", Day  ); len += sizeof( Day  );
		memcpy( &buf[ len ], " ", 1 ); len += 1;
		sprintf(&buf[len], "%s", Hour ); len += sizeof( Hour );
		memcpy( &buf[ len ], ":", 1 ); len += 1;
		sprintf(&buf[len], "%s", Min  ); len += sizeof( Min  );
		memcpy( &buf[ len ], "  No.", 4 ); len += 4;

		sprintf(&buf[ len ], "%s", ctrl.RejiNo ); len += sizeof( ctrl.RejiNo );
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		int intCnt;
		long lngNum = 0, lngKingaku = 0, lngKyakuNo = 0;
		long lngGenkin = 0, lngCredit = 0, lngKinken = 0, lngCoupon = 0;

		long lngBaikaWithTax = 0;
		long lngTotalWithTax = 0;
		//税率設定
		dblTaxRate = atofn( ctrl.TaxRate, sizeof( ctrl.TaxRate ) ) / 100 ;

		for ( intCnt = 0; intCnt < ctrl.URDataCnt; intCnt++) {
			ram_read( intCnt , &urdata, URDATAF );
			lngNum    = lngNum + atoln( urdata.Num, sizeof( urdata.Num ) );
			lngKingaku = lngKingaku + atoln( urdata.Num, sizeof( urdata.Num )) *  atoln( urdata.Baika, sizeof( urdata.Baika ));

			long jyoudai = atoln( urdata.Baika, sizeof( urdata.Baika ));
			long num = atoln( urdata.Num, sizeof( urdata.Num )) ;
			lngBaikaWithTax = calculateTax(jyoudai, taxrate) * num;
			lngTotalWithTax += lngBaikaWithTax;
			//ZNATZ

			lngKyakuNo = lngKyakuNo + atoln( urdata.KyakuNo, sizeof( urdata.KyakuNo ) );
			lngGenkin = lngGenkin + atoln( urdata.Genkin, sizeof( urdata.Genkin ) ) - urdata.lngOturi;
			lngCredit = lngCredit + atoln( urdata.Credit, sizeof( urdata.Credit ) );
			lngKinken = lngKinken + atoln( urdata.Kinken, sizeof( urdata.Kinken ) );
			lngCoupon = lngCoupon + atoln( urdata.Coupon, sizeof( urdata.Coupon ) );


			//消費税計算
			dblZei = atoln( urdata.Baika, sizeof( urdata.Baika ));
			//lngZei = dblZei - dblZei * ( 1 / ( 1.0  + dblTaxRate ) ) ; ZNATZ
			//lngZei = calculateTax(dblZei,taxrate);
			lngZeiGoukei = lngZeiGoukei + ( lngZei * atoln( urdata.Num, sizeof( urdata.Num )) );

		}
		lngZei = lngTotalWithTax - lngKingaku;

		memcpy( &buf[ len ], "売上点数　       ", 17 ); len += 17;
		insComma( lngNum  , strKingaku );
		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		memcpy( &buf[ len ], "売上金額　       ", 17 ); len += 17;
		insComma( lngKingaku , strKingaku );
		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		memcpy( &buf[ len ], "消費税　　       ", 17 ); len += 17;
		insComma( lngZei , strKingaku );
		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		memcpy( &buf[ len ], "税込金額　       ", 17 ); len += 17;
		insComma( lngTotalWithTax  , strKingaku );
		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		memcpy( &buf[ len ], "----------------------------\r", 29 ); len += 29;
		memcpy( &buf[ len ], "クレジット       ", 17 ); len += 17;
		insComma( lngCredit , strKingaku );
		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

//		memcpy( &buf[ len ], "金券　　　       ", 17 ); len += 17;
//		insComma( lngKinken ,strKingaku );
//		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
//		memcpy( &buf[ len ], "\r", 1 ); len += 1;
//
//		memcpy( &buf[ len ], "商品券　　       ", 17 ); len += 17;
//		insComma( lngCoupon ,strKingaku );
//		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
//		memcpy( &buf[ len ], "\r", 1 ); len += 1;


		memcpy( &buf[ len ], "現金在高　       ", 17 ); len += 17;
		insComma( lngGenkin , strKingaku );
		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		memcpy( &buf[ len ], "----------------------------\r", 29 ); len += 29;


		memcpy( &buf[ len ], "客数　　　       ", 17 ); len += 17;
		insComma( lngKyakuNo , strKingaku );
		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;


		memcpy( &buf[ len ], "客点数　　       ", 17 ); len += 17;
		dblNum = lngNum;
		dblNo  = lngKyakuNo;
		dblNum = dblNum / dblNo;
		sprintf( &buf[ len ], "%10.1f", dblNum ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		memcpy( &buf[ len ], "客単価　　       ", 17 ); len += 17;
		lngKingaku = lngKingaku / lngKyakuNo;
		insComma( lngKingaku , strKingaku );
		sprintf( &buf[ len ], "%10s", strKingaku ); len += 10;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;

	} else if ( Flag == 1 ) { //レシート印刷
		if ( memcmp( info.tanto, "00", 2 ) == 0 ) {
			memcpy( &buf[ len ], "     ＜＜ 練 習 用 ＞＞\r", 24 ); len += 24;
		} else {
			//memcpy( &buf[ len ], "     ＜＜ レシート ＞＞\r", 24 );len += 24;
		}
//		memcpy( &buf[ len ], "TEL:053-487-0511", 16 );len += 16;
//		memcpy( &buf[ len ], "  ",2 ); len += 2;
//		sprintf(&buf[ len ], "%s", info.tenpo1 ); len += sizeof( info.tenpo1 );
//		memcpy( &buf[ len ], "-", 1 ); len += 1;
//		sprintf(&buf[ len ], "%s", ctrl.RejiNo ); len += sizeof( ctrl.RejiNo );
//		memcpy( &buf[ len ], "-", 1 ); len += 1;
//		sprintf(&buf[ len ], "%s", info.tanto ); len += sizeof( info.tanto );
//		memcpy( &buf[ len ], "\r",1 ); len += 1;

		getrtc4( now );
		memcpy( Year,  &now[0], 4 );
		memcpy( Month, &now[4], 2 );
		memcpy( Day,   &now[6], 2 );
		memcpy( Hour,  &now[8], 2 );
		memcpy( Min,   &now[10], 2 );

//		ram_read( 0, &infour2, INFOURF2 );
//		memcpy( &Year, 	infour2.Date, 4);		// ZNATZ INSERT Year
//		memcpy( &Month, &infour2.Date[4], 2);		// ZNATZ INSERT MONTH
//		memcpy( &Day, 	&infour2.Date[6] , 2);		// ZNATZ INSERT Day
//		memcpy( &Hour, 	&infour2.Time[0] , 2);		// ZNATZ INSERT Day
//		memcpy( &Min, 	&infour2.Time[2] , 2);		// ZNATZ INSERT Day

		sprintf(&buf[len], "%s", Year ); len += sizeof( Year );
		memcpy( &buf[ len ], "/", 1 ); len += 1;
		sprintf(&buf[len], "%s", Month); len += sizeof( Month );
		memcpy( &buf[ len ], "/", 1 ); len += 1;
		sprintf(&buf[len], "%s", Day  ); len += sizeof( Day  );
		memcpy( &buf[ len ], " ", 1 ); len += 1;
		sprintf(&buf[len], "%s", Hour ); len += sizeof( Hour );
		memcpy( &buf[ len ], ":", 1 ); len += 1;
		sprintf(&buf[len], "%s", Min  ); len += sizeof( Min  );

		memcpy( &buf[ len ], "  ", 2 ); len += 2;
		sprintf(&buf[ len ], "%s", info.tenpo1 ); len += sizeof( info.tenpo1 );
		memcpy( &buf[ len ], "-", 1 ); len += 1;
		sprintf(&buf[ len ], "%s", ctrl.RejiNo ); len += sizeof( ctrl.RejiNo );
		memcpy( &buf[ len ], "-", 1 ); len += 1;
		sprintf(&buf[ len ], "%s", info.tanto ); len += sizeof( info.tanto );
		memcpy( &buf[ len ], "\r", 1 ); len += 1;



		memcpy( &buf[ len ], " 品名          数量   売価  \r", 29 ); len += 29;
		buf[len++] = 0x1b; memcpy( &buf[ len ], "L0102", 5 ); len += 5;
		buf[len++] = 0x1b; memcpy( &buf[ len ], "K9B", 3 ); len += 3;
		memcpy( &buf[ len ], "----------------------------\r", 29 ); len += 29;
		buf[len++] = 0x1b; memcpy( &buf[ len ], "L0101", 5 ); len += 5;
		buf[len++] = 0x1b; memcpy( &buf[ len ], "K9B", 3 ); len += 3;

		//消費税率計算
		dblTaxRate = atofn( ctrl.TaxRate, sizeof( ctrl.TaxRate ) );

		for ( i = 0; i < ctrl.InfoUrCnt; i++) {
			//ram_read( i, &infour, INFOURF );
			ram_read( i, &infour, INFOURF );
			memcpy( Name , infour.Name , sizeof( Name) );
			lngNum = atoln( infour.Num, sizeof( infour.Num )) ;
			lngBaika = atoln( infour.Baika, sizeof( infour.Baika ));

			sprintf(&buf[ len ], "%s", Name); len += sizeof( Name );
			memcpy( &buf[ len ], "\r", 1 ); len += 1;
			memcpy( &buf[ len ], "              ", 14 ); len += 14;
			sprintf(&buf[ len ], "%4d", lngNum ); len += 4;
			memcpy( &buf[ len ], " ", 1 ); len += 1;
			//ZNATZ
			dblZei = lngBaika;
			lngBaika = lngBaika + calculateTax(lngBaika, dblTaxRate);
			insComma( lngBaika * lngNum , strKingaku );
			sprintf(&buf[ len ], "%7s", strKingaku ); len += 7;
			memcpy( &buf[ len ], "\r", 1 ); len += 1;

			memcpy( Name , 0x00 , sizeof( Name ) );
			lngTensuu = lngTensuu + lngNum;

			//消費税計算

			//lngZei = dblZei - dblZei * ( 1 / ( 1.0  + ( dblTaxRate/100 ) ) ) ;　ZNATZ
			lngZei = calculateTax(dblZei, dblTaxRate);
			lngZeiGoukei = lngZeiGoukei + ( lngZei * lngNum );
		}
		ram_read( 0, &infour2, INFOURF2 );

		buf[len++] = 0x1b; memcpy( &buf[ len ], "L0102", 5 ); len += 5;
		buf[len++] = 0x1b; memcpy( &buf[ len ], "K9B", 3 ); len += 3;
		memcpy( &buf[ len ], "----------------------------\r", 29 ); len += 29;
		buf[len++] = 0x1b; memcpy( &buf[ len ], "L0101", 5 ); len += 5;
		buf[len++] = 0x1b; memcpy( &buf[ len ], "K9B", 3 ); len += 3;

		memcpy( &buf[ len ], "税込計　　          ", 20 ); len += 20;
		insComma( infour2.Goukei , strKingaku );
		sprintf(&buf[ len ], "%7s", strKingaku ); len += 7;
		//sprintf(&buf[ len ], "%7d", infour2.Goukei ); len += 7;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;


		memcpy( &buf[ len ], "（内消費税  ", 12 ); len += 12;
		sprintf(&buf[ len ], "%2.1f％ ", dblTaxRate ); len += 7;
		insComma( lngZeiGoukei , strKingaku );
		sprintf(&buf[ len ], "%7s", strKingaku ); len += 7;
		//sprintf(&buf[ len ], "%7d", lngKingaku ); len += 7;
		memcpy( &buf[ len ], "）", 2 ); len += 2;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		lngKingaku =  atoln( infour2.Credit, sizeof( infour2.Credit ));
		if (lngKingaku != 0) {
			memcpy( &buf[ len ], "クレジット          ", 20 ); len += 20;
			insComma( lngKingaku , strKingaku );
			sprintf(&buf[ len ], "%7s",  strKingaku ); len += 7;
			//sprintf(&buf[ len ], "%7d", lngKingaku ); len += 7;
			memcpy( &buf[ len ], "\r", 1 ); len += 1;
		}
		lngKingaku =  atoln( infour2.Kinken, sizeof( infour2.Kinken ));
		if (lngKingaku != 0) {
			memcpy( &buf[ len ], "クレジット　          ", 20 ); len += 20;
			insComma( lngKingaku , strKingaku );
			sprintf(&buf[ len ], "%7s",  strKingaku ); len += 7;
			//sprintf(&buf[ len ], "%7d",  lngKingaku ); len += 7;
			memcpy( &buf[ len ], "\r", 1 ); len += 1;
		}
		lngKingaku =  atoln( infour2.Coupon, sizeof( infour2.Coupon ));
		if (lngKingaku != 0) {
			memcpy( &buf[ len ], "商品券　　          ", 20 ); len += 20;
			insComma( lngKingaku , strKingaku );
			sprintf(&buf[ len ], "%7s",  strKingaku ); len += 7;
			//sprintf(&buf[ len ], "%7d", lngKingaku ); len += 7;
			memcpy( &buf[ len ], "\r", 1 ); len += 1;
		}
		lngKingaku =  atoln( infour2.Genkin, sizeof( infour2.Genkin ));
		if (lngKingaku != 0) {
			memcpy( &buf[ len ], "お預り　　          ", 20 ); len += 20;
			insComma( lngKingaku , strKingaku );
			sprintf(&buf[ len ], "%7s", strKingaku ); len += 7;
			//sprintf(&buf[ len ], "%7d", lngKingaku ); len += 7;
			memcpy( &buf[ len ], "\r", 1 ); len += 1;
		}
		buf[len++] = 0x1b;
		buf[len++] = 'Z';
		ret = LapinBTSend(buf, len);


		memset(buf, 0x00, sizeof(buf));
		len = 0;
		buf[len++] = 0x1b;
		buf[len++] = 'A';
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "PC0", 3 );	len += 3;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E2", 2 );		len += 2;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "L0102", 5 );	len += 5;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "K9B", 3 );	len += 3;
		memcpy( &buf[ len ], "お釣り　　          ", 20 ); len += 20;
		//memcpy( &buf[ len ], "お釣り ", 7 ); len += 7;
		insComma( infour2.Oturi , strKingaku );
		//sprintf(&buf[ len ], "%7s", strKingaku ); len += 7;
		sprintf(&buf[ len ], "%7s", strKingaku ); len += 7;

		memcpy( &buf[ len ], "\r", 1 ); len += 1;
		buf[len++] = 0x1b;
		buf[len++] = 'Z';
		ret = LapinBTSend(buf, len);

		memset(buf, 0x00, sizeof(buf));
		len = 0;
		buf[len++] = 0x1b;
		buf[len++] = 'A';
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E2", 2 );					len += 2;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "K9B", 3 );				len += 3;
		//担当名
		memcpy( &buf[ len ], "担当:", 5 ); len += 5;
		TantoFind( info.tanto );
//		sprintf(&buf[ len ], "%s", tmst.Name ); len += sizeof( tmst.Name );
		memcpy( &buf[ len ], "\r", 1 ); len += 1;
		//ﾚｼｰﾄNo
		sprintf( RecNo, "%06d", ctrl.RecNo);
		memcpy( &buf[ len ], "ﾚｼｰﾄNO:", 7 ); len += 7;
		sprintf(&buf[ len ], "%s", RecNo ); len += sizeof( RecNo );
		memcpy( &buf[ len ], "   お買上:", 10 ); len += 10;
		sprintf(&buf[ len ], "%d点", lngTensuu ); len += sizeof( lngTensuu + 2 );
		memcpy( &buf[ len ], "\r", 1 ); len += 1;


		memcpy( &buf[ len ], "\r", 1 ); len += 1;

//		for ( i = 0; i < ctrl.ReasonCnt; i++) {
//			ram_read( i, &reason, REASONF );
//			sprintf(&buf[ len ], "%30s", reason.Name ); len += 30;
//			memcpy( &buf[ len ], "\r", 1 ); len += 1;
//		}
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
	}

	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "RF010002", 4 );
	len += 4;

	// データ送出終了
	if (mode == COMMAND_MODE_SBPL) {
		buf[len++] = 0x1b;
		buf[len++] = 'Z';

		ret = LapinBTSend(buf, len);
		if (ret != LAPIN_OK) {
			goto end;
		}
	} else {
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "Q001", 4 );					len += 4;
		buf[len++] = 0x1b;
		buf[len++] = 'Z';

		ret = LapinBTSend(buf, len);
		if (ret != LAPIN_OK) {
			goto end;
		}
	}


end:
	if (ret != LAPIN_OK) {
		LapinBTDisconnect();
		progress_stop();

		memset(buf, 0x00, sizeof(buf));
		if ( ret == -104) {
			sprintf(buf, "%s", "ﾌﾟﾘﾝﾀの電源を確認して下さい" );
		} else if ( ret == -24 ) {
			sprintf(buf, "%s\n%s", "用紙チェック", "ﾌﾟﾘﾝﾀの再起動" );
		} else if ( ret == -21 ) {
			sprintf(buf, "%s", "ﾌﾟﾘﾝﾀのｶﾊﾞｰを確認して下さい" );
		} else {
			sprintf(buf, "[%d]", ret);
		}
		intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "エラー\n", buf, "再印刷", "終了");
	} else {
		ret = LapinBTDisconnect();
		progress_stop();

		if (ret != LAPIN_OK) {
			memset(buf, 0x00, sizeof(buf));
			sprintf(buf, "Disconnect %d", ret);
			intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, "再印刷", "終了");
		} else {
			msgbox(LCD_MSGBOX_INFORMATION, 0, "正常終了", NULL, NULL, NULL);
			intMsg = 1;
		}
	}
	return intMsg;
}






/********************************************************
 * 領収書印刷
 ********************************************************/
int PrintRyousyu(char* addr, bool crc, int mode , short Flag )
{
	int ret;
	char buf [2048];
	short len;
	int i;
	int intMsg;
	char now[14], Year[4], Month[2], Day[2];
	char Hour[2], Min[2];
	char Name[20];
	char RecNo[6];
	char strKingaku[MAXLEN];
	long lngNum = 0, lngBaika = 0, lngTensuu = 0;
	long lngKingaku = 0 , lngZei = 0 , lngZeiGoukei = 0;
	double dblZei = 0 , dblTaxRate = 0 , dblNum = 0 , dblNo = 0 ;

	long adrs;

	char title[24] =  "     ＜＜ 領 収 書 ＞＞\r";    	// |
	char kyaku[29] =  "                        様  \r";	// |
	char aline[29] =  "----------------------------\r";	// |
	char empty[29] =  "                            \r";	// |

	insComma( infour2.Goukei , strKingaku );
	char total[29];
	sprintf(&total[ 0 ], "             ￥%7s-   ", strKingaku );


	progress_start(LCD_PROGRESS_USE_ANIMATION, "印刷中");
	progress_update(NULL, NULL, 0);

	//if( Flag == 0 ){//#01 日計表
	ret = LapinBTOpen(addr, crc, mode);
	if (ret != LAPIN_OK) {
		goto end;
	}
	//}

	// 初期化コマンドはプリンタコマンド表を元に適宜設定すること。
	if ( mode == COMMAND_MODE_SBPL ) {

		memset(buf, 0x00, sizeof(buf));
		len = 0;
		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// データ送出開始
		memcpy( &buf[ len ], "A", 1 );					len += 1;

		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// プリンタ動作設定
		memcpy( &buf[ len ], "PG", 2);					len += 2;
		memcpy( &buf[ len ], "3", 1);					len += 1;	// 印字速度 50mm/sec
		memcpy( &buf[ len ], "2", 1);					len += 1;	// 印字濃度レベル 中薄
		memcpy( &buf[ len ], "A", 1);					len += 1;	// 印字濃度 A
		memcpy( &buf[ len ], "3", 1);					len += 1;	// バックフィード無し
		memcpy( &buf[ len ], "0", 1);					len += 1;	// ゼロスラッシュ無効
		memcpy( &buf[ len ], "1", 1);					len += 1;	// 漢字コード シフトJIS
		memcpy( &buf[ len ], "0", 1);					len += 1;	// 初期フィード無効
		memcpy( &buf[ len ], "0", 1);					len += 1;	// プロポーショナルピッチ解除
		memcpy( &buf[ len ], "2400", 4);				len += 4;	// 縦ラベルサイズ
		memcpy( &buf[ len ], "0832", 4);				len += 4;	// 横ラベルサイズ
		memcpy( &buf[ len ], "+000", 4);				len += 4;	// 縦基点補正値
		memcpy( &buf[ len ], "+000", 4);				len += 4;	// 横基点補正値
		memcpy( &buf[ len ], "+00", 3);					len += 3;	// 用紙ピッチオフセット
		memcpy( &buf[ len ], "+00", 3);					len += 3;	// ティアオフオフセット
		memcpy( &buf[ len ], "+00", 3);					len += 3;	// ハクリオフセット
		memcpy( &buf[ len ], "000", 3);					len += 3;	// オートパワーオフ無し
		memcpy( &buf[ len ], "1", 1);					len += 1;	// ジャーナル発行モード
		memcpy( &buf[ len ], "1", 1);					len += 1;	// 動作モード 連続
		memcpy( &buf[ len ], "1", 1);					len += 1;	// センサ種指定 透過式センサ

		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// データ送出終了
		memcpy( &buf[ len ], "Z", 1 );					len += 1;

	} else {
		/*プリンタ設定*/
		memset(buf, 0x00, sizeof(buf));
		/*データ送信開始指定*/
		len = 0;
		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	/* データ送出開始				*/
		memcpy( &buf[ len ], "A", 1 );					len += 1;
		/*自動改行設定*/
		/*文字フィールド指定*/
		buf[len++] = 0x1b;
		//abcdefghiiiijjjjkkkkllllmmmnnnooo
		memcpy( &buf[len], "PG43A3010012800384+000+000+00+00+00", 35 );
		len += 35;
		memcpy( &buf[len], "000", 3);
		len += 3;
		memcpy( &buf[len], "111", 3 );					//	ジャーナル発行モード
		len += 3;

		/*データ送信終了指定*/
		buf[len++] = 0x1b;
		buf[len++] = 'Z';
	}

	// 初期化コマンド送信

	ret = LapinBTSend(buf, len);
	if (ret != LAPIN_OK) {
		goto end;
	}


	// 印字コマンドは印字内容によって適宜設定すること
	if (mode == COMMAND_MODE_SBPL) {
		memset(buf, 0x00, sizeof(buf));
		len = 0;
		buf[len++] = 0x1b;
		buf[len++] = 'A';
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E2", 2 );					len += 2;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "K9B", 3 );				len += 3;

	} else {
		memset(buf, 0x00, sizeof(buf));
		len = 0;
		buf[len++] = 0x1b;
		buf[len++] = 'A';
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E001", 4 );				len += 4;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E2", 2 );					len += 2;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "L0001000108111000", 17 );	len += 17;
		buf[len++] = 0x1b;
		buf[len++] = 'D';
	}


	if ( Flag == 0 ) {

		//	<< 領収書 >>
		ret = PrintDoubleHeight(title, 24);


		memset(buf, 0x00, sizeof(buf));
		len = 0;
		buf[len++] = 0x1b;
		buf[len++] = 'A';
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E2", 2 );					len += 2;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "K9B", 3 );				len += 3;

		memcpy( &buf[ len ], "\r", 1 ); len += 1;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		getrtc4( now );
		memcpy( Year,  &now[0], 4 );
		memcpy( Month, &now[4], 2 );
		memcpy( Day,   &now[6], 2 );
		memcpy( Hour,  &now[8], 2 );
		memcpy( Min,   &now[10], 2 );

		// 	Year/Month/Day Hour:Min Tenpo-Reji-Tanto
		sprintf(&buf[len], "%s", Year ); len += sizeof( Year );
		memcpy( &buf[ len ], "/", 1 ); len += 1;
		sprintf(&buf[len], "%s", Month); len += sizeof( Month );
		memcpy( &buf[ len ], "/", 1 ); len += 1;
		sprintf(&buf[len], "%s", Day  ); len += sizeof( Day  );
		memcpy( &buf[ len ], " ", 1 ); len += 1;
		sprintf(&buf[len], "%s", Hour ); len += sizeof( Hour );
		memcpy( &buf[ len ], ":", 1 ); len += 1;
		sprintf(&buf[len], "%s", Min  ); len += sizeof( Min  );

		memcpy( &buf[ len ], "  ", 2 ); len += 2;

		sprintf(&buf[ len ], "%s", info.tenpo1 ); len += sizeof( info.tenpo1 );
		memcpy( &buf[ len ], "-", 1 ); len += 1;
		sprintf(&buf[ len ], "%s", ctrl.RejiNo ); len += sizeof( ctrl.RejiNo );
		memcpy( &buf[ len ], "-", 1 ); len += 1;
		sprintf(&buf[ len ], "%s", info.tanto ); len += sizeof( info.tanto );


		memcpy( &buf[ len ], "\r", 1 ); len += 1;
		memcpy( &buf[ len ], "\r", 1 ); len += 1;

		buf[len++] = 0x1b;
		buf[len++] = 'Z';
		ret = LapinBTSend(buf, len);


		// 	様
		ret = PrintNormal(empty, 29);
		ret = PrintDoubleHeight(kyaku, 29);
		ret = PrintDoubleHeight(aline, 29);


		ret = PrintDoubleHeight(total, 29);


		ret = PrintDoubleHeight(aline, 29);




		memset(buf, 0x00, sizeof(buf));
		len = 0;
		buf[len++] = 0x1b;
		buf[len++] = 'A';
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "E2", 2 );					len += 2;
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "K9B", 3 );				len += 3;


		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;

//		for ( i = 0; i < ctrl.RecmasCnt; i++) {
//			ram_read( i, &recmas, RECMASF );
//			sprintf(&buf[ len ], "%30s", recmas.Comment ); len += 30;
//			memcpy( &buf[ len ], "\r", 1 ); len += 1;
//		}


		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;
		memcpy( &buf[ len ], " \r", 2 ); len += 2;

	}

	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "RF010002", 4 );
	len += 4;


	// データ送出終了
	if (mode == COMMAND_MODE_SBPL) {
		buf[len++] = 0x1b;
		buf[len++] = 'Z';

		ret = LapinBTSend(buf, len);
		if (ret != LAPIN_OK) {
			goto end;
		}
	} else {
		buf[len++] = 0x1b;
		memcpy( &buf[ len ], "Q001", 4 );					len += 4;
		buf[len++] = 0x1b;
		buf[len++] = 'Z';

		ret = LapinBTSend(buf, len);
		if (ret != LAPIN_OK) {
			goto end;
		}
	}

// ERROR HANDLING
end:
	if (ret != LAPIN_OK) {
		LapinBTDisconnect();
		progress_stop();

		memset(buf, 0x00, sizeof(buf));
		if ( ret == -104) {
			sprintf(buf, "%s", "ﾌﾟﾘﾝﾀの電源を確認して下さい" );
		} else if ( ret == -24 ) {
			sprintf(buf, "%s\n%s", "用紙チェック", "ﾌﾟﾘﾝﾀの再起動" );
		} else if ( ret == -21 ) {
			sprintf(buf, "%s", "ﾌﾟﾘﾝﾀのｶﾊﾞｰを確認して下さい" );
		} else {
			sprintf(buf, "[%d]", ret);
		}
		intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "エラー\n", buf, "再印刷", "終了");
	} else {
		ret = LapinBTDisconnect();
		progress_stop();

		if (ret != LAPIN_OK) {
			memset(buf, 0x00, sizeof(buf));
			sprintf(buf, "Disconnect %d", ret);
			intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, "再印刷", "終了");
		} else {
			msgbox(LCD_MSGBOX_INFORMATION, 0, "正常終了", NULL, NULL, NULL);
			intMsg = 1;
		}
	}
	return intMsg;
}



// ZNATZ
int PrintDoubleHeight ( char* contents, int length ) {
	int ret;
	char buf [2048];
	short len;

	int i;

	memset(buf, 0x00, sizeof(buf));
	len = 0;
	buf[len++] = 0x1b;
	buf[len++] = 'A';
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "PC0", 3 );	len += 3;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "E2", 2 );		len += 2;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "L0102", 5 );	len += 5;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "K9B", 3 );	len += 3;

	for ( i = 0; i < length; i++ ) {
		memcpy ( &buf[ len ], &contents[ i ], 1); len += 1;
	}


	memcpy( &buf[ len ], "\r", 1 ); len += 1;
	buf[len++] = 0x1b;
	buf[len++] = 'Z';
	ret = LapinBTSend(buf, len);
	return ret;
}


int PrintNormal ( char* contents, int length ) {
	int ret;
	char buf [2048];
	short len;

	int i;

	memset(buf, 0x00, sizeof(buf));
	len = 0;
	buf[len++] = 0x1b;
	buf[len++] = 'A';
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "PC0", 3 );	len += 3;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "E2", 2 );		len += 2;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "L0101", 5 );	len += 5;
	buf[len++] = 0x1b;
	memcpy( &buf[ len ], "K9B", 3 );	len += 3;

	for ( i = 0; i < length; i++ ) {
		memcpy ( &buf[ len ], &contents[ i ], 1); len += 1;
	}


	memcpy( &buf[ len ], "\r", 1 ); len += 1;
	buf[len++] = 0x1b;
	buf[len++] = 'Z';
	ret = LapinBTSend(buf, len);
	return ret;
}


//
//
//int PrintPricetag(void)
//{
//	
//	bt_conf_t conf;
//	char *ptrName;
//	unsigned char *ptrBC;
//	int ret;
//	int retry_count;
//	int restart_count;
//	int i;
//	
//	char buf [256];
//	unsigned char addr[13];
//	unsigned char name[41];
//
//	char goods_name[100];
//
//	char center[] = {0x1B, 0x61, 0x01};
//	char eol[] = {0x0A};
//	char feed[] = {0x1B, 0x64, 0x08};
//	char buffer[] = {0x1D, 0x48, 0x02, 0x1D, 0x6B, 0x02};
////			0x1B, 0x61, 0x01, 	  	/* center */
////			0x1B, 0x33, 0x05, 	  	/* ?????? */
////			0x1D, 0x68, 0x32, 	  	/* ?????? */
////			0x1D, 0x48, 0x02, 		/* number */
////			0x1D, 0x6B, 0x02
////	};
//
//
//
//	restart_count = 0;
//
//restart:
//	cls();
//	setcolor(COLOR_WHITE, COLOR_BLUE);
//	putsc("  　値札発行　  ");
//	setcolor(COLOR_BLACK, COLOR_WHITE);
//	ret = BtConfig(BT_GET, &conf);
//	if (ret != 0) {
//		sprintf(buf, "BtConfig %d", ret);
//		goto exit;
//	}
//	//PINｺｰﾄﾞ設定
//	strncpy(conf.localPIN, "0000", sizeof(conf.localPIN));
//	conf.timeout = 30;
//	conf.MITM_Enable = DISABLE;
//
//
//	retry_count = 0;
//retry:
//	ret = EzBtConnect("00137B48A48B", UUID_SPP);
//	if (ret == BT_AT_NO_ANSWER ) {
//		retry_count++;
//		if (retry_count > 2) {
//			goto exit;
//		}
//		goto retry;
//	} else if (ret != 0) {
//		sprintf(buf, "EzBtConnect %d", ret);
//		goto exit;
//	} else {
//	}
//
//	rsettime(PORT_BLUETOOTH, 3);
//
//
//	//画面表示
//	ckputss( 0, 13, "ENT:印刷", False, CLR_BASE );
//	ckputss( 0, 15, "F4:再接続", False, CLR_BASE );
//
//	restart_count++;
//	if (restart_count < 2) {
//		goto exit;
//	}
//
//	ret = getch();
//	if (ret == 100) {
//		goto exit;
//	}
//
//
//	ptrName = &buffer;
//	for ( i = 0; i <= 5; i++) {
//		rputc(PORT_BLUETOOTH, *ptrName);
//		ptrName++;
//	}
//
//	ram_read( 0x00, &bcmst, BCMSTF );
//	ptrName = &bcmst.Code;
//	for ( i = 0; i <= 12; i++) {
//		rputc(PORT_BLUETOOTH, *ptrName);
//		ptrName++;
//	}
//
//	return 1;
//
//exit:
//	rclose(PORT_BLUETOOTH);
//	goto restart;
//}
