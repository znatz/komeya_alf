#ifndef	CTRL_H
#define	CTRL_H

#ifndef	True
#define True 1
#endif
#ifndef False
#define False 0
#endif

#include	<capture.h>

#define	VER	"Ver1.00"
#define	VER2	"POSCO  Ver1.00"
#define DEBUG   1
#define MAXLEN 10

enum JobsOfTypes{
	JOB_MIN = 0,
	JOB_URI,
	JOB_SIIRE,
	JOB_TANA,
	JOB_IDO,
	JOB_MAX
};

enum ColorsOfTypes {
	CLR_MIN = 1,
	CLR_BASE,
	CLR_TITLE,
	CLR_SI_TITLE,
	CLR_ZA_TITLE,
	CLR_NE_TITLE,
	CLR_ID_TITLE,
	CLR_TE_TITLE,
	CLR_DE_TITLE,
	CLR_KP_TITLE,
	CLR_UR_TITLE,
	CLR_PR_TITLE,
	CLR_SE_TITLE,
	CLR_LABEL,
	CLR_MSGBOX,
	CLR_MAX
};

/* ?��J?��?��?��[?��T?��?��?��v?��?�� */
#define CLR_BASE_BACK 	RGB( 200, 255, 200 )		// ?��?��{?��w?��i?��F	RGB(204, 255, 204)			RGB( 173, 216, 230 )
#define CLR_BASE_TEXT 	RGB(   0,   0,   0 )		// ?��?��{?��?��?��?��?��F				( ?��?�� )
#define CLR_MSGBOX_BACK RGB( 255, 180, 255 )		// ?��?��?��b?��Z?��[?��W?��{?��b?��N?��X?��w?��i?��F
#define CLR_MSGBOX_TEXT RGB(   0,   0,   0 )		// ?��?��?��b?��Z?��[?��W?��{?��b?��N?��X?��?��?��?��?��F
#define CLR_SCREEN    	RGB( 204, 255, 204 )		// ?��?��?��b?��Z?��[?��W?��_?��C?��A?��?��?��O?��w?��i?��F
#define CLR_TITLE_TEXT 	RGB(   0,   0,   0 )		// ?��^?��C?��g?��?��?��?��?��?��?��F			( ?��?�� )
#define CLR_TITLE_BACK 	RGB( 255, 180,   0 )		// ?��^?��C?��g?��?��?��w?��i?��F			255, 213,   0
#define CLR_LABEL_TEXT 	RGB( 255, 255, 255 )		// ?��?��?��x?��?��?��?��?��?��?��F				( ?��?�� )
#define CLR_LABEL_BACK 	RGB(   0,  84, 227 )		// ?��?��?��x?��?��?��w?��i?��F				( ?��?�� )
#define CLR_REF_TEXT 	RGB(   0,   0,   0 )		// ?��Q?��ƍ�?��ڕ�?��?��?��F			( ?��?�� )
#define CLR_REF_BACK 	RGB( 255, 255, 128 )		// ?��Q?��ƍ�?��ڔw?��i?��F			( ?��?�� )
#define CLR_NO_TEXT 	RGB( 255, 255, 255 )		// ?��i?��?��?��o?��[?��?��?��?��?��F			( ?��?�� )
#define CLR_NO_BACK 	RGB(   0,   0,   0 )		// ?��i?��?��?��o?��[?��w?��i?��F			( ?��?�� )
#define CLR_FUNC_TEXT 	RGB(   0,   0,   0 )		// ?��t?��@?��?��?��N?��V?��?��?��?��?��?��?��?��?��F		( ?��?�� )
#define CLR_FUNC_BACK 	RGB( 255, 255, 255 )		// ?��t?��@?��?��?��N?��V?��?��?��?��?��w?��i?��F		( ?��?�� )

#define CLR_FUNC_GREEN 	RGB(   0, 255,   0 )		// ?��?��
#define CLR_FUNC_GREEN1 RGB(   0, 150,   0 )		// ?��Z?��?��?��?��
#define CLR_FUNC_GREEN2 RGB( 128, 255, 128 )		// ?��?��?��?��?��?��
#define CLR_FUNC_GREEN3 RGB( 200, 255, 200 )		// ?��?��?��?��?��Ɣ�?��?��?��?��
#define CLR_FUNC_BLUE 	RGB(   0,   0, 255 )		// ?��?��
#define CLR_FUNC_BLUE1 	RGB(   0,  84, 227 )		// ?��Z?��?��?��?��
#define	CLR_SILVER		RGB( 192, 192, 192 )		// ?��?��F
#define	CLR_GLAY		RGB(  48,  48,  48 )		// ?��D?��F
#define CLR_YELLOW		RGB( 255, 255,   0 )		// ?��?��?��F
#define CLR_LIGHTYELLOW		RGB( 255, 255, 128 )		// ?��?��?��?��?��F
#define CLR_PINK    		RGB( 247, 171, 166 )	// ?��?��ݸ
#define CLR_ORANGE		RGB( 255, 165,   0 )		// ?��I?��?��?��?��?��W?��F
#define CLR_RED			RGB( 255,   0,   0 )		// ?��?��
#define CLR_BROWN		RGB( 165,  42,  42 )		// ?��?��?��F
#define CLR_LIME		RGB(   0, 255,   0 )		// ?��?��?��C?��?��
#define CLR_GREEN		RGB( 190, 209,  45 )		// ?��ΐF
#define CLR_OLIVE		RGB( 128, 128,   0 )		// ?��I?��?��?��[?��u
#define CLR_CYAN		RGB(   0, 255, 255 )		// ?��V?��A?��?��
#define CLR_BLUE		RGB(   0,   0, 255 )		// ?��F
#define CLR_DARKBLUE	RGB(   0,   0, 160 )		// ?��Z?��?��?��?��
#define CLR_LIGHTBLUE	RGB( 173, 216, 230 )		// ?��?��?��?��?��?��
#define CLR_PURPLE		RGB( 167,  87, 168 )		// ?��?��
#define CLR_FUCHSIA		RGB( 255,   0, 255 )

/*************************************************/
/*  ?��R?��?��?��g?��?��?��[?��?��?��t?��@?��C?��?��                         */
/*************************************************/
typedef struct CONTROL_BUFF{
	char	LoadFlag[30];		/*   0	?��?��?��[?��f?��B?��?��?��O?��?��?��	*/
	short	SDataCnt;		/*  38	?��d?��?��?��f?��[?��^?��?��?��?��		*/
	short	TDataCnt;		/*  42	?��I?��?��?��f?��[?��^?��?��?��?��		*/
	short	IDataCnt;		/*  44	?��ړ�?��f?��[?��^?��?��?��?��		*/
	short	NDataCnt;		/*  46	?��l?��?��?��f?��[?��^?��?��?��?��		*/
	char	Cursor;			/*  48	?��J?��[?��\?��?��?��^?��C?��v 		*/
	char	Beep;			/*  49	?��?��?��ݒ� 			*/
	char	Bright;			/*  50	?��?��ʋP?��x?��ݒ�		*/
	short	BUMSTCnt;		/*  54	?��?��?��?��}?��X?��^?��?��?��?��   	*/
	short	HIMSTCnt;		/*  58	?��i?��?��}?��X?��^?��?��?��?��		*/
	short	URDataCnt;		/*  62	?��?��?��?��f?��[?��^?��?��?��?��		*/
	short	InfoUrCnt;		/*  66	?��?��?��?��f?��[?��^?��?��?��?��		*/
	long	RecNo;			/*  74	?��?��?��V?��[?��gNO		*/
	char	RejiNo[2];		/*  76	?��?��?��WNO			*/
	char	RecPrint[1];		/*  77	?��?��?��V?��[?��g?��?��		*/
	char	TaxRate[2];		/*  79	?��V?��X?��e?��?��?��ŗ�		*/	
	char    TaxType[1];		/*  80	?��V?��X?��e?��?��?��ŋ�		*/	
	char	filler[49];		/*  49	?��\?��?��			*/
} CONTROLDATA;				/* 128?��o?��C?��g			*/


/*****************************************************************************/
/* ?��?��?��?��}?��X?��^	2+1+20+1+10+1+2 = 37															 */
/*****************************************************************************/
typedef struct BUMST_TAG {
	char	Code[2];			//   ?��R?��[?��h	 2
	char	FillerA;			//   ?��J?��?��?��} 	 1		
	char	Name[20];			//   ?��?��?��喼	20		
	char	FillerB;			//   ?��J?��?��?��} 	 1		
	char	Ryaku[10];			//   ?��?��?��?��	10
	char	FillerC;			//   ?��J?��?��?��} 	 1		
	char	Zeiritu[2];			//   ?��ŗ�	 2		
} BUMST;

/*****************************************************************************/
/* ?��i?��?��}?��X?��^	4+1+20+1+10+1+2+1+2+1+2=45															 */
/*****************************************************************************/
typedef struct HIMST_TAG {
	char	Code[4];			//   ?��R?��[?��h	 4
	char	FillerA;			//   ?��J?��?��?��} 	 1		
	char	Name[20];			//   ?��i?�����	20		
	char	FillerB;			//   ?��J?��?��?��} 	 1		
	char	Ryaku[10];			//   ?��?��?��?��	10
	char	FillerC;			//   ?��J?��?��?��} 	 1		
	char	Wari[2];			//   ?��?��?��?��?��?��	 2	
	char	FillerD;			//   ?��J?��?��?��} 	 1		
	char	Zeiku[2];			//   ?��ŋ�	 2		
	char	Filler5;			//   ?��J?��?��?��} 	 1		
	char	Tenku[2];			//   ?��_?��?��	 2		
} HIMST;

/*****************************************************************************/
/* ?��I?��?��?��f?��[?��^																 */
/*****************************************************************************/
typedef struct TDATA_TAG {
	char	Date[8];			/*   0	?��?��?��t		*/
	char	Shop[2];			/*  14	?��X?��܃R?��[?��h	*/
	char	Code1[13];			/*  18	?��?��i?��R?��[?��h	*/
	char	Code2[13];			/*  31	?��?��?��i?��R?��[?��h	*/
	char	Code3[8];			/*  44	?��l?��?��?��R?��[?��h	*/
	char	Num[5];				/*  52	?��?��?��?��		*/
} TDATA;

/*****************************************************************************/
/* ?��ړ�?��f?��[?��^																 */
/*****************************************************************************/
typedef struct IDATA_TAG {
	char	Date[8];			/*   0	?��?��?��t		*/
	char	Shop[2];			/*  14	?��o?��דX?��R?��[?��h	*/
	char	Shop2[2];			/*  16	?��?��?��?��X?��R?��[?��h	*/
	char	Code1[13];			/*  18	?��?��i?��R?��[?��h	*/
	char	Code2[13];			/*  31	?��?��?��i?��R?��[?��h	*/
	char	Code3[8];			/*  44	?��l?��?��?��R?��[?��h	*/
	char	Num[5];				/*  52	?��?��?��?��		*/
} IDATA;
	/* 	57?��o?��C?��g		*/

/*****************************************************************************/
/* ?��d?��?��?��f?��[?��^																 */
/*****************************************************************************/
typedef struct SDATA_TAG {
	char	Date[8];			/*   0	?��?��?��t		*/
	char	DenNo[8];			/*   0	?��`?��[No		*/
	char	Shop[2];			/*  14	?��o?��דX?��R?��[?��h	*/
	char	Code1[13];			/*  18	?��?��i?��R?��[?��h	*/
	char	Code2[13];			/*  31	?��?��?��i?��R?��[?��h	*/
	char	Code3[8];			/*  44	?��l?��?��?��R?��[?��h	*/
	char	Num[5];				/*  52	?��?��?��?��		*/
} SDATA;
	/* 	57?��o?��C?��g		*/
/*****************************************************************************/
/* ?��l?��?��?��f?��[?��^																 */
/*****************************************************************************/
typedef struct NDATA_TAG {
	char	Date[8];			/*   0	?��?��?��t		*/
	char	Shop[1];			/*  14	?��o?��דX?��R?��[?��h	*/
	char	Code1[13];			/*  18	?��?��i?��R?��[?��h	*/
	char	Code2[13];			/*  31	?��?��?��i?��R?��[?��h	*/
	char	Code3[8];			/*  44	?��l?��?��?��R?��[?��h	*/
	char	Code4[13];			/*  44	?��l?��?��?��R?��[?��h	*/
	char	Num[5];				/*  52	?��?��?��?��		*/
} NDATA;/* 	57?��o?��C?��g		*/

/****************************************************************************/
/* ?��e?��?��?��?��
/****************************************************************************/
typedef struct INFO_DATA{
	char	Date[8];			//   0	?��?��?��t		
	char	Time[6];
	char	tenpo1[2];			//  ?��o?��דX?��R?��[?��h	
	char	tenpo2[2];			//  ?��?��?��?��X?��R?��[?��h
	char	Bumon[2];
	char	Hinsyu[4];
	char	Code1[13];			//  JAN?��R?��[?��h
	char 	Joudai[10];
	char	Gedai[10];
	char	tanto[2];
	char	Code2[13];
	char	Code3[8];			//  ?��l?��?��
	char	Num[4];
	long    BumonTaxRate;
	long    lngSystemTaxRateTax;
	long    lngBumonTaxRateTax;
	long	lngHinsyuZeiku;
	long	lngSystemZeiku;
	int	flag;
} INFO;
	
/*****************************************************************************/
/* ?��?��?��?��f?��[?��^																 */
/*****************************************************************************/
typedef struct URDATA_TAG {
	char	Date[8];			//   0	?��?��?��t		
	char	Time[6];
	char	Shop[2];			//  14	?��o?��דX?��R?��[?��h	
	char	RejiNo[2];
	char	Tanto[2];
	char	Code1[13];			//  18	?��?��i?��R?��[?��h	
	char	Code2[13];			//  31	?��?��?��i?��R?��[?��h	
	char	Code3[8];			//  44	?��l?��?��?��R?��[?��h	
	char	Name[20];
	char	Baika[10];
	char	Num[5];				//  52	?��?��?��?��		
	char 	Genkin[6];
	char	Credit[6];
	char 	Kinken[6];
	char	Coupon[6];
	char 	RecNo[6];
	char 	KyakuNo[1];
	char  	SyouhiFlg[1];
	char 	Reason[2];
	long	lngOturi;
	long    BumonTaxRate;
	long    lngSystemTaxRateTax;
	long    lngBumonTaxRateTax;
} URDATA;

/*****************************************************************************/
/* ?��ꎞ�?��?��?��f?��[?��^																 */
/*****************************************************************************/
typedef struct INFOUR_TAG {	
	char	Date[8];			//   0	?��?��?��t		
	char	Time[6];
	char	Code1[13];			//  18	?��?��i?��R?��[?��h	
	char	Code2[13];			//  18	?��?��i?��R?��[?��h	
	char	Code3[8];			//  44	?��l?��?��?��R?��[?��h	
	char	Name[20];
	char	Baika[10];
	char	Num[4];				//  52	?��?��?��?��	

} INFOUR;

/*****************************************************************************/
/* ?��ꎞ�?��?��?��f?��[?��^																 */
/*****************************************************************************/
typedef struct INFOUR_TAG2 {	
	char	Date[8];			//   0	?��?��?��t		
	char	Time[6];
	char 	Genkin[6];
	char	Credit[6];
	char 	Kinken[6];
	char	Coupon[6];
	long 	Goukei;
	long	Oturi;
	long	Tensuu;
} INFOUR2;

typedef struct TENPO_DATA{
	char	tenpo1[2];			/*  14	?��o?��דX?��R?��[?��h	*/
	char	tenpo2[2];
	int	flag;
} TENPO;/* 	5?��o?��C?��g		*/

extern	CONTROLDATA	ctrl;
extern	TDATA		tdata;		/* ?��I?��?��?��f?��[?��^		*/
extern	IDATA		idata;		/* ?��ړ�?��f?��[?��^		*/
extern	NDATA		ndata;		/* ?��l?��?��?��f?��[?��^		*/
extern	SDATA		sdata;		/* ?��d?��?��?��f?��[?��^		*/
extern	BUMST		bumst;		/* ?��S?��?��?��f?��[?��^		*/
extern	HIMST		himst;		/* ?��?��?��?��f?��[?��^		*/
extern  TENPO		tenpo;
extern	char		scanBuf[30],dt[32], dspBuf[50];

#endif

extern	void	menu( void );
extern	void	Tscan( void );
extern	void	Sscan( int flag );
extern	void	Iscan( void );
extern	void	Nscan( void );
extern	void	Kscan( void );
extern  void 	AlfRecvMst( void );
extern  void 	AlfDataSend( short job );
extern  void 	AlfRecvMst( void );
extern  void 	displayMsg( long );
extern  void 	displayStringMsg( char* );
extern	long 	getJyoudai(char *barcode);
extern	long	getGedai(const char *barcode);
extern	long	GetNesage(const char *barcode, long jyoudai);
extern	int	ctoi(char c);
extern	long	powOfTen(int exp);
extern	void	insComma( long, char[] );
extern	void	printNumberWithComma( long num, int col, int row ) ;
// ?��?��?��?��
extern	URDATA		urdata;	
extern  INFO		info;
extern  INFOUR		infour;
extern  INFOUR2		infour2;

extern	short	BumonFind( char *key );
extern	long	BumonTaxFind( char *key );
extern	long	BumonTaxFindByCode1( char *code1 );
extern	long	HinsyuZeikuFindByCode1( char *code1 );
extern	void	drawLineCrossScreen(long y);
extern	void	convertToString( long num, char str[MAXLEN] );