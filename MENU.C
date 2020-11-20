#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"

static void		SndMenu( void );
static void		DelDat( void );
/************************************************************************/
/*	転送メニュー														*/
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
		ckputss( 0,  4, " 1:仕入  2:棚卸 ", False, CLR_BASE );
		ckputss( 0,  6, " 3:移動  9:中止 ", False, CLR_BASE );
		ckputss( 0,  8, "                ", False, CLR_BASE );
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
static char JobStr[5][5] = { "", "仕入", "棚卸", "移動", "値下" };
	
	ClsColor();
	while( 1 ){
		ckputss( 0,  0, " <<データ削除>> ", False, CLR_DE_TITLE );
		kputspc( 0, 5, 16, CLR_BASE );
		kputspc( 0, 8, 16, CLR_BASE );
		kputspc( 0, 11, 16, CLR_BASE );
		ckputss( 0,  4, " 1:仕入  2:棚卸 ", False, CLR_BASE );
		ckputss( 0,  6, " 3:移動  9:中止 ", False, CLR_BASE );
		ckputss( 0,  8, "                ", False, CLR_BASE );
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
			}/* else if( ret == '4' ){
				if( ctrl.NDataCnt )	cnt = ctrl.NDataCnt;
				else				cnt = 0;
				break;
			}*/ else if( ret == '9' ){
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
						ctrl.NDataCnt = 0;		break;
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
		ckputss(  0,  4, " 1:仕入  2:仕返 ", False, CLR_BASE );
		ckputss(  0,  6, " 3:棚卸  4:移動 ", False, CLR_BASE );
		ckputss(  0,  8, " 5:売上  6:受信 ", False, CLR_BASE );
		ckputss(  0, 10, " 7:転送  8:検品 ", False, CLR_BASE );
		ckputss(  0, 12, " 9:削除         ", False, CLR_BASE );
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
			}
		}
	}
}