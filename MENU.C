#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"
#include "print.h"

static void		SndMenu( void );
static void		DelDat( void );
/************************************************************************/
/*	転送メニュー														  */
/************************************************************************/
static void SndMenu( void )
{
	short	ret, cnt;
	
	ClsColor();
	while( 1 ){
		ckputss( 0,  0, " <<データ転送>> ", False, CLR_TE_TITLE );
		kputspc( 0, 5, 16, CLR_BASE );
		kputspc( 0, 8, 16, CLR_BASE );
		kputspc( 0, 11, 16, CLR_BASE );
		ckputss( 0,  4, " 1:売上  2:仕入 ", False, CLR_BASE );
		ckputss( 0,  6, " 3:棚卸  4:移動 ", False, CLR_BASE );
		ckputss( 0,  8, "   　　  9:中止 ", False, CLR_BASE );
		ckputss( 0, 10, "                ", False, CLR_BASE );
		while( 2 ){

			ret = getch();
			if ( ret == '1') {
				if( ctrl.URDataCnt ) cnt = ctrl.URDataCnt;
				else				cnt = 0;
				break;
			} else if( ret == '2' ){
				if( ctrl.SDataCnt )	cnt = ctrl.SDataCnt;
				else				cnt = 0;
				break;
			} else if( ret == '3' ){
				if( ctrl.TDataCnt )	cnt = ctrl.TDataCnt;
				else				cnt = 0;
				break;
			} else if( ret == '4' ){
				if( ctrl.IDataCnt )	cnt = ctrl.IDataCnt;
				else				cnt = 0;
				break;
			}/* else if( ret == '4' ){
				if( ctrl.NDataCnt )	cnt = ctrl.NDataCnt;
				else				cnt = 0;
				break;
			} */else if( ret == '9' ){
				return;
			}
		}
		if( !cnt ) {
			kputspc( 0, 4, 16, CLR_BASE );
			kputspc( 0, 6, 16, CLR_BASE );
			ckputss( 0,   5, "転送するデータが", False, CLR_BASE );
			ckputss( 0,   8, "   ありません。 ", False, CLR_BASE );
			ckputss( 0,  11, "    ENT: 確認   ", False, CLR_BASE );
			beeb( 20, 3, 10 );
			while( !okng() );
			//msgbox( LCD_MSGBOX_WARNING, 0x00, "", "転送するデータがありません", "確認", 0x00 );
		} else {
			AlfDataSend( ret-'0' );
			ClsColor();
		}
	}
}

/************************************************************************/
/*																		*/
/************************************************************************/
static void DelDat( void )
{
	short	ret, cnt;
	// * 20210106 売上削除追加 値下元々comment outされたので、それを復活して売上に変更
	// static char JobStr[5][5] = { "", "仕入", "棚卸", "移動", "値下" };
	static char JobStr[5][5] = { "", "仕入", "棚卸", "移動", "売上" };
	
	ClsColor();
	while( 1 ){

		ckputss( 0,  0, " <<データ削除>> ", False, CLR_DE_TITLE );
		kputspc( 0, 5, 16, CLR_BASE );
		kputspc( 0, 8, 16, CLR_BASE );
		kputspc( 0, 11, 16, CLR_BASE );
		ckputss( 0,  4, " 1:仕入  2:棚卸 ", False, CLR_BASE );
		// * 20210106 売上削除追加
		// ckputss( 0,  6, " 3:移動  9:中止 ", False, CLR_BASE );
		// ckputss( 0,  8, "                ", False, CLR_BASE );
		ckputss( 0,  6, " 3:移動  4:売上 ", False, CLR_BASE );
		ckputss( 0,  8, "   　　  9:中止 ", False, CLR_BASE );

		while( 2 ){
			ret = getch();
			if( ret == '1' ){
				if( ctrl.SDataCnt )	cnt = ctrl.SDataCnt;
				else				cnt = 0;
				break;
			} else if( ret == '2' ){
				if( ctrl.TDataCnt )	cnt = ctrl.TDataCnt;
				else				cnt = 0;
				break;
			} else if( ret == '3' ){
				if( ctrl.IDataCnt )	cnt = ctrl.IDataCnt;
				else				cnt = 0;
				break;
			} else if( ret == '4' ){
				// if( ctrl.NDataCnt )	cnt = ctrl.NDataCnt;
				// * 20210106 売上削除追加
				if( ctrl.URDataCnt )	cnt = ctrl.URDataCnt;
				else				cnt = 0;
				break;
			} else if( ret == '9' ){
				return;
			}
		}
		if( !cnt ){
			kputspc( 0, 4, 16, CLR_BASE );
			kputspc( 0, 6, 16, CLR_BASE );
			ckputss( 0,   5, "削除するデータが", False, CLR_BASE );
			ckputss( 0,   8, "   ありません。 ", False, CLR_BASE );
			ckputss( 0,  11, "    ENT: 確認   ", False, CLR_BASE );
			beeb( 20, 3, 10 );
			while( !okng() );
			//msgbox( LCD_MSGBOX_WARNING, 0x00, "", "削除するデータがありません", "確認", 0x00 );
			//beeb( 20, 3, 20 );
		} else {
			ckputss( 0,  0, "<<    ﾃﾞｰﾀ削除>>", False, CLR_DE_TITLE );
			ckputss( 2,  0, JobStr[ret-'0'], False, CLR_DE_TITLE );
			kputspc( 0, 4, 16, CLR_BASE );
			kputspc( 0, 6, 16, CLR_BASE );
			ckprintf( 4, 5, False, CLR_BASE, "%4d件", cnt );
			ckputss( 0,  7, "   データ削除   ", False, CLR_BASE );
			ckputss( 0,  9, " 実行しますか？ ", False, CLR_BASE );
			ckputss( 0, 15, "1:はい  2:いいえ", False, CLR_BASE );
			if( okngEx( '1', '2' ) ){
				switch( ret ){
					case	'1':
						ctrl.SDataCnt = 0;		break;
					case	'2':
						ctrl.TDataCnt = 0;		break;
					case	'3':
						ctrl.IDataCnt = 0;		break;
					case	'4':
						// ctrl.NDataCnt = 0;		break;
						// * 20210106 売上削除追加
						ctrl.URDataCnt = 0;		break;
				}
				ram_write( 0, &ctrl, CTRLF );
				ckputss( 0, 5, "  削除しました  ", False, CLR_BASE );
				kputspc( 0, 7, 16, CLR_BASE );
				kputspc( 0, 9, 16, CLR_BASE );
				kputspc( 0,15, 16, CLR_BASE );
				//beeb( 20, 3, 20 );
				beep( 50, 1 );
				ckputss( 0, 15, "    ENT:確認    ", False, CLR_BASE );
				while( !okng() );
			}
			ClsColor();
		}
	}
}


/************************************************************************/
/*	各種設定														*/
/************************************************************************/
void Setting( void ){
	short ret,Flag = 0;
	ClsColor();
	ckputss( 0,  0, "   <<設  定>>   ", False, CLR_SE_TITLE );
	
	ckputss( 0,  2, "1.店舗NO: ", False, CLR_BASE );
	ckputsn( 9,  2, ctrl.ShopNo,  2, False, CLR_BASE );

	ckputss( 0,  4, "2.レジNO: ", False, CLR_BASE );
	ckputsn( 9,  4, ctrl.RejiNo,  2, False, CLR_BASE );
	
	ckputss( 0,  6, "3.消費税率:  ％ ", False, CLR_BASE );
	ckputsn( 11, 6, ctrl.TaxRate,  2, False, CLR_BASE );
	
	ckputss( 0, 8, "4.税計算:     ", False, CLR_BASE );
	ckputsn( 11,8, ctrl.TaxType, 1, False, CLR_BASE );
	ckputss( 0, 10, "1四捨五入 2切捨て", False, CLR_BASE );
	
	ckputss( 0, 12, "5.ﾚｼｰﾄ発行:     ", False, CLR_BASE );
	ckputsn( 11,12, ctrl.RecPrint, 1, False, CLR_BASE );

	ckputss( 0, 14, " 1.する 2.しない", False, CLR_BASE );
	ckputss( 0, 16, "F1:戻る ENT:設定", False, CLR_BASE );
	while( 1 ){
		if( Flag == 0 ){
			ret = NumInput( 9,  2, ctrl.ShopNo, sizeof( ctrl.ShopNo ), 0, 99L, 
						KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
		}else if( Flag == 1 ){
			ret = NumInput( 9,  4, ctrl.RejiNo, sizeof( ctrl.RejiNo ), 0, 99L, 
						KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
		}else if( Flag == 2 ){
			ret = NumInput( 11, 6, ctrl.TaxRate, sizeof( ctrl.TaxRate ), 0, 99L, 
						KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
		}else if( Flag == 3 ){
			ret = NumInput( 11, 8, ctrl.TaxType, sizeof( ctrl.TaxType ), 0, 9L, 
						KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );				
		}else{	
			ret = NumInput( 11, 12, ctrl.RecPrint, sizeof( ctrl.RecPrint ), 0, 9L, 
						KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
		}
		if( ret  == ENTRY || ret == SENTRY ){
			if( Flag == 0 ){
				ram_write( 0, &ctrl, CTRLF );
				Flag = 1;
				continue;
			}else if( Flag == 1 ){
				ram_write( 0, &ctrl, CTRLF );
				Flag = 2;
				continue;
			}else if( Flag == 2 ){
				ram_write( 0, &ctrl, CTRLF );
				Flag = 3;
				continue;
			}else if( Flag == 3 ){
				ram_write( 0, &ctrl, CTRLF );
				Flag = 4;
				continue;
			}else{
				if( memcmp( ctrl.RecPrint,"1",1 ) == 0 || memcmp( ctrl.RecPrint,"2",1 ) == 0 ){
					ram_write( 0, &ctrl, CTRLF );
					beep( 10, 2 );
					menu();
				}else{
					beep( 50, 1 );
				}
			}
		}else if( ret == F1KEY ){
			menu();
		}
	}
}	
/************************************************************************/
/*																		*/
/************************************************************************/
void menu( void )
{
	short		ret;

	while( 1 ) {
		setautoff( 60, 600 );			/* アイドル時間６０秒、パワーオフ１０分 */
		offmode( AUTOFF_RESUME_OFF );	/* レジュームＯＦＦ	*/
		setpict(1);
		ClsColor();
		offkey(1);
		ckputss(  0,  0, " <<Main Menu>>  ", False, CLR_TITLE );
		ckputss(  0,  3, " 1:仕入  2:仕返 ", False, CLR_BASE );
		ckputss(  0,  5, " 3:棚卸  4:移動 ", False, CLR_BASE );
		ckputss(  0,  7, " 5:売上  6:受信 ", False, CLR_BASE );
		ckputss(  0,  9, " 7:転送  8:検品 ", False, CLR_BASE );
		ckputss(  0, 11, " 9:削除  0:設定 ", False, CLR_BASE );
		ckputss(  0, 13, " F1:ﾌﾟﾘﾝﾀ ﾃｽﾄ   ", False, CLR_BASE );
		ckputsn(  1, 16, VER2, sizeof( VER2 ), False, CLR_BASE );
		//ckputss(  0,  8, " 5:転送  9:削除 ", False, CLR_BASE );
		while( 2 ){
			ret = getch();
			if( ret == '1' ){	        /* 仕入 */
				Sscan(0);
				break;
			} else if( ret == '2' ){	/* 仕返 */
				Sscan(1);
				break;
			} else if( ret == '3' ){	/* 棚卸 */
				Tscan();
				break;				
			} else if( ret == '4' ){	/* 移動 */
				Iscan();
				break;
			} else if( ret == '5' ){	/* 売上 */
				uriage(0, 0);
				break;			
			} else if( ret == '6' ){	/* 受信 */
				AlfRecvMst();
				break;				
			} else if( ret == '7' ){	/* 転送 */
				SndMenu();
				break;
			} else if( ret == '8' ){	/* 検品 */
				Kscan();
				break;				
			} else if( ret == '9' ){	/* 削除 */
				DelDat();
				break;
			} else if( ret == '0' ){	/* 削除 */
				Setting();
				break;
			} else if( ret == 97 ) {
				test_print(1);
				break;
			}

		}
	}
}

void print_receipt_header() {

	short ret;
	char buf[256];

	// * -- ロゴ
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)printRegisterImage1, sizeof(printRegisterImage1));

	// * -- 日本語
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignCenter, sizeof(bAlignCenter));

	// * -- レシートマスタ(3行中央+1行左)
	if(RemasFind("01") != -1) {
		memset(buf,0x0,sizeof(buf));
		snprintf(buf, 30, "%30s", remst.Name);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, 30);
		ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行
	}

	if(RemasFind("02") != -1) {
		memset(buf,0x0,sizeof(buf));
		snprintf(buf, 30, "%30s", remst.Name);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, 30);
		ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行
	}

	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignLeft, sizeof(bAlignLeft));

	if(RemasFind("03") != -1) {
		memset(buf,0x0,sizeof(buf));
		snprintf(buf, 24, "%24s", remst.Name);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, 24);
	}

	// * -- 店舗-レジ-担当
	memset(buf,0x0,sizeof(buf));
	snprintf(buf, sizeof("00-00-00"), "%02d-%02d-%02d", atoin(ctrl.ShopNo, sizeof(ctrl.ShopNo)),atoin(ctrl.RejiNo, sizeof(ctrl.RejiNo)),99 );
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("00-00-00"));

	ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行

	// * -- ローマ字
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bSizeNor, sizeof(bSizeNor));

	// * -- 現時間(yy/MM/dd HH:nn)
	char now[8];
	memset(now,0x0, sizeof(now));
	getrtc4( now );

	memset(buf,0x0,11);
	memcpy(buf,now,4);
	memcpy(buf+4,"/",1);
	memcpy(buf+5,now+4,2);
	memcpy(buf+7,"/",1);
	memcpy(buf+8,now+6,2);
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf+2, strlen(buf)-2);

	ret = rputs(PORT_BLUETOOTH, " ", sizeof(" "));

	char time[sizeof("00:00")];
	memset(time,0x0,sizeof(time));
	getrtcHourMinute(time);
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)time, sizeof(time));

	// * -- 顧客連番
	memset(buf,0x0,sizeof(buf));
	snprintf(buf, sizeof("12345678NO:1234567"), "        NO:%07ld", 5001 );
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("12345678NO:1234567"));

}

//****************************************************************************/
/* 	印刷設定
/*	Flag=1:日計表
/*	Flag=2:レシート
/*	Flag=3:値札(不使用)
//****************************************************************************/
int test_print( short Flag ){
	bt_conf_t conf;
	int ret ,intMsg = 0;
	unsigned char addr[13];
	unsigned char name[41];
	char buf[256] , strGoukei[MAXLEN] ;
	int key;
	
	
	cls();
	// ckputss( 0,  0, "<プリンタテスト>", False, CLR_UR_TITLE );
	ckputss( 0,  0, " <Printer Test> ", False, CLR_UR_TITLE );
	
	ret = BtConfig(BT_GET, &conf);
	if (ret != 0) {
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "BtConfig %d", ret);
		msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, NULL, NULL); 
	} else {
		// PINコード 
		strncpy(conf.localPIN, "0000000000000000", sizeof(conf.localPIN));
		conf.timeout = 30;
		conf.MITM_Enable = DISABLE;
		
		ret = BtConfig(BT_SET, &conf);
		if (ret != 0) {
			memset(buf, 0x00, sizeof(buf));
			sprintf(buf, "BtConfig %d", ret);
			msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, NULL, NULL); 
		}
	}
	
	memset(addr, 0x00, sizeof(addr));
	memset(name, 0x00, sizeof(name));

	BtGetLastAddr((unsigned char*)addr);

	while (1) {

		ckputss( 1,  3, "1.テスト印刷  ", False, CLR_BASE );
		ckputss( 1,  5, "2.接続確認    ", False, CLR_BASE );
		ckputss( 1,  7, "3.テスト終了  ", False, CLR_BASE );	
		ckputss( 0,  12, "  ﾌﾟﾘﾝﾀ ｱﾄﾞﾚｽ　 ", False, CLR_UR_TITLE );
		
		if (strlen((char*)addr) == 0) {
			ckputss( 1,  14, " 未設定", False, CLR_BASE );
		} else {
			ckputsn( 1,  14, (char *)addr, 16, False, CLR_BASE );
		}

		key = getch();
		
		if (key == '1') {
	
			ret = EzBtSelect(EZ_BT_MODE_MENU, addr, name);
			if (ret != 0) {
				sprintf(buf, "EzBtSelect %d", ret);
				goto exit;
			}

			// 接続無応答となることがあったのでリトライする
			int retry_count = 0; 
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
			} 

			rsettime(PORT_BLUETOOTH, 3);

			// * ------------------------  印字構築部分開始
			print_receipt_header();

			// * -- 改行
			ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行

			// * -- 日本語
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignLeft, sizeof(bAlignLeft));

			// * -- 設定印字
			memset(buf,0x0,sizeof(buf));
			snprintf(buf, 10, "%s%s", "店舗NO:", ctrl.ShopNo);
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
			ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行

			memset(buf,0x0,sizeof(buf));
			snprintf(buf, 10, "%s%s", "レジNO:", ctrl.RejiNo);
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
			ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行

			memset(buf,0x0,sizeof(buf));
			snprintf(buf, 10, "%s%s", "  税率:", ctrl.TaxRate);
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
			ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行

			memset(buf,0x0,sizeof(buf));
			snprintf(buf, 9, "%s%s", "  税区:", ctrl.TaxType);
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
			ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行

			// * -- 担当印字
			memset(buf,0x0,sizeof(buf));
			if(TantoFind("99") != -1) {
				// * ---------------------------------------------------- DEBUG用
				// displayStringMsg(&tamst.Name);
				// * ---------------------------------------------------- DEBUG用
				snprintf(buf, sizeof("  担当:1234567890123456"), "  担当:%16s", tamst.Name);
				ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("  担当:1234567890123456"));
				ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行
			}


			// * -- Printer & Line Feed
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)nFeedLine, sizeof(nFeedLine));

			// * -- カット
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)bCut, sizeof(bCut));

			// * ------------------------  印字構築部分終了
			break;
		} else if (key == '2') {
			ret = EzBtSelect(EZ_BT_MODE_MENU, addr, name);
			if (ret != 0 && ret != -10) {
				memset(buf, 0x00, sizeof(buf));
				sprintf(buf, "EzBtSelect %d", ret);
				msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, NULL, NULL); 
			}
		} else if (key == '3'){
			break;
		}
	}

exit:
	rclose(PORT_BLUETOOTH);
	return 1;
}