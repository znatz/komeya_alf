#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"

enum {
	SHOP = 1,
	//TANTO,
	CODE1,
	CODE2,
	NUM,
	NESAGE1,
	NESAGE2,
	DENTRY,
	DELETE
};

static short		SaveItem;
static char		Code[13];

static void		meisaiclr( void );
static void		Display( short item , short Func );
static void		entrydata( short sw );
/*****************************************************************************/
/* 	明細データ初期化														 */
/*****************************************************************************/
static void meisaiclr( void )
{
	memset( ndata.Code1, 0x00, sizeof( ndata.Code1 ) );
	memset( ndata.Code2, 0x00, sizeof( ndata.Code2 ) );
	memset( ndata.Code3, 0x00, sizeof( ndata.Code3 ) );
	//memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
	memset( ndata.Num, 	 0x00, sizeof( ndata.Num   ) );
	memset( Code,		 0x00, sizeof( Code ) );
}
/*****************************************************************************/
/* 	値下２場合分け														 */
/*****************************************************************************/
static void Nesage2( void )
{
	entrydata( False );
	memcpy( ndata.Code1, ndata.Code4 , sizeof( ndata.Code1 ) );
	memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
	
}

/*****************************************************************************/
/* 			     															 */
/*****************************************************************************/
static void Display( short item , short Func )
{
	switch( item ){
		case SHOP:
			if( SaveItem != SHOP ){
				ClsColor();	  /* 0123456789012345*/
				ckputss( 0,  0, "<値下入力>      ", False, CLR_NE_TITLE );
				ckputss( 0,  5, "    店舗:       ", False, CLR_BASE );
				ckputss( 0, 13, "F1:ﾒﾆｭｰ ENT:確定", False, CLR_BASE );				
			}
			/* 件数 */
			ckprintf( 10, 0, False, CLR_NE_TITLE , "%4d件", ctrl.NDataCnt );
			/* 店舗コード */
			if( ndata.Shop[0] ){
				ckputsn( 9, 5, ndata.Shop, sizeof( ndata.Shop ), False, CLR_BASE );
				/*if( mmst.Code[0] ){
					ckputsn( 2, 7,  mmst.Name, 		14, False, CLR_BASE );
					//ckputsn( 3, 9, &mmst.Name[10], 	10, False, CLR_BASE );
				} else {
					kputspc( 2, 7, 14, CLR_BASE );
					//kputspc( 3, 9, 10, CLR_BASE );
				}
			} else {
				kputspc( 9, 5,  1, CLR_BASE );
				kputspc( 2, 7, 14, CLR_BASE );
				//kputspc( 3, 9, 10, CLR_BASE );*/
			}
			break;
		/*case TANTO:
			if( SaveItem != TANTO ){
				ClsColor();	  // 0123456789012345
				ckputss( 0,  0, "<棚卸入力>      ", False, CLR_TITLE );
				ckputss( 0,  5, "    担当:       ", False, CLR_BASE );
				ckputss( 0, 15, "F1:戻り ENT:確定", False, CLR_BASE );
			}
			// 件数 
			ckprintf( 10, 0, False, CLR_TITLE, "%4d件", ctrl.NDataCnt );
			// 担当コード 
			if( ndata.Tanto[0] ){
				ckputsn( 9, 5, ndata.Tanto, sizeof( ndata.Tanto ), False, CLR_BASE );
				if( tmst.Code[0] ){
					ckputsn( 3, 7,  tmst.Name, 		10, False, CLR_BASE );
					ckputsn( 3, 9, &tmst.Name[10], 	10, False, CLR_BASE );
				} else {
					kputspc( 3, 7, 10, CLR_BASE );
					kputspc( 3, 9, 10, CLR_BASE );
				}
			} else {
				kputspc( 9, 5,  2, CLR_BASE );
				kputspc( 3, 7, 10, CLR_BASE );
				kputspc( 3, 9, 10, CLR_BASE );
			}
			break;*/
		case CODE1:
			if( SaveItem != CODE1)
			{
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<値下入力>      ", False, CLR_NE_TITLE );
				ckputss( 0,  2, "店舗:           ", False, CLR_BASE );
				ckputss( 0,  4, "▲              ", False, CLR_BASE );
				ckputss( 0,  6, "▽              ", False, CLR_BASE );
				ckputss( 0, 10, "値1             ", False, CLR_BASE );
				ckputss( 0, 12, "値2             ", False, CLR_BASE );
				ckputss( 0, 14, "F1:戻る F4:複数 ", False, CLR_BASE );
				ckputss( 0, 16, "        F2:削除 ", False, CLR_BASE );
				if( Func == 2){
					ckputss( 0, 8, "数量:           ", False, CLR_BASE );
					ckputss( 0, 14, "F1:戻る F4:1点  ", False, CLR_BASE );
				}
				ckputsn(  5, 2, ndata.Shop,  sizeof( ndata.Shop  ), False, CLR_BASE );
			}
			/* 件数 */
			ckprintf( 10, 0, False,CLR_NE_TITLE , "%4d件", ctrl.NDataCnt );
			// 上段コード 
			if( ndata.Code1[0] ){
				ckputsn( 3, 4, ndata.Code1, sizeof( ndata.Code1 ), False, CLR_BASE );
				/*// 上段コードの先頭から３桁が501～999の場合 
				if( atoin( ndata.Code1, 3 ) > 500 && atoin( ndata.Code1, 3 ) < 999 ){
					ckputsn( 0, 4, ndata.Code1, 3, False, CLR_BASE );
					ckputss( 3, 4, "-", False, CLR_BASE );
					ckputsn( 4, 4, &ndata.Code1[3], 4, False, CLR_BASE );
					ckprintf( 11, 4, False, CLR_BASE, "\\%4d", atoln( &ndata.Code1[8], 4 ) );
				} else if( !memcmp( ndata.Code1, "45", 2 ) || !memcmp( ndata.Code1, "49", 2 ) ||
						   !memcmp( ndata.Code1, "20", 2 ) ){
					// 上段コードの先頭から２桁が４５、１９、２０の場合 
					ckputsn( 0, 4, ndata.Code1, sizeof( ndata.Code1 ), False, CLR_BASE );
				} else if( !memcmp( ndata.Code1, "2289", 4 ) ){
					// 上段コードの先頭から４桁が２２８９の場合 
					ckputss( 0, 4, "AT", False, CLR_BASE );
				} else if( !memcmp( ndata.Code1, "2299", 4 ) ){
					/ 上段コードの先頭から４桁が２２９９の場合 
					ckputss( 0, 4, "FJ", False, CLR_BASE );
				}*/
			} else {
				ckputss( 0,  4, "▲              ", False, CLR_BASE );
			}
			break;
		case CODE2:
			if( SaveItem != CODE2){
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<値下入力>      ", False, CLR_NE_TITLE );
				ckputss( 0,  2, "店舗:           ", False, CLR_BASE );
				ckputss( 0,  4, "△              ", False, CLR_BASE );
				ckputss( 0,  6, "▼              ", False, CLR_BASE );
				ckputss( 0, 10, "値1             ", False, CLR_BASE );
				ckputss( 0, 12, "値2             ", False, CLR_BASE );
				ckputss( 0, 14, "F1:戻る         ", False, CLR_BASE );
				ckputss( 0, 16, "                ", False, CLR_BASE );
				ckputsn(  5, 2, ndata.Shop,  sizeof( ndata.Shop  ), False, CLR_BASE );
				//ckputsn( 13, 3, ndata.Tanto, sizeof( ndata.Tanto ), False, CLR_BASE );
			}
			/* 件数 */
			ckprintf( 10, 0, False, CLR_NE_TITLE , "%4d件", ctrl.NDataCnt );
			/* 上段コード */
			if( ndata.Code1[0] ){
				ckputsn( 3, 4, ndata.Code1, sizeof( ndata.Code1 ), False, CLR_BASE );
			}
			/* 下段コード */
			if( ndata.Code2[0] ){
				ckputsn( 3, 6, ndata.Code2, sizeof( ndata.Code2 ), False, CLR_BASE );
			}
			break;
		case NUM:
			if( SaveItem != NUM)
			{
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<値下入力>      ", False, CLR_NE_TITLE  );
				ckputss( 0,  2, "店舗:           ", False, CLR_BASE );
				ckputss( 0,  4, "△              ", False, CLR_BASE );
				ckputss( 0,  6, "▽              ", False, CLR_BASE );
				ckputss( 0,  8, "数量:           ", False, CLR_BASE );
				ckputss( 0, 10, "値1             ", False, CLR_BASE );
				ckputss( 0, 12, "値2             ", False, CLR_BASE );
				ckputss( 0, 14, "F1:戻る C :ｸﾘｱ  ", False, CLR_BASE );
				ckputss( 0, 16, "                ", False, CLR_BASE );
				ckputsn(  5, 2, ndata.Shop,  sizeof( ndata.Shop  ), False, CLR_BASE );
				//ckputsn( 13, 3, ndata.Tanto, sizeof( ndata.Tanto ), False, CLR_BASE );
			}
			/* 件数 */
			ckprintf( 10, 0, False, CLR_NE_TITLE , "%4d件", ctrl.NDataCnt );
			/* 上段コード */
			if( ndata.Code1[0] ){
				ckputsn( 3, 4, ndata.Code1, sizeof( ndata.Code1 ), False, CLR_BASE );
			}
			/* 下段コード */
			if( ndata.Code2[0] ){
				ckputsn( 3, 6, ndata.Code2, sizeof( ndata.Code2 ), False, CLR_BASE );
			}
			/* 数量 */
			if( !ndata.Num[0] ){
				memcpy( ndata.Num, "00001", sizeof( ndata.Num ) );
			}
			break;
		case NESAGE1:
			/* 数量 */
			if( !ndata.Num[0] ){
				memcpy( ndata.Num, "00001", sizeof( ndata.Num ) );
			}
			if( SaveItem != NESAGE1)
			{
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<値下入力>      ", False, CLR_NE_TITLE  );
				ckputss( 0,  2, "店舗:           ", False, CLR_BASE );
				ckputss( 0,  4, "△              ", False, CLR_BASE );
				ckputss( 0,  6, "▽              ", False, CLR_BASE );
				ckputss( 0,  8, "数量:           ", False, CLR_BASE );
				ckputss( 0, 10, "値1             ", False, CLR_BASE );
				ckputss( 0, 12, "値2             ", False, CLR_BASE );
				ckputss( 0, 14, "F1:戻る         ", False, CLR_BASE );
				ckputss( 0, 16, "                ", False, CLR_BASE );
				ckputsn(  5, 2, ndata.Shop,  sizeof( ndata.Shop  ), False, CLR_BASE );
				ckprintf( 5, 8, False, CLR_BASE , "%ld", atoln( ndata.Num, sizeof( ndata.Num ) ) );
				//ckputsn( 13, 3, ndata.Tanto, sizeof( ndata.Tanto ), False, CLR_BASE );
			}
			/* 件数 */
			ckprintf( 10, 0, False, CLR_NE_TITLE, "%4d件", ctrl.NDataCnt );
			/* 上段コード */
			if( ndata.Code1[0] ){
				ckputsn( 3, 4, ndata.Code1, sizeof( ndata.Code1 ), False, CLR_BASE );
			}
			/* 下段コード */
			if( ndata.Code2[0] ){
				ckputsn( 3, 6, ndata.Code2, sizeof( ndata.Code2 ), False, CLR_BASE );
			}
			/* 値下コード */
			if( ndata.Code3[0] ){
				if( ndata.Code3[2] == '9' )
					ckprintf( 4, 10, False, CLR_BASE, "->%2d%OFF", atoin( &ndata.Code3[3], 2 ) );
				else
					ckprintf( 4, 10, False, CLR_BASE, "->\\%ld", atoln( &ndata.Code3[2], 5 ) );
			} else {
				kputspc( 4, 10, 10, CLR_BASE );
			}
			break;
		case NESAGE2:
			if( SaveItem != NESAGE2)
			{
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<値下入力>      ", False, CLR_NE_TITLE );
				ckputss( 0,  2, "店舗:           ", False, CLR_BASE );
				ckputss( 0,  4, "△              ", False, CLR_BASE );
				ckputss( 0,  6, "▽              ", False, CLR_BASE );
				ckputss( 0,  8, "数量:           ", False, CLR_BASE );
				ckputss( 0, 10, "値1             ", False, CLR_BASE );
				ckputss( 0, 12, "値2             ", False, CLR_BASE );
				ckputss( 0, 14, "F1:戻る F4:複数 ", False, CLR_BASE );
				ckputss( 0, 16, "        F3:確定 ", False, CLR_BASE );
				ckputsn(  5, 2, ndata.Shop,  sizeof( ndata.Shop  ), False, CLR_BASE );
				ckprintf( 5, 8, False, CLR_BASE , "%ld", atoln( ndata.Num, sizeof( ndata.Num ) ) );
				//ckputsn( 13, 3, ndata.Tanto, sizeof( ndata.Tanto ), False, CLR_BASE );
			}
			/* 件数 */
			ckprintf( 10, 0, False, CLR_NE_TITLE, "%4d件", ctrl.NDataCnt );
			/* 上段コード */
			if( ndata.Code1[0] ){
				ckputsn( 3, 4, ndata.Code1, sizeof( ndata.Code1 ), False, CLR_BASE );
			}
			/* 下段コード */
			if( ndata.Code2[0] ){
				ckputsn( 3, 6, ndata.Code2, sizeof( ndata.Code2 ), False, CLR_BASE );
			}
			/* 値下コード */
			if( ndata.Code3[0] ){
				if( ndata.Code3[2] == '9' )
					ckprintf( 4, 10, False, CLR_BASE, "->%2d%OFF", atoin( &ndata.Code3[3], 2 ) );
				else
					ckprintf( 4, 10, False, CLR_BASE, "->\\%ld", atoln( &ndata.Code3[2], 5 ) );
			} else {
				kputspc( 4, 10, 10, CLR_BASE );
			}
			//値下コード2 
			if( ndata.Code4[0] ){
				if( ndata.Code4[2] == '9' )
					ckprintf( 4, 12, False, CLR_BASE, "->%2d%OFF", atoin( &ndata.Code4[3], 2 ) );
				else
					ckprintf( 4, 12, False, CLR_BASE, "->\\%ld", atoln( &ndata.Code4[2], 5 ) );
			} else {
				kputspc( 4, 12, 10, CLR_BASE );
			}
			break;
		case DENTRY:
			if(SaveItem != DENTRY )
			{
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<仕入入力>      ", False, CLR_NE_TITLE );
				ckputss( 0,  2, "店舗:           ", False, CLR_BASE );
				ckputss( 0,  4, "△              ", False, CLR_BASE );
				ckputss( 0,  6, "▽              ", False, CLR_BASE );
				ckputss( 0,  8, "数量:           ", False, CLR_BASE );
				ckputss( 0, 10, "値1             ", False, CLR_BASE );
				ckputss( 0, 12, "値2             ", False, CLR_BASE );
				ckputss( 0, 14, "F1:戻る         ", False, CLR_BASE );
				ckputsn(  5, 2, ndata.Shop,  sizeof( ndata.Shop  ), False, CLR_BASE );
				//ckputsn( 13, 3, ndata.Tanto, sizeof( ndata.Tanto ), False, CLR_BASE );
			}
			/* 件数 */
			ckprintf( 10, 0, False, CLR_NE_TITLE , "%4d件", ctrl.NDataCnt );
			/* 上段コード */
			if( ndata.Code1[0] ){
				ckputsn( 3, 4, ndata.Code1, sizeof( ndata.Code1 ), False, CLR_BASE );
			}
			/* 下段コード */
			if( ndata.Code2[0] ){
				ckputsn( 3, 6, ndata.Code1, sizeof( ndata.Code1 ), False, CLR_BASE );
			}
			/* 値下コード */
			if( ndata.Code3[0] ){
				if( ndata.Code3[2] == '9' )
					ckprintf( 4, 12, False, CLR_BASE, "->%2d%OFF", atoin( &ndata.Code3[3], 2 ) );
				else
					ckprintf( 4, 12, False, CLR_BASE, "->\\%ld", atoln( &ndata.Code3[2], 5 ) );
			} else {
				kputspc( 4, 12, 10, CLR_BASE );
			}
			/* 数量 */
			if( !ndata.Num[0] ){
				memcpy( ndata.Num, "00001", sizeof( ndata.Num ) );
			}
			ckprintf( 5, 8, False, CLR_BASE, "%ld", atoln( ndata.Num, sizeof( ndata.Num ) ) );
			break;
		case DELETE:
			if(SaveItem != DELETE )
			{
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<削除>          ", False, CLR_NE_TITLE );
				ckputss( 0,  3, "△              ", False, CLR_BASE );
				ckputss( 0,  6, "▽              ", False, CLR_BASE );
				ckputss( 0, 11, "数量:           ", False, CLR_BASE );
				ckputss( 0, 15, "削除?  OK:1 NO:2", False, CLR_BASE );
			}
			ram_read( ctrl.NDataCnt-1, &ndata, NDATAF );
			/* 上段コード */
			if( ndata.Code1[0] ){
				ckputsn( 3, 3, ndata.Code1, 13, False, CLR_BASE );
			}
			/* 下段コード */
			if( ndata.Code2[0] ){
				ckputsn( 3, 6, ndata.Code2, 13, False, CLR_BASE );
			}
			/* 数量 */
			if( !ndata.Num[0] ){
				memcpy( ndata.Num, "00001", sizeof( ndata.Num ) );
			}
			ckprintf( 5, 11, False, CLR_BASE, "%ld", atoln( ndata.Num, sizeof( ndata.Num ) ) );
			break;
	}
	SaveItem = item;
}

/*****************************************************************************/
/* 	明細データ登録 															 */
/*****************************************************************************/
static void entrydata( short sw )
{
	char	now[20];
	
	//if( sw ){
		/* 新規登録 */
		getrtc4( now );
		memcpy( ndata.Date, now, sizeof( ndata.Date ) );
		ram_write( ctrl.NDataCnt, &ndata, NDATAF );
		++ctrl.NDataCnt;
		ram_write( 0, &ctrl, CTRLF );
	//} else {
		//最後に登録したデータを更新 
		//ram_write( ctrl.NDataCnt-1, &ndata, NDATAF );
	//}
	beep( 10, 2 );
	meisaiclr();
}

/*****************************************************************************/
/* 	棚卸業務     															 */
/*****************************************************************************/
void Nscan( void )
{
	short	item, ret, Func;//Func=1:1点 Func=2:複数　

	memset( &ndata, 0x00, sizeof( ndata ) );
	setpict( 1 );
	item = SHOP;
	Func = 1;
	SaveItem = -1;
	while( item ){
		if( MaxCheck( ctrl.NDataCnt, NDATA_MAX ) ) return;
		Display( item , Func );
		switch( item ){
			case SHOP:
				ret = NumInput( 9, 5, ndata.Shop, sizeof( ndata.Shop ), 0, 9L, 
								KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
				break;
			//case TANTO:
				//ret = NumInput( 9, 5, ndata.Tanto, sizeof( ndata.Tanto ), 0, 99L, 
				//				KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
				//break;
			case CODE1:
				ret = CodeInput( 3, 4, ndata.Code1, sizeof( ndata.Code1 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;
			case CODE2:
				ret = CodeInput( 3, 6, ndata.Code2, sizeof( ndata.Code2 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;
			case NUM:
				ret = NumInput(  5, 8, ndata.Num, sizeof( ndata.Num ), 0, 99999L,
								IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
				break;
			case NESAGE1:
				//memset( ndata.Code3, 0x00, sizeof( ndata.Code3 ) );
				ret = CodeInput(  3, 10, ndata.Code3, sizeof( ndata.Code3 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;
			case NESAGE2:
				//memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
				ret = CodeInput(  3, 12, ndata.Code4, sizeof( ndata.Code4 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;
			case DENTRY:
				/* 登録 */
				meisaiclr();
				memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
				entrydata( True );
				item = CODE1;
				Func=1;
				continue;
			case DELETE:
				ret = getch();
				if( ret == '1' ){			/* 削除 */
					--ctrl.NDataCnt;
					ram_read( ctrl.NDataCnt, &ndata, NDATAF );
					meisaiclr();
					ram_write( 0, &ctrl, CTRLF );
					item = CODE1;	
				} else if( ret == '2' ){	/* 棚卸 */
					ram_read( ctrl.NDataCnt, &ndata, NDATAF );
					meisaiclr();
					item = CODE1;
				}
				memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
				continue;
		}
		if( ret == ENTRY || ret == SENTRY || ret == SKIP || ret == BARENT ){
			/*if( item == SHOP ){
				// 店舗マスタ検索 
				if( ShopFind( ndata.Shop ) == -1 ){
					beep( 50, 1 );
					continue;
				}
			} else if( item == TANTO ){
				// 担当マスタ検索 
				if( TantoFind( ndata.Tanto ) == -1 ){
					beep( 50, 1 );
					continue;
				}
			}*/ 	if( item == SHOP ){
					if( !memcmp( ndata.Shop, "0", 1 ) ){
						menu();
					}
				} else if( item == CODE1 ){
				/* 上段コードの先頭から1桁が7の場合、下段コード入力へ */
				if( !memcmp( ndata.Code1, "7", 1 ) ){
					item = CODE2;
					continue;
				/* 上段コードの先頭から1桁が9の場合、下段コード入力へ */
				} else if( !memcmp( ndata.Code1, "9", 1 ) ){
					item = CODE2;
					continue;
				/* 上段コードの先頭から2桁が21の場合、下段コード入力へ */
				} else if( !memcmp( ndata.Code1, "21", 2 ) ){
					item = CODE2;
					continue;
				} else if( !memcmp( ndata.Code1, "45", 2 ) || !memcmp( ndata.Code1, "49", 2 ) ||
						   !memcmp( ndata.Code1, "20", 2 ) ){
					/* 上段コードの先頭から２桁が４５、１９、２０の場合、データ登録へ */
						if(Func == 1){
							item = NESAGE1;
						}
						if(Func == 2){
							item = NUM;
						}
					continue;
				} else if( !memcmp( ndata.Code1, "2289", 4 ) || !memcmp( ndata.Code1, "2299", 4 ) ){
					item = CODE2;
					continue;
				} /*else if( ctrl.NDataCnt && !memcmp( ndata.Code1, "30", 2 ) && ndata.Code1[8] == ' ' ){
					// 登録済みデータが存在するかつ上段コードの先頭から２桁が３０、かつ８桁の場合 
					// 割引率または値下金額を最後の登録データへセットしデータ修正を行う			
					memcpy( Code, ndata.Code1, sizeof( Code ) );
					ram_read( ctrl.NDataCnt-1, &ndata, NDATAF );
					memcpy( ndata.Code3, Code, sizeof( ndata.Code3 ) );
					memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
					if(Func == 1){
						item = NESAGE2;
					}
					if(Func == 2){
						item = NUM;
					}
					continue;
				}*/ else {
					/* 上記に該当しない場合はエラー */
					beep( 50, 1 );
					meisaiclr();
					memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
					continue;
				}
			} else if( item == CODE2 ){
				/* 上段コードの先頭から1桁が7の場合、6から始まるコード以外不可 */
				if( !memcmp( ndata.Code1, "7", 1 ) ){
					if( !memcmp( ndata.Code2, "6", 1 ) ){
						if(Func == 1){
							item = NESAGE1;
						}
						if(Func == 2){
							item = NUM;
						}
						continue;
					} else {
						/* 上記に該当しない場合はエラー */
						memset( ndata.Code2, 0x00, sizeof( ndata.Code2 ) );
						beep( 50, 1 );
						continue;
					}
				/* 上段コードの先頭から1桁が9の場合、8から始まるコード以外不可 */
				} else if( !memcmp( ndata.Code1, "9", 1 ) ){
					if( !memcmp( ndata.Code2, "8", 1 ) ){
						if(Func == 1){
							item = NESAGE1;
						}
						if(Func == 2){
							item = NUM;
						}
						continue;
					} else {
						/* 上記に該当しない場合はエラー */
						memset( ndata.Code2, 0x00, sizeof( ndata.Code2 ) );
						beep( 50, 1 );
						continue;
					}
				/* 上段コードの先頭から2桁が21の場合、24から始まるコード以外不可 */
				} else if( !memcmp( ndata.Code1, "21", 2 ) ){
					if( !memcmp( ndata.Code2, "24", 2 ) ){
						if(Func == 1){
							item = NESAGE1;
						}
						if(Func == 2){
							item = NUM;
						}
						continue;
					} else {
						/* 上記に該当しない場合はエラー */
						memset( ndata.Code2, 0x00, sizeof( ndata.Code2 ) );
						beep( 50, 1 );
						continue;
					}
				} else if( !memcmp( ndata.Code1, "2289", 4 ) || !memcmp( ndata.Code1, "2299", 4 ) ){
					if( !memcmp( ndata.Code2, "2500", 4 ) ){
						if(Func == 1){
							item = NESAGE1;
						}
						if(Func == 2){
							item = NUM;
						}
						continue;
					} else {
						/* 上記に該当しない場合はエラー */
						memset( ndata.Code2, 0x00, sizeof( ndata.Code2 ) );
						beep( 50, 1 );
						continue;
					}
				} else {
					/* 上記に該当しない場合はエラー */
					memset( ndata.Code2, 0x00, sizeof( ndata.Code2 ) );
					beep( 50, 1 );
					continue;
				}
			} else if( item == NUM ){
				/* マイナスを含まない場合、入力値が９９９を超えたらエラーとする */
				if( atoln( ndata.Num, sizeof( ndata.Num ) ) >= 0 && atoln( ndata.Num, sizeof( ndata.Num ) ) > 999 ){
					beep( 50, 1 );
					continue;
				}
				/* マイナスを含む場合、入力値が-９９９を超えたらエラーとする */
				if( atoln( ndata.Num, sizeof( ndata.Num ) ) < 0 && atoln( ndata.Num, sizeof( ndata.Num ) ) <= -1000 ){
					beep( 50, 1 );
					continue;
				}
				/* 登録 */
				//entrydata( False );
				item = NESAGE1;
				//Func=1;
				continue;
			} else if( item == NESAGE1 ){
				if( !memcmp( ndata.Code3,"30", 2 ) ){
					item = NESAGE2;
					memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
					continue;
					//上段コードの先頭から２桁が３０かつ８桁の場合 
				} else {
					/* 上記に該当しない場合はエラー */
					memset( ndata.Code3, 0x00, sizeof( ndata.Code3 ) );
					beep( 50, 1 );
					continue;
				}
			
			} else if( item == NESAGE2 ){
				//ckprintf( 10, 0, False, CLR_NE_TITLE , "%d件", strlen( ndata.Code4 ) );
				if( !memcmp( ndata.Code4, "30", 2 ) && ndata.Code4[8] == ' ' ){
					memset( ndata.Code3, 0x00, sizeof( ndata.Code3 ) );
					memcpy( ndata.Code3, ndata.Code4 , sizeof( ndata.Code4 ) );

					entrydata( False );
					item = CODE1;
					Func=1;
					continue;
				}else if( !memcmp( ndata.Code4, "7", 1 ) ){
					Nesage2();
					item = CODE2;
					Func=1;
					continue;
				/* 上段コードの先頭から1桁が9の場合、下段コード入力へ */
				} else if( !memcmp( ndata.Code4, "9", 1 ) ){
					Nesage2();
					item = CODE2;
					Func=1;
					continue;
				/* 上段コードの先頭から2桁が21の場合、下段コード入力へ */
				} else if( !memcmp( ndata.Code4, "21", 2 ) ){
					Nesage2();
					item = CODE2;
					Func=1;
					continue;
				} else if( !memcmp( ndata.Code4, "45", 2 ) || !memcmp( ndata.Code4, "49", 2 ) ||
						   !memcmp( ndata.Code4, "20", 2 ) ){
					/* 上段コードの先頭から２桁が４５、１９、２０の場合、データ登録へ */
					Nesage2();
					item = NESAGE1;
					Func=1;
					continue;
				} else if( !memcmp( ndata.Code4, "2289", 4 ) || !memcmp( ndata.Code4, "2299", 4 ) ){
					Nesage2();
					item = CODE2;
					Func=1;
					continue;
				}  else {
					/* 上記に該当しない場合はエラー */
					memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
					beep( 50, 1 );
					continue;
				}
			}
			
			++item;
	
		}else if( ret == F2KEY ){
			if ( ctrl.NDataCnt > 0 ) {
				if(item == CODE1){
					item = DELETE;
				}
			}
			continue;
		}else if( ret == F3KEY ){
			if( item == NESAGE2 ){
				/* 登録 */
				entrydata( False );
				meisaiclr();
				memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
				item = CODE1;
				Func=1;
				continue;
			}
			continue;
		}else if( ret == F4KEY ){
			if( item == CODE1 ){
				/* 店舗マスタ検索 */
				if( Func == 1 ){
					Func = 2;
					ckputss( 0, 8, "数量:           ", False, CLR_BASE );
					ckputss( 0, 14, "F1:戻る F4:1点  ", False, CLR_BASE );
				}
				else if( Func == 2){
					Func = 1;
					ckputss( 0, 8, "                ", False, CLR_BASE );
					ckputss( 0, 14, "F1:戻る F4:複数 ", False, CLR_BASE );
				}
			}
			else if( item == NESAGE2 ){
				/* 登録 */
				//entrydata( False );
				meisaiclr();
				Nesage2();
				item = CODE1;
				Func=2;
				continue;
			}
			continue;
		}else if( ret == F1KEY ){
			if( item == CODE2 || item == NUM ){
				meisaiclr();
				memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
				if( item == NUM ){
					item = CODE1;
					continue;
				}
			}else if( item == CODE1 ){
				menu();	
			}else if( item == NESAGE1 ){
				if( Func == 1 ){
					meisaiclr();
					memset( ndata.Code4, 0x00, sizeof( ndata.Code4 ) );
					item = CODE1;
					continue;
				}
			}
			else if( item == NESAGE2 ){
				memset( ndata.Code3, 0x00, sizeof( ndata.Code3 ) );
			}
			--item;
		}
	}
}