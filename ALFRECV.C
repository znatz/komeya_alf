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

static char	_RecvBuf[4096];
static ALFCOMINF 	_AlfInf;
static	RecAlfAlphaConnectDef _PrefixRec;

/*****************************************************************************/
/* 	マスタ受信    															 */
/*****************************************************************************/
void AlfRecvMst( void )
{
	short	ret;
	
	setautoff( 60, -1 );
	ClsColor();
	ckputss( 0,  0, " <<マスタ受信>> ", False, CLR_TITLE );
	ckputss( 0,  5, "  マスタ受信を  ", False, CLR_BASE );
	ckputss( 0,  7, "  行いますか？  ", False, CLR_BASE );
	ckputss( 0, 15, "1:はい  2:いいえ", False, CLR_BASE );
	if( !okngEx( '1', '2' ) ){
		return;
	}
	
	offkey(0);
	kputspc( 0,  5, 16, CLR_BASE );
	kputspc( 0, 15, 16, CLR_BASE );
	ckputss( 0,  4, "    準備中...   ", False, CLR_BASE );
	ckputss( 0,  7, "   部門   品種  ", False, CLR_BASE );
	ckputss( 0,  9, "  [    ] [    ] ", False, CLR_BASE );
	ckputss( 0,  11, "   担当   ﾚｼｰﾄ", False, CLR_BASE );
	ckputss( 0,  13, "  [    ] [    ] ", False, CLR_BASE );
	ckputss( 0, 15, "    CLR:中止    ", False, CLR_BASE );
	
	short BUMSTCnt = 0;
	short HIMSTCnt = 0;
	short TAMSTCnt = 0;
	short REMSTCnt = 0;
	
	/* ALF手順通信情報設定 */
	_AlfInf.port	 = USB;
	_AlfInf.sendLen	 = 0;
	_AlfInf.recvLen	 = sizeof(BUMST)+2; //37;	/* コード(2)+部門名(20)+略称(10)+税率(2)+crlf(2) */ TODO
	_AlfInf.abort	 = KN_CLR;	/* CLR */
	_AlfInf.revflag	 = False;
	memset( _AlfInf.remoteAddr, 0x00, sizeof( _AlfInf.remoteAddr ) );
	
	/* PLUS手順通信情報設定 */
	memset( &_PrefixRec, 		' ', 		sizeof( _PrefixRec ) );
	memcpy( _PrefixRec.AlfMark, "ALPHA", 	sizeof( _PrefixRec.AlfMark ) );
	/***端末を特定できる情報を２０バイト以内でセット***/
	memcpy( _PrefixRec.TermNo, "部門マスタ受信",  14 );
	/***送信時は「Ｓ」固定　受信時は「Ｒ」固定***/
	_PrefixRec.Kubun[0] = 'R';
	/***送信時のレコード長***/
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.BlockLen ), _AlfInf.recvLen );
	memcpy( _PrefixRec.BlockLen, dspBuf, strlen( dspBuf ) );
	/***ファイル名***/
	memcpy( _PrefixRec.FileName, "FILE1", 5 );
	/***ファイルサイズ　件数×レコード長***/
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.FileSize ), 0 );
	memcpy( _PrefixRec.FileSize, dspBuf, strlen( dspBuf ) );
	/***件数***/
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.RecordCount ), 0 );
	memcpy( _PrefixRec.RecordCount, dspBuf, strlen( dspBuf ) );
	
	// #if DEBUG
	// 	displayMsg(_AlfInf.recvLen);
	// #endif	
	/* 受信電文を送信 */
	ret = alfOpen_Rev_Next_PrefixSendEx( &_AlfInf, &_PrefixRec );
	if( ret != COM_OK ){
		goto EndRecv;
	}

	ckputss( 0,  4, "     受信中     ", False, CLR_BASE );
	for( ;; ){
		memset( _RecvBuf, 0x00, sizeof( _RecvBuf ) );
		ret = alfRecv( _RecvBuf );
		if( ret == COM_EOF ) 		break;
		else if( ret != COM_OK  ) 	goto EndRecv;
		if( BUMSTCnt >= BUMST_MAX ) continue;
		memcpy( &bumst, _RecvBuf, sizeof( bumst ) );
		ram_write( BUMSTCnt, &bumst, BUMSTF );
		++BUMSTCnt;
		// #if DEBUG
		// 	msgbox( LCD_MSGBOX_WARNING, 0x00, "", &bumst, "確認", 0x00 );
		// #endif		
		ckprintf( 5, 9, False, CLR_BASE, "%2ld", BUMSTCnt );
	}
	
	/* --------------------------------------------------------------  連続受信の場合 */
	alf_Rev_Close();
	/* 品種マスタ受信準備 */
	/***端末を特定できる情報を２０バイト以内でセット***/
	memcpy( _PrefixRec.TermNo,"品種マスタ受信",  14 );
	/***ファイル名***/
	memcpy( _PrefixRec.FileName, "FILE2", 5 );
	_AlfInf.recvLen	 = sizeof(HIMST)+2;
	// #if DEBUG
	// 	displayMsg(_AlfInf.recvLen);
	// #endif
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.BlockLen ), _AlfInf.recvLen );
	memcpy( _PrefixRec.BlockLen, dspBuf, strlen( dspBuf ) );	
	/* 受信電文を送信 */
	ret = alfOpen_Rev_Next_PrefixSendEx( &_AlfInf, &_PrefixRec );
	if( ret != COM_OK ){
		goto EndRecv;
	}

	for( ;; ){
		memset( _RecvBuf, 0x00, sizeof( _RecvBuf ) );
		ret = alfRecv( _RecvBuf );
		if( ret == COM_EOF ) 		break;
		else if( ret != COM_OK  ) 	goto EndRecv;
		if( HIMSTCnt >= HIMST_MAX ) continue;
		memcpy( &himst, _RecvBuf, sizeof( himst ) );
		ram_write( HIMSTCnt, &himst, HIMSTF );
		++HIMSTCnt;
		// #if DEBUG
		// 	msgbox( LCD_MSGBOX_WARNING, 0x00, "", &himst, "確認", 0x00 );
		// #endif
		ckprintf( 11, 9, False, CLR_BASE, "%2ld", HIMSTCnt );
	}
		
	/* --------------------------------------------------------------  連続受信の場合 */
	alf_Rev_Close();
	/* 担当マスタ受信準備 */
	/***端末を特定できる情報を２０バイト以内でセット***/
	memcpy( _PrefixRec.TermNo,"担当マスタ受信",  14 );
	/***ファイル名***/
	memcpy( _PrefixRec.FileName, "FILE3", 5 );
	_AlfInf.recvLen	 = sizeof(TAMST)+2;
	// #if DEBUG
	// 	displayMsg(_AlfInf.recvLen);
	// #endif
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.BlockLen ), _AlfInf.recvLen );
	memcpy( _PrefixRec.BlockLen, dspBuf, strlen( dspBuf ) );	
	/* 受信電文を送信 */
	ret = alfOpen_Rev_Next_PrefixSendEx( &_AlfInf, &_PrefixRec );
	if( ret != COM_OK ){
		goto EndRecv;
	}

	for( ;; ){
		memset( _RecvBuf, 0x00, sizeof( _RecvBuf ) );
		ret = alfRecv( _RecvBuf );
		if( ret == COM_EOF ) 		break;
		else if( ret != COM_OK  ) 	goto EndRecv;
		if( TAMSTCnt >= TAMST_MAX ) continue;
		memcpy( &tamst, _RecvBuf, sizeof( tamst ) );
		ram_write( TAMSTCnt, &tamst, TAMSTF );
		++TAMSTCnt;
		ckprintf( 5, 13, False, CLR_BASE, "%2ld", TAMSTCnt );
	}
			
	/* --------------------------------------------------------------  連続受信の場合 */
	alf_Rev_Close();
	/* レシートマスタ受信準備 */
	/***端末を特定できる情報を２０バイト以内でセット***/
	memcpy( _PrefixRec.TermNo,"レシートマスタ受信",  14 );
	/***ファイル名***/
	memcpy( _PrefixRec.FileName, "FILE4", 5 );
	_AlfInf.recvLen	 = sizeof(REMST)+2;
	// #if DEBUG
	// 	displayMsg(_AlfInf.recvLen);
	// #endif
	sprintf( dspBuf, "%0*d", sizeof( _PrefixRec.BlockLen ), _AlfInf.recvLen );
	memcpy( _PrefixRec.BlockLen, dspBuf, strlen( dspBuf ) );	
	/* 受信電文を送信 */
	ret = alfOpen_Rev_Next_PrefixSendEx( &_AlfInf, &_PrefixRec );
	if( ret != COM_OK ){
		goto EndRecv;
	}

	for( ;; ){
		memset( _RecvBuf, 0x00, sizeof( _RecvBuf ) );
		ret = alfRecv( _RecvBuf );
		if( ret == COM_EOF ) 		break;
		else if( ret != COM_OK  ) 	goto EndRecv;
		if( REMSTCnt >= REMST_MAX ) continue;
		memcpy( &remst, _RecvBuf, sizeof( remst ) );
		ram_write( REMSTCnt, &remst, REMSTF );
		++REMSTCnt;
		// #if DEBUG
		// 	msgbox( LCD_MSGBOX_WARNING, 0x00, "", &remst, "確認", 0x00 );
		// #endif
		ckprintf( 12, 13, False, CLR_BASE, "%2ld", REMSTCnt );
	}
		
// TODO

EndRecv:
	if( ret == COM_OK ){
		ret = alfClose();
	} else {
		alfClose();
	}
	if( ret != COM_OK && ret != COM_EOF ){
		if( ret == COM_ABORT )
			ckputss( 0, 4, "    強制終了    ", False, CLR_BASE );
		else if( ret == COM_TIMEOUT )
			ckputss( 0, 4, "  タイムアウト  ", False, CLR_BASE );
		else if( ret == COM_ERROR )
			ckputss( 0, 4, "   通信エラー   ", False, CLR_BASE );
		else
			ckputss( 0, 4, "    異常終了    ", False, CLR_BASE );
		beep( 50, 1 );
		ckputss( 0, 15, "    ENT:確認    ", False, CLR_BASE );
	} else {
		if( BUMSTCnt )	ctrl.BUMSTCnt = BUMSTCnt;
		if( HIMSTCnt )	ctrl.HIMSTCnt = HIMSTCnt;
		if( TAMSTCnt )	ctrl.TAMSTCnt = TAMSTCnt;
		if( REMSTCnt )	ctrl.REMSTCnt = REMSTCnt;
		ram_write( 0, &ctrl, CTRLF );
		ckputss( 0, 4, "    受信完了    ", False, CLR_BASE );
		//beeb( 50, 2, 20 );
		beep( 100,1 );
		ckputss( 0, 15, "    ENT:確認    ", False, CLR_BASE );
	}
	while( !okng() );
	offkey(1);
	setautoff( 60, 600 );
}

