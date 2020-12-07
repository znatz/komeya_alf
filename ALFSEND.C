#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ALF7000X_P.H"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"

static char		_SendBuf[146];
static ALFCOMINF 	_AlfInf;
static RecAlfAlphaConnectDef _PrefixRec;
static short		_SndCnt;

static short TDataSend( void );
static short SDataSend( void );
static short IDataSend( void );
static short NDataSend( void );

/*****************************************************************************/
/* 棚卸				  														 */
/*****************************************************************************/
static short TDataSend( void )
{
	short	i, ret, sPos;

	ret = COM_OK;
	for( i=0; i<ctrl.TDataCnt; i++ ){
		ram_read( i, &tdata, TDATAF );
		sPos = 0;
		memset( _SendBuf, ' ', sizeof( _SendBuf ) );
		/* 日付(YYYY/MM/DD) */
		memcpy( &_SendBuf[sPos], tdata.Date, 	4 );						sPos += 4;
		_SendBuf[sPos] = '/';												++sPos;
		memcpy( &_SendBuf[sPos], &tdata.Date[4],2 );						sPos += 2;
		_SendBuf[sPos] = '/';												++sPos;
		memcpy( &_SendBuf[sPos], &tdata.Date[6],2 );						sPos += 2;
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 店舗コード */
		memcpy( &_SendBuf[sPos], tdata.Shop, 	sizeof( tdata.Shop ) );		sPos += sizeof( tdata.Shop );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 店舗2コード */
		memcpy( &_SendBuf[sPos], tdata.Shop, 	sizeof( tdata.Shop ) );		sPos += sizeof( tdata.Shop );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 上段バーコード */
		memcpy( &_SendBuf[sPos], tdata.Code1, 	sizeof( tdata.Code1 ) );	sPos += sizeof( tdata.Code1 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 下段バーコード */
		if( tdata.Code2[0] )
			memcpy( &_SendBuf[sPos], tdata.Code2, 	sizeof( tdata.Code2 ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( tdata.Code2 ) );
		sPos += sizeof( tdata.Code2 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 値下バーコード */
		if( tdata.Code3[0] )
			memcpy( &_SendBuf[sPos], tdata.Code3, 	sizeof( tdata.Code3 ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( tdata.Code3 ) );
		sPos += sizeof( tdata.Code3 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 数量 */
		//memset( &_SendBuf[sPos], '0', 			sizeof( tdata.Num ) );	sPos += sizeof( tdata.Num );
		memcpy( &_SendBuf[sPos], tdata.Num, 		sizeof( tdata.Num ) );	sPos += sizeof( tdata.Num );
		_SendBuf[sPos] = ',';												++sPos;
		memset( &_SendBuf[sPos], '0',8 );						sPos += 8;
		///* 区切り文字 */
		//_SendBuf[sPos] = ',';												++sPos;
		/* 改行コード */
		_SendBuf[sPos] = 0x0d;												++sPos;
		_SendBuf[sPos] = 0x0a;												++sPos;
		/* 送信 */
		ret = alfSend( _SendBuf );
		if( ret != COM_OK ){
			break;
		}
		ckprintf( 3, 7, False, CLR_BASE, "%4d", i+1 );
	}
	return( ret );
}
/*****************************************************************************/
/* 仕入				  														 */
/*****************************************************************************/
static short SDataSend( void )
{
	short	i, ret, sPos;

	ret = COM_OK;
	for( i=0; i<ctrl.SDataCnt; i++ ){
		ram_read( i, &sdata, SDATAF );
		sPos = 0;
		memset( _SendBuf, ' ', sizeof( _SendBuf ) );
		/* 日付(YYYY/MM/DD) */
		memcpy( &_SendBuf[sPos], sdata.Date, 	4 );						sPos += 4;
		_SendBuf[sPos] = '/';												++sPos;
		memcpy( &_SendBuf[sPos], &sdata.Date[4],2 );						sPos += 2;
		_SendBuf[sPos] = '/';												++sPos;
		memcpy( &_SendBuf[sPos], &sdata.Date[6],2 );						sPos += 2;
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 店舗コード */
		memcpy( &_SendBuf[sPos], sdata.Shop, 	sizeof( sdata.Shop ) );		sPos += sizeof( sdata.Shop );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 店舗2コード */
		memcpy( &_SendBuf[sPos], sdata.Shop, 	sizeof( sdata.Shop ) );		sPos += sizeof( sdata.Shop );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 上段バーコード */
		memcpy( &_SendBuf[sPos], sdata.Code1, 	sizeof( sdata.Code1 ) );	sPos += sizeof( sdata.Code1 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 下段バーコード */
		if( sdata.Code2[0] )
			memcpy( &_SendBuf[sPos], sdata.Code2, 	sizeof( sdata.Code2 ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( sdata.Code2 ) );
		sPos += sizeof( sdata.Code2 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 値下バーコード */
		if( sdata.Code3[0] )
			memcpy( &_SendBuf[sPos], sdata.Code3, 	sizeof( sdata.Code3 ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( sdata.Code3 ) );
		sPos += sizeof( sdata.Code3 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 数量 */
		//memset( &_SendBuf[sPos], '0', 			sizeof( sdata.Num ) );	sPos += sizeof( sdata.Num );
		memcpy( &_SendBuf[sPos], sdata.Num, 		sizeof( sdata.Num ) );	sPos += sizeof( sdata.Num );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 下段バーコード */
		if( sdata.DenNo[0] )
			memcpy( &_SendBuf[sPos], sdata.DenNo, 	sizeof( sdata.DenNo ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( sdata.DenNo ) );
		sPos += sizeof( sdata.DenNo );
		/* 改行コード */
		_SendBuf[sPos] = 0x0d;												++sPos;
		_SendBuf[sPos] = 0x0a;												++sPos;
		/* 送信 */
		ret = alfSend( _SendBuf );
		if( ret != COM_OK ){
			break;
		}
		ckprintf( 3, 7, False, CLR_BASE, "%4d", i+1 );
	}
	return( ret );
}
/*****************************************************************************/
/* 移動				  														 */
/*****************************************************************************/
static short IDataSend( void )
{
	short	i, ret, sPos;

	ret = COM_OK;
	for( i=0; i<ctrl.IDataCnt; i++ ){
		ram_read( i, &idata, IDATAF );
		sPos = 0;
		memset( _SendBuf, ' ', sizeof( _SendBuf ) );
		/* 日付(YYYY/MM/DD) */
		memcpy( &_SendBuf[sPos], idata.Date, 	4 );						sPos += 4;
		_SendBuf[sPos] = '/';												++sPos;
		memcpy( &_SendBuf[sPos], &idata.Date[4],2 );						sPos += 2;
		_SendBuf[sPos] = '/';												++sPos;
		memcpy( &_SendBuf[sPos], &idata.Date[6],2 );						sPos += 2;
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;	
		/* 店舗コード */
		memcpy( &_SendBuf[sPos], idata.Shop, 	sizeof( idata.Shop ) );		sPos += sizeof( idata.Shop );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 店舗2コード */
		memcpy( &_SendBuf[sPos], idata.Shop2, 	sizeof( idata.Shop2 ) );	sPos += sizeof( idata.Shop2 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 上段バーコード */
		memcpy( &_SendBuf[sPos], idata.Code1, 	sizeof( idata.Code1 ) );	sPos += sizeof( idata.Code1 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 下段バーコード */
		if( idata.Code2[0] )
			memcpy( &_SendBuf[sPos], idata.Code2, 	sizeof( idata.Code2 ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( idata.Code2 ) );
		sPos += sizeof( idata.Code2 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 値下バーコード */
		if( idata.Code3[0] )
			memcpy( &_SendBuf[sPos], idata.Code3, 	sizeof( idata.Code3 ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( idata.Code3 ) );
		sPos += sizeof( idata.Code3 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 数量 */
		//memset( &_SendBuf[sPos], '0', 			sizeof( idata.Num ) );	sPos += sizeof( idata.Num );
		memcpy( &_SendBuf[sPos], idata.Num, 		sizeof( idata.Num ) );	sPos += sizeof( idata.Num );
		_SendBuf[sPos] = ',';												++sPos;
		memset( &_SendBuf[sPos], '0',8 );						sPos += 8;
		/* 改行コード */
		_SendBuf[sPos] = 0x0d;												++sPos;
		_SendBuf[sPos] = 0x0a;												++sPos;
		/* 送信 */
		ret = alfSend( _SendBuf );
		if( ret != COM_OK ){
			break;
		}
		ckprintf( 3, 7, False, CLR_BASE, "%4d", i+1 );
	}
	return( ret );
}


/*****************************************************************************/
/*  売上			  														 */
/*****************************************************************************/
static short URDataSend( void )
{
	short	i, ret, sPos;
	char strBaika[10];
	char strBumon_taxrate[2];
	short taxrate = atoin( ctrl.TaxRate, sizeof(ctrl.TaxRate)) + 100.0;
	long joudai;
	long baika;

	ret = COM_OK;
	for( i=0; i<ctrl.URDataCnt; i++ ){
		ram_read( i, &urdata, URDATAF );
		sPos = 0;
		memset( _SendBuf, ' ', sizeof( _SendBuf ) );
		
		/* ----------------------------------------------------------------------------------------------  col1  :  日付(YYYY/MM/DD) 	*/
		memcpy( &_SendBuf[sPos], urdata.Date, 	4 );		sPos += 4;
		_SendBuf[sPos] = '/';	++sPos;
		memcpy( &_SendBuf[sPos], &urdata.Date[4],2 );		sPos += 2;
		_SendBuf[sPos] = '/';	++sPos;
		memcpy( &_SendBuf[sPos], &urdata.Date[6],2 );		sPos += 2;
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col2  :  店舗コード 		*/
		memcpy( &_SendBuf[sPos], urdata.Shop, 	sizeof( urdata.Shop ) );	sPos += sizeof( urdata.Shop );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col3  :  担当コード 		*/
		memcpy( &_SendBuf[sPos], urdata.Tanto, 	sizeof( urdata.Tanto ) );	sPos += sizeof( urdata.Tanto );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col4  :  上段バーコード 3?	*/
		memcpy( &_SendBuf[sPos], urdata.Code1, 	sizeof( urdata.Code1 ) );	sPos += sizeof( urdata.Code1 );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col5	 :  下段バーコード 	*/
		if( urdata.Code2[0] )
			memcpy( &_SendBuf[sPos], urdata.Code2, 	sizeof( urdata.Code2 ) );
		else
			memset( &_SendBuf[sPos], '0', 		sizeof( urdata.Code2 ) );
												sPos += sizeof( urdata.Code2 );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col6  :  バーコード3 	*/
		if( urdata.Code3[0] )
			memcpy( &_SendBuf[sPos], urdata.Code3, 	sizeof( urdata.Code3 ) );
		else
			memset( &_SendBuf[sPos], '0', 		sizeof( urdata.Code3 ) );
												sPos += sizeof( urdata.Code3 );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col7  :  部門消費税		*/
		sprintf( strBaika , "%8ld",  urdata.lngBumonTaxRateTax);
		memcpy( &_SendBuf[sPos], strBaika , 8); 			sPos += 8;
		_SendBuf[sPos] = ',';	++sPos;
		
		
		/* ----------------------------------------------------------------------------------------------  col8	 :  数量 		*/
		memcpy( &_SendBuf[sPos], urdata.Num, sizeof( urdata.Num ) );			sPos += 4;
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col9  :  ﾚｼｰﾄNo 		*/
		memcpy( &_SendBuf[sPos], urdata.RecNo, 	sizeof( urdata.RecNo )); 		sPos += sizeof( urdata.RecNo );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col10 :  顧客フラグ 		*/
		memcpy( &_SendBuf[sPos], urdata.KyakuNo, sizeof( urdata.KyakuNo )); 		++sPos;
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col11 :  上代		*/
		sprintf( strBaika , "%10d", atoln( urdata.Joudai, sizeof( urdata.Joudai ) ) );
		memcpy( &_SendBuf[sPos], strBaika , sizeof( strBaika )); 			sPos += sizeof( strBaika );
		_SendBuf[sPos] = ',';	++sPos;

		/* ----------------------------------------------------------------------------------------------  col12 :  売価		*/
		sprintf( strBaika , "%10d", atoln( urdata.Baika, sizeof( urdata.Baika ) ) );
		memcpy( &_SendBuf[sPos], strBaika , sizeof( strBaika )); 			sPos += sizeof( strBaika );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col13 :  現金   		*/
		memcpy( &_SendBuf[sPos], urdata.Genkin , sizeof( urdata.Genkin )); 		sPos += sizeof( urdata.Genkin );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col14 :  実際は売掛	*/
		memcpy( &_SendBuf[sPos], urdata.Credit , sizeof( urdata.Credit )); 		sPos += sizeof( urdata.Credit ); 
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col15 :  金券		*/
		// memcpy( &_SendBuf[sPos], urdata.Kinken , sizeof( urdata.Kinken )); 		sPos += sizeof( urdata.Kinken ); 
		// _SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col16 :  サービス券		*/
		// memcpy( &_SendBuf[sPos], urdata.Coupon , sizeof( urdata.Coupon )); 		sPos += sizeof( urdata.Coupon ); 
		// _SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col17 :  レジNo		*/
		memcpy( &_SendBuf[sPos],ctrl.RejiNo, sizeof( ctrl.RejiNo ) ); 			sPos += sizeof( ctrl.RejiNo );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col18 :  消費理由(レシート)"00"	*/
		// memset( &_SendBuf[sPos], '0',2 );						sPos += 2; 
		// _SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col19 :  消費FLAG	"0"	*/
		// memset( &_SendBuf[sPos], '0',1 );						sPos += 1;
		// _SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col20 :   DateTime		*/
		memcpy( &_SendBuf[sPos], urdata.Date, sizeof( urdata.Date ) ); sPos += sizeof( urdata.Date );
		memcpy( &_SendBuf[sPos], urdata.Time, sizeof( urdata.Time ) ); sPos += sizeof( urdata.Time );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col21 : Year/Month/Day	*/
		memcpy( &_SendBuf[sPos], urdata.Date, 	4 );					sPos += 4;
		_SendBuf[sPos] = '/';								++sPos;
		memcpy( &_SendBuf[sPos], &urdata.Date[4],2 );					sPos += 2;
		_SendBuf[sPos] = '/';								++sPos;
		memcpy( &_SendBuf[sPos], &urdata.Date[6],2 );					sPos += 2;
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col22 : 担当コード		*/
		// memcpy( &_SendBuf[sPos], urdata.Tanto, sizeof( urdata.Tanto ) );	sPos += 2;
		// _SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col23 :  部門税率		*/
		sprintf( strBumon_taxrate , "%2d", urdata.BumonTaxRate);
		memcpy( &_SendBuf[sPos], strBumon_taxrate , sizeof( strBumon_taxrate )); 			sPos += sizeof( strBumon_taxrate );
		_SendBuf[sPos] = ',';	++sPos;
		
		/* ----------------------------------------------------------------------------------------------  col24 :  システム税率金額		*/
		sprintf( strBaika , "%10d", urdata.lngSystemTaxRateTax);
		memcpy( &_SendBuf[sPos], strBaika , sizeof( strBaika )); 			sPos += sizeof( strBaika );
		_SendBuf[sPos] = ',';	++sPos;

		/* ----------------------------------------------------------------------------------------------  col25 :  軽減税率金額		*/
		sprintf( strBaika , "%10d", urdata.lngBumonTaxRateTax);
		memcpy( &_SendBuf[sPos], strBaika , sizeof( strBaika )); 			sPos += sizeof( strBaika );	
		
		
		// 改行コード 
		_SendBuf[sPos] = 0x0d;	++sPos;
		_SendBuf[sPos] = 0x0a;	++sPos;
		
		/* 送信 */
		ret = alfSend( _SendBuf );
		sPos = 0;
		if( ret != COM_OK ){
			break;
		}
		ckprintf( 3, 7, False, CLR_BASE, "%4d", i+1 );
	}
	return( ret );
}


/*****************************************************************************/
/* 値引				  														 */
/*****************************************************************************/
static short NDataSend( void )
{
	short	i, ret, sPos;

	ret = COM_OK;
	for( i=0; i<ctrl.NDataCnt; i++ ){
		ram_read( i, &ndata, NDATAF );
		sPos = 0;
		memset( _SendBuf, ' ', sizeof( _SendBuf ) );
		/* 日付(YYYY/MM/DD) */
		memcpy( &_SendBuf[sPos], ndata.Date, 	4 );						sPos += 4;
		_SendBuf[sPos] = '/';												++sPos;
		memcpy( &_SendBuf[sPos], &ndata.Date[4],2 );						sPos += 2;
		_SendBuf[sPos] = '/';												++sPos;
		memcpy( &_SendBuf[sPos], &ndata.Date[6],2 );						sPos += 2;
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 店舗コード */
		memcpy( &_SendBuf[sPos], ndata.Shop, 	sizeof( ndata.Shop ) );		sPos += sizeof( ndata.Shop );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;		
		/* 店舗2コード */
		memcpy( &_SendBuf[sPos], ndata.Shop, 	sizeof( ndata.Shop ) );		sPos += sizeof( ndata.Shop );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;	
		/* 上段バーコード */
		memcpy( &_SendBuf[sPos], ndata.Code1, 	sizeof( ndata.Code1 ) );	sPos += sizeof( ndata.Code1 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 下段バーコード */
		if( ndata.Code2[0] )
			memcpy( &_SendBuf[sPos], ndata.Code2, 	sizeof( ndata.Code2 ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( ndata.Code2 ) );
		sPos += sizeof( ndata.Code2 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 値下バーコード */
		if( ndata.Code3[0] )
			memcpy( &_SendBuf[sPos], ndata.Code3, 	sizeof( ndata.Code3 ) );
		else
			memset( &_SendBuf[sPos], '0', 			sizeof( ndata.Code3 ) );
		sPos += sizeof( ndata.Code3 );
		/* 区切り文字 */
		_SendBuf[sPos] = ',';												++sPos;
		/* 数量 */
		memcpy( &_SendBuf[sPos], ndata.Num, 		sizeof( ndata.Num ) );	sPos += sizeof( ndata.Num );
		/* 改行コード */
		_SendBuf[sPos] = 0x0d;												++sPos;
		_SendBuf[sPos] = 0x0a;												++sPos;
		/* 送信 */
		ret = alfSend( _SendBuf );
		if( ret != COM_OK ){
			break;
		}
		ckprintf( 3, 7, False, CLR_BASE, "%4d", i+1 );
	}
	return( ret );
}
/*****************************************************************************/
/* 			     															 */
/*****************************************************************************/
void AlfDataSend( short job )
{
	short	ret;
	static char JobStr[6][5] = { "", "売上", "仕入", "棚卸", "移動", "値下" };

	switch( job ){
		case	JOB_SIIRE:
			_SndCnt = ctrl.SDataCnt;		break;
		case	JOB_TANA:
			_SndCnt = ctrl.TDataCnt;		break;
		case	JOB_IDO:
			_SndCnt = ctrl.IDataCnt;		break;
		case	JOB_URI:
			_SndCnt = ctrl.URDataCnt;		break;
	}
TopFunc:
	offkey(1);
	setautoff( 60, 600 );
	offmode( AUTOFF_RESUME_ON );	/* レジュームＯＮ（サスペンド）	*/
	ClsColor();
	ckputss( 0,  0, "<<    ﾃﾞｰﾀ転送>>", False, CLR_TE_TITLE );
	ckputss( 2,  0, JobStr[job], False, CLR_TE_TITLE );
	ckprintf( 4, 5, False, CLR_BASE, "%4d件", _SndCnt );
	ckputss( 0,  7, "   データ転送   ", False, CLR_BASE );
	ckputss( 0,  9, " 実行しますか？ ", False, CLR_BASE );
	ckputss( 0, 15, "1:はい  2:いいえ", False, CLR_BASE );
	if( !okngEx( '1', '2' ) ){
		return;
	}

	offkey(0);
	setautoff( 60, -1 );
	ckputss( 0,  5, "    準備中...   ", False, CLR_BASE );
	kputspc( 0,  7, 16, CLR_BASE );
	kputspc( 0,  9, 16, CLR_BASE );
	kputspc( 0, 15, 16, CLR_BASE );

	/* 通信情報設定 */
	_AlfInf.port	 = USB;
	_AlfInf.sendLen	 = 68+2;	/* 改行コード付き	*/
	switch( job ){
		case 	JOB_URI:
			_AlfInf.sendLen	 = 145+21+2; break;
		case	JOB_SIIRE:
			_AlfInf.sendLen	 = 68+2; break;
		case	JOB_TANA:
			_AlfInf.sendLen	 = 68+2; break;
		case	JOB_IDO:
			_AlfInf.sendLen	 = 68+2; break;
	}	
	_AlfInf.recvLen	 = 0;
	_AlfInf.abort	 = KN_CLR;	/* CLR */
	_AlfInf.revflag	 = False;
	memset( _AlfInf.remoteAddr, 0x00, sizeof( _AlfInf.remoteAddr ) );
	
	/* PLUS手順通信情報設定 */
	memset( &_PrefixRec, 		' ', 		sizeof( _PrefixRec ) );
	memcpy( _PrefixRec.AlfMark, "ALPHA", 	sizeof( _PrefixRec.AlfMark ) );
	/***端末を特定できる情報を２０バイト以内でセット***/
	memcpy( _PrefixRec.TermNo, 	"データ転送", 10 );
	/***送信時は「Ｓ」固定　受信時は「Ｒ」固定***/
	_PrefixRec.Kubun[0] = 'S';
	/***送信時のレコード長***/
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.BlockLen ), _AlfInf.sendLen );
	memcpy( _PrefixRec.BlockLen, dspBuf, strlen( dspBuf ) );
	/***ファイル名***/
	
	switch( job ){
		case	JOB_SIIRE:
			memcpy( _PrefixRec.FileName, "FILE1", 5 );		break;
		case	JOB_TANA:
			memcpy( _PrefixRec.FileName, "FILE2", 5 );		break;
		case	JOB_IDO:
			memcpy( _PrefixRec.FileName, "FILE3", 5 );		break;
		case	JOB_URI:
			memcpy( _PrefixRec.FileName, "FILE4", 5 );		break;
	}
	/***ファイルサイズ　件数×レコード長***/
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.FileSize ), _SndCnt * _AlfInf.sendLen );
	memcpy( _PrefixRec.FileSize, dspBuf, strlen( dspBuf ) );
	/***件数***/
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.RecordCount ), _SndCnt );
	memcpy( _PrefixRec.RecordCount, dspBuf, strlen( dspBuf ) );
	
	/* OPEN */
	ret = alfOpen_PrefixSend_Length( &_AlfInf,
					_PrefixRec.TermNo, 		sizeof( _PrefixRec.TermNo ),
					_PrefixRec.Kubun, 		sizeof( _PrefixRec.Kubun ),
					_PrefixRec.BlockLen, 	sizeof( _PrefixRec.BlockLen ),
					_PrefixRec.FileName, 	sizeof( _PrefixRec.FileName ),
					_PrefixRec.FileSize, 	sizeof( _PrefixRec.FileSize ),
					_PrefixRec.RecordCount, sizeof( _PrefixRec.RecordCount ) );
	if( ret != COM_OK ){
		goto EndSend;
	}

	ckputss( 0,  5, "  データ転送中  ", False, CLR_BASE );
	ckputss( 0,  7, "  [    ／    ]  ", False, CLR_BASE );
	ckputss( 0, 15, "    CLR:中止    ", False, CLR_BASE );
	ckprintf( 9, 7, False, CLR_BASE, "%4d", _SndCnt );

	/* データ送信処理 */
	switch( job ){
		case	JOB_SIIRE:
			ret = SDataSend();  	break;
		case	JOB_TANA:
			ret = TDataSend();		break;
		case	JOB_IDO:
			ret = IDataSend();		break;
		case	JOB_URI:
			ret = URDataSend();		break;
	}

EndSend:
	if( ret == COM_OK )
		ret = alfClose();
	else
		alfClose();
	if( ret != COM_OK && ret != COM_EOF ){
		if( ret == COM_ABORT )
			ckputss( 0, 5, "    強制終了    ", False, CLR_BASE );
		else if( ret == COM_TIMEOUT )
			ckputss( 0, 5, "  タイムアウト  ", False, CLR_BASE );
		else if( ret == COM_ERROR )
			ckputss( 0, 5, "   通信エラー   ", False, CLR_BASE );
		else
			ckputss( 0, 5, "    異常終了    ", False, CLR_BASE );
		beep( 50, 1 );
		ckputss( 0, 15, "    ENT:再送    ", False, CLR_BASE );
		while( !okng() );
		goto TopFunc;
	} else {
		//ckputss( 0,  5, "    送信完了    ", False, CLR_BASE );
		//ckputss( 0, 15, "    ENT:確認    ", False, CLR_BASE );
		beep( 50, 1 );
		//while( !okng() );
		
		/* 削除確認 */
		ckputss( 2,  0, JobStr[job], False, CLR_TE_TITLE  );
		kputspc( 0, 5, 16, CLR_BASE );
		ckprintf( 4, 5, False, CLR_BASE, "%4d件", _SndCnt );
		ckputss( 0,  7, "   データ削除   ", False, CLR_BASE );
		ckputss( 0,  9, " 実行しますか？ ", False, CLR_BASE );
		ckputss( 0, 15, "1:はい  2:いいえ", False, CLR_BASE );
		if( okngEx( '1', '2' ) ){
			switch( job ){
				case	JOB_SIIRE:
					ctrl.SDataCnt = 0;		break;
				case	JOB_TANA:
					ctrl.TDataCnt = 0;		break;
				case	JOB_IDO:
					ctrl.IDataCnt = 0;		break;
				case	JOB_URI:
					ctrl.URDataCnt = 0;		break;
			}
			ram_write( 0, &ctrl, CTRLF );
			ckputss( 0, 5, "  削除しました  ", False, CLR_BASE );
			kputspc( 0, 7, 16, CLR_BASE );
			kputspc( 0, 9, 16, CLR_BASE );
			kputspc( 0,15, 16, CLR_BASE );
			beep( 50, 1 );
			ckputss( 0, 15, "    ENT:確認    ", False, CLR_BASE );
			while( !okng() );
		}
	}
	offkey(1);
	offmode( AUTOFF_RESUME_OFF );	/* レジュームＯＦＦ	*/
}

