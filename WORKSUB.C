#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"

static	char	inpBuf[50];
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
short CodeInput( short x, short y, char *s, short length, unsigned long prm )
{
	short		ret, len;

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
				memcpy( s, inpBuf, len );
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

/*****************************************************************************/
/* 			     															 */
/*****************************************************************************/
short NumInput( short x, short y, char *s, short length, long Min, long Max, unsigned long prm, short Type, short Zoro )
{
	short	ret;
	long	Check_Num, Save_Num;
	char	*decad;

	memset( inpBuf, 0x00, sizeof( inpBuf ) );
	if( s[0] ){
		if( prm & KEY_DECPOINT ){
			if( prm & DEC_ONE )
				sprintf( inpBuf, "%ld.%01d", atoln( s, length-1 ), atoin( &s[length-1], 1 ) );
			else
				sprintf( inpBuf, "%ld.%02d", atoln( s, length-2 ), atoin( &s[length-2], 2 ) );
		} else {
			if( Type )
				sprintf( inpBuf, "%ld", atoln( s, length ) );
			else
				memncpy( inpBuf, s, length );
		}
	}
	Save_Num = -1;
	while( 1 ){
		ret = input( x, y, inpBuf, ( prm & MIN_ADD )?(length-1):length, prm | TRIG_ENT );
		if( ret == ENTRY || ret == SENTRY || ret == BARENT ){
			if( ret == BARENT ){
				memset( s, ' ', 13 );
				memcpy( s, inpBuf, strlen( inpBuf ) );
				break;
			}
			if( Min != -1 || Max != -1 ){
				Check_Num = abs( atol( inpBuf ) );
				if( Check_Num < Min || Check_Num > Max ){
					beepEx( 50, 1 );
					memset( inpBuf, 0x00, sizeof( inpBuf ) );
					if( s[0] ){
						if( prm & KEY_DECPOINT ){
							if( prm & DEC_ONE )
								sprintf( inpBuf, "%ld.%01d", atoln( s, length-1 ), atoin( &s[length-1], 1 ) );
							else
								sprintf( inpBuf, "%ld.%02d", atoln( s, length-2 ), atoin( &s[length-2], 2 ) );
						} else {
							if( Type )
								sprintf( inpBuf, "%ld", atoln( s, length ) );
							else
								memncpy( inpBuf, s, length );
						}
					}
					continue;
				}
			}
			if( Zoro ){
				if( abs( atol( inpBuf ) ) >= 100 && abs( atol( inpBuf ) ) != Save_Num ){
					Save_Num = abs( atol( inpBuf ) );
					beepEx( 50, 1 );
					continue;
				}
			}
			if( prm & KEY_DECPOINT ){
				decad = strchr( inpBuf, '.' );
				if( prm & DEC_ONE ){
					sprintf( dspBuf, "%0*ld", length-1, atol( inpBuf ) );
					if( decad ){
						sprintf( &dspBuf[length-1], "%01ld", atol( &decad[1] ) );
					} else {
						memcpy( &dspBuf[length-1], "0", 1 ); dspBuf[length] = 0x00;
					}
				} else {
					sprintf( dspBuf, "%0*ld", length-2, atol( inpBuf ) );
					if( decad ){
						if( strlen( &decad[1] ) < 2 )
							sprintf( &dspBuf[length-2], "%02ld", atol( &decad[1] )*10 );
						else
							sprintf( &dspBuf[length-2], "%02ld", atol( &decad[1] ) );
					} else {
						memcpy( &dspBuf[length-2], "00", 2 ); dspBuf[length] = 0x00;
					}
				}
			} else if( Min != -1 || Max != -1 ){
				sprintf( dspBuf, "%0*ld", length, atol( inpBuf ) );
			} else {
				memset( dspBuf, 0x00, sizeof( dspBuf ) );
				strcpy( dspBuf, inpBuf );
			}
			memcpy( s, dspBuf, length );
			break;
		} else if( ret == SKIP ){
			memset( s, 0x00, length );
			break;
		} else if( ret == BAR_ERR ){
			memset( inpBuf, 0x00, sizeof( inpBuf ) );
			beepEx( 50, 1 );
		} else if( ret == F1KEY || ret == F2KEY || ret == F3KEY ||
				   ret == F4KEY || ret == DEC_CLR_KEY ||
				   ret == BSKEY || ret == UPKEY || ret == DOWNKEY ){
			break;
		}
	}
	return( ret );
}

/**************************************************/
/*  selectInput                                   */
/*    short x       --> x座標                     */
/*    short y       --> y座標                     */
/*    char  *s      --> 出力ﾊﾞｯﾌｧ                 */
/*    char  *item   --> 表示ｱｲﾃﾑ                  */
/*    short size    --> 1ｱｲﾃﾑｻｲｽﾞ                 */
/*    short cnt     --> ｱｲﾃﾑ数                    */
/*                                                */
/**************************************************/
short selectInput( short x, short y, char *s, char *item, short size, short cnt )
{
	short	ret, inp;

	inp = *s - '0';
	while( 1 ){
		ckputss( x, y, &item[inp*size], True, CLR_BASE );
		ret = getch();
		if( ret == ENT ){
			*s = inp + '0';
			ret = ENTRY;
			break;
		} else if( ret == UP ){
			if( --inp < 0 ) inp = cnt - 1;
		} else if( ret == DOWN ){
			if( ++inp >= cnt ) inp = 0;
		} else if( ret == F1 ){
			ret = F1KEY;
			break;
		} else if( ret == F2 ){
			ret = F2KEY;
			break;
		} else if( ret == F3 ){
			ret = F3KEY;
			break;
		} else if( ret == F4 ){
			ret = F4KEY;
			break;
		}
	}
	inp = *s - '0';
	ckputss( x, y, &item[inp*size], False, CLR_BASE );
	return( ret );
}

/**************************************************/
/*                                                */
/**************************************************/
short	yyyymmddInput( short x, short y, char *s, char *now, unsigned long prm, short check )
{
	short			item, ret, xx, ank, len;
	unsigned long	prmm;

	ank = ( prm & ANK_INPUT ) ? True : False;
	item = 0;
	memcpy( dt, s, 8 );
	if( s[0] ){
		sprintf( dspBuf, "%04d/%02d/%02d", atoin( dt, 4 ),
								   	  	   atoin( &dt[4], 2 ),
										   atoin( &dt[6], 2 ) );
		cputstr( x, y, ank, 0, dspBuf );
	}
	while( 1 ){
		len = item?2:4;
		if( item ){
			xx = x + item * 3 + 2;
			memncpy( inpBuf, &dt[item*2+2], 2 );
		} else {
			xx = x;
			memncpy( inpBuf, dt, 4 );
		}
		prmm = IN_NUMERIC | KEY_NOTCR | KEY_FUNC | KEY_CLR | TRIG_ENT | prm;
		ret = input( xx, y, inpBuf, len, prmm );
		if( ret == ENTRY || ret == SENTRY ){
			if( item ){
				sprintf( dspBuf, "%02d", atoi( inpBuf ) );
				memcpy( &dt[item*2+2], dspBuf, 2 );
			} else {
				sprintf( dspBuf, "%04d", atoi( inpBuf ) );
				memcpy( dt, dspBuf, 4 );
			}
			cputstr( xx, y, ank, 0, dspBuf );
			/* 月チェック */
			if( item == 1 && ( atoi( inpBuf ) < 1 || atoi( inpBuf ) > 12 ) ){
				beepEx( 50, 1 );
				continue;
			}
			if( item >= 2 || ( item == 0 && ret == SENTRY ) ){
				if( datechk( dt, now, check ) ){
					/* 本日<=入力日<=１ヶ月先 以外 */
					/* 本日>=入力日 以外 */
					item = 0;
					beepEx( 50, 1 );
					continue;
				}
				memcpy( s, dt, 8 );
				break;
			}
			++item;
		} else if( ret == CLRKEY ){
			memset( dt, 0x00, 8 );
			cputstr( x, y, ank, 0, "    /  /  " );
			item = 0;
		} else if( ret == SKIP ){
			memset( s, 0x00, 8 );
			break;
		} else if( ret == BSKEY ){
			if( --item < 0 ) break;
		} else if( ret == F1KEY || ret == F2KEY || ret == F3KEY || ret == F4KEY ||  
				   ret == BSKEY || ret == UPKEY || ret == DOWNKEY ){
			break;
		}
	}
	return( ret );
}

/**************************************************/
/*                                                */
/**************************************************/
short	yymmddInput( short x, short y, char *s, char *now, unsigned long prm, short check )
{
	short			item, ret, xx, ank;
	unsigned long	prmm;

	ank = ( prm & ANK_INPUT ) ? True : False;
	item = 0;
	memcpy( dt, s, 6 );
	if( s[0] ){
		sprintf( dspBuf, "%02d/%02d/%02d", atoin( dt, 2 ),
								   	  	   atoin( &dt[2], 2 ),
										   atoin( &dt[4], 2 ) );
		cputstr( x, y, ank, 0, dspBuf );
		item = 2;
	}
	while( 1 ){
		xx = x + item * 3;
		memncpy( inpBuf, &dt[item*2], 2 );
		prmm = IN_NUMERIC | KEY_NOTCR | KEY_FUNC | KEY_CLR | TRIG_ENT | prm;
		ret = input( xx, y, inpBuf, 2, prmm );
		if( ret == ENTRY || ret == SENTRY ){
			sprintf( dspBuf, "%02d", atoi( inpBuf ) );
			memcpy( &dt[item*2], dspBuf, 2 );
			cputstr( xx, y, ank, 0, dspBuf );
			/* 月チェック */
			if( item == 1 && ( atoi( inpBuf ) < 1 || atoi( inpBuf ) > 12 ) ){
				beepEx( 50, 1 );
				continue;
			}
			if( item >= 2 || ( item == 0 && ret == SENTRY ) ){
				if( datechk( dt, now, check ) ){
					/* 本日<=入力日<=１ヶ月先 以外 */
					/* 本日>=入力日 以外 */
					item = 0;
					beepEx( 50, 1 );
					continue;
				}
				memcpy( s, dt, 6 );
				break;
			}
			++item;
		} else if( ret == CLRKEY ){
			memset( dt, 0x00, 6 );
			cputstr( x, y, ank, 0, "  /  /  " );
			item = 0;
		} else if( ret == SKIP ){
			memset( s, 0x00, 6 );
			break;
		} else if( ret == BSKEY ){
			if( --item < 0 ) break;
		} else if( ret == F1KEY || ret == F2KEY || ret == F3KEY || ret == F4KEY || 
				   ret == BSKEY || ret == UPKEY || ret == DOWNKEY ){
			break;
		}
	}
	return( ret );
}

/**************************************************/
/*                                                */
/**************************************************/
short	MaxCheck( short cnt, short max )
{
	if( cnt >= max ){
		ClsColor();		/*  0123456789012345*/
		ckputss( 0, 2, "データが一杯の為", False, CLR_BASE );
		ckputss( 0, 4, "データは登録出来", False, CLR_BASE );
		ckputss( 0, 6, "ません。        ", False, CLR_BASE );
		ckputss( 0, 8, "送信してから再度", False, CLR_BASE );
		ckputss( 0,10, "登録して下さい。", False, CLR_BASE );
		ckputss( 0,15, "    ENT:確認    ", False, CLR_BASE );
		gline( 6, 6, 138, 66, True );
		beepEx( 50, 1 );
		while( !okng() );
		return( True );
	} else {
		return( False );
	}
}

/**************************************************/
/*  部門マスタ検索	                              */
/**************************************************/
short	BumonFind( char *key )
{
	long	adrs;

	//memset( &mmst, 0x00, sizeof( mmst ) );
#if 0	/* ２分法 */
	//adrs = binarySearch( key, sizeof( mmst.Code ), MMSTF, ctrl.MMstCnt, 0 );
#else	/* 先頭から検索 */
	adrs = beginSearch( key, 1, 0, sizeof( bumst.Code ), 0, ctrl.BUMSTCnt, BUMSTF );
#endif
	if( adrs != -1 ){
		ram_read( adrs, &bumst, BUMSTF );
	}
	return adrs;
}

/**************************************************/
/*  部門税率検索	                              */
/**************************************************/
long	BumonTaxFind( char *key )
{
	long	adrs;

	//memset( &mmst, 0x00, sizeof( mmst ) );
#if 0	/* ２分法 */
	//adrs = binarySearch( key, sizeof( mmst.Code ), MMSTF, ctrl.MMstCnt, 0 );
#else	/* 先頭から検索 */
	adrs = beginSearch( key, 1, 0, sizeof( bumst.Code ), 0, ctrl.BUMSTCnt, BUMSTF );
#endif
	if( adrs != -1 ){
		ram_read( adrs, &bumst, BUMSTF );
		return atoln(bumst.Zeiritu, sizeof(bumst.Zeiritu));
	}
	return 10;
}


/**************************************************/
/*  部門税率検索	                                */
/*  呼び出し例: 　	                                */
/* displayMsg(BumonTaxFindByCode1(&tdata.Code1)); */
/**************************************************/
long	BumonTaxFindByCode1( char *code1 )
{
	long	adrs;

	//memset( &mmst, 0x00, sizeof( mmst ) );
#if 0	/* ２分法 */
	//adrs = binarySearch( key, sizeof( mmst.Code ), MMSTF, ctrl.MMstCnt, 0 );
#else	/* 先頭から検索 */
	adrs = beginSearch( code1+3, 1, 0, sizeof( bumst.Code ), 0, ctrl.BUMSTCnt, BUMSTF );
#endif
	if( adrs != -1 ){
		ram_read( adrs, &bumst, BUMSTF );
		return atoln(bumst.Zeiritu, sizeof(bumst.Zeiritu));
	}
	return 10;
}

/*****************************************************/
/*  品種税区検索	                                   */
/*  呼び出し例: 　	                                   */
/* displayMsg(HinsyuZeikuFindByCode1(&tdata.Code1)); */
/*  !! ☆ 二段バーコードの上段上代が税抜きセットされるため、   */
/*        0　しか返さない                               */
/*****************************************************/
long	HinsyuZeikuFindByCode1( char *code1 )
{

	return 0;

// 	long	adrs;

// 	//memset( &mmst, 0x00, sizeof( mmst ) );
// #if 0	/* ２分法 */
// 	//adrs = binarySearch( key, sizeof( mmst.Code ), MMSTF, ctrl.MMstCnt, 0 );
// #else	/* 先頭から検索 */
// 	adrs = beginSearch( code1+3, 1, 0, sizeof( himst.Code ), 0, ctrl.HIMSTCnt, HIMSTF );
// #endif
// 	if( adrs != -1 ){
// 		ram_read( adrs, &himst, HIMSTF );
// 		return atoln(himst.Zeiku, sizeof(himst.Zeiku));
// 	}
// 	return 0;

}

/*****************************************************/
/*  品種検索    	                                   */
/*  呼び出し例: 　	                                   */
/*****************************************************/
long	HinsyuFindByCode1( char *code1 )
{
	long	adrs;

	//memset( &mmst, 0x00, sizeof( mmst ) );
#if 0	/* ２分法 */
	//adrs = binarySearch( key, sizeof( mmst.Code ), MMSTF, ctrl.MMstCnt, 0 );
#else	/* 先頭から検索 */
	adrs = beginSearch( code1+3, 1, 0, sizeof( himst.Code ), 0, ctrl.HIMSTCnt, HIMSTF );
#endif
	if( adrs != -1 ){
		ram_read( adrs, &himst, HIMSTF );
	}
	return adrs;
}

/**************************************************/
/*  店舗マスタ検索	                              */
/**************************************************/
short	ShopFind( char *key )
{
	long	adrs;

	//memset( &mmst, 0x00, sizeof( mmst ) );
#if 0	/* ２分法 */
	//adrs = binarySearch( key, sizeof( mmst.Code ), MMSTF, ctrl.MMstCnt, 0 );
#else	/* 先頭から検索 */
	//adrs = beginSearch( key, 1, 0, sizeof( mmst.Code ), 0, ctrl.MMstCnt, MMSTF );
#endif
	//if( adrs != -1 ){
	//	ram_read( adrs, &mmst, MMSTF );
	//}
	//return adrs;
}

/**************************************************/
/*  担当マスタ検索	                              */
/**************************************************/
short	TantoFind( char *key )
{
	long	adrs;

	//memset( &tmst, 0x00, sizeof( tmst ) );
#if 0	/* ２分法 */
	//adrs = binarySearch( key, sizeof( tmst.Code ), TMSTF, ctrl.TMstCnt, 0 );
#else	/* 先頭から検索 */
	//adrs = beginSearch( key, 1, 0, sizeof( tmst.Code ), 0, ctrl.TMstCnt, TMSTF );
#endif
	//if( adrs != -1 ){
	//	ram_read( adrs, &tmst, TMSTF );
	//}
	//return adrs;
}

/**************************************************/
/*                                                */
/**************************************************/
long	GetAllCnt( void )
{
	return(
		(long)ctrl.TDataCnt
	);
}

/**************************************************/
/*                                                */
/**************************************************/
void	beepEx( short tim, short loop )
{
	if( ctrl.Beep )
		beep( tim, loop );
}

/**************************************************/
/*                                                */
/**************************************************/
void	beebEx( short tim, short loop, short wait )
{
	if( ctrl.Beep )
		beeb( tim, loop, wait );
}

void	playHzEx( short hz, short tim, short loop )
{
	if( ctrl.Beep )
		playHz( hz, tim, loop );
}

