/*****************************************************************************/
/* ARK-7000用 関数															 */
/*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "sub.h"

/*********************************************************/
/* ブザー関係										 	 */
/*********************************************************/
void	beep( short tim, short loop )
{
#define	BEEP_HzP		3000
	for( ; loop>0; loop-- ){
		buzzer( BEEP_HzP );
		delay( tim );
		buzzer( 0 );
		if( loop > 1 )
			delay( 6 );
	}
	kbclr();
}

void	beeb( short tim, short loop, short wait )
{
#define	BEEP_HzB		3000
	for ( ; loop > 0; loop-- ) {
		buzzer( BEEP_HzB );
		delay( tim );
		buzzer( 0 );
		if( loop > 1 )
			delay( wait );
	}
	kbclr();
}

void	playHz( short hz, short tim, short loop )
{
	for ( ; loop>0; loop-- ){
		buzzer( hz );
		delay( tim/2 );
		buzzer( 0 );
	}
}

/*********************************************************/
/* 画面表示												 */
/*********************************************************/

/* 色指定 */
void	ReDrowDisplay( short clr )
{
	switch( clr ){
		case CLR_BASE:
				setcolor( CLR_BASE_TEXT, CLR_BASE_BACK );		break;
		case CLR_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_TITLE_BACK );		break;
		case CLR_SI_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_FUNC_GREEN2 );	break;
		case CLR_ZA_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_PURPLE );		  	break;
		case CLR_NE_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_PINK );		    break;
		case CLR_ID_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_LIGHTBLUE );		break;
		case CLR_TE_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_GREEN );		    break;
		case CLR_DE_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_LIGHTYELLOW );	break;
		case CLR_KP_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_LIME );    		break;
		case CLR_UR_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_CYAN );			break;
		case CLR_SE_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_LIME );			break;
		case CLR_PR_TITLE:
				setcolor( CLR_TITLE_TEXT, CLR_SILVER );			break;				
		case CLR_LABEL:
				setcolor( CLR_LABEL_TEXT, CLR_LABEL_BACK );		break;
		case CLR_MSGBOX:
				setcolor( CLR_MSGBOX_TEXT, CLR_MSGBOX_BACK );	break;
	}
}

/* 画面と背景色の初期化 */
void	ClsColor( void )
{
	setcolor( CLR_BASE_TEXT, CLR_BASE_BACK );
	cls();
}

/* 反転指定表示（大文字列） */
void	ckputss( short x, short y, char *str, char revflag, short clr )
{
	ReDrowDisplay( clr );
	curp( x, y );
	revonf( revflag );
	putsc( str );
	if( revflag ) revonf( 0 );
}

/* 反転指定表示（小文字列） */
void	caputss( short x, short y, char *str, char revflag, short clr )
{
	ReDrowDisplay( clr );
	curp( x, y );
	revonf( revflag );
	putss( str );
	if( revflag ) revonf( 0 );
}

/* 反転・長さ指定表示（大文字列） */
void	ckputsn( short x, short y, char *str, short len, char revflag, short clr )
{
	char	wk[30];

	ReDrowDisplay( clr );
	strncpy( wk, str, len );
	wk[len] = 0x00;
	curp( x, y );
	revonf( revflag );
	putsc( wk );
	if( revflag ) revonf( 0 );
}

/* 反転・長さ指定表示（小文字列） */
void	caputsn( short x, short y, char *str, short len, char revflag, short clr )
{
	char	wk[30];

	ReDrowDisplay( clr );
	strncpy( wk, str, len );
	wk[len] = 0x00;
	curp( x, y );
	revonf( revflag );
	putss( wk );
	if( revflag ) revonf( 0 );
}

void	kputspc( short x, short y, short len, short clr )
{
	short		i;

	ReDrowDisplay( clr );
	curp( x, y );
	for( i=0; i<len; i++ )
		putch( ' ' );
}

void	aputspc( short x, short y, short len, short clr )
{
	short		i;

	ReDrowDisplay( clr );
	curp( x, y );
	for( i=0; i<len; i++ )
		putchs( ' ' );
}

void	kputchar( short x, short y, char c, short len, short clr )
{
	short		i;

	ReDrowDisplay( clr );
	curp( x, y );
	for( i=0; i<len; i++ )
		putch( c );
}

void	aputchar( short x, short y, char c, short len, short clr )
{
	short		i;

	ReDrowDisplay( clr );
	curp( x, y );
	for( i=0; i<len; i++ )
		putchs( c );
}

/* 反転指定表示（大文字列） */
void	ckputssEx( short x, short y, char *str, char revflag, short clr )
{
	char	*st, *spc;
	char	wk[25];
	short	len;

	ReDrowDisplay( clr );
	st = str;
	revonf( revflag );
	for( ; *st; ){
		spc = strchr( st, '_' );
		if( spc ){
			len = spc - st;
			if( *st != '_' ){
				memncpy( wk, st, len );
				curp( x, y );putsc( wk );
			}
			st = spc + 1;
			x = x + len + 1;
		} else {
			curp( x, y );putsc( st );
			break;
		}
	}
	if( revflag ) revonf( 0 );
}

/* 反転指定表示（大文字列） */
void	caputssEx( short x, short y, char *str, char revflag, short clr )
{
	short		i;

	ReDrowDisplay( clr );
	curp( x, y );
	revonf( revflag );
	for( i=0; i<strlen(str); i++ ){
		curp( x+i, y );
		if( *(str+i) != '_' )
			putchs( *(str+i) );
	}
	if( revflag ) revonf( 0 );
}

void	ckprintf( short x, short y, short rev, short clr, char *format, ... )
{
	char		buf[30];
  	va_list		ap;

	ReDrowDisplay( clr );
	va_start( ap, format );
	vsprintf( buf, format, ap );
	va_end( ap );
	ckputss( x, y, buf, rev, clr );
}

void	caprintf( short x, short y, short rev, short clr, char *format, ... )
{
	char		buf[30];
  	va_list		ap;

	va_start( ap, format );
	vsprintf( buf, format, ap );
	va_end( ap );
	ReDrowDisplay( clr );
	caputss( x, y, buf, rev, CLR_BASE );
}

/*********************************************************/
/* メモリ関係											 */
/*********************************************************/
long	atoln( char *buf, short len )
{
	char	wk[10];

	strncpy( wk, buf, len );
	wk[len] = 0x00;
	return	atol( wk );
}

short		atoin( char *buf, short len )
{
	char 	wk[10];

	strncpy( wk, buf, len );
	wk[len] = 0x00;
	return	atoi( wk );
}

double	atofn( char *buf, short len )
{
	char 	wk[10];

	strncpy( wk, buf, len );
	wk[len] = 0x00;
	return	atof( wk );
}

void	memncpy( char *out, char *in, short len )
{
	memcpy( out, in, len );
	out[len] = 0x00;
}

void	addDot( char *in, short Fixlen )
{
	char	dmy1[11], dmy2[11], mflg;
	short 	i, len, dotNum;

	if( in[0] == '-' ){
		mflg = 1;
		strcpy( dmy1, &in[1] );
	} else {
		mflg = 0;
		strcpy( dmy1, in );
	}
	*( strchr( dmy1, '.' ) ) = 0x00;
	len	= strlen( dmy1 );
	dotNum	= len * 10 / 31;
	dmy2[ len + dotNum ] = 0x00;
	for( i=0; i<len; i++ ){
		if( i && i % 3 == 0 ){
			dmy2[ (len-i+dotNum)-1 ] = ',';
			--dotNum;
		}
		dmy2[ (len-i+dotNum)-1 ] = dmy1[ len-(i+1) ];
	}
	if( mflg ){
		dmy1[0] = '-';
		strcpy( &dmy1[1], dmy2 );
	} else {
		strcpy( dmy1, dmy2 );
	}
	strcat( dmy1, strchr( in, '.' ) );
	len = strlen( dmy1 );
	if( Fixlen != -1 && len != Fixlen ){
#if 1
		/* 頭ｽﾍﾟｰｽ埋め */
		memset( dmy2, ' ', Fixlen-len );
		strcpy( &dmy2[Fixlen-len], dmy1 );
#else
		memset( dmy2, ' ', Fixlen ); dmy2[Fixlen] = 0x00;
		memcpy( dmy2, dmy1, strlen( dmy1 ) );
#endif
	} else {
		strcpy( dmy2, dmy1 );
	}
	strcpy( in, dmy2 );
}

/*********************************************************/
/* 入力関係												 */
/*********************************************************/
short		okng( void )
{
	short		ret;

	kbclr();
	while( 1 ){
		ret = getch();
		if( ret == CLR ){
			ret = False;
			break;
		} else if( ret == ENT ){
			ret = True;
			break;
		}
	}
	return( ret );
}

short		okngEx( short Yes, short No )
{
	short		ret;

	kbclr();
	while( 1 ){
		ret = getch();
		if( ret == Yes ){
			ret = True;
			break;
		} else if( ret == No ){
			ret = False;
			break;
		}
	}
	return( ret );
}

/*********************************************************/
/* 日付関係												 */
/*********************************************************/
#define DIM(x)	(sizeof(x)/sizeof(x[0]))
static	short  	daytbl[] = { 0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/********************************************************/
/*	getweek												*/
/*														*/
/*	[argument]											*/
/*            year   : 	0 - 9999						*/
/*            month  :  1 - 12							*/
/*            day    :  1 - 31							*/
/*														*/
/*	[return]											*/
/*            week day 									*/
/*               0:日 1:月 2:火 3:水 4:木 5:金 6:土 	*/
/*														*/
/********************************************************/
short getweek(short year, short month, short day)
{
	static short mtab[] = {
		/* 1  2  3  4  5  6  7  8  9 10 11 12 */
		   0,
		  31,
		  31+28,
		  31+28+31,
		  31+28+31+30,
		  31+28+31+30+31,
		  31+28+31+30+31+30,
		  31+28+31+30+31+30+31,
		  31+28+31+30+31+30+31+31,
		  31+28+31+30+31+30+31+31+30,
		  31+28+31+30+31+30+31+31+30+31,
		  31+28+31+30+31+30+31+31+30+31+30,
		  31+28+31+30+31+30+31+31+30+31+30+31,
	};
	short leapyear_days, days, wday;
	short leapyear = 0;
	if( year < 80 ) year += 2000;
	else			year += 1900;
	if ((year%4) == 0) {
		leapyear = 1;
		if ((year%100) == 0) {
			leapyear = 0;
			if ((year%400) == 0)
				leapyear = 1;
		}
	}
	days = mtab[month-1]+day-1;
	if (leapyear && month > 2) days++;
	leapyear_days = ((year-1)/4) - (year-1)/100 + (year-1)/400;
	wday = (year-1 + leapyear_days + days + 1)%7;
	return wday;
}

/**************************************************/
/* 年は２桁で渡す                                 */
/**************************************************/
void getrtc4( char *s )
{
	short	year;

	getrtc( &s[2] );
	year = atoin( &s[2], 2 );
	if( atoin( &s[2], 2 ) < 90 ) memcpy( s, "20", 2 );
	else						 memcpy( s, "19", 2 );
}

/**************************************************/
/* HH:MMを返す									   */
/**************************************************/
void getrtcHourMinute( char *s ) {

	char buf[14];
	getrtc( &buf[0]);

	const int c = sizeof(":");

	memcpy( s, &buf[6], 2);
	memcpy( s+2, ":", c);
	memcpy( s+2+c, &buf[8], 2);
}

/**************************************************/
/* HH:MM:SSを返す								   */
/**************************************************/
void getrtcHourMinuteSecond( char *s ) {

	char buf[14];
	getrtc( &buf[0]);

	// const int c = sizeof(":");

	short hh = atoin( &buf[6], 2 );
	short nn = atoin( &buf[8], 2 );
	short ss = atoin( &buf[10], 2 );
	snprintf(s, sizeof("00:00:00"), "%02d:%02d:%02d", hh,nn,ss);
	// memcpy(s, buf, sizeof("00:00:00"));
	// memcpy( s, &buf[6], 2);
	// memcpy( s+2, ":", c);
	// memcpy( s+2+c, &buf[8], 2);
	// memcpy( s+2+c+2, ":", c);
	// memcpy( s+2+c+2+c, &buf[10], 2);
}


/**************************************************/
/* 年をインクリメント                             */
/* 年は２桁で渡す                                 */
/**************************************************/
void	addYear( char *outYear, char *inYear )
{
	short	year;
	char	dmy[5];
	
	year = atoin( inYear, 2 );
	++year;
	if( year >= 100 ) year -= 100;
	sprintf( dmy, "%02d", year );
	memcpy( outYear, dmy, 2 );
}

/*******************************************************/
/* 日付をインクリメント                                */
/*******************************************************/
void addDay( char *yymmdd, short addCnt )
{
	short	yy, mm, dd, i;
	char	wk[10];
	
	yy = atoin( yymmdd, 2 );
	mm = atoin( &yymmdd[2], 2 );
	dd = atoin( &yymmdd[4], 2 );
	
	if( yy < 80 ) yy += 2000;
	else yy += 1900;

	for( i=0; i<addCnt; i++ ){
		if ( yy % 4 == 0 && yy % 100 != 0 || yy % 400 == 0 )
			daytbl[2] = 29;
		else
			daytbl[2] = 28;
		++dd;
		if( dd > daytbl[mm] ){
			dd = 1;
			++mm;
			if( mm > 12 ){
				mm = 1;
				++yy;
			}
		}
	}
	if( yy >= 2000 ) yy -= 2000;
	else yy -= 1900;
	sprintf( wk, "%02d%02d%02d", yy, mm, dd );
	memcpy( yymmdd, wk, 6 );
}

/*******************************************************/
/* 納品日のチェック                                    */
/* 1:本日<=入力日<１ヶ月先                             */
/*******************************************************/
short datechk( char *date, char *now, short check )
{
	short		inYY, inMM, inDD;
	short		nowYY, nowMM, nowDD;
	char		inDate[10], nowDate[10];
	
/* ノーマル日付チェック */
	inYY = atoin( date, 2 );
	if( inYY < 80 )
		inYY += 2000;
	else
		inYY += 1900;
	inMM = atoin( &date[2], 2 );
	inDD = atoin( &date[4], 2 );

	if( inYY % 4 == 0 && inYY % 100 != 0 || inYY % 400 == 0 )
		daytbl[2] = 29;
	else
		daytbl[2] = 28;
	if( inMM >= 1 && inMM <= 12 && inDD >= 1 && inDD <= daytbl[inMM] );
	else return True;
	/* 月日が０なら抜ける */
	if( !memcmp( &now[2], "0000", 4 ) ) return( False );
	sprintf( inDate, "%04d%02d%02d", inYY, inMM, inDD );
	/* 過去日付チェック */
	nowYY = atoin( now, 2 );
	if( nowYY < 80 )
		nowYY += 2000;
	else
		nowYY += 1900;
	nowMM = atoin( &now[2], 2 );
	nowDD = atoin( &now[4], 2 );
	sprintf( nowDate, "%04d%02d%02d", nowYY, nowMM, nowDD );
	if( check == 1 ){
		if( strcmp( nowDate, inDate ) > 0 )
			return True;
	} else if( check == 2 ){
		if( strcmp( nowDate, inDate ) < 0 )
			return True;
	}
	return False;
}
//****************************************************************************
/* Z.N
    char -> int
*/
//****************************************************************************
int ctoi(char c) {
	switch (c) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		default: return 0;
	}
}
//****************************************************************************
/* Z.N
    通常のpow関数のdouble制限があるから、
    自作
*/
//****************************************************************************
long powOfTen(int exp) {
    long base = 1;
    for (int i=0;i<exp;i++){
        base *= 10;
    }
    return base;
}

//****************************************************************************
//* 	数字から文字列														 */
//****************************************************************************
void convertToString( long num, char str[MAXLEN] ){
	int i, j, keta, temp;
	int minus = 0;

	// 下位から順に数字を取り出し3桁の区切りに,を入れる 
	i = keta = 0;
	if( num < 0 ) {
		num = -num;
		minus = 1;
	}
	do {
		str[i++] = num % 10 + '0';  
		keta++;
		num /= 10;
	} while( num != 0 && i < MAXLEN );
	
	if( minus ) str[i++] = '-';
	str[i] = '\0';

	// 逆順にコピーする
	j = i-1;
	for( i = 0; j > i; i++, j-- ) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}
}
//****************************************************************************
//* 	三桁区切り														 */
//****************************************************************************
void insComma( long num, char str[MAXLEN] ){
	int i, j, keta, temp;
	int minus = 0;

	// 下位から順に数字を取り出し3桁の区切りに,を入れる 
	i = keta = 0;
	if( num < 0 ) {
		num = -num;
		minus = 1;
	}
	do {
		str[i++] = num % 10 + '0';  
		keta++;
		num /= 10;
		if( keta % 3 == 0 && num != 0 ) str[i++] = ',';
	} while( num != 0 && i < MAXLEN );
	
	if( minus ) str[i++] = '-';
	str[i] = '\0';

	// 逆順にコピーする
	j = i-1;
	for( i = 0; j > i; i++, j-- ) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
	}
}

void printNumberWithComma( long num, int col, int row ) {
	char strKingaku[7];
    	insComma( num,strKingaku );
	ckprintf(col, row, False, CLR_BASE, "%7s", strKingaku );
}

void drawLineCrossScreen(long y) {
	gline(0, y*8+16, 127, y*8+16, 1);
}
