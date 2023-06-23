#ifndef RAM_IO_H
#define RAM_IO_H

#define Error		-1
#define Normal		0
/*******************************************************/
/* ファイル数とファイル				       */
/*******************************************************/
#define	SDATA_MAX		5000	/* 仕入ＭＡＸ件数	  		*/
#define	TDATA_MAX		5000	/* 棚卸ＭＡＸ件数	  		*/
#define	IDATA_MAX		5000	/* 移動ＭＡＸ件数	  		*/
#define	NDATA_MAX		5000	/* 移動ＭＡＸ件数	  		*/
#define	BUMST_MAX		100	/* 部門ＭＡＸ件数	  		*/
#define	HIMST_MAX		10000	/* 品種ＭＡＸ件数	  		*/
#define	TAMST_MAX		100	/* 担当ＭＡＸ件数	  		*/
#define	REMST_MAX		10	/* ﾚｼｰﾄＭＡＸ件数	  		*/
#define	TSMST_MAX		1000	/* 商品ＭＡＸ件数	  		*/
#define	URDATA_MAX		2900	/* 売上ＭＡＸ件数	  		*/
#define	INFOUR_MAX		50	/* 売上一時ＭＡＸ件数	  		2021/08/02 20->50 */ 

enum{
	CTRLF = 0,
	TDATAF,		/* 棚卸データ			*/
	SDATAF,		/* 仕入データ			*/
	IDATAF,		/* 移動データ			*/
	NDATAF,		/* 値下データ			*/
	BUMSTF,		/* 部門マスタ			*/
	HIMSTF,		/* 品種マスタ			*/	
	TAMSTF,		/* 担当マスタ			*/	
	REMSTF,		/* ﾚｼｰﾄマスタ			*/	
	TSMSTF,		/* 商品マスタ			*/	
	URDATAF,	/* 売上データ			*/
	INFOURF,	/* 売上データ			*/	
	INFOURF2,	/* 売上データ			*/	
	FILEMAX
};

/*----- Function prototypes -----*/
extern	short		ram_read( long , void *, short );
extern	short		ram_write( long , void *, short );
extern	short		ram_delete( long cnt, short fno, short max, short loop );
extern	short		binarySearch( void *keyStr, short keyLen, short fileNo, long maxCount, short skipByte );
extern	short		beginSearch( void *keyStr, short keycnt, short skip, short keylen, short fstartcnt, long fmaxcnt, short fno );
extern	long		ram_size( void );

#endif

