/*****************************************************************************/
/* ARK-7000用																 */
/* キー、ＢＣＲ入力関数														 */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"

static	char	upcTbl[][13] = {		/* ＵＰＣ－ＥからＡへの変換表 */
		{ '0', '0',  A1,  A2, '0', '0', '0', '0', '0',  A3,  A4,  A5, CD },
		{ '0', '0',  A1,  A2, '1', '0', '0', '0', '0',  A3,  A4,  A5, CD },
		{ '0', '0',  A1,  A2, '2', '0', '0', '0', '0',  A3,  A4,  A5, CD },
		{ '0', '0',  A1,  A2,  A3, '0', '0', '0', '0', '0',  A4,  A5, CD },
		{ '0', '0',  A1,  A2,  A3,  A4, '0', '0', '0', '0', '0',  A5, CD },
		{ '0', '0',  A1,  A2,  A3,  A4,  A5, '0', '0', '0', '0', '5', CD },
		{ '0', '0',  A1,  A2,  A3,  A4,  A5, '0', '0', '0', '0', '6', CD },
		{ '0', '0',  A1,  A2,  A3,  A4,  A5, '0', '0', '0', '0', '7', CD },
		{ '0', '0',  A1,  A2,  A3,  A4,  A5, '0', '0', '0', '0', '8', CD },
		{ '0', '0',  A1,  A2,  A3,  A4,  A5, '0', '0', '0', '0', '9', CD },
	};
	
static	char	alphaStr[][10] = {
#if 1
					"0+-*.\0", "1/@:_\0", "2ABCabc\0", "3DEFdef\0", "4GHIghi\0", "5JKLjkl\0", "6MNOmno\0", "7PQRSpqrs\0", "8TUVtuv\0", "9WXYZwxyz\0"
#else
					"0*$%#", "1PQRSpqrs", "2TUVtuv", "3WXYZwxyz", "4GHIghi", "5JKLjkl", "6MNOmno", "7/@:_", "8ABCabc", "9DEFdef"
#endif
				};
				
static	char	HexStr[][6] = {
					"0\0", "1\0", "2ABC\0", "3DEF\0",  "4\0", "5\0", "6\0", "7\0", "8\0", "9\0"
				};
static	short	prmMsk( long prm, long msk );
static	short	mod10( char *code, short len );
static	short	upc_EtoA( char *upc_a, char *upc_e );
static	short	calcUPC_Ecd( char *upc_e );
/******************************************************************************/
/*																			  */
/******************************************************************************/
static short prmMsk( long prm, long msk )
{
	if( prm & msk ) return 1;
	else			return 0;
}

void	cputspc( short x, short y, short ank, short rev, short len )
{

	if( x < 0 || x > 17 || 
		y < 0 || ( ank && y > 17 ) || ( !ank && y > 16 ) || 
		len < 1 || len > 48 ) return;

	revonf( rev );
	curp( x, y );
	while( len-- ){
		if( ank )
			putchs(' ');
		else
			putch(' ');
	}
	if( rev )
		revonf( 0 );
}

void	cputstr( short x, short y, short ank, short rev, char *message )
{

	if( x < 0 || x > 17 || 
		y < 0 || ( ank && y > 17 ) || ( !ank && y > 16 ) ) return;

	revonf( rev );
	curp( x, y );
	if( ank )
		putss( message );
	else
		putsc( message );
	if( rev )
		revonf( 0 );
}

void	cputchar( short x, short y, short ank, short rev, short ch )
{

	if( x < 0 || x > 17 || 
		y < 0 || ( ank && y > 17 ) || ( !ank && y > 16 ) ) return;

	revonf( rev );
	curp( x, y );
	if( ank )
		putchs( ch );
	else
		putch( ch );
	if( rev )
		revonf( 0 );
}
void	cputchs( short x, short y, short ank, char c, short rev, short len )
{
	if( x < 0 || x > 17 || 
		y < 0 || ( ank && y > 17 ) || ( !ank && y > 16 ) ) return;
		
	revonf( rev );
	curp( x, y );
	while( len-- ){
		if( ank )
			putchs(c);
		else
			putch(c);
	}
	if( rev )
		revonf( 0 );
}

/******************************************************************************/
/* Prototypes : short input( short x, short y, char *buf, short len           */
/*            :													, short prm );*/
/* Procedure  : キー、バーコード入力制御									  */
/* Arguments  : x, y ......... 表示開始座標									  */
/*            : buf ....... 入力文字格納領域								  */
/*            : len .......... 入力最大桁数									  */
/*            : prm .......... 入力制御スイッチ								  */
/*            :    ■入力文字モード( デフォルトでIN_NUMERICが設定されている ) */
/*            :       IN_NUMERIC      数字のみの入力						  */
/*            :       IN_ALPHA        文字のみの入力						  */
/*            :       IN_ALLCHARS     数字、文字の入力						  */
/*            :    ■キー入力制御											  */
/*            :       KEY_FIX         キー入力          0:可変長   1:固定長	  */
/*            :       KEY_MINUS       マイナス入力      0:不可     1:可能	  */
/*            :       KEY_DECPOINT    小数点入力        0:不可     1:可能	  */
/*            :       KEY_SKIP        空ＥＮＴ          0:不可     1:可能	  */
/*            :       KEY_UPDOWN      ▲▼キー          0:不可     1:可能	  */
/*            :       KEY_NOTCR       ＥＮＴキー        0:必要     1:不要	  */
/*            :       KEY_CLRRET      クリアキー通知    0:不要     1:必要	  */
/*            :       ※ KEY_MINUS, KEY_DECPOINTはIN_NUMERICモード時のみ有効  */
/*            :    ■バーコード入力制御										  */
/*            :       BCR_NOTCD       Ｃ／Ｄチェック    0:する     1:しない   */
/*            :       BCR_NOTDRW      表示              0:する     1:しない   */
/*            :       BCR_WPC         ＷＰＣ読み取り    0:不可     1:可能	  */
/*            :       BCR_C39         Ｃ３９読み取り    0:不可     1:可能	  */
/*            :       BCR_NW7         ＮＷ７読み取り    0:不可     1:可能	  */
/* Return     : SKIP ............ 空ＥＮＴされた							  */
/*            : ENTRY ........... キーで登録された							  */
/*            : BARENT .......... ＢＣＲで登録された						  */
/*            : BSKEY ........... BSキーが押された							  */
/*            : UPKEY ........... ▲キーが押された							  */
/*            : DOWNKEY ......... ▼キーが押された							  */
/*            : F1KEY ～ F8KEY .. ファンクションキーが押された				  */
/*            : ETC_KEY_INT ..... その他のイベントキーが押された			  */
/*            : PRM_ERR ......... ﾊﾟﾗﾒｰﾀｴﾗｰが発生した						  */
/*            : ERROR ........... 不明のエラー								  */
/******************************************************************************/
static short  DEC_NUM;
short	input( short x, short y, char *buf, short len, long prm )
{
	long		bcr, mode;
	unsigned char fix, minus, dec, skip, updown, notcr, cur, 
				  func, ank, clr, minAdd, trig, comma, pass;
	short		pos, decflg, minflg, ch, ret, bcrLen, declen,
				SaveCh, InNum, bcrSts;
	char		wk[50];

	/*** 入力制御スイッチ ***/
	fix		= prmMsk( prm, KEY_FIX );		/* キー入力			0:可変長   1:固定長	 */
	minus	= prmMsk( prm, KEY_MINUS);		/* マイナス入力		0:不可     1:可能	 */
	dec		= prmMsk( prm, KEY_DECPOINT );	/* 小数点入力		0:不可     1:可能	 */
	skip	= prmMsk( prm, KEY_SKIP );		/* 空ＥＮＴ			0:不可     1:可能	 */
	notcr	= prmMsk( prm, KEY_NOTCR );		/* ＥＮＴキー		0:必要     1:不要	 */
	updown	= prmMsk( prm, KEY_UPDOWN );	/* ▲▼キー			0:不可     1:可能	 */
	bcr		= prm & BCRFLAGMASK;			/* バーコード読み取り指定				 */
	mode  	= prm & INMODEMASK;				/* 入力文字種指定						 */
	cur		= prmMsk( prm, CUR_BLOCK );		/* カーソルタイプ */
	func	= prmMsk( prm, KEY_FUNC );		/* ファンクション */
	ank		= prmMsk( prm, ANK_INPUT );		/* 小ﾌｫﾝﾄ */
	clr		= prmMsk( prm, KEY_CLR );		/* ｸﾘｱｷｰ通知 */
	minAdd	= prmMsk( prm, MIN_ADD );		/* ﾏｲﾅｽを入力長数えない */
	trig  	= prmMsk( prm, TRIG_ENT );		/* トリガをＥＮＴに */
	comma  	= prmMsk( prm, COMMA_INPUT );	/* カンマ入力 */
	pass  	= prmMsk( prm, INP_PASS );		/* パスワード入力 */

	DEC_NUM = ( ( prmMsk( prm, DEC_ONE ) ) ? 1 : 2 );

	/* 入力モードをピクト表示 */
	if( ( mode & IN_HEX ) || ( mode & IN_ALPHA ) ){
		inputpict( LCD_INPUT_PICT_ALPHA );
	} else {
		inputpict( LCD_INPUT_PICT_NUMBER );
	}
	
	len += minAdd;
	len += dec;
	declen = 0;
	SaveCh = InNum = -1;
	cputspc( x+strlen(buf), y, ank, 0, len - strlen( buf ) );
	pos = decflg = minflg = 0;
	/*kbclr();*/
	while( 1 ){
		if( pass ){
			cputchs( x, y, ank, '*', 0, strlen( buf ) );
		} else {
			cputstr( x, y, ank, 0, buf );
		}
		curp( (x+pos)%16, y+((x+pos)/16)*2 );	/* １６文字以上の入力の際の表示対応 */
		//curp( x+pos, y );
		if( ctrl.Cursor ) curonf( ctrl.Cursor, ank );
		else			  curonf( 3, ank );
//ckprintf( 0, 0, True, CLR_BASE, "x=%d y=%d", (x+pos)%16,  y+((x+pos)/16)*2 );
		if( trig && ( !bcr || pos ) ){
			setk( 2, 13, 0x0d );
			setk( 3, 13, 0x0d );
			setk( 4, 13, 0x0d );
			setk( 5, 13, 0x0d );
		}
		bcrSts = -1;
		if( !pos && bcr ){
			bcrSts = bgets( wk );
			if( bcrSts == -1 ) continue;
			if( !bcrSts ) ch = TRIG;
		} else {
			while( !kbhit() );
		}
		if( bcrSts ){
			ch = getch();
			curonf( 0, ank );
		}
		if( trig && ( !bcr || pos ) ){
			setk( 2, 0, 0 );
			setk( 3, 0, 0 );
			setk( 4, 0, 0 );
			setk( 5, 0, 0 );
		}
		if( comma && ch == '.' ) ch = ',';
		if( minus && ch == '.' && !pos ) ch = '-';
		switch( ch ){
			case TRIG:
				strcpy( buf, &wk[1] );
				if( !( bcr & BCR_NOTCD ) && !isBarcode( buf, 1 ) ){
					ret = BAR_ERR;
					goto EndFunc;
				}
				if( !( bcr & BCR_NOTDRW ) ){
					curonf( 0, ank );
					cputspc( x, y, ank, 0, len );
					cputstr( x, y, ank, 0, buf );
					if( ctrl.Cursor ) curonf( ctrl.Cursor, ank );
					else			  curonf( 3, ank );
				}
				ret = BARENT;
				goto EndFunc;
			case UP:
				if( updown && !pos ){
					ret = UPKEY + ( ch - UP );
					goto EndFunc;
				}
				if( !pos ){
					ret = BSKEY;
					goto EndFunc;
				} else {
					if( declen ){
						--declen;
					}
					if( buf[pos] ){
						//cputchar( x+pos, y, ank, 0, ' ' );
						cputchar( (x+pos)%16, y+((x+pos)/16)*2, ank, 0, ' ' );	/* １６文字以上の入力の際の表示対応 */
						if( buf[pos] == '.' ) decflg = 0;
						if( buf[pos] == '-' ) minflg = 0;
						buf[pos] = 0x00;
					} else {
						//cputchar( x+pos, y, ank, 0, ' ' );
						cputchar( (x+pos)%16, y+((x+pos)/16)*2, ank, 0, ' ' );	/* １６文字以上の入力の際の表示対応 */
						--pos;
						//cputchar( x+pos, y, ank, 0, ' ' );
						cputchar( (x+pos)%16, y+((x+pos)/16)*2, ank, 0, ' ' );	/* １６文字以上の入力の際の表示対応 */
						if( buf[pos] == '.' ) decflg = 0;
						if( buf[pos] == '-' ) minflg = 0;
						buf[pos] = 0x00;
					}
				}
				SaveCh = -1;
				break;
			case DOWN:
				if( updown && !pos ){
					ret = UPKEY + ( ch - UP );
					goto EndFunc;
				}
				break;
			case F1:
			case F2:
			case F3:
			case F4:
				if( pos ) break;
				if( func ){
					ret = F1KEY + ( ch - F1 );
					goto EndFunc;
				}
				break;
			case CLR:
				if( clr ){
					ret = CLRKEY;
					goto EndFunc;
				}
				cputspc( x, y, ank, 0, len );
				memset( buf, 0x00, len );
				pos = decflg = minflg = declen = 0;
				SaveCh = InNum = -1;
				break;
			case ENT:
				if( ( mode & IN_HEX ) && SaveCh != -1 ){
					buf[pos] = HexStr[SaveCh-'0'][InNum];
					//cputchar( x+pos, y, 0, 0, HexStr[SaveCh-'0'][InNum] );
					cputchar( (x+pos)%16, y+((x+pos)/16)*2, 0, 0, HexStr[SaveCh-'0'][InNum] );		/* １６文字以上の入力の際の表示対応 */
					if( ++pos >= len ) --pos;
					SaveCh = InNum = -1;
				} else if( ( mode & IN_ALPHA ) && SaveCh != -1 ){
					buf[pos] = alphaStr[SaveCh-'0'][InNum];
					//cputchar( x+pos, y, 0, 0, alphaStr[SaveCh-'0'][InNum] );
					cputchar( (x+pos)%16, y+((x+pos)/16)*2, 0, 0, alphaStr[SaveCh-'0'][InNum] );		/* １６文字以上の入力の際の表示対応 */
					if( ++pos >= len ) --pos;
					SaveCh = InNum = -1;
				} else {
					if( !pos ){
						if( !buf[0] ){
							if( skip ) ret = SKIP;
							else break;
						} else ret = SENTRY;
					} else {
						if( !fix || ( fix && strlen( buf ) >= len ) )
							ret = ENTRY;
						else
							break;
					}
					goto EndFunc;
				}
				break;
			default:
				if( ch >= '0' && ch <= '9' ){				/* 数字 */
					if( mode & IN_HEX ){
						if( ch != SaveCh && SaveCh != -1 ){
							buf[pos] = HexStr[SaveCh-'0'][InNum];
							cputchar( (x+pos)%16,  y+((x+pos)/16)*2, 0, 0, HexStr[SaveCh-'0'][InNum] );	/* １６文字以上の入力の際の表示対応 */
							if( ++pos >= len ) --pos;
							SaveCh = InNum = -1;
						}
						if( !HexStr[ch-'0'][++InNum] )
							 InNum = 0;
						memset( &buf[pos], 0x00, len-pos );
						cputspc( x+pos, y, ank, 0, len-pos );
						cputchar( (x+pos)%16,  y+((x+pos)/16)*2, 0, 0, HexStr[ch-'0'][InNum] );	/* １６文字以上の入力の際の表示対応 */
						SaveCh = ch;
					} else if( mode & IN_ALPHA ){
						if( ch != SaveCh && SaveCh != -1 ){
							buf[pos] = alphaStr[SaveCh-'0'][InNum];
							//cputchar( x+pos, y, 0, 0, alphaStr[SaveCh-'0'][InNum] );
							cputchar( (x+pos)%16,  y+((x+pos)/16)*2, 0, 0, alphaStr[SaveCh-'0'][InNum] );	/* １６文字以上の入力の際の表示対応 */
							if( ++pos >= len ) --pos;
							SaveCh = InNum = -1;
						}
						if( !alphaStr[ch-'0'][++InNum] )
							 InNum = 0;
						/*if( ++InNum > 6 ) InNum = 0;*/
						memset( &buf[pos], 0x00, len-pos );
						cputspc( x+pos, y, ank, 0, len-pos );
						//cputchar( x+pos, y, 0, 0, alphaStr[ch-'0'][InNum] );	
						cputchar( (x+pos)%16,  y+((x+pos)/16)*2, 0, 0, alphaStr[ch-'0'][InNum] );	/* １６文字以上の入力の際の表示対応 */
						//ckprintf( 0, 0, True, CLR_BASE, "x=%d y=%d c=%c", (x+pos)%16,  y+((x+pos)/16)*2, alphaStr[ch-'0'][InNum] );
						SaveCh = ch;
					} else {
						if( mode & IN_ALPHA ) break;
						if( dec && !decflg ){
							if( minAdd && !minflg ){
								if( pos >= ( len-DEC_NUM-2 ) ) break;
							} else {
								if( pos >= ( len-DEC_NUM-1 ) ) break;
							}
						} else if( dec && decflg ){
							if( declen >= DEC_NUM ) break;
						}
						if( !pos ){
							cputspc( x+1, y, ank, 0, len-1 );
							memset( buf, 0x00, len+1 );
						}
						buf[pos] = ch;
						if( minAdd && !minflg ){
							if( ++pos >= ( len-1 ) ) --pos;
						} else {
							if( ++pos >= len ) --pos;
						}
						if( dec && decflg ) ++declen;
					}
				} else if( ch == '.' ){
					while( ktouch() == 0x00000800 );
					if( ktouch() == 0x00100800 ){
						ret = DEC_CLR_KEY;
						goto EndFunc;
					}
					if( dec && !decflg ){	/* 小数点 */
						if( pos < len-1 ){
							decflg = 1;
							if( !pos ){
								cputspc( x, y, ank, 0, len );
								memset( buf, 0x00, len );
							}
							buf[pos] = ch;
							++pos;
						}
					} else {
						break;
					}
				} else if( minus && ch == '-' ){	/* マイナス */
					if( minflg ){
						cputchar( x+pos, y, ank, 0, ' ' );
						if( !buf[pos] ){
							--pos;
							strcpy( wk, &buf[1] );
							strcpy( buf, wk );
							cputchar( x+pos, y, ank, 0, ' ' );
						} else {
							if( minAdd ) --pos;
							strcpy( wk, &buf[1] );
							strcpy( buf, wk );
						}
						minflg = 0;
					} else {
						if( !pos ){
							cputspc( x, y, ank, 0, len );
							memset( buf, 0x00, len );
							buf[pos] = '-';
							minflg = 1;
						} else {
							if( !buf[pos] || minAdd ){
								strcpy( &wk[1], buf );
								wk[0] = '-';
								strcpy( buf, wk );
								minflg = 1;
							}
						}
						if( ++pos >= len ) --pos;
					}
				} else if( ch >= ' ' && ch <= 0x5c ){	/* 文字 */
					if( !mode ) break;
					if( !pos ){
						cputspc( x, y, ank, 0, len );
						memset( buf, 0x00, len );
					}
					buf[pos] = ch;
					if( ++pos >= len ) --pos;
				}
				if( notcr && pos >= len-1 && buf[pos] ){
					if( buf[0] )	cputchar( x+pos, y, ank, 0, buf[pos] );
					ret = ENTRY;
					goto EndFunc;
				}
				break;
		}
	}
EndFunc:
	/* 入力モードのピクトを非表示 */
	inputpict( LCD_INPUT_PICT_OFF );
	curonf( 0, ank );
	/*kbclr();*/
	return	ret;
}

/*****************************************************************************/
/* Ｃ／Ｄチェック															 */
/*****************************************************************************/
short	calcMod10( char *code, short Weight, short len )
{
	short	val = 0;
	short	loop, sw = 0;
	for ( loop = len - 2; loop >= 0 ; loop--, sw++ ) {
		if ( sw%2 == 0 )
			val += ( code[loop] - '0' ) * Weight;
		else
			val += code[loop] - '0';
	}
	val = val % 10;
	if ( val )
		val = 10 - val;
	return	val;
}

static	short	mod10( char *code, short len )
{
	if ( ( code[len-1] - '0' ) == calcMod10( code, 3, len ) )
		return	1;
	return	0;
}

/*****************************************************************************/
/* Function Name : short  upc_EtoA( char *upc_a, char *upc_e )               */
/* Procedure     : ＵＰＣ－ＥをＵＰＣ－Ａに変換する                          */
/* Argument      : upc_a ===> ＵＰＣ－Ａの格納先                             */
/*               : upc_e ===> ＵＰＣ－Ｅ                                     */
/* Return        : 変換ができた ==> 1                                     */
/*               : 変換が出来なかった ===> 0                             */
/*****************************************************************************/
static	short	upc_EtoA( char *upc_a, char *upc_e )
{
	short 	len, tblNo, i, ad;
	char	wk[20];
	len = strlen( upc_e );
	if ( len < 6 || len > 8 )
		return 0;

	if ( len == 6 ) {			/* 先頭の０が省略、CDなし */
		wk[0] = '0';
		strncpy( &wk[1], upc_e, len );
		wk[7] = 0x00;		/* CDに0x00をいれる */
	} else if ( len == 7 ) {		/* CDなし */
		strncpy( wk, upc_e, len );
		wk[7] = 0x00;		/* CDに0x00をいれる */
	} else if ( len == 8 ) {		/* CD付き */
		strncpy( wk, upc_e, len );
		wk[len] = 0x00;
	}

	tblNo = wk[6] - '0';
	for( i = 0, ad = 1; i < 13; i++ ) {
		switch ( upcTbl[tblNo][i] ) {
			case A1:
			case A2:
			case A3:
			case A4:
			case A5:
			case CD:
				/* ＣＤが無ければ計算して付加する */
				if ( upcTbl[tblNo][i] == CD && len != 8 )
					upc_a[i] = calcMod10( upc_a, 3, 13 ) + '0';
				else
					upc_a[i] = wk[ad++];
				break;
			default:
				upc_a[i] = upcTbl[tblNo][i];
				break;
		}
		upc_a[i+1] = 0x00;
	}
	return	1;
}

static	short	calcUPC_Ecd( char *upc_e )
{
	char	wk[20];

	strncpy( wk, upc_e, 7 ); wk[7] = 0x00;			/* C/Dに0x00をセット */
	upc_EtoA( wk, wk );
	return	wk[12] - '0';
}

short	isBarcode( char *code, short inpType )
{
	char	dmy[20], *s;
	short	ret = 0, len;
	
	s = 0x00;
	s = strchr( code, ' ' );
	if( s ) *s = 0x00;
	len = strlen( code );
	if( len == 16 || len == 14 ){
		ret = mod10( code, len );
		if( ret == 1 ){
#if 0
			if( !memcmp( &code[len-13], "00000", 5 ) ){
				memcpy( dmy, &code[len-8], 8 );
				dmy[8] = 0x00;
				dmy[7] = calcMod10( dmy, 3, 8 ) + '0';
				memcpy( code, dmy, 8 );
				code[8] = 0x00;
			} else {
				memcpy( dmy, &code[len-13], 13 );
				dmy[13] = 0x00;
				dmy[12] = calcMod10( dmy, 3, 13 ) + '0';
				memcpy( code, dmy, 13 );
				code[13] = 0x00;
			}
#endif
			ret = 1;
		} else {
			ret = 0;
		}
	} else if( len == 8 || len == 13 ){	/* WPC */
		ret = mod10( code, len );
	}
	return	ret;
}

