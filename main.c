#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"

				/* 012345678901234567890123456789*/
#define	LOADINGFLAG		" ポスコ様向け  Ver100 "

CONTROLDATA	ctrl;
BUMST		bumst;	/* 部門マスタ			*/
HIMST		himst;	/* 品種マスタ			*/
TAMST		tamst;	/* 担当マスタ			*/
REMST		remst;	/* ﾚｼｰﾄマスタ			*/
TDATA		tdata;	/* 棚卸データ			*/
IDATA		idata;  /* 移動データ			*/
SDATA		sdata;  /* 仕入データ			*/
NDATA		ndata;  /* 値引データ			*/
TENPO		tenpo;  /* 値引データ			*/
INFOUR		infour; /* 売上データ			*/
INFOUR2		infour2;/* 売上データ			*/
INFO		info;	/* 売上データ			*/
URDATA		urdata; /* 売上データ			*/

char		scanBuf[30];
char		dt[32], dspBuf[50];

static void StartLoading( void );
static void	Wakeup( void );
static void	InitScaner( void );
/****************************************************************/
/*	初期ﾛｰﾃﾞｨﾝｸﾞ												*/
/****************************************************************/
static void StartLoading( void )
{
	/*setfont(1);			/* ﾌｫﾝﾄｻｲｽﾞ 16ﾄﾞｯﾄ */
	kbclr();			/* キーバッファのクリア	*/
	ram_read( 0, &ctrl, CTRLF );
  	if( strncmp( ctrl.LoadFlag, LOADINGFLAG, sizeof( LOADINGFLAG ) ) != 0 ){
		cls();		/*  0123456789012345*/
		ckputss( 0, 4, "   初期化中...  ", False, CLR_BASE );
  		if( ( ram_size() >= disksize() ) ){
			ckprintf( 0, 4, False, CLR_BASE, "[%ld>=%ld]", ram_size(), disksize() );
			beep( 50, 1 );
			getch();
		}
		memset( &ctrl, 0x00, sizeof( ctrl ) );
		strcpy( ctrl.LoadFlag, LOADINGFLAG );
  		ctrl.Cursor   = 3;
  		ram_write( 0, &ctrl, CTRLF );
	}
}

/****************************************************************/
/*																*/
/****************************************************************/
static void	Wakeup( void )
{
	static	char	yymmdd[20];

	playHz( 1000, 20, 1 );
	playHz( 1500, 20, 1 );
	memset( yymmdd, 0x00, sizeof( yymmdd ) );
    getrtc4( yymmdd );
	cls();
			 	   /*0123456789012345*/
	ckputss(  0, 4, "  ＨＴシステム  ", False, CLR_BASE );
	ckputss(  5, 6, VER, False, CLR_BASE );
	ckprintf( 0, 8, False, CLR_BASE, "%4d/%02d/%02d %02d:%02d", atoin( yymmdd, 4 ), atoin( &yymmdd[4], 2 ),
			 atoin( &yymmdd[6], 2 ), atoin( &yymmdd[8], 2 ), atoin( &yymmdd[10], 2 ) );
	delay( 200 );
}

/****************************************************************/
/*																*/
/****************************************************************/
static void	InitScaner( void )
{
	// コマンド詳細は、コマンド表参照のこと。
	// 	                             secu, 全読, 全読, ﾀｲﾏ-, 3sec, d2o5, d2o5, itf , itf , sufix 
	// unsigned char command[32] = { 0x00, 0xFF, 0x6F, 0x15, 0x83, 0x0C, 0x00, 0x0E, 0x00, 0x00,
	//                                op1,  op2,  c39 , c39 , gs1 , nw7 , nw7 , c128, c128, none
								//   0x17, 0x43, 0x00, 0x80, 0x07, 0x00, 0x00, 0xC1, 0x37, 0x00,
	//                               led , beep, vib , c11 , c11 , m2o5, m2o5, msi , msi , c93
								//   0x05, 0x05, 0x00, 0x04, 0x37, 0x0E, 0x00, 0x06, 0xB7, 0x04,
	//                               c93 , none
						 	    //   0x37, 0x00 };
	// ***** コマンド概要 *****
	// 全読み、コードID付加、タイマー優先、3秒タイムアウト、
	// ************************
	// トリガ優先モードにするとトリガー押下中は発光を続けてタイムアウトしない。
	// トリガ押下に連動させて発光したい場合は、トリガ優先モードにする。
	
	// * by Z.N 2桁目 0x48 : ITFも読む   0x40: JANのみ
	// if( bmode( ( unsigned char * )"\x10\x48\x00\x50\x05\x0C\x00\x0E\x10\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x05\x05\x00\x04\x37\x0E\x00x06xB7x04x37x00" ) ){
	bmd_clr();					/* 1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28 29 30 31 32 */
	if( bmode( ( unsigned char * )"\x10\x40\x00\x50\x05\x0C\x00\x0E\x10\x00\x27\x43\x00\x00\x00\x00\x00\x00\x00\x00\x05\x05\x00\x04\x37\x0E\x00x06xB7x04x37x00" ) ){
		ckputss( 0, 4, "  ｽｷｬﾅ設定失敗  ", False, CLR_BASE );
		beep( 50, 1 );
		getch();
	}
}

/****************************************************************/
/*																*/
/****************************************************************/
void	main(void)
{
	ClsColor();
	StartLoading();
	InitScaner();
	Wakeup();
	menu();
}

void 	displayMsg( long msg ) 
{
	char buf[256];
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", msg);
	msgbox( LCD_MSGBOX_WARNING, 0x00, "", buf, "確認", 0x00 );
	return;
}

void 	displayStringMsg( char* msg ) 
{
	char buf[256];
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%s", msg);
	msgbox( LCD_MSGBOX_WARNING, 0x00, "", buf, "確認", 0x00 );
	return;
}

/* Hybrid 二段上代 */
long getJyoudai(const char *barcode) {
    char wk[10];
    strncpy( wk, barcode+8, 4 );
    wk[4]=0x00;
    long chkpoint = ctoi(wk[3]);
    if( chkpoint < 5) {
        wk[3]=0x00;
        return atol(wk)*(powOfTen(chkpoint));
    }
    return atol(wk);
}

/* Hybrid 二段下代 */
long getGedai(const char *barcode) {
    char wk[10];
    // * : 1...8,9,10,11(下代4桁), 12(桁数), CD
    strncpy( wk, barcode+7, 5 );
    // * : 12桁目
    long chkpoint = ctoi(wk[4]);
    // * : 0,1,2,3,4 set to null terminated
    wk[4]=0x00;
    return atol(wk)*(powOfTen(chkpoint))/2;
}

/* Hybrid 三段値下 */
long GetNesage(const char *barcode, long jyoudai) {

	/* VB Code VBには重複処理しないように
	'売価
	If StrComp(Mid(dat$(5), 1, 2), "30") = 0 Then
		'%値引シール?
		If StrComp(Mid(dat$(5), 1, 3), "309") = 0 And StrComp(Mid(dat$(5), 6, 2), "99") = 0 Then
			Bai$ = Trim(Str(Val(Jyo$) - Val(Jyo$) * Val(Mid(dat$(5), 4, 2)) / 100))
		Else
			Bai$ = Mid$(dat$(5), 3, 5)
		End If
	Else
		Bai$ = Jyo$
	End If
	*/

	if ( atoin(barcode,3) == 309 ) {

	} else {
		char wk[10];
		strncpy( wk, barcode+2, 5 );
		wk[5]=0x00;
		return atol(wk);
	}

}