/******************************************************************************
 * ram_io.c
 * ラムディスク
 *****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "sub.h"
#include "ram_io.h"

#define	MaxMoveBytes		512
static long  	ramadd;
static char	rwbuff[MaxMoveBytes+1];

static struct {
	short			len;		/* 長さ 		*/
	long			max;		/* ＭＡＸ件数 	*/
	short			add;		/* 位置計算 	*/
} io_tbl[FILEMAX] = {
	{ sizeof( CONTROLDATA     )	, 1,		True },		/* コントロールファイル	 */
	{ sizeof( TDATA )		, TDATA_MAX,	True },		/* 棚卸データ		 */
	{ sizeof( SDATA )		, SDATA_MAX,	True },		/* 仕入データ		 */
	{ sizeof( IDATA )		, IDATA_MAX,	True },		/* 移動データ		 */
	{ sizeof( NDATA )		, NDATA_MAX,	True },		/* 値引データ		 */
	{ sizeof( BUMST )		, BUMST_MAX,	True },		/* 部門マスタ		 */
	{ sizeof( HIMST )		, HIMST_MAX,	True },		/* 品種マスタ		 */
	{ sizeof( TAMST )		, TAMST_MAX,	True },		/* 担当マスタ		 */
	{ sizeof( REMST )		, REMST_MAX,	True },		/* ﾚｼｰﾄマスタ		 */
	{ sizeof( TSMST )		, TSMST_MAX,	True },		/* ﾚｼｰﾄマスタ		 */
	{ sizeof( URDATA )		, URDATA_MAX,	True },		/* 売上データ		 */
	{ sizeof( INFOUR )		, INFOUR_MAX,	True },		/* 売上一時データ	 */
	{ sizeof( INFOUR2 )		, 1,		True }		/* 売上一時データ	 */
};

static long	ram_adressget( short fno );

static long	ram_adressget( short fno )
{
	long	adress = 0l;
	short	loop;
	
	if ( fno ) {
		for ( loop = 0; loop < fno; loop++ ){
			if( io_tbl[loop].add )
				adress += ( long )io_tbl[loop].len * ( long )io_tbl[loop].max;
		}
	}
	return adress;
}

/**************************************************/
/*                                                */
/**************************************************/
long	ram_size( void )
{
	short	i;
	long	max_size;

	max_size = 0;
	for( i=0; i<FILEMAX; i++ ){
		if( io_tbl[i].add )
			max_size += ( ( long )io_tbl[i].len * ( long )io_tbl[i].max );
	}
	return( max_size );
}

/******************************************************************************
 * Function prototype : short ram_read( short cnt, void _FAR *data, short fno );
 *                    : short ram_write( short cnt, void _FAR *data, short fno );
 * Procedure          : ＲＡＭディスクアクセス
 * Argument           : cnt  => アクセス位置
 *                    : data => アクセスデータ
 *                    : fno  => ファイル番号
 * return             : Normal => 正常
 *                    : Error  => アクセス失敗
 *****************************************************************************/
short	ram_read( long cnt, void *data, short fno )
{
	short	ret;

	ramadd  = ram_adressget( fno );
	ramadd += ( long )cnt * ( long )io_tbl[fno].len;
	ret = mread( ramadd, ( char * )data, io_tbl[fno].len );
	if ( ret != io_tbl[fno].len ){
		return Error;
	}
	return Normal;
}

short	ram_write( long cnt, void *data, short fno )
{
	short		ret;

	if( io_tbl[fno].max <= cnt ) return( Error );
	ramadd  = ram_adressget( fno );
	ramadd += ( long )cnt * ( long )io_tbl[fno].len;
	ret = mwrite( ramadd, ( char * )data, io_tbl[fno].len );
	if ( ret != io_tbl[fno].len ){
		return Error;
	}
	return Normal;
}

short	ram_delete( long cnt, short fno, short max, short loop )
{
	long	ramadd2;

	if ( cnt >= max - 1 )
		return Normal;		/* 最後のデータの削除ならなら何もしない */

	ramadd2  = ramadd = ram_adressget( fno );
	ramadd  += ( long )( cnt + loop ) * ( long )io_tbl[fno].len;
	ramadd2 += ( long )cnt * ( long )io_tbl[fno].len;
	for ( max -= ( loop + cnt ), cnt = 0; cnt <= max; cnt++ ) {
		mread( ramadd, rwbuff, io_tbl[fno].len );
		mwrite( ramadd2, rwbuff, io_tbl[fno].len );
		ramadd += ( long )io_tbl[fno].len;
		ramadd2+= ( long )io_tbl[fno].len;
	}
	return Normal;
}

short	binarySearch( void *keyStr, short keyLen, short fileNo, long maxCount, short skipByte )
{
	long	startAdrs, max, sAdrs;
	short	ret, min;

	if ( maxCount < 1 )		return	-1;		/* マスタがなければ戻す */

	startAdrs = ram_adressget( fileNo );
	if ( skipByte )
		startAdrs += ( long )skipByte;

	min = 0;
	max = maxCount;
	for ( ; ; ) {
		sAdrs = min + ( ( max - min ) / 2 );
		ramadd = ( long )sAdrs * ( long )io_tbl[fileNo].len;
		mread( startAdrs + ramadd, rwbuff, keyLen );
		ret = memcmp( keyStr, rwbuff, keyLen );
		if ( ret > 0 ) {		/* キーが大きい */
			if ( min >= max - 1 )
				break;
			min = sAdrs;
		} else if ( ret < 0 ) {	/* キーが小さい */
			if ( min >= max - 1 )
				break;
			max = sAdrs;
		} else{					/* キーが一致 */
			return	sAdrs;
		}
	}
	return	-1;
}

/******************************************************************************
 * Function prototype : short  beginSearch( char *keycode, short keylen, short fno,
 *													short fmaxcnt, short skip )
 * Procedure          : じみちに最初から検索する
 * Argument           : keycode   => 検索キー
 *                    : keycnt    => キー数
 *                                   ２つ以上キーがある場合、キーバッファサイズは
 *                                   「キー長」×「キー数」となる
 *                    : skip      => キーの位置 (何バイト目から比較かを指定)
 *                    : keylen    => キー長
 *                    : fstartcnt => 開始データ位置
 *                    : fmaxcnt   => ファイルＭＡＸ件数
 *                    : fno       => ファイル番号
 * return             : Normal    => 正常
 *                    : Error     => アクセス失敗
 *****************************************************************************/
short	beginSearch( void *keyStr, short keycnt, short skip, short keylen, short fstartcnt, long fmaxcnt, short fno )
{
	char 	readbuf[50], *keycode;
	short  	loop, i;

	if( fmaxcnt <= 0 || fstartcnt > fmaxcnt ) return( -1 );
	keycode = keyStr;
	ramadd = ram_adressget( fno );
	ramadd += ( ( long )io_tbl[fno].len * (long)fstartcnt );
	if( skip )
		ramadd += ( long )skip;

	for( loop=fstartcnt; loop<fmaxcnt; loop++ ){
		mread( ramadd, readbuf, 1 );
		for( i=0; i<keycnt; i++ ){
			if( readbuf[0] == keycode[i*keylen] ){
				if( keylen == 1 )
					return loop;
				mread( ramadd + 1l, &readbuf[1], keylen - 1 );
				if( !memcmp( &readbuf[1], &keycode[i*keylen+1], keylen - 1 ) )
					return loop;
			}
		}
		ramadd += ( long )io_tbl[fno].len;
	}
	return( -1 );
}