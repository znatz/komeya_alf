//////////////////////////////////
// ç≈èIçXêV 2013/04/26
// #01:è¨åvéûBTê⁄ë±
// #02:ÉNÉåÉWÉbÉg,è§ïiåîèàóùâèú
//
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
static char		Baika[6];
static long		lngOturi;
static long		lngOturiChk;
static long		lngGoukei;
static long		lngTensuu;

static void		meisaiclr( void );
static void		Display( short item );
static void		entrydata( short sw );
static void		entryUriage();

// ZNATZ
static int		reprint;
static int		teisei;
static long 		calculateTax(long lngPrice, short rate);


//****************************************************************************
//* 	ñæç◊ÉfÅ[É^èâä˙âª														 */
//****************************************************************************
static void meisaiclr( void )
{	
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

//****************************************************************************
//* 	INFOÉfÅ[É^èâä˙âª														 */
//****************************************************************************
static void infoclr( void ){
	memset( info.tenpo1, 0x00, sizeof( info.tenpo1 ));
	memset( info.tenpo2, 0x00, sizeof( info.tenpo2 ));
	memset( info.tanto, 0x00, sizeof( info.tanto ));
	memset( info.Code1, 0x00, sizeof( info.Code1 ) );
	memset( info.Code2, 0x00, sizeof( info.Code2 ) );
	memset( info.Code3, 0x00, sizeof( info.Code2 ) );
	memset( info.Num, 0x00, sizeof( info.Num ) );
	memset( info.Joudai, 0x00, sizeof( info.Num ) );
//	memset( mmst.Name,0x00, sizeof( mmst.Name ) );
//	memset( tmst.Name,0x00, sizeof( tmst.Name ) );
	memset( Name,0x00, sizeof( Name ) );
	
}

static void infoclrExceptTenpoTanto( void ){
	memset( info.Code1, 0x00, sizeof( info.Code1 ) );
	memset( info.Code2, 0x00, sizeof( info.Code2 ) );
	memset( info.Code3, 0x00, sizeof( info.Code2 ) );
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

static long calculateTax(long lngPrice, short rate)
{
	// ZZZZ
	long result = lngPrice;
	return result;
}

static long calculateTax2(long lngPrice, short rate)
{
	// ZZZZ
	float t = (float) rate;
	float result = round(lngPrice * rate / 100.0);
	// TODO
	//if ( ctrl.TaxType[0] == '2') result = lngPrice * rate / 100.0;
	return (long) result;
}


static long calculateTaxNoRoundUp(long lngPrice, short rate)
{
	float t = (float) rate;
	float result;
	result = lngPrice * rate / 100;
	return (long) result;
}

/*****************************************************************************/
/* 			     															 */
/*****************************************************************************/
static void Display( short item )
{
	// ZNATZ
	long lngGenkin = 0;
	long lngCredit = 0;
	
	long jyoudaiWithTax;
	
	// è¡îÔê≈ó¶Çê›íËÇ©ÇÁéÊÇËèoÇ∑ ZNATZ
	short taxrate = atoin( ctrl.TaxRate, sizeof(ctrl.TaxRate)) + 100.0;
	
	long lngNum = 0,lngKyakuNo = 0,lngKingaku = 0 ;
	char strBaika[MAXLEN],strGoukei[MAXLEN];
	int intCnt;
	switch( item ){
		case YEAR:
			reprint = 0; teisei = 0;
			if( SaveItem != YEAR ){
				ClsColor();
				ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
				// å≥ì˙ïtå≥ÅXçsÇÕ3
				ckputss( 0,  6, "ì˙ït:    /  /  ", False, CLR_BASE );
				ckputsn(10,  6, Month, 2, False, CLR_BASE );
				ckputsn(13,  6, Day, 2, False, CLR_BASE );
				// ckputss( 0,  6, " ìXï‹:          ", False, CLR_BASE );
				// ckputss( 0, 10, " íSìñ:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:“∆≠∞ ENT:ämíË", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :ì˙åv", False, CLR_BASE );	
			}
			break;
		case MONTH:
			if( SaveItem != MONTH ){
				ClsColor();
				ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
				// å≥ì˙ïtå≥ÅXçsÇÕ3
				ckputss( 0,  6, "ì˙ït:    /  /  ", False, CLR_BASE );
				ckputsn( 5,  6, Year, 4, False, CLR_BASE );
				ckputsn( 13, 6, Day,  2, False, CLR_BASE );
				// ckputss( 0,  6, " ìXï‹:          ", False, CLR_BASE );
				// ckputss( 0, 10, " íSìñ:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:“∆≠∞ ENT:ämíË", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :ì˙åv", False, CLR_BASE );				
			}
			break;
		case DAY:
			if( SaveItem != DAY ){
				ClsColor();
				ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
				// å≥ì˙ïtå≥ÅXçsÇÕ3
				ckputss( 0,  6, "ì˙ït:    /  /   ", False, CLR_BASE );
				ckputsn( 5,  6, Year,  4, False, CLR_BASE );
				ckputsn( 10, 6, Month, 2, False, CLR_BASE );
				// ckputss( 0,  6, " ìXï‹:          ", False, CLR_BASE );
				// ckputss( 0, 10, " íSìñ:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:“∆≠∞ ENT:ämíË", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :ì˙åv", False, CLR_BASE );				
			}
			break;
		case SHOP:
			if( SaveItem != SHOP ){
				ClsColor();
				ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
				ckputss( 0,  3, " ì˙ït:    /  /   ", False, CLR_BASE );
				ckputsn( 6,  3, Year,  4, False, CLR_BASE );
				ckputsn( 11, 3, Month, 2, False, CLR_BASE );
				ckputsn( 14, 3, Day,  2, False, CLR_BASE );
				// ckputss( 0,  6, " ìXï‹:          ", False, CLR_BASE );
				// ckputss( 0, 10, " íSìñ:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:“∆≠∞ ENT:ämíË", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :ì˙åv", False, CLR_BASE );		
			}
			break;
		case TANTO:
			if( SaveItem != TANTO ){
				ClsColor();
				ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
				ckputss( 0,  3, " ì˙ït:    /  /   ", False, CLR_BASE );
				ckputsn( 6,  3, Year,  4, False, CLR_BASE );
				ckputsn( 11, 3, Month, 2, False, CLR_BASE );
				ckputsn( 14, 3, Day,  2, False, CLR_BASE );
				// ckputss( 0,  6, " ìXï‹:          ", False, CLR_BASE );
				// ckputsn( 6,  6, info.tenpo1,  2, False, CLR_BASE );
				// ckputss( 0, 10, " íSìñ:          ", False, CLR_BASE );
				ckputss( 0, 14, "F1:“∆≠∞ ENT:ämíË", False, CLR_BASE );	
				ckputss( 0, 16, "        F4 :ì˙åv", False, CLR_BASE );					
			}
			break;
		case CODE1:
			reprint = 0; teisei = 0;
			if( SaveItem != CODE1){
				ClsColor();
				if( memcmp( info.tanto,"00",2 ) == 0 ){
					ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
					ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}else{
					ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
					ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  5, "Å£              ", False, CLR_BASE );
				ckputss( 0,  7, "Å§              ", False, CLR_BASE );				
				ckputss( 0, 12, "åv        Å@Å@", False, CLR_SI_TITLE );
				insComma( calculateTax(lngGoukei, taxrate), strGoukei );			//ZNATZ
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7sâ~", strGoukei );
				ckprintf(11,12, False, CLR_SI_TITLE, "%3dì_", lngTensuu );
				ckputss( 0, 14, "F1:ñﬂÇÈ  F2:éÊè¡", False, CLR_BASE );	
				// ckputss( 0, 16, "F3:îÑïœ  F4:ê∏éZ", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:ê∏éZ", False, CLR_BASE );	
			}
			break;
		case CODE2:
			reprint = 0; teisei = 0;
			if( SaveItem != CODE2){
				ClsColor();
				if( memcmp( info.tanto,"00",2 ) == 0 ){
					ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
					ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}else{
					ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
					ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  5, "Å¢              ", False, CLR_BASE );
				ckputsn( 3,  5, info.Code1, 13, False, CLR_BASE );
				ckputss( 0,  7, "Å•              ", False, CLR_BASE );
				ckputss( 0, 12, "åv        Å@Å@", False, CLR_SI_TITLE );
				insComma( calculateTax(lngGoukei, taxrate), strGoukei );			//ZNATZ
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7sâ~", strGoukei );
				ckprintf(11,12, False, CLR_SI_TITLE, "%3dì_", lngTensuu );
				ckputss( 0, 14, "F1:ñﬂÇÈ  F2:éÊè¡", False, CLR_BASE );	
				// ckputss( 0, 16, "F3:îÑïœ  F4:ê∏éZ", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:ê∏éZ", False, CLR_BASE );	
			}
			break;			
		case NUM:
			if( SaveItem != NUM){
				ClsColor();	
				if( memcmp( info.tanto,"00",2 ) == 0 ){
					ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
					ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}else{
					ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
					ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				
				//ckputss( 0,  5, "BC:          ", False, CLR_BASE );
				//ckputsn( 3,  5, info.Code1, 13, False, CLR_BASE );
				
				ckputss( 0,  5, "Å¢              ", False, CLR_BASE );
				ckputss( 0,  7, "Å§              ", False, CLR_BASE );
				ckputsn( 3,  5, info.Code1, 13, False, CLR_BASE );
				ckputsn( 3,  7, info.Code2, 13, False, CLR_BASE );
//				ckputsn( 0,  7, btsmas.Name, 16, False, CLR_BASE ); 
				
				//ZNATZ SHOW_TAX_INCLUDED
				jyoudaiWithTax = calculateTax2(atoln( info.Joudai, sizeof( info.Joudai )), taxrate);
				insComma( jyoudaiWithTax, strBaika );		
						
				//ZNATZ SHOW_TAX_INCLUDED
				
				ckprintf(0,  9, False, CLR_BASE , "Åè%7sÅ~",strBaika );
				ckputss( 0, 12, "åv        Å@Å@", False, CLR_SI_TITLE );
				insComma( calculateTax(lngGoukei, taxrate), strGoukei ); 			//ZNATZ
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7sâ~", strGoukei );
				ckprintf(11,12, False, CLR_SI_TITLE , "%3dì_", lngTensuu );
				ckputss( 0, 14, "F1:ñﬂÇÈ ENT:ämíË", False, CLR_BASE );	
				// ckputss( 0, 16, "F3:îÑïœ  F4:ê∏éZ", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:ê∏éZ", False, CLR_BASE );	
			}
			break;
		case BAIHEN:
			if( SaveItem != NUM){
				ClsColor();
				if( memcmp( info.tanto,"00",2 ) == 0 ){
					ckputss( 0,  0, "<îÑâøïœçX(ó˚èK)>", False, CLR_PR_TITLE );
				}else{
					ckputss( 0,  0, "  <îÑâøïœçX>    ", False, CLR_UR_TITLE );
				}
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  5, "BC:          ", False, CLR_BASE );
				ckputsn( 3,  5, info.Code1, 13, False, CLR_BASE );
//				ckputsn( 0,  7, btsmas.Name, 16, False, CLR_BASE );
				ckputss( 0, 10, "îÑâø :         ", False, CLR_BASE );
				insComma( atoln( info.Joudai, sizeof( info.Joudai )), strGoukei );
				ckprintf(7, 10, False, CLR_BASE, "%-7sâ~", strGoukei );
				ckputss( 0, 12, " Å®           â~", False, CLR_BASE );
				ckputss( 0, 15, "F1:ñﬂÇÈ ENT:ämíË", False, CLR_BASE );	
			}
			break;
		case DENTRY:
			if( SaveItem != DENTRY){
				ClsColor();	
				if( memcmp( info.tanto,"00",2 ) == 0 ){
					ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
					ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}else{
					ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
					ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  5, "BC:          ", False, CLR_BASE );
				ckputsn( 3,  5, info.Code1, 13, False, CLR_BASE );
//				ckputsn( 0,  7, btsmas.Name, 16, False, CLR_BASE );
				
				// ZNATZ_SHOW_TAX_INCLUDED
				jyoudaiWithTax = calculateTax2(atoln( info.Joudai, sizeof( info.Joudai )), taxrate);
				insComma( jyoudaiWithTax, strBaika );		
				
				ckprintf(0,  9, False, CLR_BASE , "Åè%7sÅ~",strBaika );
				ckprintf(12, 9, False, CLR_BASE , "%d",atoln( info.Num, sizeof( info.Num )) );
				ckputss( 0, 12, "åv        Å@Å@", False, CLR_SI_TITLE );
				insComma( calculateTax(lngGoukei, taxrate), strGoukei );		//ZNATZ
				ckprintf(2, 12, False, CLR_SI_TITLE, "%7sâ~", strGoukei );
				
				ckprintf(11,12, False, CLR_SI_TITLE , "%3dì_", lngTensuu );
				ckputss( 0, 14, "F1:ñﬂÇÈ ENT:ämíË", False, CLR_BASE );	
				// ckputss( 0, 16, "F3:îÑïœ  F4:ê∏éZ", False, CLR_BASE );	
				ckputss( 0, 16, "         F4:ê∏éZ", False, CLR_BASE );	
			}
			break;	
		case GENKIN:
			if( SaveItem != GENKIN){
				ClsColor();	
				if( memcmp( info.tanto,"00",2 ) == 0 ){
					ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
					ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}else{
					ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
					ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  4, " è¨  åv :       ", False, CLR_BASE );
				
				// ZNATZ 
				if ( reprint == 0 || teisei == 0 ) {
					insComma( calculateTax(lngGoukei, taxrate), strGoukei );
				} else {
					insComma( lngGoukei, strGoukei );		
				}
				
				ckprintf(9,  4, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  6, " åª  ã‡ ", False, CLR_SI_TITLE );
				ckputss( 8,  6, ":", False, CLR_BASE );
				ckputss( 0,  8, " ∏⁄ºﬁØƒ :       ", False, CLR_BASE );//#02
				insComma( atoln( Credit, sizeof( Credit )), strGoukei );
				ckprintf(9,  8, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0, 10, " Ç®íﬁÇË :       ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0, 16, "F1:ñﬂÇÈ ENT:ämíË", False, CLR_BASE );		
			}
			break;
		case CREDIT:
			if( SaveItem != CREDIT){
				ClsColor();	
				if( memcmp( info.tanto,"00",2 ) == 0 ){
					ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
					ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}else{
					ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
					ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  4, " è¨  åv :       ", False, CLR_BASE );
//				insComma( calculateTax(lngGoukei, taxrate), strGoukei );		// ZNATZ

				if ( reprint == 0 || teisei == 0 ) {
					insComma( calculateTax(lngGoukei, taxrate), strGoukei );
				} else {
					insComma( lngGoukei, strGoukei );		
				}
				
				ckprintf(9, 4, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  6, " åª  ã‡ :       ", False, CLR_BASE );
				insComma( atoln( Genkin, sizeof( Genkin )), strGoukei );
				ckprintf(9,  6, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  8, " ∏⁄ºﬁØƒ ", False, CLR_SI_TITLE );
				ckputss( 8,  8, ":", False, CLR_BASE );
				ckputss( 0, 10, " Ç®íﬁÇË :       ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0, 16, "F1:ñﬂÇÈ ENT:ämíË", False, CLR_BASE );		
			}
			break;
		// case KINKEN:
		// 	if( SaveItem != KINKEN){
		// 		ClsColor();	
		// 		if( memcmp( info.tanto,"00",2 ) == 0 ){
		// 			ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
		// 			ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
		// 		}else{
		// 			ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
		// 			ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
		// 		}
		// 		ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
		// 		ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
		// 		ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
		// 		ckputss( 0,  4, " è¨  åv :       ", False, CLR_BASE );
		// 		insComma( calculateTax(lngGoukei, taxrate), strGoukei );		// ZNATZ
		// 		ckprintf(9,  4, False, CLR_BASE, "%7s", strGoukei );
		// 		ckputss( 0,  6, " åª  ã‡ :       ", False, CLR_BASE );
		// 		insComma( atoln( Genkin, sizeof( Genkin )), strGoukei );
		// 		ckprintf(9,  6, False, CLR_BASE, "%7s", strGoukei );
		// 		ckputss( 0,  8, " ∏⁄ºﬁØƒ :       ", False, CLR_BASE );
		// 		insComma( atoln( Credit, sizeof( Credit )), strGoukei );
		// 		ckprintf(9,  8, False, CLR_BASE, "%7s", strGoukei ); //#02
		// 		ckputss( 0, 10, " Ç®íﬁÇË :       ", False, CLR_BASE );
		// 		insComma( lngOturi, strGoukei );
		// 		ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
		// 		ckputss( 0, 16, "F1:ñﬂÇÈ ENT:ämíË", False, CLR_BASE );		
		// 	}
		// 	break;
		// case COUPON:
		// 	if( SaveItem != COUPON){
		// 		ClsColor();	
		// 		if( memcmp( info.tanto,"00",2 ) == 0 ){
		// 			ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
		// 			ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
		// 		}else{
		// 			ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
		// 			ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
		// 		}
		// 		ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
		// 		ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
		// 		ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
		// 		ckputss( 0,  4, " è¨  åv :       ", False, CLR_BASE );
		// 		insComma( calculateTax(lngGoukei, taxrate), strGoukei );		// ZNATZ
		// 		ckprintf(9, 4, False, CLR_BASE, "%7s", strGoukei );
		// 		ckputss( 0,  6, " åª  ã‡ :       ", False, CLR_BASE );
		// 		insComma( atoln( Genkin, sizeof( Genkin )), strGoukei );
		// 		ckprintf(9,  6, False, CLR_BASE, "%7s", strGoukei );
		// 		ckputss( 0,  8, " ∏⁄ºﬁØƒ :       ", False, CLR_BASE );
		// 		insComma( atoln( Credit, sizeof( Credit )), strGoukei );
		// 		ckprintf(9,  8, False, CLR_BASE, "%7s", strGoukei );
		// 		ckputss( 0, 14, " Ç®íﬁÇË :       ", False, CLR_BASE );
		// 		insComma( lngOturi, strGoukei );
		// 		ckprintf(9, 14, False, CLR_BASE, "%7s", strGoukei );
		// 		ckputss( 0, 16, "F1:ñﬂÇÈ ENT:ämíË", False, CLR_BASE );		
		// 	}
		// 	break;
		case SEISAN:
			if( SaveItem != COUPON){
				ClsColor();	
				if( memcmp( info.tanto,"00",2 ) == 0 ){
					ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
					ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}else{
					ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
					ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				}
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  4, " è¨  åv :       ", False, CLR_BASE );
				insComma( calculateTax(lngGoukei, taxrate), strGoukei );		// ZNATZ
				ckprintf(9,  4, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  6, " åª  ã‡ :       ", False, CLR_BASE );
				insComma( atoln( Genkin, sizeof( Genkin )), strGoukei );
				ckprintf(9,  6, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  8, " ∏⁄ºﬁØƒ :       ", False, CLR_BASE );//#02
				insComma( atoln( Credit, sizeof( Credit )), strGoukei );
				ckprintf(9,  8, False, CLR_BASE, "%7s", strGoukei ); 
				ckputss( 0, 10, " Ç®íﬁÇË : ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
				
				ckputss( 0, 16, "F1:ñﬂÇÈ ENT:èIóπ", False, CLR_BASE );		
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
				// if( memcmp( info.tanto,"00",2 ) == 0 ){
				// 	ckputss( 0,  0, "<ó˚èK”∞ƒﬁ>      ", False, CLR_PR_TITLE );
				// 	ckprintf(10, 0, False, CLR_PR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				// }else{
				// 	ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
				// 	ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				// }
				ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
				ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				ckputss( 0,  2, "ìXï‹:   íSìñ:   ", False, CLR_BASE );
				ckputsn( 5,  2, info.tenpo1, sizeof( info.tenpo1 ), False, CLR_BASE );
				ckputsn( 13, 2, info.tanto, sizeof( info.tanto ), False, CLR_BASE );
				ckputss( 0,  4, " è¨  åv :       ", False, CLR_BASE );
				insComma( lngGoukei, strGoukei );		// ZNATZÅ@çƒÇ—ê≈åvéZÇµÇ»Ç¢ÇÊÇ§Ç…ÅI
				ckprintf(9,  4, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  6, " åª  ã‡ :       ", False, CLR_BASE );
				insComma( atoln( Genkin, sizeof( Genkin )), strGoukei );
				ckprintf(9,  6, False, CLR_BASE, "%7s", strGoukei );
				ckputss( 0,  8, " ∏⁄ºﬁØƒ :       ", False, CLR_BASE );
				insComma( atoln( Credit, sizeof( Credit )), strGoukei );
				ckprintf(9,  8, False, CLR_BASE, "%7s", strGoukei ); 

				ckputss( 0, 10, " Ç®íﬁÇË :       ", False, CLR_BASE );
				insComma( lngOturi, strGoukei );
				ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );

				ckputss( 0, 14, "F1:í˘ê≥F2:çƒî≠çs", False, CLR_BASE );		
				ckputss( 0, 16, "F3:óÃé˚èëENT:èI", False, CLR_BASE );
			}	
			break;
	}
	SaveItem = item;
}

//****************************************************************************/
// 	ÉfÅ[É^ìoò^ 							
//****************************************************************************/
static void entrydata( short sw ){
	memcpy( infour.Code1, info.Code1 , sizeof( infour.Code1 ));
	memcpy( infour.Code2, info.Code2 , sizeof( infour.Code2 ));
	memcpy( infour.Baika, info.Joudai , sizeof( infour.Baika ));
	memcpy( infour.Num, info.Num , sizeof( infour.Num ));


	if( sw ){
		// êVãKìoò^ 
		ram_write( ctrl.InfoUrCnt, &infour, INFOURF );
		++ctrl.InfoUrCnt;
		ram_write( 0, &ctrl, CTRLF );
	} else {
		//ç≈å„Ç…ìoò^ÇµÇΩÉfÅ[É^ÇçXêV 
		ram_write( ctrl.InfoUrCnt-1, &infour, INFOURF );
	}
	
	//beep( 10, 1 );
	meisaiclr();
}

//****************************************************************************/
// 	îÑè„ê∏éZÅAîÑè„ÉfÅ[É^ìoò^ 							
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
			memcpy( urdata.Name , infour.Name  , sizeof( urdata.Name ));
			memcpy( urdata.Baika, infour.Baika , sizeof( urdata.Baika ));
			
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

			// êVãKìoò^ 
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
/* 	àÛç¸ê›íË
/*	Flag=1:ì˙åvï\
/*	Flag=2:ÉåÉVÅ[Ég
/*	Flag=3:íléD(ïségóp)
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
	if ( reprint == 0 && teisei == 0 ) lngGoukei = calculateTax(lngGoukei, taxrate); // ZNATZ
	
	if( Flag == 0 ){//ì˙åvï\		
		ckputss( 0,  0, "  <ì˙åvï\àÛç¸>  ", False, CLR_UR_TITLE );
	} else if( Flag == 1 ){//⁄º∞ƒ	
	} else if( Flag == 3 ){
		cls();
	}
	
	ret = BtConfig(BT_GET, &conf);
	if (ret != 0) {
		memset(buf, 0x00, sizeof(buf));
		sprintf(buf, "BtConfig %d", ret);
		msgbox(LCD_MSGBOX_ERROR, 0, "ÉGÉâÅ[", buf, NULL, NULL); 
	} else {
		// PINÉRÅ[Éh 
		strncpy(conf.localPIN, "0000", sizeof(conf.localPIN));
		conf.timeout = 30;
		conf.MITM_Enable = DISABLE;
		
		ret = BtConfig(BT_SET, &conf);
		if (ret != 0) {
			memset(buf, 0x00, sizeof(buf));
			sprintf(buf, "BtConfig %d", ret);
			msgbox(LCD_MSGBOX_ERROR, 0, "ÉGÉâÅ[", buf, NULL, NULL); 
		}
	}
	
	memset(addr, 0x00, sizeof(addr));
	memset(name, 0x00, sizeof(name));

	BtGetLastAddr((unsigned char*)addr);

	while (1) {
		if( Flag == 0 ){
			ckputss( 1,  3, "1.ì˙åvï\àÛç¸  ", False, CLR_BASE );
		}else if( Flag == 1 ){
			break;
		} else if ( Flag == 3 ) {
			ckputss( 1,  3, "1.íléDàÛç¸  ", False, CLR_BASE );
		}
		ckputss( 1,  5, "2.ê⁄ë±ämîF    ", False, CLR_BASE );
		ckputss( 1,  7, "3.àÛç¸èIóπ    ", False, CLR_BASE );	
		ckputss( 0,  12, "  Ãﬂÿ›¿ ±ƒﬁ⁄ΩÅ@ ", False, CLR_UR_TITLE );
		
		if (strlen((char*)addr) == 0) {
			ckputss( 1,  14, " ñ¢ê›íË", False, CLR_BASE );
		} else {
			ckputsn( 1,  14, (char *)addr, 16, False, CLR_BASE );
		}
		key = getch();
		
		if (key == '1') {
			if(Flag == 3 ) { 
				//PrintPricetag((char*)addr, false, 0); 
				return 1;
			}
			while( 1 ){
				intMsg = PrintMain((char*)addr, false, 0 , Flag );
				if( intMsg == 1 )break;
			}
			Display( 0 );
			break;
		} else if (key == '2') {
			ret = EzBtSelect(EZ_BT_MODE_MENU, addr, name);
			if (ret != 0 && ret != -10) {
				memset(buf, 0x00, sizeof(buf));
				sprintf(buf, "EzBtSelect %d", ret);
				msgbox(LCD_MSGBOX_ERROR, 0, "ÉGÉâÅ[", buf, NULL, NULL); 
			}
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
		memcpy( &infour2.Date[4], Month , 2);		// ZNATZ INSERT MONTH
		memcpy( &infour2.Date[6], Day , 2);		// ZNATZ INSERT Day
		
		infour2.Oturi = lngOturi;
		infour2.Goukei = lngGoukei;
		infour2.Tensuu = lngTensuu;
		ram_write( 0, &infour2, INFOURF2 );
		while( 1 ){
			// ñ{äiàÛç¸ ZNATZ_PRINT
			intMsg = PrintMain((char*)addr, false, 0 , Flag );
			// if( intMsg == 1 )break;
			if( intMsg == 1 ) { return 0;}
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
// 	äeéÌèàóù    															 
//*****************************************************************************
void uriage( int flag, int firsttime )
{
	short item, ret ;
	char now[8];
	long lngCheck = 0 , lngCheck2 = 0;
	
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
		msgbox( LCD_MSGBOX_WARNING, 0x00, "", "èâÇﬂÇ…ê›íËÇçsÇ¡Çƒâ∫Ç≥Ç¢", "ämîF", 0x00 );
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
				ret = CodeInput( 3, 5, info.Code1, sizeof( info.Code1 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;			
			case CODE2:
				ret = CodeInput( 3, 7, info.Code2, sizeof( info.Code2 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;				
			case NUM:
				memcpy( info.Num ,"0001" ,sizeof( info.Num )); 
				ret = NumInput( 12, 9, info.Num, sizeof( info.Num ), 0, 99999L,
								IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
				break;
			// case BAIHEN:
			// 	memcpy( Baika ,info.Joudai ,sizeof( Baika )); 
			// 	ret = NumInput( 6, 12, Baika, sizeof( Baika ), 0, 999999L,
			// 					IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
			// 	break;
			case DENTRY:
				if( !memcmp( Code, "30", 2 ) ){
					entrydata( False );
				} else {
					entrydata( True );
				}
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
			case KINKEN:
				//ret = NumInput( 10, 10, Kinken, 6, 0, 999999L,
				//				IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK ); //#02
				ret = NumInput( 10, 8, Kinken, 6, 0, 999999L,
								IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK );
				break;
			case COUPON:
				ret = NumInput( 10, 12, Coupon, 6, 0, 999999L,
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
				}else if( ret == 13 ){//ENT ê∏éZäJén
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
			if( item == YEAR ){//îNéwíË
				if( atoln( Year, sizeof( Year ) ) >= 2000 ){
					item = MONTH;
				}else{
					memset( Year, 0x00, sizeof( Year ) );
					beeb(10,2, 1);
				}
				continue;
			}else if( item == MONTH ){//åééwíË
				if( atoln( Month, sizeof( Month ) ) > 0 && atoln( Month, sizeof( Month ) ) <= 12 ){
					item = DAY;
				}else{
					memset( Month, 0x00, sizeof( Month ) );
					beeb(10,2, 1);
				}
				continue;
			}else if( item == DAY ){//ì˙éwíË
				if( atoln( Day, sizeof( Day ) ) > 0 && atoln( Day, sizeof( Day ) ) <= 31 ){
					// íºê⁄Ç…CODE1Ç…
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
				// ìXï‹É}ÉXÉ^åüçı 
				if( ShopFind( info.tenpo1 ) == -1 ){
					beeb(10,2, 1);
					memcpy( info.tenpo1, 0x00 , sizeof( info.tenpo1 ) );
					continue;
				}else{//äYìñ√ﬁ∞¿Ç™ë∂ç›
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
				// íSìñÉ}ÉXÉ^åüçı 
				if( atoln( info.tanto, sizeof( info.tanto ) ) == 0 ){
					item = CODE1;
					ckputsn( 3, 12, "ó˚èKÉÇÅ[Éh", 10, False, CLR_BASE );
					continue;
				}
				if( TantoFind( info.tanto ) == -1 ){
					beeb(10,2, 1);
					memset( info.tanto, 0x00, sizeof( info.tanto ) );
					continue;
				}else{//äYìñ√ﬁ∞¿Ç™ë∂ç›
					item = CODE1;
					ckputsn( 3, 12, tmst.Name, 13, False, CLR_BASE );
					continue;
				}
				*/
				
				lngGoukei = 0;
				
			}else if( item == CODE1 ){
				if( MaxCheck( ctrl.URDataCnt, URDATA_MAX ) ) return;//@01

				if( info.Code1[0] != 0x00 ){

					if( ctrl.InfoUrCnt == INFOUR_MAX ){//åèêîÇí¥Ç¶ÇÈèÍçáÇÕìoò^ïsâ¬
						beeb(10,2, 1);
						memset( info.Code1, 0x00, sizeof( info.Code1 ) );
						continue;
					}
					//3åÖÇ™501Å`999ÇÃèÍçá
					if( atoin( info.Code1, 3 ) > 500 && atoin( info.Code1, 3 ) < 999 ){
						item = CODE2;
						displayMsg(BumonTaxFind("33"));
						// TODO
		//						memcpy( info.Joudai, btsmas.Joudai, sizeof( info.Joudai ) );
		//						memcpy( Name, btsmas.Name ,sizeof( Name ) );
						item = CODE2;
						continue;
					} else if( ctrl.URDataCnt > 0 && !memcmp( info.Code1, "30", 2 ) && info.Code1[8] == ' ' ){
						/* ìoò^çœÇ›ÉfÅ[É^Ç™ë∂ç›Ç∑ÇÈÇ©Ç¬è„íiÉRÅ[ÉhÇÃêÊì™Ç©ÇÁÇQåÖÇ™ÇRÇOÅAÇ©Ç¬ÇWåÖÇÃèÍçá */
						/* äÑà¯ó¶Ç‹ÇΩÇÕílâ∫ã‡äzÇç≈å„ÇÃìoò^ÉfÅ[É^Ç÷ÉZÉbÉgÇµÉfÅ[É^èCê≥ÇçsÇ§			*/
						memcpy( Code, info.Code1, sizeof( Code ) );
						ram_read( ctrl.URDataCnt-1, &urdata, URDATAF );
						memcpy( urdata.Code3, Code, sizeof( urdata.Code3 ) );
						item = DENTRY;
						continue;
					} else {
						/* è„ãLÇ…äYìñÇµÇ»Ç¢èÍçáÇÕÉGÉâÅ[ */
						beep( 10, 2 );
						meisaiclr();
						continue;
					}
				}
				
			} else if( item == CODE2 ){

				if( MaxCheck( ctrl.URDataCnt, URDATA_MAX ) ) return;//@01

				if( ctrl.InfoUrCnt == INFOUR_MAX ){//åèêîÇí¥Ç¶ÇÈèÍçáÇÕìoò^ïsâ¬
					beeb(10,2, 1);
					memset( info.Code1, 0x00, sizeof( info.Code1 ) );
					continue;
				}
				/* è„íiÉRÅ[ÉhÇÃêÊì™Ç©ÇÁÇRåÖÇ™501Å`999ÇÃèÍçáÅAÇOÇXÇXÇXÅAÇPÅAÇRÇ©ÇÁénÇ‹ÇÈÉRÅ[Éhà»äOïsâ¬ */
				if( atoin( info.Code1, 3 ) > 500 && atoin( info.Code1, 3 ) < 999 ){
					if( (info.Code2[0] == '0' || info.Code2[0] == '1' || info.Code2[0] == '2' || info.Code2[0] == '3') && sdata.Code2[12] != ' ' ){
						long bk = getJyoudai(info.Code1);
						lngGoukei = lngGoukei + calculateTax2(bk, taxrate);
						lngTensuu = lngTensuu + 1; 
						item = NUM;
						continue;
					} else {
						/* è„ãLÇ…äYìñÇµÇ»Ç¢èÍçáÇÕÉGÉâÅ[ */
						memset( info.Code2, 0x00, sizeof( info.Code2 ) );
						beep( 10, 2 );
						continue;
					}
				} else {
					/* è„ãLÇ…äYìñÇµÇ»Ç¢èÍçáÇÕÉGÉâÅ[ */
					memset( sdata.Code2, 0x00, sizeof( sdata.Code2 ) );
					beep( 10, 2 );
					continue;
				}
				
 			} else if( item == NUM ){
				//0ÇÕíeÇ≠
				if( atoln( info.Num, sizeof( info.Num ) ) == 0 ){
					beeb(10,2, 1);
					continue;
				}
				// É}ÉCÉiÉXÇä‹Ç‹Ç»Ç¢èÍçáÅAì¸óÕílÇ™ÇXÇXÇXÇí¥Ç¶ÇΩÇÁÉGÉâÅ[Ç∆Ç∑ÇÈ 
				if( atoln( info.Num, sizeof( info.Num ) ) >= 0 && atoln( info.Num, sizeof( info.Num ) ) > 999 ){
					beeb(10,2, 1);
					continue;
				}
				// É}ÉCÉiÉXÇä‹ÇﬁèÍçáÅAì¸óÕílÇ™-ÇXÇXÇXÇí¥Ç¶ÇΩÇÁÉGÉâÅ[Ç∆Ç∑ÇÈ 
				if( atoln( info.Num, sizeof( info.Num ) ) < 0 && atoln( info.Num, sizeof( info.Num ) ) <= -1000 ){
					beeb(10,2, 1);
					continue;
				}
				lngCheck = lngGoukei;
				//lngGoukei = lngGoukei + ( atoln( info.Joudai, sizeof( info.Joudai )) * ( atoln( info.Num, sizeof( info.Num )) - 1 ) );
				
				// ZZZZ
//				long temp = atoln( info.Joudai, sizeof( info.Joudai ));
//				temp =  calculateTax(temp, taxrate) - calculateTaxNoRoundUp(temp, taxrate);
//				
//				long tempTotal = ( atoln( info.Joudai, sizeof( info.Joudai )) * ( atoln( info.Num, sizeof( info.Num )) ) ) ;
//				tempTotal =  calculateTax(tempTotal, taxrate) - calculateTaxNoRoundUp(tempTotal, taxrate);
//				
//				// if ( temp <= 0.4 && tempTotal > 0.4 ) {--lngGoukei;}

				long price = atoln( info.Joudai, sizeof( info.Joudai ));
				lngGoukei = lngGoukei + calculateTax2(price, taxrate) * (atoln( info.Num, sizeof( info.Num )) - 1 ) ;
				
				
				if( lngGoukei > 999999 || lngGoukei < -99999 ){
					lngGoukei = lngCheck;
					beep( 50 , 1 );
					continue;
				}
				lngCheck2 = lngTensuu;
				lngTensuu = lngTensuu + atoln( info.Num, sizeof( info.Num )) - 1; 
				if( lngTensuu > 999 || lngTensuu < -99 ){
					lngGoukei = lngCheck;
					lngTensuu = lngCheck2;
					beep( 50 , 1 );
					continue;
				}
				// ìoò^ 
				item = DENTRY;
				continue;
			} else if( item == BAIHEN ){
				if( atoln( Baika, sizeof( Baika )) > atoln( info.Joudai, sizeof( info.Joudai )) || atoln( Baika, sizeof( Baika )) <= 0 ){
					memcpy( Baika , info.Joudai, sizeof( Baika ) );
					beep( 50,1 );
					continue;
				}
				// çáåväzÇÃí≤êÆ  ZZZZZZZ

				memcpy( info.Joudai , Baika , sizeof( info.Joudai ) );				
				long price = atoln( info.Joudai, sizeof( info.Joudai ));
				lngGoukei = lngGoukei + calculateTax2(price, taxrate);

				item = NUM;
				continue;
			} else if( item == GENKIN ){
				
				displayMsg(lngGoukei);
				if ( lngGoukei < 0 ) { item = CREDIT; continue; }

				// 0â~Ç≈ì¸óÕÇµÇΩèÍçáÅAîÑä|Ç…à⁄Ç∑
				long current_cash = atoln( Genkin, sizeof( Genkin ));
				if ( current_cash == 0 ) {
					item = CREDIT; 
					continue;
				}

				// îÑä|ä˘Ç…ì¸óÕÇµÇΩèÍçáÅAåªã‡Ç0Ç…ÉZÉbÉg
				long current_credit = atoln( Credit, sizeof( Credit ));
				if ( current_credit > 0 ) {
					memcpy( Genkin , "000000" , sizeof( Genkin ) );
					item = CREDIT;
					continue;
				}

				// lngOturiChk = atoln( Genkin, sizeof( Genkin )) + atoln( Credit, sizeof( Genkin )) + atoln( Kinken, sizeof( Kinken )) + atoln( Coupon, sizeof( Coupon ));

				//Ç®íﬁÇËÇ™èoÇÈèÍçáÇÃÇ›åvéZ
				if( current_cash >= calculateTax(lngGoukei, taxrate) ){ 		//ZNATZ					
					
					if ( teisei == 0 ){
						lngOturi = current_cash - calculateTax(lngGoukei, taxrate) ; //ZNATZ
					} else {
						lngOturi = current_cash - lngGoukei ; //ZNATZ
					}
						
					if( lngOturi > 9999 ){
						beep( 50,1 );
						memcpy( Genkin , "000000" , sizeof( Genkin ) );
						lngOturi = 0;
						continue;
					}
					
					ckputss( 0, 10, " Ç®íﬁÇË :          ", False, CLR_BASE );
					insComma( lngOturi, strGoukei );
					ckprintf(9, 10, False, CLR_BASE, "%7s", strGoukei );
					
					
					if( memcmp( info.tanto,"00",2 ) != 0 && reprint == 0 && teisei == 0 ){
						++ctrl.RecNo;
						if( ctrl.RecNo > 999999 )ctrl.RecNo = 1;
					}

					if( memcmp( ctrl.RecPrint,"1",1 ) == 0 ){
						// ê¨å˜Ç»ÇÁàÛç¸ ZNATZ_PRINT reprint=1Ç»ÇÁå≥âÊñ ñﬂÇÈ
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
				}else{
					lngOturi = 0;
					item = CREDIT;
				}
				continue;
				
			} else if( item == CREDIT ){
				
				// lngOturiChk = atoln( Credit, sizeof( Credit ));
				
				lngOturiChk = atoln( Genkin, sizeof( Genkin )) + atoln( Credit, sizeof( Credit )) ;
				
				//ÉNÉåÉWÉbÉgÇ™è¨åvÇí¥Ç¶ÇƒÇ¢Ç»Ç¢Ç©É`ÉFÉbÉN
				if( lngGoukei >= 0 ){
					if ( teisei == 0 ) {
						if( atoln( Credit, sizeof( Credit )) > calculateTax(lngGoukei, taxrate) ){	
							beeb(10,2, 1);
							memset( Credit, 0x00, sizeof( Credit ) );
							lngOturi = 0;
							item = CREDIT;
							continue;
						}
						if( lngOturiChk < calculateTax(lngGoukei, taxrate) ){ item = GENKIN; continue;}
						
						lngOturi = lngOturiChk - calculateTax(lngGoukei, taxrate) ; 
					}
					if ( teisei == 1 ) {
						if( atoln( Credit, sizeof( Credit )) > lngGoukei ){	
							beeb(10,2, 1);
							memset( Credit, 0x00, sizeof( Credit ) );
							lngOturi = 0;
							item = CREDIT;
							continue;
						}
						if( lngOturiChk < lngGoukei ){ item = GENKIN; continue;}
						lngOturi = lngOturiChk - lngGoukei ; //ZNATZ
					}
				} else {
					lngOturi = lngOturiChk - lngGoukei ;
				}
				
				
					
				ckputss( 0, 10, " Ç®íﬁÇË :          ", False, CLR_BASE );
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
				ckputss( 0,  0, "<îÑè„ì¸óÕ>      ", False, CLR_UR_TITLE );
				ckprintf(10, 0, False, CLR_UR_TITLE , "%4dåè", ctrl.InfoUrCnt );
				ckputss( 0, 12, "åv        Å@Å@", False, CLR_SI_TITLE );
				ckprintf(3, 12, False, CLR_SI_TITLE , "%6dâ~", lngGoukei );
				ckprintf(11,12, False, CLR_SI_TITLE , "%3dì_", lngTensuu );	
				ckputss( 0,  5, "Å£              ", False, CLR_BASE );
				ckputss( 0,  7, "Å§              ", False, CLR_BASE );	
			}
			continue;
		}else if( ret == F1KEY ){//ñﬂÇÈ
			if( item == NUM ){
				// TODO 20201123 ê≈åvéZ
				lngGoukei = lngGoukei - atoln( info.Joudai, sizeof( info.Joudai ));
				lngTensuu = lngTensuu - 1; 
				meisaiclr();
				item = CODE1;
				continue;
			}else if( item == CODE2 ) {
				meisaiclr();
				item = CODE1;
				continue;
			}else if( item == YEAR ){
				infoclr();
				menu();
			}else if( item == BAIHEN ){
				lngGoukei = lngGoukei + atoln( info.Joudai, sizeof( info.Joudai ));
				item = NUM;
				continue;
			}else if( item == GENKIN ){
				lngOturi = 0;
				/*if( memcmp( ctrl.RecPrint,"1",1 ) == 0 ){//#01
					LapinBTDisconnect();
				}*/
				item = CODE2;
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
		// }else if( ret == F3KEY ){
		// 	if( item ==  YEAR || item == MONTH || item == DAY || item == SHOP || item == TANTO ){
		// 		//item = KAKUNIN;
		// 		continue;
		// 	}else if( item == NUM ){
		// 		long bk = atoln( info.Joudai, sizeof( info.Joudai ));
		// 		lngGoukei = lngGoukei - calculateTax2(bk, taxrate);				
		// 		// item = BAIHEN;
		// 		Display( item );
		// 		continue;
		// 	}
		}else if( ret == F4KEY ){//ì˙åvï\ÅEåªã‡
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