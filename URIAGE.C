//////////////////////////////////
// * 最終更新 2020/12/02
//////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"
#include "PRINT.h"
#include <Math.h>

enum {
	YEAR = 1,
	MONTH,
	DAY,
	SHOP,
	TANTO,
	CODE1,
	CODE2,
	CODE3, //値下
	NUM,
	BAIHEN,
	GENKIN,
	CREDIT,
	KINKEN,
	COUPON,
	SEISAN,
	DENTRY,
	KAKUNIN,
	RYOUSYU
};

static short	SaveItem;
static char		Code[13];
static char		Name[13];
static char		Year[4];
static char		Month[2];
static char		Day[2];
static char		Genkin[6];
static char		Credit[6];
static char		Kinken[6];
static char		Coupon[6];
static char		Baika[10];
static long		lngOturi;
static long		lngOturiChk;
static long		lngGoukei;
static long		lngTensuu;
static long		lngBaika; //20201207
static long		lngJoudai;
static long		lngSingleTax;

static void		meisaiclr( void );
static void		Display( short item );
static void		entrydata( short sw );
static void		entryUriage();

// ZNATZ
static int		reprint;
static int		teisei;

// * 税額取得
static long calculateTax2(long lngPrice, short rate)
{
	// ZZZZ
	float t = (float) rate;
	// 1:四捨五入
	float result = round(lngPrice * rate / 100.0);
	// 2:切捨て
	if ( ctrl.TaxType[0] == '2') result = lngPrice * rate / 100.0;
	return (long) result;
}

// * 上段バーコードから税込金額計算
static long getZeikomiKingaku(char *barcode, short count) { 
	// displayMsg(BumonTaxFindByCode1(barcode));
	long jyoudai = getJyoudai(barcode);
	// * 上段バーコードの上代が税抜しかないため、品種の税区実際見ない、実際0しかならない
	long zeiku = HinsyuZeikuFindByCode1(barcode);
	long bumon_tax ;
	// * 品種税区 0:外税 1:内税
	if (zeiku == 1) {
		bumon_tax = 0;
	} else {
		bumon_tax = calculateTax2(jyoudai, BumonTaxFindByCode1(barcode));
	}
	return (jyoudai + bumon_tax) * count;
}

// * 上段バーコードから税込金額計算、値下が上代より少ないなら、値下する
static long getZeikomiKingakuWithNesgae(char *barcode, short count, char *sageBarcode) { 
	long jyoudai = getJyoudai(barcode);
	long bumon_tax = calculateTax2(jyoudai, BumonTaxFindByCode1(barcode));

	short taxrate = atoin( ctrl.TaxRate, sizeof(ctrl.TaxRate)) + 100.0;
	long sage_baika_inc_tax = calculateTax2(GetNesage(sageBarcode, jyoudai), taxrate);

	if (sage_baika_inc_tax <= (jyoudai + bumon_tax)) {
		return sage_baika_inc_tax * count;
	} else {
		return (jyoudai + bumon_tax) * count;
	}
}

/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
short CodeInput8orNull( short x, short y, char *s, short length, unsigned long prm )
{
	short	ret, len;
	char	inpBuf[50];

	memset( inpBuf, 0x00, sizeof( inpBuf ) );
	if( s[0] ){
		memncpy( inpBuf, s, length );
		if( inpBuf[7] == ' ' ) inpBuf[7] = 0x00;
	}
	while( 1 ){
		/*if( ctrl.BcrType == '1' ) prm |= BCR_PEN;*/
		ret = input( x, y, inpBuf, length, prm | TRIG_ENT );

		if( !(prm & BCR_NOTCD) && ( prm & BCRFLAGMASK ) && ( ret == SENTRY || ret == ENTRY ) ){
			if( !atol( inpBuf ) || !isBarcode( inpBuf, False ) ){
				beepEx( 50, 1 );
				continue;
			}
		}
		if( ret == ENTRY || ret == SENTRY || ret == SKIP || ret == BARENT ){
			len = strlen( inpBuf );
			memset( s, ' ', length );
			if( ret != SKIP ){
				if ( len == 8 ) {
					memcpy( s, inpBuf, len );
				}
			}
			break;
		} else if( ret == BAR_ERR ){
			beepEx( 50, 1 );
		} else if( ret == F1KEY || ret == F2KEY || ret == F3KEY ||
				   ret == F4KEY || ret == DEC_CLR_KEY ||
				   ret == BSKEY || ret == UPKEY || ret == DOWNKEY ){
			break;
		}
	}
	return( ret );
}


//****************************************************************************
//* 	明細データ初期化														 */
//****************************************************************************
static void meisaiclr( void )
{	
	memset( infour.Code1, 0x00, sizeof( infour.Code1 ) );
	memset( infour.Code3, 0x00, sizeof( infour.Code3 ) );
	memset( infour.Baika, 0x00, sizeof( infour.Baika  ) );
	memset( infour.Num, 0x00, sizeof( infour.Num   ) );
	memset( infour.Name, 0x00, sizeof( infour.Name  ) );
	infour.lngFinalTaxRateTax = 0;
	infour.lngBumonTaxRateTax = 0;
	infour.lngSystemTaxRateTax = 0;
	
	memset( info.Code1, 0x00, sizeof( info.Code1 ) );
	memset( info.Code2, 0x00, sizeof( info.Code2 ) );
	memset( info.Code3, 0x00, sizeof( info.Code3 ) );
	memset( info.Num, 0x00, sizeof( info.Num ) );
	memset( info.Joudai, 0x00, sizeof( info.Joudai ) );
	memset( Code,	0x00, sizeof( Code ) );
	memset( Name,	0x00, sizeof( Name ) );
	info.lngFinalTaxRateTax = 0;
	info.lngBumonTaxRateTax = 0;
	info.lngSystemTaxRateTax = 0;

	//20201207
	lngJoudai = 0;
	lngBaika = 0;
	lngSingleTax = 0;
}

//****************************************************************************
//* 	INFOデータ初期化														 */
//****************************************************************************
static void infoclr( void ){
	memset( info.tenpo1, 0x00, sizeof( info.tenpo1 ));
	memset( info.tenpo2, 0x00, sizeof( info.tenpo2 ));
	memset( info.tanto, 0x00, sizeof( info.tanto ));
	memset( info.Code1, 0x00, sizeof( info.Code1 ) );
	memset( info.Code2, 0x00, sizeof( info.Code2 ) );
	memset( info.Code3, 0x00, sizeof( info.Code3 ) );
	memset( info.Num, 0x00, sizeof( info.Num ) );
	memset( info.Joudai, 0x00, sizeof( info.Num ) );
//	memset( mmst.Name,0x00, sizeof( mmst.Name ) );
//	memset( tmst.Name,0x00, sizeof( tmst.Name ) );
	memset( Name,0x00, sizeof( Name ) );
	
}

static void infoclrExceptTenpoTanto( void ){
	memset( info.Code1, 0x00, sizeof( info.Code1 ) );
	memset( info.Code2, 0x00, sizeof( info.Code2 ) );
	memset( info.Code3, 0x00, sizeof( info.Code3 ) );
	memset( info.Num, 0x00, sizeof( info.Num ) );
	memset( info.Joudai, 0x00, sizeof( info.Num ) );
//	memset( mmst.Name,0x00, sizeof( mmst.Name ) );
//	memset( tmst.Name,0x00, sizeof( tmst.Name ) );
	memset( Name,0x00, sizeof( Name ) );
	
	
	memset( infour.Code1, 0x00, sizeof( infour.Code1 ) );
	memset( infour.Code3, 0x00, sizeof( infour.Code3 ) );
	memset( infour.Baika, 0x00, sizeof( infour.Baika  ) );
	memset( infour.Num, 0x00, sizeof( infour.Num   ) );
	memset( infour.Name, 0x00, sizeof( infour.Name  ) );
	
	memset( info.Code1, 0x00, sizeof( info.Code1 ) );
	memset( info.Code2, 0x00, sizeof( info.Code2 ) );
	memset( info.Code3, 0x00, sizeof( info.Code2 ) );
	memset( info.Num, 0x00, sizeof( info.Num ) );
	memset( info.Joudai, 0x00, sizeof( info.Joudai ) );
	memset( Code,	0x00, sizeof( Code ) );
	memset( Name,	0x00, sizeof( Name ) );
	
}
/*****************************************************************************/
/* 			     															 */
/*****************************************************************************/
static void DisplayHeader() {
	ckputss( 0, 0, "売　　　        ", False, CLR_UR_TITLE );
	ckprintf(10, 0, False, CLR_UR_TITLE , "%4d件", ctrl.InfoUrCnt );
	ckputsn( 3, 0, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_UR_TITLE );
	ckputsn( 5, 0, info.tanto, sizeof( info.tanto ), False, CLR_UR_TITLE );
}

static void Display( short item )
{
	// ZNATZ
	long lngGenkin = 0;
	long lngCredit = 0;
	
	long jyoudaiWithTax;
	
	// 消費税率を設定から取り出す ZNATZ
	short taxrate = atoin( ctrl.TaxRate, sizeof(ctrl.TaxRate)) + 100.0;
	
	long lngNum = 0,lngKyakuNo = 0,lngKingaku = 0 ;
	char strBaika[MAXLEN],strGoukei[MAXLEN];
	int intCnt;
	switch( item ){
		case YEAR:
			reprint = 0; teisei = 0;
			if( SaveItem != YEAR ){
				ClsColor();
				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				// 元日付元々行は3
				ckputss( 0,  6, "日付:    /  /  ", False, CLR_BASE );
				ckputsn(10,  6, Month, 2, False, CLR_BASE );
				ckputsn(13,  6, Day, 2, False, CLR_BASE );
				// ckputss( 0,  6, " 店舗:          ", False, CLR_BASE );
				// ckputss( 0, 10, " 担当:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:ﾒﾆｭｰ ENT:確定", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :日計", False, CLR_BASE );	
			}
			break;
		case MONTH:
			if( SaveItem != MONTH ){
				ClsColor();
				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				// 元日付元々行は3
				ckputss( 0,  6, "日付:    /  /  ", False, CLR_BASE );
				ckputsn( 5,  6, Year, 4, False, CLR_BASE );
				ckputsn( 13, 6, Day,  2, False, CLR_BASE );
				// ckputss( 0,  6, " 店舗:          ", False, CLR_BASE );
				// ckputss( 0, 10, " 担当:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:ﾒﾆｭｰ ENT:確定", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :日計", False, CLR_BASE );				
			}
			break;
		case DAY:
			if( SaveItem != DAY ){
				ClsColor();
				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				// 元日付元々行は3
				ckputss( 0,  6, "日付:    /  /   ", False, CLR_BASE );
				ckputsn( 5,  6, Year,  4, False, CLR_BASE );
				ckputsn( 10, 6, Month, 2, False, CLR_BASE );
				// ckputss( 0,  6, " 店舗:          ", False, CLR_BASE );
				// ckputss( 0, 10, " 担当:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:ﾒﾆｭｰ ENT:確定", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :日計", False, CLR_BASE );				
			}
			break;
		case SHOP:
			if( SaveItem != SHOP ){
				ClsColor();
				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				ckputss( 0,  3, " 日付:    /  /   ", False, CLR_BASE );
				ckputsn( 6,  3, Year,  4, False, CLR_BASE );
				ckputsn( 11, 3, Month, 2, False, CLR_BASE );
				ckputsn( 14, 3, Day,  2, False, CLR_BASE );
				// ckputss( 0,  6, " 店舗:          ", False, CLR_BASE );
				// ckputss( 0, 10, " 担当:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:ﾒﾆｭｰ ENT:確定", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :日計", False, CLR_BASE );		
			}
			break;
		case TANTO:
			if( SaveItem != TANTO ){
				ClsColor();
				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				ckputss( 0,  3, " 日付:    /  /   ", False, CLR_BASE );
				ckputsn( 6,  3, Year,  4, False, CLR_BASE );
				ckputsn( 11, 3, Month, 2, False, CLR_BASE );
				ckputsn( 14, 3, Day,  2, False, CLR_BASE );
				// ckputss( 0,  6, " 店舗:          ", False, CLR_BASE );
				// ckputsn( 6,  6, info.tenpo1,  2, False, CLR_BASE );
				// ckputss( 0, 10, " 担当:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:ﾒﾆｭｰ ENT:確定", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :日計", False, CLR_BASE );					
			}
			break;
		case CODE1:
			reprint = 0; teisei = 0;
			if( SaveItem != CODE1){
				ClsColor();

				DisplayHeader();

				ckputss( 0,  2, "▲              ", False, CLR_BASE );
				ckputss( 0,  4, "▽              ", False, CLR_BASE );				
				ckputss( 0,  6, "値下:           ", False, CLR_BASE );				

				// * 現在小計金額
				ckputss( 0, 12, "計        　　", False, CLR_SI_TITLE );
				insComma( lngGoukei, strGoukei ); 								
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7s円", strGoukei );

				// * 現在小計点数
				ckprintf(11,12, False, CLR_SI_TITLE, "%3d点", lngTensuu );

				ckputss( 0, 14, "F1:戻る  F2:取消", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:精算", False, CLR_BASE );	
			}
			break;
		case CODE2:
			reprint = 0; teisei = 0;
			if( SaveItem != CODE2){
				ClsColor();

				DisplayHeader();

				ckputss( 0,  2, "△              ", False, CLR_BASE );
				ckputsn( 3,  2, info.Code1, 13, False, CLR_BASE );
				ckputss( 0,  4, "▼              ", False, CLR_BASE );
				ckputss( 0,  6, "値下:           ", False, CLR_BASE );				

				// * 現在小計金額
				ckputss( 0, 12, "計        　　", False, CLR_SI_TITLE );
				insComma( lngGoukei, strGoukei ); 								
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7s円", strGoukei );

				// * 現在小計点数
				ckprintf(11,12, False, CLR_SI_TITLE, "%3d点", lngTensuu );

				ckputss( 0, 14, "F1:戻る  F2:取消", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:精算", False, CLR_BASE );	
			}
			break;			
		case CODE3:
			if( SaveItem != CODE3){
				ClsColor();	

				DisplayHeader();

				ckputss( 0,  2, "△              ", False, CLR_BASE );
				ckputss( 0,  4, "▽              ", False, CLR_BASE );
				ckputss( 0,  6, "値下:           ", False, CLR_BASE );				
				ckputsn( 3,  2, info.Code1, 13, False, CLR_BASE );
				ckputsn( 3,  4, info.Code2, 13, False, CLR_BASE );
				
				// * 売価表示
				insComma( getZeikomiKingaku(info.Code1, 1), strBaika);
				ckprintf(0,  9, False, CLR_BASE , "￥%7s",strBaika );

				// * 現在小計金額
				ckputss( 0, 12, "計        　　", False, CLR_SI_TITLE );
				insComma( lngGoukei, strGoukei );
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7s円", strGoukei );

				// * 現在小計点数
				ckprintf(11,12, False, CLR_SI_TITLE , "%3d点", lngTensuu );

				ckputss( 0, 14, "F1:戻る ENT:確定", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:精算", False, CLR_BASE );	
			}
			break;


		case NUM:
			if( SaveItem != NUM){
				ClsColor();	

				DisplayHeader();

				ckputss( 0,  2, "△              ", False, CLR_BASE );
				ckputss( 0,  4, "▽              ", False, CLR_BASE );
				ckputss( 0,  6, "値下:           ", False, CLR_BASE );				
				ckputsn( 3,  2, info.Code1, 13, False, CLR_BASE );
				ckputsn( 3,  4, info.Code2, 13, False, CLR_BASE );
				ckputsn( 5,  6, info.Code3, 8, False, CLR_BASE );
				
				// * 売価表示
				// * 売変の反映はCODE3を読んだ後 20201207
				// insComma( getZeikomiKingaku(info.Code1, 1), strBaika);
				insComma( lngBaika, strBaika);
				ckprintf(0,  9, False, CLR_BASE , "￥%7s×",strBaika );

				// * 現在小計金額
				ckputss( 0, 12, "計        　　", False, CLR_SI_TITLE );
				insComma( lngGoukei, strGoukei ); 								
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7s円", strGoukei );

				// * 現在小計点数
				ckprintf(11,12, False, CLR_SI_TITLE , "%3d点", lngTensuu );

				ckputss( 0, 14, "F1:戻る ENT:確定", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:精算", False, CLR_BASE );	
			}
			break;

		case DENTRY:
			if( SaveItem != DENTRY){
				ClsColor();	

				DisplayHeader();

				ckputss( 0,  2, "△              ", False, CLR_BASE );
				ckputss( 0,  4, "▽              ", False, CLR_BASE );
				ckputss( 0,  6, "値下:           ", False, CLR_BASE );				
				ckputsn( 3,  2, info.Code1, 13, False, CLR_BASE );
				ckputsn( 3,  4, info.Code2, 13, False, CLR_BASE );
				ckputsn( 5,  6, info.Code3, 8, False, CLR_BASE );

				// * 売価表示
				insComma( lngBaika, strBaika);
				ckprintf(0,  9, False, CLR_BASE , "￥%7s×",strBaika );

				// * 数量表示
				ckprintf(12, 9, False, CLR_BASE , "%d",atoln( info.Num, sizeof( info.Num )) );

				// * 現在小計金額
				ckputss( 0, 12, "計        　　", False, CLR_SI_TITLE );
				insComma( lngGoukei, strGoukei );		
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7s円", strGoukei );
				
				ckprintf(11,12, False, CLR_SI_TITLE , "%3d点", lngTensuu );
				ckputss( 0, 14, "F1:戻る ENT:確定", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:精算", False, CLR_BASE );	
			}
			break;	
		case GENKIN:
			if( SaveItem != GENKIN){
				ClsColor();	
				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				ckprintf(10, 0, False, CLR_UR_TITLE , "%4d件", ctrl.InfoUrCnt );
				ckputss( 0,  2, "店舗:   担当:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );

				ckputss( 0,  4, " 小  計 :       ", False, CLR_BASE );
				insComma( lngGoukei, strGoukei );		
				ckprintf(9,  4, False, CLR_BASE, "%7s", strGoukei );

				ckputss( 0,  6, " 現  金 ", False, CLR_SI_TITLE );
				ckputss( 8,  6, ":", False, CLR_BASE );
				ckputss( 0,  8, " 売  掛 :       ", False, CLR_BASE );
				insComma( atoln( Credit, sizeof( Credit )), strGoukei );
				ckprintf(9,  8, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0, 10, " お釣り :       ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0, 16, "F1:戻る ENT:確定", False, CLR_BASE );		
			}
			break;
		case CREDIT:
			if( SaveItem != CREDIT){
				ClsColor();	
				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				ckprintf(10, 0, False, CLR_UR_TITLE , "%4d件", ctrl.InfoUrCnt );
				ckputss( 0,  2, "店舗:   担当:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );

				ckputss( 0,  4, " 小  計 :       ", False, CLR_BASE );
				insComma( lngGoukei, strGoukei );		
				ckprintf(9, 4, False, CLR_BASE, "%7s", strGoukei );

				ckputss( 0,  6, " 現  金 :       ", False, CLR_BASE );
				insComma( atoln( Genkin, sizeof( Genkin )), strGoukei );
				ckprintf(9,  6, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  8, " 売  掛 ", False, CLR_BASE );
				ckputss( 8,  8, ":", False, CLR_BASE );
				ckputss( 0, 10, " お釣り :       ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0, 16, "F1:戻る ENT:確定", False, CLR_BASE );		
			}
			break;

		case SEISAN:
			if( SaveItem != COUPON){
				ClsColor();	
				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				ckprintf(10, 0, False, CLR_UR_TITLE , "%4d件", ctrl.InfoUrCnt );
				ckputss( 0,  2, "店舗:   担当:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  4, " 小  計 :       ", False, CLR_BASE );
				insComma( lngGoukei, strGoukei );
				ckprintf(9,  4, False, CLR_BASE, "%7s", strGoukei );

				ckputss( 0,  6, " 現  金 :       ", False, CLR_BASE );
				insComma( atoln( Genkin, sizeof( Genkin )), strGoukei );
				ckprintf(9,  6, False, CLR_BASE, "%7s", strGoukei );

				ckputss( 0,  8, " 売  掛 :       ", False, CLR_BASE );
				insComma( atoln( Credit, sizeof( Credit )), strGoukei );
				ckprintf(9,  8, False, CLR_BASE, "%7s", strGoukei ); 
				ckputss( 0, 10, " お釣り : ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
				
				ckputss( 0, 16, "F1:戻る ENT:終了", False, CLR_BASE );
			}
			break;
		case KAKUNIN:
			lngNum = 0;
			lngKingaku = 0;
			lngGenkin = 0;
			lngCredit = 0;
			break;
		case RYOUSYU:	// ZNATZ_RYOUSYU
			if( SaveItem != RYOUSYU){
				ClsColor();	

				ckputss( 0,  0, "<売上入力>      ", False, CLR_UR_TITLE );
				ckprintf(10, 0, False, CLR_UR_TITLE , "%4d件", ctrl.InfoUrCnt );
				ckputss( 0,  2, "店舗:   担当:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  4, " 小  計 :       ", False, CLR_BASE );
				insComma( lngGoukei, strGoukei );
				ckprintf(9,  4, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  6, " 現  金 :       ", False, CLR_BASE );
				insComma( atoln( Genkin, sizeof( Genkin )), strGoukei );
				ckprintf(9,  6, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  8, " 売  掛 :       ", False, CLR_BASE );
				insComma( atoln( Credit, sizeof( Credit )), strGoukei );
				ckprintf(9,  8, False, CLR_BASE, "%7s", strGoukei ); 

				ckputss( 0, 10, " お釣り :       ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );

				ckputss( 0, 14, "F1:訂正F2:再発行", False, CLR_BASE );		
				// ckputss( 0, 16, "F3:領収書ENT:終", False, CLR_BASE );
				ckputss( 0, 16, "  　　ENT:終了 ", False, CLR_BASE );
			}	
			break;
	}
	SaveItem = item;
}

void print_ASCI_underline(int length) {
	// * -- ローマ字
	rputs(PORT_BLUETOOTH, (unsigned char *)bRomaji, sizeof(bRomaji));
	// * -- アンダーライン
	rputs(PORT_BLUETOOTH, (unsigned char *)bUnderline1, sizeof(bUnderline1));
	for (int i = 0; i<length; i++) {
		rputs(PORT_BLUETOOTH, "a", sizeof("a"));
	}
	displayMsg(length);
}
//****************************************************************************/
// 	印刷レシート詳細
//****************************************************************************/
int entryprintdata() {

	int ret;
	char buf[256];//転送用バッファ
	char strNumber[MAXLEN];
	long lngNumber;

	short taxrate;
	long diff_taxrate = 0;
	taxrate = atoin( ctrl.TaxRate, sizeof(ctrl.TaxRate)); // + 100.0;

	// print_ASCI_underline(32);

	// * 漢字一つで3mm, (レシート幅58mm - 左右余白10mm)/3mm約16, 半角になると32
	// * strlen(himst.Name) = 40なので40以下にする必要がある
	const long LEN_ONELINE = 32;
	const long LEN_HINMEI  = 18; // sizeof(himst.Name)/2;
	const long LEN_AMOUNT  = 9;
	const long LEN_COUNT   = 5; //LEN_ONELINE-LEN_HINMEI-LEN_AMOUNT;
	char underline[32]; 
	memset(underline, '-', sizeof(underline));

	long kei_zeikomi = 0;
	long kei_zeinuki = 0;
	long kei_systemtax = 0; 
	long kei_bumontax = 0;

	if( memcmp( info.tanto,"00",2 ) != 0 ){


		// * -- 日本語
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));
		// * -- 中央寄せ
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignCenter, sizeof(bAlignCenter));
		// * -- アンダーライン
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)bUnderline1, sizeof(bUnderline1));
		// * -- ヘッダー
		snprintf(buf, LEN_ONELINE+1, "%s", "                                         ");
		snprintf(buf, LEN_ONELINE+1, "%s", "品　名　　　　　　数　量　金　額");
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("品　名　　　　　　数　量　金　額"));

				ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));
		// * -- 日本語
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));
		// * -- 左寄せ
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignLeft, sizeof(bAlignLeft));
		// * -- アンダーライン取り消す
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)bUnderline0, sizeof(bUnderline0));

		for ( int intCnt = 0; intCnt < ctrl.InfoUrCnt; intCnt++) {
		
			// point to address of current receipt 
			ram_read( intCnt , &infour, INFOURF );

			// TODO 値下
			kei_zeikomi = kei_zeikomi + atoln(infour.Baika, sizeof(infour.Baika)) * atoi(infour.Num);
			kei_zeinuki = kei_zeinuki + atoln(infour.Joudai, sizeof(infour.Joudai)) * atoi(infour.Num); 

			// * 軽減の時、軽減に使用した税率を一時保存
			if ( ( infour.lngBumonTaxRateTax != 0 ) && ( diff_taxrate == 0 )) {
				diff_taxrate = infour.lngFinalTaxRate;
			}

	
			// * 20201208
			if(HinsyuFindByCode1(infour.Code1) != 0) {

				// * ---------------------------------------------------- DEBUG用
				// displayStringMsg(&himst.Name);
				// * ---------------------------------------------------- DEBUG用

				// * -- 品名
				// * -- 左寄せ
				ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignLeft, sizeof(bAlignLeft));
				if ( infour.lngFinalTaxRate != taxrate ) {
					snprintf(buf, LEN_HINMEI+1, "%s", "                                    ");
					snprintf(buf, sizeof("☆("), "%s", "☆(");
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("☆"));

					snprintf(buf, LEN_HINMEI+1, "%s", "                                    ");
					snprintf(buf, 2, "%ld", infour.lngFinalTaxRate);
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, 2);

					snprintf(buf, LEN_HINMEI+1, "%s", "                                    ");
					snprintf(buf, sizeof("%)"), "%s", "%)");
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("%)"));

					snprintf(buf, LEN_HINMEI+1, "%s", "                                    ");
					snprintf(buf, LEN_HINMEI+1, "%s", himst.Name);
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, LEN_HINMEI - sizeof("(☆%)") );
				} else {
					snprintf(buf, LEN_HINMEI+1, "%s", "                                    ");
					snprintf(buf, LEN_HINMEI+1, "%s", himst.Name);
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, LEN_HINMEI);
				}

				// * -- 日本語
				ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));

				// * -- 個数
				int counts_of_goods = atoi(infour.Num);
				// * -- --- 1ではなければ、売価出す
				if (counts_of_goods != 1) {
					// * -- 改行
					ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));

					// * -- 売価 X 数量
					memset(strNumber,0x0,sizeof(strNumber));
					lngNumber = atoln(infour.Baika, sizeof(infour.Baika)) ;
					insComma( lngNumber, strNumber );
					snprintf(buf, sizeof("123456789012345678 X 12"), "%s", "                                                ");
					snprintf(buf, sizeof("123456789012345678 X 12"), "%18s X %2d", strNumber, counts_of_goods);
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("123456789012345678 X 12"));

					// * -- 金額
					memset(strNumber,0x0,sizeof(strNumber));
					lngNumber = atoln(infour.Baika, sizeof(infour.Baika)) * atoi(infour.Num);
					insComma( lngNumber, strNumber );
					snprintf(buf, LEN_AMOUNT+1, "%s", "                                    ");
					snprintf(buf, LEN_AMOUNT+1, "%9s", strNumber);
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, 9);

				} else {

					// * -- 数量
					snprintf(buf, LEN_COUNT+1, "%s", "                                    ");
					memset(strNumber,0x0,sizeof(strNumber));
					convertToString(counts_of_goods, strNumber);
					snprintf(buf, LEN_COUNT+1, "%5s", strNumber);
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, LEN_COUNT);

					// * -- 金額
					memset(strNumber,0x0,sizeof(strNumber));
					lngNumber = atoln(infour.Baika, sizeof(infour.Baika)) * atoi(infour.Num);
					insComma( lngNumber, strNumber );
					snprintf(buf, LEN_AMOUNT+1, "%s", "                                    ");
					snprintf(buf, LEN_AMOUNT+1, "%9s", strNumber);
					ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, 9);

				}
				ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));

				// * -- 対象金額集計
				if ( infour.lngFinalTaxRate != taxrate ) {
					kei_bumontax = kei_bumontax + lngNumber;
				} else {
					kei_systemtax = kei_systemtax + lngNumber;
				}

			}

		}
		// * -- アンダーライン
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)bUnderline1, sizeof(bUnderline1));
		// * -- ヘッダー
		snprintf(buf, LEN_ONELINE+1, "%s", "                                         ");
		snprintf(buf, LEN_ONELINE+1, "%s", "　　　　　　　　　　　　　　　　");
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("　　　　　　　　　　　　　　　　"));
		// * -- アンダーライン消す
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)bUnderline0, sizeof(bUnderline0));

		// * -- 文字拡大
		ret = rputs(PORT_BLUETOOTH, bSizeDbl, sizeof(bSizeDbl));

		// * -- 税込
		memset(strNumber,0x0,sizeof(strNumber));
		insComma( kei_zeikomi, strNumber );
		snprintf(buf, LEN_HINMEI+1, "%s", "                                    ");
		snprintf(buf, sizeof("税込計1234567890"), "税込計%10s", strNumber);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("税込計1234567890"));

		// * -- 改行
		ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));

		// * -- 文字戻す
		ret = rputs(PORT_BLUETOOTH, bSizeNor, sizeof(bSizeNor));

		// * -- 税抜
		memset(strNumber,0x0,sizeof(strNumber));
		insComma( kei_zeinuki, strNumber );
		snprintf(buf, LEN_HINMEI + LEN_COUNT + 1, "%s", "                                        ");
		snprintf(buf, sizeof("[税抜計123456789012345678901234]"), "[税抜計%24s]", strNumber);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("[税抜計123456789012345678901234]"));

		// * -- 改行
		ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));

		if ( kei_systemtax != 0 ) {

			// * -- 10%対象
			memset(strNumber,0x0,sizeof(strNumber));
			insComma( kei_systemtax, strNumber );
			snprintf(buf, LEN_HINMEI + LEN_COUNT + 1, "%s", "                                          ");
			snprintf(buf, sizeof("(10%対象12345678901234567890123)"), "(%2d%対象%23s)", taxrate, strNumber);
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("(10%対象12345678901234567890123)"));

			// * -- 改行
			ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));
		}

		if ( kei_bumontax != 0 ) {
			// displayMsg(diff_taxrate);

			// * --  8%対象
			memset(strNumber,0x0,sizeof(strNumber));
			insComma( kei_bumontax, strNumber );
			snprintf(buf, LEN_HINMEI + LEN_COUNT + 1, "%s", "                                          ");
			snprintf(buf, sizeof("(08%対象12345678901234567890123)"), "(%2d%対象%23s)", diff_taxrate, strNumber);
			ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("(10%対象12345678901234567890123)"));

			// * -- 改行
			ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));
		}

		// * -- お預り金額
		memset(strNumber,0x0,sizeof(strNumber));
		insComma( atoln(Genkin, sizeof(Genkin)), strNumber );
		snprintf(buf, LEN_HINMEI + LEN_COUNT + 1, "%s", "                                    ");
		snprintf(buf, sizeof("お預り12345678901234567890123456"), "お預り%26s", strNumber);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("お預り12345678901234567890123456"));

		// * -- 改行
		ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));

		// * -- 文字拡大
		ret = rputs(PORT_BLUETOOTH, bSizeDbl, sizeof(bSizeDbl));

		// * -- お釣り
		memset(strNumber,0x0,sizeof(strNumber));
		insComma( lngOturi, strNumber );
		snprintf(buf, LEN_HINMEI+1, "%s", "                                    ");
		snprintf(buf, sizeof("お釣り1234567890"), "お釣り%10s", strNumber);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, sizeof("お釣り1234567890"));

		// * -- 改行
		ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));

		// * -- 文字戻す
		ret = rputs(PORT_BLUETOOTH, bSizeNor, sizeof(bSizeNor));


	}

	return ret;

}

int	PrintMain_NEX_M230(char* _addr, bool _crc, int _mode , short _Flag ) {
	bt_conf_t conf;
	int ret;
	int retry_count;
	unsigned char addr[13];
	unsigned char name[41];
	char buf[256];//転送用バッファ
	//各種ステータスを調べるためのコマンド(DLE EOT n)
	char printer_status[] = { 0x10, 0x1b, 0x76, 0x01 };

	progress_start(LCD_PROGRESS_USE_ANIMATION, "印刷中");
	progress_update(NULL, NULL, 0);
	
restart:
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
	
	// * ---------------------------------------------------- 
	// * 	接続テストの場合、下記の五行を有効する
	// * ---------------------------------------------------- 
	// ret = EzBtSelect(EZ_BT_MODE_MENU, addr, name);
	// if (ret != 0) {
	// 	sprintf(buf, "EzBtSelect %d", ret);
	// 	goto exit;
	// }

	// * ---------------------------------------------------- 
	// * 	接続テストではない場合、前回接続成功のアドレスで接続
	// * ---------------------------------------------------- 
	BtGetLastAddr(addr);

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
		
	// * ------------------------  印字構築部分開始

	// * -- 日本語
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));

	// * -- 中央寄せ
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignCenter, sizeof(bAlignCenter));

	// * -- 文字拡大
	// ret = rputs(PORT_BLUETOOTH, bSizeTri, sizeof(bSizeTri));
	ret = rputs(PORT_BLUETOOTH, bJPx4Set, sizeof(bJPx4Set));

	// * -- タイトル
	memset(buf,0x0, sizeof(buf));
	sprintf(buf, "ユアーズコメヤ\n");
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));

	// * -- 文字サイズ戻す
	ret = rputs(PORT_BLUETOOTH, bJPx4Release, sizeof(bJPx4Release));

	// * -- 左寄せ
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignLeft, sizeof(bAlignLeft));

	// * -- YY/MM/dd
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

	// * -- 改行
	ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- 改行

	// * -- 日本語
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignLeft, sizeof(bAlignLeft));

	// * -- 明細印字
	entryprintdata();

	// * -- Printer & Line Feed
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)nFeedLine, sizeof(nFeedLine));

	// * -- カット
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bCut, sizeof(bCut));

	// * ------------------------  印字構築部分終了
	/////////////////////////////////////////
	//short rputs(int port, unsigned char *ptr, short size)
	//通信ポートportの送信バッファにptrで指定したバッファからsize分の文字を書き込む。
	/////////////////////////////////////////
	
	//プリンターステータス
	// rxbclr(PORT_BLUETOOTH);
	// ret = rputs(PORT_BLUETOOTH, (unsigned char *)printer_status, sizeof(printer_status));
	// ret = rgetc(PORT_BLUETOOTH);
	// curp(0, 9);
	// if (ret & RSTAT_BIT_RXDATA) {
	// 	sprintf(buf, "printer %02x", ret & 0xFF);
	// } else {
	// 	// 「状態 20なら OK」
	// 	sprintf(buf, "  状態   %02x", (ret >> 8) & 0xFF);
	// }
	// putsc(buf);
	
exit:
	rclose(PORT_BLUETOOTH);
	cls();
	return 0;
}
//****************************************************************************/
// 	データ登録 							
//****************************************************************************/
static void entrydata( short sw ){
	memcpy( infour.Code1, info.Code1 , sizeof( infour.Code1 ));
	memcpy( infour.Code2, info.Code2 , sizeof( infour.Code2 ));
	memcpy( infour.Code3, info.Code3 , sizeof( infour.Code3 ));

	memcpy( infour.Joudai, info.Joudai , sizeof( infour.Joudai ));
	memcpy( infour.Baika, info.Baika , sizeof( infour.Baika ));
	memcpy( infour.Num, info.Num , sizeof( infour.Num ));

	// * 税区
	infour.lngZeiku = info.lngZeiku;
	// * 最終採用税率
	infour.lngFinalTaxRate = info.lngFinalTaxRate;
	// * 最終採用税額(数量加味)
	infour.lngFinalTaxRateTax = info.lngFinalTaxRateTax;
	// * システム税率と違う税率を採用した税額(数量加味)
	infour.lngBumonTaxRateTax = info.lngBumonTaxRateTax;
	// * システム税率を採用した税額(数量加味)
	infour.lngSystemTaxRateTax = info.lngSystemTaxRateTax;

	if( sw ){
		// 新規登録 
		ram_write( ctrl.InfoUrCnt, &infour, INFOURF );
		++ctrl.InfoUrCnt;
		ram_write( 0, &ctrl, CTRLF );
	} else {
		//最後に登録したデータを更新 
		ram_write( ctrl.InfoUrCnt-1, &infour, INFOURF );
	}
	
	//beep( 10, 1 );
	meisaiclr();
}

//****************************************************************************/
// 	売上精算、売上データ登録 							
//****************************************************************************/
static void entryUriage(){
	char	now[20];
	int     intCnt;
	char 	RecNo[6];//,chkGenkin[6];
	
	sprintf( RecNo, "%06d",ctrl.RecNo);
	if( memcmp( info.tanto,"00",2 ) != 0 ){
		for ( intCnt = 0; intCnt < ctrl.InfoUrCnt; intCnt++) {
			ram_read( intCnt , &infour, INFOURF );
			memcpy( urdata.Tanto, info.tanto , sizeof( urdata.Tanto ));
			memcpy( urdata.Shop,  info.tenpo1, sizeof( urdata.Shop ));
			memcpy( urdata.RejiNo, ctrl.RejiNo , sizeof( urdata.RejiNo ));
			
			memcpy( urdata.SyouhiFlg, "0" , sizeof( urdata.SyouhiFlg ));
			memcpy( urdata.Reason, "00" , sizeof( urdata.Reason ));
			memcpy( urdata.RecNo, RecNo , sizeof( urdata.RecNo ));

			memcpy( urdata.Code1, infour.Code1 , sizeof( urdata.Code1 ));
			memcpy( urdata.Code2, infour.Code2 , sizeof( urdata.Code2 ));
			memcpy( urdata.Code3, infour.Code3 , sizeof( urdata.Code3 ));
			memcpy( urdata.Name , infour.Name  , sizeof( urdata.Name ));
			memcpy( urdata.Joudai, infour.Joudai , sizeof( urdata.Joudai ));
			memcpy( urdata.Baika, infour.Baika , sizeof( urdata.Baika ));

			urdata.lngZeiku = infour.lngZeiku;
			urdata.lngFinalTaxRate = infour.lngFinalTaxRate;
			urdata.lngFinalTaxRateTax = infour.lngFinalTaxRateTax;
			urdata.lngBumonTaxRateTax = infour.lngBumonTaxRateTax;
			urdata.lngSystemTaxRateTax = infour.lngSystemTaxRateTax;

			// * 20201208
			if(HinsyuFindByCode1(urdata.Code1) != 0) {
				memcpy(urdata.Name, himst.Name, sizeof(urdata.Name));
				// * ---------------------------------------------------- DEBUG用
				// displayStringMsg(&himst.Name);
				// * ---------------------------------------------------- DEBUG用
			}

			memcpy( urdata.Num , infour.Num , sizeof( urdata.Num ));
			if( intCnt == ctrl.InfoUrCnt - 1 ){
				ram_read( 0  , &infour2, INFOURF2 );
				//sprintf( chkGenkin,"%06d", atoln( infour2.Genkin, sizeof( infour2.Genkin ) ) - infour2.Oturi );
				//memcpy( urdata.Genkin, chkGenkin , 6 );//sizeof( urdata.Genkin ));
				memcpy( urdata.Genkin, infour2.Genkin ,6);
				memcpy( urdata.Credit ,infour2.Credit ,6 );//sizeof( urdata.Credit ));
				memcpy( urdata.Kinken, infour2.Kinken ,6 );//sizeof( urdata.Kinken ));
				memcpy( urdata.Coupon, infour2.Coupon ,6 );//sizeof( urdata.Coupon ));
				memcpy( urdata.KyakuNo , "1" , sizeof( urdata.KyakuNo ));
				urdata.lngOturi = lngOturi;
			}else{
				memcpy( urdata.Genkin, "000000" , sizeof( urdata.Genkin ));
				memcpy( urdata.Credit ,"000000" , sizeof( urdata.Credit ));
				memcpy( urdata.Kinken, "000000" , sizeof( urdata.Kinken ));
				memcpy( urdata.Coupon, "000000" , sizeof( urdata.Coupon ));
				memcpy( urdata.KyakuNo , "0" , sizeof( urdata.KyakuNo ));
				urdata.lngOturi = 0;
			}

			// 新規登録 
			getrtc4( now );
			memcpy( urdata.Date, now, sizeof( urdata.Date ) + sizeof( urdata.Time ) );
			memcpy( infour.Date, now, sizeof( infour.Date ) + sizeof( infour.Time ) );
			
			// ZNATZ_CHANGEDATE						
			memcpy( urdata.Date, Year , 4);			// ZNATZ INSERT Year
			memcpy( &urdata.Date[4], Month , 2);		// ZNATZ INSERT MONTH
			memcpy( &urdata.Date[6], Day , 2);		// ZNATZ INSERT Day
			
			ram_write( ctrl.URDataCnt, &urdata, URDATAF );
			++ctrl.URDataCnt;
			ram_write( 0, &ctrl, CTRLF );
		}
	}
	
	beep( 10, 2 );
	lngOturi = 0;
	lngOturiChk = 0;
	lngGoukei = 0;
	lngTensuu = 0;
	
	memcpy( infour2.Genkin ,0x00 , sizeof( infour2.Genkin ));
	memcpy( infour2.Credit ,0x00 , sizeof( infour2.Credit ));
	memcpy( infour2.Kinken ,0x00 , sizeof( infour2.Kinken ));
	memcpy( infour2.Coupon ,0x00 , sizeof( infour2.Coupon ));
	infour2.Oturi = 0;
	ram_write( 0, &infour2, INFOURF2 );
	meisaiclr();
}

//****************************************************************************/
/* 	印刷設定
/*	Flag=1:日計表
/*	Flag=2:レシート
/*	Flag=3:値札(不使用)
//****************************************************************************/
int print( short Flag ){
	bt_conf_t conf;
	int ret ,intMsg = 0;
	unsigned char addr[13];
	unsigned char name[41];
	char buf[256] , strGoukei[MAXLEN] ;
	int key;
	
	long lngOriginalGoukei = lngGoukei;
	
	short taxrate;
	taxrate = atoin( ctrl.TaxRate, sizeof(ctrl.TaxRate)) + 100.0;
	
	// TODO
	// if ( reprint == 0 && teisei == 0 ) lngGoukei = calculateTax(lngGoukei, taxrate); // ZNATZ
	
	if( Flag == 0 ){//日計表		
		ckputss( 0,  0, "  <日計表印刷>  ", False, CLR_UR_TITLE );
	} else if( Flag == 1 ){//ﾚｼｰﾄ	
	} else if( Flag == 3 ){
		cls();
	}
	
	ret = BtConfig(BT_GET, &conf);
	if (ret != 0) {
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "BtConfig %d", ret);
		msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, NULL, NULL); 
	} else {
		// PINコード 
		strncpy(conf.localPIN, "0000", sizeof(conf.localPIN));
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
		// * レシート印刷部分に行く
		if ( Flag == 1 ) {
			break;
		}

		ckputss( 1,  3, "1.日計表印刷  ", False, CLR_BASE );
		ckputss( 1,  5, "2.接続確認    ", False, CLR_BASE );
		ckputss( 1,  7, "3.印刷終了    ", False, CLR_BASE );	
		ckputss( 0,  12, "  ﾌﾟﾘﾝﾀ ｱﾄﾞﾚｽ　 ", False, CLR_UR_TITLE );
		
		if (strlen((char*)addr) == 0) {
			ckputss( 1,  14, " 未設定", False, CLR_BASE );
		} else {
			ckputsn( 1,  14, (char *)addr, 16, False, CLR_BASE );
		}
		key = getch();
		
		if (key == '1') {
			while( 1 ){
				intMsg = PrintMain_NEX_M230((char*)addr, false, 0 , Flag );
				if( intMsg == 0 )break;
			}
			Display( 0 );
			break;
		} else if (key == '2') {
			ret = EzBtSelect(EZ_BT_MODE_MENU, addr, name);
			if (ret != 0 && ret != -10) {
				memset(buf, 0x00, sizeof(buf));
				sprintf(buf, "EzBtSelect %d", ret);
				msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, NULL, NULL); 
			}
			// * 20201211
			Display( 0 );
			break;
		} else if (key == '3'){
			Display( 0 );
			break;
		}
	}
	
	
	if( Flag == 1 ){
		memcpy( infour2.Genkin, Genkin , sizeof( infour2.Genkin ));
		memcpy( infour2.Credit, Credit , sizeof( infour2.Credit ));
		memcpy( infour2.Kinken, Kinken , sizeof( infour2.Kinken ));
		memcpy( infour2.Coupon, Coupon , sizeof( infour2.Coupon ));
		
		memcpy( infour2.Date, Year , 4);		// ZNATZ INSERT Year
		memcpy( &infour2.Date[4], Month , 2);	// ZNATZ INSERT MONTH
		memcpy( &infour2.Date[6], Day , 2);		// ZNATZ INSERT Day
		
		infour2.Oturi = lngOturi;
		infour2.Goukei = lngGoukei;
		infour2.Tensuu = lngTensuu;
		ram_write( 0, &infour2, INFOURF2 );
		while( 1 ){
			// 本格印刷 ZNATZ_PRINT
			intMsg = PrintMain_NEX_M230((char*)addr, false, 0 , Flag );
			if( intMsg == 0 ) { return 0;}
		}
	}
	
	// ZNAT_PRINT_RYOUSYU
	if ( Flag == 2 ) {
		PrintRyousyu((char*)addr, false, 0 ,0);
	}
	lngGoukei = lngOriginalGoukei;
	
	return 1;
}

//*****************************************************************************
// 	各種処理    															 
//*****************************************************************************
void uriage( int flag, int firsttime )
{
	short item, ret ;
	char now[8];
	long lngTmpKin = 0 , lngTmpSuu = 0;
	
	char strGoukei[MAXLEN] ;
	char log[MAXLEN] ;
	
	short taxrate;
	taxrate = atoin( ctrl.TaxRate, sizeof(ctrl.TaxRate)) + 100.0;
	reprint = 0;
	teisei  = 0;
	
	//bt_conf_t conf;//#01
	//unsigned char addr[13];//#01

	infoclr();
	setpict( 1 );

	if( MaxCheck( ctrl.URDataCnt, URDATA_MAX ) ) return;
	/*
	if( memcmp( ctrl.RecPrint,"1",1 ) != 0 && memcmp( ctrl.RecPrint,"2",1 ) != 0 ){
		msgbox( LCD_MSGBOX_WARNING, 0x00, "", "初めに設定を行って下さい", "確認", 0x00 );
		menu();
	}
	*/
	
	// ZNATZ CHANGE_DATE
	if (firsttime == 0) {
		getrtc4( now );
		memcpy( Year, now,4 );
		memcpy( Month, &now[4],2 );
		memcpy( Day, &now[6],2 );
		firsttime ++;
	}
	ctrl.InfoUrCnt = 0; 
	item = YEAR;

	SaveItem = -1;
	while( item ){
		Display( item );
		switch( item ){
			case YEAR:
				ret = NumInput( 5, 6, Year, sizeof( Year ), 0, 9999L, 
							KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
				break;
			case MONTH:
				ret = NumInput(10, 6, Month, sizeof( Month ), 0, 99L, 
							KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
				break;
			case DAY:
				ctrl.InfoUrCnt = 0;
				lngGoukei = 0;
				lngTensuu = 0;
				ret = NumInput(13, 6, Day, sizeof( Day ), 0, 99L, 
							KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
				break;
			case SHOP:
				ctrl.InfoUrCnt = 0;
				lngGoukei = 0;
				lngTensuu = 0;
				ret = NumInput( 6, 6, info.tenpo1, sizeof( info.tenpo1 ), 0, 99L, IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
				break;
			case TANTO:
				ctrl.InfoUrCnt = 0;
				lngGoukei = 0;
				lngTensuu = 0;
				ret = NumInput( 6, 10, info.tanto, sizeof( info.tanto ),  0, 99L, IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
				break;
			case CODE1:
				ret = CodeInput( 3, 2, info.Code1, sizeof( info.Code1 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;			
			case CODE2:
				ret = CodeInput( 3, 4, info.Code2, sizeof( info.Code2 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;				
			case CODE3:
				ret = CodeInput8orNull( 5, 6, info.Code3, sizeof( info.Code3 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC | KEY_SKIP );
				break;				
			case NUM:
				memcpy( info.Num ,"0001" ,sizeof( info.Num )); 
				ret = NumInput( 12, 9, info.Num, sizeof( info.Num ), 0, 99999L,
								IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
				break;
			case DENTRY:
				entrydata( True );
				item = CODE1;
				break;
			case GENKIN:
				ret = NumInput( 10, 6, Genkin, 6, 0, 999999L,
								IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
				break;
			case CREDIT:
				ret = NumInput( 10, 8, Credit, 6, 0, 999999L,	
								IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
				break;
			case SEISAN:
				if( memcmp( ctrl.RecPrint,"1",1 ) == 0 ){
					ret = 13;
				} else {
					ret = getch();
				}
				if( ret == 97 ){//F1
					if( memcmp( info.tanto,"00",2 ) != 0 ){
						--ctrl.RecNo;
						if( ctrl.RecNo < 1 ) ctrl.RecNo = 999999;
					}
					lngOturi = 0;
					memcpy( Genkin , "000000" , sizeof( Genkin ) );
					memcpy( Kinken , "000000", sizeof( Kinken ) );
					item = GENKIN;
				}else if( ret == 13 ){//ENT 精算開始
					reprint = 0;
					teisei = 0;
					entryUriage();
					item = CODE1;
					ctrl.InfoUrCnt = 0;
					lngGoukei = 0;
					lngTensuu = 0;
				}
				break;
			case KAKUNIN:
				meisaiclr();
				infoclrExceptTenpoTanto();
				entrydata(1);
				SaveItem = TANTO;
				item = CODE1;

				/*
				ret = getch();
				if( ret == 97 ){//F1
					item = YEAR;
				}
				*/
				break;
			case RYOUSYU:
				// ZNATZ_PRINT
				ret = getch();
				if ( ret == 97 ) {
					teisei = 1;
					item = GENKIN;
				} else if ( ret == 98 ) {
							print(1);
							reprint = 1;
							item=RYOUSYU; 

					//infoclr();
					//menu();
				} else if ( ret == 99 ) {
					// * 再発行
					print(2);
				} else if ( ret == 13) {
					// ZNATZ RESET
					reprint = 0;
					teisei = 0;
//						++ctrl.RecNo;
//						if( ctrl.RecNo > 999999 )ctrl.RecNo = 1;
					entryUriage();
					item = CODE1;
					ctrl.InfoUrCnt = 0;
					lngGoukei = 0;
					lngTensuu = 0;
				}
				break;
		}
		if( ret == ENTRY || ret == SENTRY || ret == SKIP || ret == BARENT ){
			if( item == YEAR ){//年指定
				if( atoln( Year, sizeof( Year ) ) >= 2000 ){
					item = MONTH;
				}else{
					memset( Year, 0x00, sizeof( Year ) );
					beeb(10,2, 1);
				}
				continue;
			}else if( item == MONTH ){//月指定
				if( atoln( Month, sizeof( Month ) ) > 0 && atoln( Month, sizeof( Month ) ) <= 12 ){
					item = DAY;
				}else{
					memset( Month, 0x00, sizeof( Month ) );
					beeb(10,2, 1);
				}
				continue;
			}else if( item == DAY ){//日指定
				if( atoln( Day, sizeof( Day ) ) > 0 && atoln( Day, sizeof( Day ) ) <= 31 ){
					// 直接にCODE1に
					// item = SHOP;
					memcpy( info.tenpo1, "06", sizeof( info.tenpo1 ) );
					memcpy( info.tanto, "99", sizeof( info.tanto ) );
					item = CODE1;
				}else{
					memset( Day, 0x00, sizeof( Day ) );
					beeb(10,2, 1);
				}
				continue;
			}else if( item == SHOP ){
				if(memcmp( info.tenpo1,"00",2 ) == 0) {
					memcpy( info.tenpo1, "06", sizeof( info.tenpo1 ) );
				}
				item = TANTO;
				//ckputsn( 2, 8, mmst.Name, 14, False, CLR_BASE );
				continue;
				/*
				// 店舗マスタ検索 
				if( ShopFind( info.tenpo1 ) == -1 ){
					beeb(10,2, 1);
					memcpy( info.tenpo1, 0x00 , sizeof( info.tenpo1 ) );
					continue;
				}else{//該当ﾃﾞｰﾀが存在
					item = TANTO;
					ckputsn( 2, 8, mmst.Name, 14, False, CLR_BASE );
					continue;
				}
				*/
				
			} else if( item == TANTO ){
				if(memcmp( info.tanto,"00",2 ) == 0) {
					memcpy( info.tanto, "99", sizeof( info.tenpo1 ) );
				}
				item = CODE1;
				continue;	
				/*
				// 担当マスタ検索 
				if( atoln( info.tanto, sizeof( info.tanto ) ) == 0 ){
					item = CODE1;
					ckputsn( 3, 12, "練習モード", 10, False, CLR_BASE );
					continue;
				}
				if( TantoFind( info.tanto ) == -1 ){
					beeb(10,2, 1);
					memset( info.tanto, 0x00, sizeof( info.tanto ) );
					continue;
				}else{//該当ﾃﾞｰﾀが存在
					item = CODE1;
					ckputsn( 3, 12, tmst.Name, 13, False, CLR_BASE );
					continue;
				}
				*/
				
				lngGoukei = 0;
				
			} else if( item == CODE1 ){
				if( MaxCheck( ctrl.URDataCnt, URDATA_MAX ) ) return;//@01

				if( info.Code1[0] ){

					if( ctrl.InfoUrCnt == INFOUR_MAX ){//件数を超える場合は登録不可
						beeb(10,2, 1);
						memset( info.Code1, 0x00, sizeof( info.Code1 ) );
						lngBaika = 0;
						continue;
					}

					//3桁が501～999の場合
					if( atoin( info.Code1, 3 ) > 500 && atoin( info.Code1, 3 ) < 999 ){

						// * 品種検索 20201208
						if(HinsyuFindByCode1(info.Code1) != 0) {
							char _buf[10];
							sprintf(_buf, "%010d", getJyoudai(info.Code1));
							memcpy(info.Joudai, _buf, sizeof(info.Joudai));
							lngBaika = getZeikomiKingaku(info.Code1, 1);
							// * ---------------------------------------------------- DEBUG用
							// displayStringMsg(&himst.Name);
							// * ---------------------------------------------------- DEBUG用
						} else {
							beep( 10, 2 );
							meisaiclr();
							continue;
						}

						item = CODE2;
						continue;
					} else {
						beep( 10, 2 );
						meisaiclr();
						continue;
					}
				}
			} else if( item == CODE2 ){

				if( MaxCheck( ctrl.URDataCnt, URDATA_MAX ) ) return;//@01

				if( ctrl.InfoUrCnt == INFOUR_MAX ){//件数を超える場合は登録不可
					beeb(10,2, 1);
					memset( info.Code1, 0x00, sizeof( info.Code1 ) );
					lngBaika = 0;
					continue;
				}
				/* 上段コードの先頭から３桁が501～999の場合、０９９９、１、３から始まるコード以外不可 */
				if( atoin( info.Code1, 3 ) > 500 && atoin( info.Code1, 3 ) < 999 ){
					if( (info.Code2[0] == '0' || info.Code2[0] == '1' || info.Code2[0] == '2' || info.Code2[0] == '3') && info.Code2[12] != ' ' ){
						item = CODE3;
						continue;
					} else {
						/* 上記に該当しない場合はエラー */
						memset( info.Code2, 0x00, sizeof( info.Code2 ) );
						beep( 10, 2 );
						continue;
					}
				} else {
					/* 上記に該当しない場合はエラー */
					memset( info.Code2, 0x00, sizeof( info.Code2 ) );
					beep( 10, 2 );
					continue;
				}
						
			} else if( item == CODE3 ){

				if( ctrl.InfoUrCnt == INFOUR_MAX ){//件数を超える場合は登録不可
					beeb(10,2, 1);
					memset( info.Code1, 0x00, sizeof( info.Code1 ) );
					memset( info.Code2, 0x00, sizeof( info.Code2 ) );
					memset( info.Code3, 0x00, sizeof( info.Code3 ) );
					lngJoudai = 0;
					lngBaika = 0;
					lngSingleTax = 0;
					continue;
				}

				// * -------------------------------------------------------------------------------------------------　売変無し
				if ( ret == SKIP ) {

					// * 20201207 売変ない場合はそのまま税込上代
					// * 売価確定
					lngBaika = getZeikomiKingaku(info.Code1, 1);
					char _buf[10];
					sprintf(_buf, "%010d", lngBaika);
					memcpy(info.Baika, _buf, sizeof(info.Baika));

					// * 上代確定 20201208
					lngJoudai = getJyoudai(info.Code1);

					// * 税区より一品消費税確定
					long lngCheckZeiku = HinsyuZeikuFindByCode1(info.Code1);
					if ( lngCheckZeiku == 0 ) {
						// * 外税
						lngSingleTax = lngBaika - lngJoudai;
					} else if ( lngCheckZeiku == 1 ) {
						// * 内税
						lngSingleTax = calculateTax2(lngJoudai, BumonTaxFindByCode1(info.Code1));
					} else {
						// * 非課税
						lngSingleTax = 0;
					}

					// displayMsg(lngBaika);
					item = NUM;
					continue;
				}

				// * -------------------------------------------------------------------------------------------------　売変有り
				if( info.Code3[0] ){
					if( !memcmp( info.Code3, "30", 2 ) && info.Code3[7] != ' ' ){

						// TODO 20201207 売変の税込金額 システム税率, 外税
						// * 売価確定
						long lngNesageKingaku = calculateTax2(GetNesage(info.Code3, info.Joudai), taxrate);
						lngBaika = lngNesageKingaku;

						// TODO 20201208
						// * 一品消費税確定(売変)
						lngSingleTax = lngNesageKingaku - GetNesage(info.Code3,info.Joudai);

						char _buf[10];
						sprintf(_buf, "%010d", lngBaika);
						memcpy(info.Baika, _buf, sizeof(info.Baika));

						item = NUM;
						continue;
					} else {
						// /* 上記に該当しない場合はエラー */
						beep( 10, 2 );
						memset( info.Code3, 0x00, sizeof( info.Code3 ) );
						continue;
					}
				} 			
 			} else if( item == NUM ){

				 long current_input_num = atoln(info.Num, sizeof(info.Num));
				// * 0は弾く
				if( current_input_num == 0 ){
					beeb(10,2, 1);
					continue;
				}
				// * マイナスを含まない場合、入力値が９９９を超えたらエラーとする 
				if( current_input_num >= 0 && current_input_num > 999 ){
					beeb(10,2, 1);
					continue;
				}
				// * マイナスを含む場合、入力値が-９９９を超えたらエラーとする 
				if( current_input_num < 0 && current_input_num <= -1000 ){
					beeb(10,2, 1);
					continue;
				}

				// * ┌------------------------------------------------------------------------------- 
				// * | lngTmpKin, lngTmpSuu で一時現在小計情報を保存
				// * | 金額オバーチェック
				// * └------------------------------------------------------------------------------- 
				lngTmpKin = lngGoukei;
				lngGoukei = lngGoukei + lngBaika * current_input_num; 
				
				if( lngGoukei > 999999 || lngGoukei < -99999 ){
					lngGoukei = lngTmpKin;
					beep( 50 , 1 );
					continue;
				}

				// * ┌------------------------------------------------------------------------------- 
				// * | lngTmpKin, lngTmpSuu で一時現在小計情報を保存
				// * | 数量オバーチェック
				// * └------------------------------------------------------------------------------- 
				lngTmpSuu = lngTensuu;
				lngTensuu = lngTensuu + current_input_num ; 

				if( lngTensuu > 999 || lngTensuu < -99 ){
					lngGoukei = lngTmpKin;
					lngTensuu = lngTmpSuu;
					beep( 50 , 1 );
					continue;
				}

				// * ┌------------------------------------------------------------------------------- 
				// * | !! 
				// * | ※ OKの場合、Info -> Infourに確定
				// * | !! 
				// * └------------------------------------------------------------------------------- 

				// * 税率 値下: system   以外: 部門
				long lngTmp;
				if( !memcmp( info.Code3, "30", 2 ) && info.Code3[7] != ' ' ){
					// * 値下
					// * 税区確定
					info.lngZeiku = 0;
					// * 税率確定
					info.lngFinalTaxRate = taxrate - 100;
					// * 税額確定
					info.lngFinalTaxRateTax = lngSingleTax * current_input_num;
					// * 部門税額
					info.lngBumonTaxRateTax = 0;
					// * システム税額
					info.lngSystemTaxRateTax = info.lngFinalTaxRateTax;
				} else {
					// * 税区確定
					info.lngZeiku = HinsyuZeikuFindByCode1(info.Code1);
					// * 税率確定
					info.lngFinalTaxRate = BumonTaxFindByCode1(info.Code1);
					// * 税額確定
					info.lngFinalTaxRateTax = lngSingleTax * current_input_num;
					if ( info.lngFinalTaxRate != (taxrate - 100) ) {
						// * 部門税額
						info.lngBumonTaxRateTax = info.lngFinalTaxRateTax;
						// * システム税額
						info.lngSystemTaxRateTax = 0;
					} else {
						// * 部門税額
						info.lngBumonTaxRateTax = 0;
						// * システム税額
						info.lngSystemTaxRateTax = info.lngFinalTaxRateTax;
					}
				}

				// * --------------------------------------------------- Debug用
				// displayMsg(info.lngFinalTaxRate);
				// displayMsg(info.lngFinalTaxRateTax);
				// displayMsg(info.lngBumonTaxRateTax);
				// displayMsg(info.lngSystemTaxRateTax);
				// * --------------------------------------------------- Debug用

				item = DENTRY;

				continue;

			} else if( item == GENKIN ){
				
				if ( lngGoukei < 0 ) { item = CREDIT; continue; }

				// * 0円で入力した場合、売掛に移す
				long current_cash = atoln( Genkin, sizeof( Genkin ));
				if ( current_cash == 0 ) {
					item = CREDIT; 
					continue;
				}

				// * 売掛既に入力した場合、現金を0にセット
				long current_credit = atoln( Credit, sizeof( Credit ));
				if ( current_credit > 0 ) {
					memcpy( Genkin , "000000" , sizeof( Genkin ) );
					item = CREDIT;
					continue;
				}

				// * お釣りが出る場合のみ計算
				if( current_cash >= lngGoukei ){
					
					lngOturi = current_cash - lngGoukei ;
						
					if( lngOturi > 9999 ){
						beep( 50,1 );
						memcpy( Genkin , "000000" , sizeof( Genkin ) );
						lngOturi = 0;
						continue;
					}
					
					ckputss( 0, 10, " お釣り :          ", False, CLR_BASE );
					insComma( lngOturi, strGoukei );
					ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
					
					
					if( memcmp( info.tanto,"00",2 ) != 0 && reprint == 0 && teisei == 0 ) {
						++ctrl.RecNo;
						if( ctrl.RecNo > 999999 )ctrl.RecNo = 1;
					}

					if( memcmp( ctrl.RecPrint,"1",1 ) == 0 ){
						// * 成功なら印刷 ZNATZ_PRINT reprint=1なら元画面戻る
						if ( print( 1 ) == 0 ) {
							reprint = 1;
							item=RYOUSYU; //GENKIN
						}
						continue;
					}else{
						memcpy( infour2.Genkin, Genkin , sizeof( infour2.Genkin ));
						memcpy( infour2.Credit, Credit , sizeof( infour2.Credit ));
						memcpy( infour2.Kinken, Kinken , sizeof( infour2.Kinken ));
						memcpy( infour2.Coupon, Coupon , sizeof( infour2.Coupon ));
						ram_write( 0, &infour2, INFOURF2 );
					}
					item = SEISAN;
					continue;
				}else{
					lngOturi = 0;
					item = CREDIT;
				}
				continue;
				
			} else if( item == CREDIT ){
				
				long current_cash = atoln( Genkin, sizeof( Genkin ));
				long current_credit = atoln( Credit, sizeof( Credit ));
				lngOturiChk = current_cash + current_credit;
				
				//クレジットが小計を超えていないかチェック
				if( lngGoukei >= 0 ){
					if ( teisei == 0 ) {
						// TODO
						if( current_credit > lngGoukei ){	
							beeb(10,2, 1);
							memset( Credit, 0x00, sizeof( Credit ) );
							lngOturi = 0;
							item = CREDIT;
							continue;
						}
						if( lngOturiChk < lngGoukei ){ item = GENKIN; continue;}
						
						lngOturi = lngOturiChk - lngGoukei ; 
					}
					if ( teisei == 1 ) {
						if( current_credit > lngGoukei ){	
							beeb(10,2, 1);
							memset( Credit, 0x00, sizeof( Credit ) );
							lngOturi = 0;
							item = CREDIT;
							continue;
						}
						if( lngOturiChk < lngGoukei ){ item = GENKIN; continue;}
						lngOturi = lngOturiChk - lngGoukei ; 
					}
				} else {
					lngOturi = lngOturiChk - lngGoukei ;
				}
				
				
					
				ckputss( 0, 10, " お釣り :          ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
				
				if( memcmp( info.tanto,"00",2 ) != 0 && reprint == 0 && teisei == 0 ){
					++ctrl.RecNo;
					if( ctrl.RecNo > 999999 )ctrl.RecNo = 1;
				}
				
				if( memcmp( ctrl.RecPrint,"1",1 ) == 0 ){
					if ( print( 1 ) == 0 ) 
						{
							reprint = 1;
							item=RYOUSYU; //GENKIN
						}
						continue;
					}else{
						memcpy( infour2.Genkin, Genkin , sizeof( infour2.Genkin ));
						memcpy( infour2.Credit, Credit , sizeof( infour2.Credit ));
						memcpy( infour2.Kinken, Kinken , sizeof( infour2.Kinken ));
						memcpy( infour2.Coupon, Coupon , sizeof( infour2.Coupon ));
						ram_write( 0, &infour2, INFOURF2 );
					}
					item = SEISAN;
								
				continue;
				
			} else if( item == KINKEN ){
				if( lngGoukei >= 0 ){
					lngOturiChk = atoln( Kinken, sizeof( Kinken ));//#02
					if( lngOturiChk > lngGoukei ){
						lngOturi = 0;
						memcpy( Kinken , "000000", sizeof( Kinken ) );
						beeb(10,2, 1);
						continue;
					}
					lngOturi = 0;
					item = GENKIN;//#02
				}
				
				continue;
			} else if( item == COUPON ){
				lngOturi = 0;
				item = GENKIN;
				continue;

			}
		}else if( ret == F2KEY ){
			if(item == CODE1){
				beep( 50 , 1 );
				meisaiclr;
				ctrl.InfoUrCnt = 0;
				lngGoukei = 0;
				lngTensuu = 0;
				item = CODE1;
				DisplayHeader();

				ckputss( 0,  2, "▲              ", False, CLR_BASE );
				ckputss( 0,  4, "▽              ", False, CLR_BASE );				
				ckputss( 0,  6, "値下:           ", False, CLR_BASE );				

				// * 現在小計金額
				ckputss( 0, 12, "計        　　", False, CLR_SI_TITLE );
				insComma( lngGoukei, strGoukei ); 								
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7s円", strGoukei );

				// * 現在小計点数
				ckprintf(11,12, False, CLR_SI_TITLE, "%3d点", lngTensuu );

				ckputss( 0, 14, "F1:戻る  F2:取消", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:精算", False, CLR_BASE );	

			}
			continue;
		}else if( ret == F1KEY ){//戻る
			if( item == NUM ){
				// TODO 20201123 税計算
				// * 20201207 未確定なので、戻す必要がなくなった。
				// lngGoukei = lngGoukei - getZeikomiKingaku(info.Code1, 1);
				// lngTensuu = lngTensuu - 1; 
				meisaiclr();
				item = CODE1;
				continue;
			}else if( item == CODE2 ) {
				memset( infour.Code1, 0x00, sizeof( infour.Code1 ) );
				memset( infour.Code2, 0x00, sizeof( infour.Code2 ) );
				memset( infour.Code3, 0x00, sizeof( infour.Code3 ) );
				memset( infour.Num, 0x00, sizeof( infour.Num ) );

				memset( info.Code1, 0x00, sizeof( info.Code1 ) );
				memset( info.Code2, 0x00, sizeof( info.Code2 ) );
				memset( info.Code3, 0x00, sizeof( info.Code3 ) );
				memset( info.Num, 0x00, sizeof( info.Num ) );
				item = CODE1;
				continue;
			}else if( item == CODE3 ) {
				memset( infour.Code2, 0x00, sizeof( infour.Code2 ) );
				memset( infour.Code3, 0x00, sizeof( infour.Code3 ) );
				memset( infour.Num, 0x00, sizeof( infour.Num ) );

				memset( info.Code2, 0x00, sizeof( info.Code2 ) );
				memset( info.Code3, 0x00, sizeof( info.Code3 ) );
				memset( info.Num, 0x00, sizeof( info.Num ) );
				item = CODE2;
				continue;
			}else if( item == YEAR ){
				infoclr();
				menu();
			}else if( item == GENKIN ){
				lngOturi = 0;
				/*if( memcmp( ctrl.RecPrint,"1",1 ) == 0 ){//#01
					LapinBTDisconnect();
				}*/
				item = CODE1;
				if ( teisei == 1 ) item = GENKIN;
				continue;
			}else if( item == CREDIT || item == KINKEN || item == COUPON || item == SEISAN ){
				lngOturi = 0;
				item = GENKIN;
				continue;
			}	
			infoclr();
			item = YEAR;
			continue;
		}else if( ret == F4KEY ){//日計表・現金
			if( item ==  YEAR || item == MONTH || item == DAY || item == SHOP || item == TANTO ){
				cls();
				print( 0 );
				continue;
			}else if( item == CODE1 || item == CODE2 ){
				if( lngTensuu != 0 ){
					memcpy( Genkin , "000000" , sizeof( Genkin ) );
					memcpy( Credit , "000000", sizeof( Credit ) );
					memcpy( Kinken , "000000", sizeof( Kinken ) );
					memcpy( Coupon , "000000", sizeof( Coupon ) );
					item = GENKIN;
				}
				continue;
			}
		}
	}
}