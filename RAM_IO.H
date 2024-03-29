#ifndef RAM_IO_H
#define RAM_IO_H

#define Error		-1
#define Normal		0
/*******************************************************/
/* �t�@�C�����ƃt�@�C��				       */
/*******************************************************/
#define	SDATA_MAX		5000	/* �d���l�`�w����	  		*/
#define	TDATA_MAX		5000	/* �I���l�`�w����	  		*/
#define	IDATA_MAX		5000	/* �ړ��l�`�w����	  		*/
#define	NDATA_MAX		5000	/* �ړ��l�`�w����	  		*/
#define	BUMST_MAX		100	/* ����l�`�w����	  		*/
#define	HIMST_MAX		10000	/* �i��l�`�w����	  		*/
#define	TAMST_MAX		100	/* �S���l�`�w����	  		*/
#define	REMST_MAX		10	/* ڼ�Ăl�`�w����	  		*/
#define	TSMST_MAX		1000	/* ���i�l�`�w����	  		*/
#define	URDATA_MAX		2900	/* ����l�`�w����	  		*/
#define	INFOUR_MAX		50	/* ����ꎞ�l�`�w����	  		2021/08/02 20->50 */ 

enum{
	CTRLF = 0,
	TDATAF,		/* �I���f�[�^			*/
	SDATAF,		/* �d���f�[�^			*/
	IDATAF,		/* �ړ��f�[�^			*/
	NDATAF,		/* �l���f�[�^			*/
	BUMSTF,		/* ����}�X�^			*/
	HIMSTF,		/* �i��}�X�^			*/	
	TAMSTF,		/* �S���}�X�^			*/	
	REMSTF,		/* ڼ�ă}�X�^			*/	
	TSMSTF,		/* ���i�}�X�^			*/	
	URDATAF,	/* ����f�[�^			*/
	INFOURF,	/* ����f�[�^			*/	
	INFOURF2,	/* ����f�[�^			*/	
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

