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

int	PrintMain_NEX_M230(char* _addr, bool _crc, int _mode , short _Flag ) {
	bt_conf_t conf;
	int ret;
	int retry_count;
	unsigned char addr[13];
	unsigned char name[41];
	char buf[256];//転送用バッファ
	//各種ステータスを調べるためのコマンド(DLE EOT n)
	// char printer_status[] = { 0x10, 0x04, 0x01 };
	char printer_status[] = { 0x10, 0x1b, 0x76, 0x01 };
	char offline_status[] = { 0x10, 0x04, 0x02 };
	char error_status[] = { 0x10, 0x04, 0x03 };
	char sensor_status[] = { 0x10, 0x04, 0x04 };
	
restart:
	cls();
	setcolor(COLOR_WHITE, COLOR_BLUE);
	putsc("  Printer Test  ");
	setcolor(COLOR_BLACK, COLOR_WHITE);
	ret = BtConfig(BT_GET, &conf);
	if (ret != 0) {
		sprintf(buf, "BtConfig %d", ret);
		goto exit;
	}
	//PINｺｰﾄﾞ設定
	strncpy(conf.localPIN, "0000000000000000", sizeof(conf.localPIN));
	conf.timeout = 30;
	conf.MITM_Enable = DISABLE;
	
	ret = BtConfig(BT_SET, &conf);
	if (ret != 0) {
		sprintf(buf, "BtConfig %d", ret);
		goto exit;
	}
	
	ret = EzBtSelect(EZ_BT_MODE_MENU, addr, name);
	if (ret != 0) {
		sprintf(buf, "EzBtSelect %d", ret);
		goto exit;
	}

	// 接続無応答となることがあったのでリトライする
	retry_count = 0;
retry:	
	ret = EzBtConnect(addr, UUID_SPP);
	if (ret == BT_AT_NO_ANSWER) {
		retry_count++;
		if (retry_count > 2) {
			goto exit;
		}
		goto retry;
	} else if (ret != 0) {
		sprintf(buf, "EzBtConnect %d", ret);
		goto exit;
	} else {
	}

	rsettime(PORT_BLUETOOTH, 3);
		
	//画面表示
	curp(0,3);
	putsc("[ENT] test print");
	curp(0,5);
	putsc("[C]   end");

	
	while (1) {
		ret = getch();//入力待ち
		if (ret == CH_CLR) {//ｸﾘｱが押された場合
			sprintf(buf, "Abort");
			break;
		} else if (ret != CH_ENT) {//Ent以外が押された場合
			continue;
		} else {
		}
		
		///////////////印字部分//////////////////
		sprintf(buf, "test\n");
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
		
		/////////////////////////////////////////
		//short rputs(int port, unsigned char *ptr, short size)
		//通信ポートportの送信バッファにptrで指定したバッファからsize分の文字を書き込む。
		/////////////////////////////////////////
		
		//プリンターステータス
		rxbclr(PORT_BLUETOOTH);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)printer_status, sizeof(printer_status));
		ret = rgetc(PORT_BLUETOOTH);
		curp(0, 9);
		if (ret & RSTAT_BIT_RXDATA) {
			sprintf(buf, "printer %02x", ret & 0xFF);
		} else {
			sprintf(buf, "rgetc   %02x", (ret >> 8) & 0xFF);
		}
		putsc(buf);

		//オフライン要因ステータス
		rxbclr(PORT_BLUETOOTH);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)offline_status, sizeof(offline_status));
		ret = rgetc(PORT_BLUETOOTH);
		curp(0, 11);
		if (ret & RSTAT_BIT_RXDATA) {
			sprintf(buf, "offline %02x", ret & 0xFF);
		} else {
			sprintf(buf, "rgetc   %02x", (ret >> 8) & 0xFF);
		}
		putsc(buf);

		//エラー要因ステータス
		rxbclr(PORT_BLUETOOTH);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)error_status, sizeof(error_status));
		ret = rgetc(PORT_BLUETOOTH);
		curp(0, 13);
		if (ret & RSTAT_BIT_RXDATA) {
			sprintf(buf, "error  %02x", ret & 0xFF);
		} else {
			sprintf(buf, "rgetc  %02x", (ret >> 8) & 0xFF);
		}
		putsc(buf);

		//連続用紙検出器ステータス
		rxbclr(PORT_BLUETOOTH);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)sensor_status, sizeof(sensor_status));
		ret = rgetc(PORT_BLUETOOTH);
		curp(0, 15);
		if (ret & RSTAT_BIT_RXDATA) {
			sprintf(buf, "sensor %02x", ret & 0xFF);
		} else {
			sprintf(buf, "rgetc  %02x", (ret >> 8) & 0xFF);
		}
		putsc(buf);
	}
	
exit:
	rclose(PORT_BLUETOOTH);
	cls();
	setcolor(COLOR_WHITE, COLOR_BLUE);
	putsc("  Printer Test  ");
	setcolor(COLOR_BLACK, COLOR_WHITE);
	curp(0,3);
	putsc("Test End");
	curp(0,9);
	putsc(buf);
	
	getch();
	goto restart;
}

/********************************************************
 * 領収書印刷
 ********************************************************/
int PrintRyousyu(char* addr, bool crc, int mode , short Flag )
{
// 	int ret;
// 	char buf [2048];
// 	short len;
// 	int i;
// 	int intMsg;
// 	char now[14], Year[4], Month[2], Day[2];
// 	char Hour[2], Min[2];
// 	char Name[20];
// 	char RecNo[6];
// 	char strKingaku[MAXLEN];
// 	long lngNum = 0, lngBaika = 0, lngTensuu = 0;
// 	long lngKingaku = 0 , lngZei = 0 , lngZeiGoukei = 0;
// 	double dblZei = 0 , dblTaxRate = 0 , dblNum = 0 , dblNo = 0 ;

// 	long adrs;

// 	char title[24] =  "     ＜＜ 領 収 書 ＞＞\r";    	// |
// 	char kyaku[29] =  "                        様  \r";	// |
// 	char aline[29] =  "----------------------------\r";	// |
// 	char empty[29] =  "                            \r";	// |

// 	insComma( infour2.Goukei , strKingaku );
// 	char total[29];
// 	sprintf(&total[ 0 ], "             ￥%7s-   ", strKingaku );


// 	progress_start(LCD_PROGRESS_USE_ANIMATION, "印刷中");
// 	progress_update(NULL, NULL, 0);

// 	//if( Flag == 0 ){//#01 日計表
// 	ret = LapinBTOpen(addr, crc, mode);
// 	if (ret != LAPIN_OK) {
// 		goto end;
// 	}
// 	//}

// 	// 初期化コマンドはプリンタコマンド表を元に適宜設定すること。
// 	if ( mode == COMMAND_MODE_SBPL ) {

// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// データ送出開始
// 		memcpy( &buf[ len ], "A", 1 );					len += 1;

// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// プリンタ動作設定
// 		memcpy( &buf[ len ], "PG", 2);					len += 2;
// 		memcpy( &buf[ len ], "3", 1);					len += 1;	// 印字速度 50mm/sec
// 		memcpy( &buf[ len ], "2", 1);					len += 1;	// 印字濃度レベル 中薄
// 		memcpy( &buf[ len ], "A", 1);					len += 1;	// 印字濃度 A
// 		memcpy( &buf[ len ], "3", 1);					len += 1;	// バックフィード無し
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// ゼロスラッシュ無効
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// 漢字コード シフトJIS
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// 初期フィード無効
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// プロポーショナルピッチ解除
// 		memcpy( &buf[ len ], "2400", 4);				len += 4;	// 縦ラベルサイズ
// 		memcpy( &buf[ len ], "0832", 4);				len += 4;	// 横ラベルサイズ
// 		memcpy( &buf[ len ], "+000", 4);				len += 4;	// 縦基点補正値
// 		memcpy( &buf[ len ], "+000", 4);				len += 4;	// 横基点補正値
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// 用紙ピッチオフセット
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// ティアオフオフセット
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// ハクリオフセット
// 		memcpy( &buf[ len ], "000", 3);					len += 3;	// オートパワーオフ無し
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// ジャーナル発行モード
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// 動作モード 連続
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// センサ種指定 透過式センサ

// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// データ送出終了
// 		memcpy( &buf[ len ], "Z", 1 );					len += 1;

// 	} else {
// 		/*プリンタ設定*/
// 		memset(buf, 0x00, sizeof(buf));
// 		/*データ送信開始指定*/
// 		len = 0;
// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	/* データ送出開始				*/
// 		memcpy( &buf[ len ], "A", 1 );					len += 1;
// 		/*自動改行設定*/
// 		/*文字フィールド指定*/
// 		buf[len++] = 0x1b;
// 		//abcdefghiiiijjjjkkkkllllmmmnnnooo
// 		memcpy( &buf[len], "PG43A3010012800384+000+000+00+00+00", 35 );
// 		len += 35;
// 		memcpy( &buf[len], "000", 3);
// 		len += 3;
// 		memcpy( &buf[len], "111", 3 );					//	ジャーナル発行モード
// 		len += 3;

// 		/*データ送信終了指定*/
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';
// 	}

// 	// 初期化コマンド送信

// 	ret = LapinBTSend(buf, len);
// 	if (ret != LAPIN_OK) {
// 		goto end;
// 	}


// 	// 印字コマンドは印字内容によって適宜設定すること
// 	if (mode == COMMAND_MODE_SBPL) {
// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'A';
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E2", 2 );					len += 2;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "K9B", 3 );				len += 3;

// 	} else {
// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'A';
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E001", 4 );				len += 4;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E2", 2 );					len += 2;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "L0001000108111000", 17 );	len += 17;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'D';
// 	}


// 	if ( Flag == 0 ) {

// 		//	<< 領収書 >>
// 		ret = PrintDoubleHeight(title, 24);


// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'A';
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E2", 2 );					len += 2;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "K9B", 3 );				len += 3;

// 		memcpy( &buf[ len ], "\r", 1 ); len += 1;
// 		memcpy( &buf[ len ], "\r", 1 ); len += 1;

// 		getrtc4( now );
// 		memcpy( Year,  &now[0], 4 );
// 		memcpy( Month, &now[4], 2 );
// 		memcpy( Day,   &now[6], 2 );
// 		memcpy( Hour,  &now[8], 2 );
// 		memcpy( Min,   &now[10], 2 );

// 		// 	Year/Month/Day Hour:Min Tenpo-Reji-Tanto
// 		sprintf(&buf[len], "%s", Year ); len += sizeof( Year );
// 		memcpy( &buf[ len ], "/", 1 ); len += 1;
// 		sprintf(&buf[len], "%s", Month); len += sizeof( Month );
// 		memcpy( &buf[ len ], "/", 1 ); len += 1;
// 		sprintf(&buf[len], "%s", Day  ); len += sizeof( Day  );
// 		memcpy( &buf[ len ], " ", 1 ); len += 1;
// 		sprintf(&buf[len], "%s", Hour ); len += sizeof( Hour );
// 		memcpy( &buf[ len ], ":", 1 ); len += 1;
// 		sprintf(&buf[len], "%s", Min  ); len += sizeof( Min  );

// 		memcpy( &buf[ len ], "  ", 2 ); len += 2;

// 		sprintf(&buf[ len ], "%s", info.tenpo1 ); len += sizeof( info.tenpo1 );
// 		memcpy( &buf[ len ], "-", 1 ); len += 1;
// 		sprintf(&buf[ len ], "%s", ctrl.RejiNo ); len += sizeof( ctrl.RejiNo );
// 		memcpy( &buf[ len ], "-", 1 ); len += 1;
// 		sprintf(&buf[ len ], "%s", info.tanto ); len += sizeof( info.tanto );


// 		memcpy( &buf[ len ], "\r", 1 ); len += 1;
// 		memcpy( &buf[ len ], "\r", 1 ); len += 1;

// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';
// 		ret = LapinBTSend(buf, len);


// 		// 	様
// 		ret = PrintNormal(empty, 29);
// 		ret = PrintDoubleHeight(kyaku, 29);
// 		ret = PrintDoubleHeight(aline, 29);


// 		ret = PrintDoubleHeight(total, 29);


// 		ret = PrintDoubleHeight(aline, 29);




// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'A';
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E2", 2 );					len += 2;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "K9B", 3 );				len += 3;


// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;
// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;

// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;
// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;
// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;
// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;

// 	}

// 	buf[len++] = 0x1b;
// 	memcpy( &buf[ len ], "RF010002", 4 );
// 	len += 4;


// 	// データ送出終了
// 	if (mode == COMMAND_MODE_SBPL) {
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';

// 		ret = LapinBTSend(buf, len);
// 		if (ret != LAPIN_OK) {
// 			goto end;
// 		}
// 	} else {
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "Q001", 4 );					len += 4;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';

// 		ret = LapinBTSend(buf, len);
// 		if (ret != LAPIN_OK) {
// 			goto end;
// 		}
// 	}

// // ERROR HANDLING
// end:
// 	if (ret != LAPIN_OK) {
// 		LapinBTDisconnect();
// 		progress_stop();

// 		memset(buf, 0x00, sizeof(buf));
// 		if ( ret == -104) {
// 			sprintf(buf, "%s", "ﾌﾟﾘﾝﾀの電源を確認して下さい" );
// 		} else if ( ret == -24 ) {
// 			sprintf(buf, "%s\n%s", "用紙チェック", "ﾌﾟﾘﾝﾀの再起動" );
// 		} else if ( ret == -21 ) {
// 			sprintf(buf, "%s", "ﾌﾟﾘﾝﾀのｶﾊﾞｰを確認して下さい" );
// 		} else {
// 			sprintf(buf, "[%d]", ret);
// 		}
// 		intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "エラー\n", buf, "再印刷", "終了");
// 	} else {
// 		ret = LapinBTDisconnect();
// 		progress_stop();

// 		if (ret != LAPIN_OK) {
// 			memset(buf, 0x00, sizeof(buf));
// 			sprintf(buf, "Disconnect %d", ret);
// 			intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, "再印刷", "終了");
// 		} else {
// 			msgbox(LCD_MSGBOX_INFORMATION, 0, "正常終了", NULL, NULL, NULL);
// 			intMsg = 1;
// 		}
// 	}
// 	return intMsg;
return 0;
}
