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

/* ?øΩJ?øΩ?øΩ?øΩ[?øΩT?øΩ?øΩ?øΩv?øΩ?øΩ */
#define CLR_BASE_BACK 	RGB( 200, 255, 200 )		// ?øΩ?øΩ{?øΩw?øΩi?øΩF	RGB(204, 255, 204)			RGB( 173, 216, 230 )
#define CLR_BASE_TEXT 	RGB(   0,   0,   0 )		// ?øΩ?øΩ{?øΩ?øΩ?øΩ?øΩ?øΩF				( ?øΩ?øΩ )
#define CLR_MSGBOX_BACK RGB( 255, 180, 255 )		// ?øΩ?øΩ?øΩb?øΩZ?øΩ[?øΩW?øΩ{?øΩb?øΩN?øΩX?øΩw?øΩi?øΩF
#define CLR_MSGBOX_TEXT RGB(   0,   0,   0 )		// ?øΩ?øΩ?øΩb?øΩZ?øΩ[?øΩW?øΩ{?øΩb?øΩN?øΩX?øΩ?øΩ?øΩ?øΩ?øΩF
#define CLR_SCREEN    	RGB( 204, 255, 204 )		// ?øΩ?øΩ?øΩb?øΩZ?øΩ[?øΩW?øΩ_?øΩC?øΩA?øΩ?øΩ?øΩO?øΩw?øΩi?øΩF
#define CLR_TITLE_TEXT 	RGB(   0,   0,   0 )		// ?øΩ^?øΩC?øΩg?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩF			( ?øΩ?øΩ )
#define CLR_TITLE_BACK 	RGB( 255, 180,   0 )		// ?øΩ^?øΩC?øΩg?øΩ?øΩ?øΩw?øΩi?øΩF			255, 213,   0
#define CLR_LABEL_TEXT 	RGB( 255, 255, 255 )		// ?øΩ?øΩ?øΩx?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩF				( ?øΩ?øΩ )
#define CLR_LABEL_BACK 	RGB(   0,  84, 227 )		// ?øΩ?øΩ?øΩx?øΩ?øΩ?øΩw?øΩi?øΩF				( ?øΩ?øΩ )
#define CLR_REF_TEXT 	RGB(   0,   0,   0 )		// ?øΩQ?øΩ∆çÔøΩ?øΩ⁄ïÔøΩ?øΩ?øΩ?øΩF			( ?øΩ?øΩ )
#define CLR_REF_BACK 	RGB( 255, 255, 128 )		// ?øΩQ?øΩ∆çÔøΩ?øΩ⁄îw?øΩi?øΩF			( ?øΩ?øΩ )
#define CLR_NO_TEXT 	RGB( 255, 255, 255 )		// ?øΩi?øΩ?øΩ?øΩo?øΩ[?øΩ?øΩ?øΩ?øΩ?øΩF			( ?øΩ?øΩ )
#define CLR_NO_BACK 	RGB(   0,   0,   0 )		// ?øΩi?øΩ?øΩ?øΩo?øΩ[?øΩw?øΩi?øΩF			( ?øΩ?øΩ )
#define CLR_FUNC_TEXT 	RGB(   0,   0,   0 )		// ?øΩt?øΩ@?øΩ?øΩ?øΩN?øΩV?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩF		( ?øΩ?øΩ )
#define CLR_FUNC_BACK 	RGB( 255, 255, 255 )		// ?øΩt?øΩ@?øΩ?øΩ?øΩN?øΩV?øΩ?øΩ?øΩ?øΩ?øΩw?øΩi?øΩF		( ?øΩ?øΩ )

#define CLR_FUNC_GREEN 	RGB(   0, 255,   0 )		// ?øΩ?øΩ
#define CLR_FUNC_GREEN1 RGB(   0, 150,   0 )		// ?øΩZ?øΩ?øΩ?øΩ?øΩ
#define CLR_FUNC_GREEN2 RGB( 128, 255, 128 )		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
#define CLR_FUNC_GREEN3 RGB( 200, 255, 200 )		// ?øΩ?øΩ?øΩ?øΩ?øΩ∆îÔøΩ?øΩ?øΩ?øΩ?øΩ
#define CLR_FUNC_BLUE 	RGB(   0,   0, 255 )		// ?øΩ?øΩ
#define CLR_FUNC_BLUE1 	RGB(   0,  84, 227 )		// ?øΩZ?øΩ?øΩ?øΩ?øΩ
#define	CLR_SILVER		RGB( 192, 192, 192 )		// ?øΩ?øΩF
#define	CLR_GLAY		RGB(  48,  48,  48 )		// ?øΩD?øΩF
#define CLR_YELLOW		RGB( 255, 255,   0 )		// ?øΩ?øΩ?øΩF
#define CLR_LIGHTYELLOW		RGB( 255, 255, 128 )		// ?øΩ?øΩ?øΩ?øΩ?øΩF
#define CLR_PINK    		RGB( 247, 171, 166 )	// ?øΩ?øΩ›∏
#define CLR_ORANGE		RGB( 255, 165,   0 )		// ?øΩI?øΩ?øΩ?øΩ?øΩ?øΩW?øΩF
#define CLR_RED			RGB( 255,   0,   0 )		// ?øΩ?øΩ
#define CLR_BROWN		RGB( 165,  42,  42 )		// ?øΩ?øΩ?øΩF
#define CLR_LIME		RGB(   0, 255,   0 )		// ?øΩ?øΩ?øΩC?øΩ?øΩ
#define CLR_GREEN		RGB( 190, 209,  45 )		// ?øΩŒêF
#define CLR_OLIVE		RGB( 128, 128,   0 )		// ?øΩI?øΩ?øΩ?øΩ[?øΩu
#define CLR_CYAN		RGB(   0, 255, 255 )		// ?øΩV?øΩA?øΩ?øΩ
#define CLR_BLUE		RGB(   0,   0, 255 )		// ?øΩ¬êF
#define CLR_DARKBLUE	RGB(   0,   0, 160 )		// ?øΩZ?øΩ?øΩ?øΩ?øΩ
#define CLR_LIGHTBLUE	RGB( 173, 216, 230 )		// ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ
#define CLR_PURPLE		RGB( 167,  87, 168 )		// ?øΩ?øΩ
#define CLR_FUCHSIA		RGB( 255,   0, 255 )

/*************************************************/
/*  ?øΩR?øΩ?øΩ?øΩg?øΩ?øΩ?øΩ[?øΩ?øΩ?øΩt?øΩ@?øΩC?øΩ?øΩ                         */
/*************************************************/
typedef struct CONTROL_BUFF{
	char	LoadFlag[30];		/*   0	?øΩ?øΩ?øΩ[?øΩf?øΩB?øΩ?øΩ?øΩO?øΩ?øΩ?øΩ	*/
	short	SDataCnt;		/*  38	?øΩd?øΩ?øΩ?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ		*/
	short	TDataCnt;		/*  42	?øΩI?øΩ?øΩ?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ		*/
	short	IDataCnt;		/*  44	?øΩ⁄ìÔøΩ?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ		*/
	short	NDataCnt;		/*  46	?øΩl?øΩ?øΩ?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ		*/
	char	Cursor;			/*  48	?øΩJ?øΩ[?øΩ\?øΩ?øΩ?øΩ^?øΩC?øΩv 		*/
	char	Beep;			/*  49	?øΩ?øΩ?øΩ›íÔøΩ 			*/
	char	Bright;			/*  50	?øΩ?øΩ ãP?øΩx?øΩ›íÔøΩ		*/
	short	BUMSTCnt;		/*  54	?øΩ?øΩ?øΩ?øΩ}?øΩX?øΩ^?øΩ?øΩ?øΩ?øΩ   	*/
	short	HIMSTCnt;		/*  58	?øΩi?øΩ?øΩ}?øΩX?øΩ^?øΩ?øΩ?øΩ?øΩ		*/
	short	URDataCnt;		/*  62	?øΩ?øΩ?øΩ?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ		*/
	short	InfoUrCnt;		/*  66	?øΩ?øΩ?øΩ?øΩf?øΩ[?øΩ^?øΩ?øΩ?øΩ?øΩ		*/
	long	RecNo;			/*  74	?øΩ?øΩ?øΩV?øΩ[?øΩgNO		*/
	char	RejiNo[2];		/*  76	?øΩ?øΩ?øΩWNO			*/
	char	RecPrint[1];		/*  77	?øΩ?øΩ?øΩV?øΩ[?øΩg?øΩ?øΩ		*/
	char	TaxRate[2];		/*  79	?øΩV?øΩX?øΩe?øΩ?øΩ?øΩ≈óÔøΩ		*/	
	char    TaxType[1];		/*  80	?øΩV?øΩX?øΩe?øΩ?øΩ?øΩ≈ãÔøΩ		*/	
	char	filler[49];		/*  49	?øΩ\?øΩ?øΩ			*/
} CONTROLDATA;				/* 128?øΩo?øΩC?øΩg			*/


/*****************************************************************************/
/* ?øΩ?øΩ?øΩ?øΩ}?øΩX?øΩ^	2+1+20+1+10+1+2 = 37															 */
/*****************************************************************************/
typedef struct BUMST_TAG {
	char	Code[2];			//   ?øΩR?øΩ[?øΩh	 2
	char	FillerA;			//   ?øΩJ?øΩ?øΩ?øΩ} 	 1		
	char	Name[20];			//   ?øΩ?øΩ?øΩÂñº	20		
	char	FillerB;			//   ?øΩJ?øΩ?øΩ?øΩ} 	 1		
	char	Ryaku[10];			//   ?øΩ?øΩ?øΩ?øΩ	10
	char	FillerC;			//   ?øΩJ?øΩ?øΩ?øΩ} 	 1		
	char	Zeiritu[2];			//   ?øΩ≈óÔøΩ	 2		
} BUMST;

/*****************************************************************************/
/* ?øΩi?øΩ?øΩ}?øΩX?øΩ^	4+1+20+1+10+1+2+1+2+1+2=45															 */
/*****************************************************************************/
typedef struct HIMST_TAG {
	char	Code[4];			//   ?øΩR?øΩ[?øΩh	 4
	char	FillerA;			//   ?øΩJ?øΩ?øΩ?øΩ} 	 1		
	char	Name[20];			//   ?øΩi?øΩ˙≤º	20		
	char	FillerB;			//   ?øΩJ?øΩ?øΩ?øΩ} 	 1		
	char	Ryaku[10];			//   ?øΩ?øΩ?øΩ?øΩ	10
	char	FillerC;			//   ?øΩJ?øΩ?øΩ?øΩ} 	 1		
	char	Wari[2];			//   ?øΩ?øΩ?øΩ?øΩ?øΩ?øΩ	 2	
	char	FillerD;			//   ?øΩJ?øΩ?øΩ?øΩ} 	 1		
	char	Zeiku[2];			//   ?øΩ≈ãÔøΩ	 2		
	char	Filler5;			//   ?øΩJ?øΩ?øΩ?øΩ} 	 1		
	char	Tenku[2];			//   ?øΩ_?øΩ?øΩ	 2		
} HIMST;

/*****************************************************************************/
/* ?øΩI?øΩ?øΩ?øΩf?øΩ[?øΩ^																 */
/*****************************************************************************/
typedef struct TDATA_TAG {
	char	Date[8];			/*   0	?øΩ?øΩ?øΩt		*/
	char	Shop[2];			/*  14	?øΩX?øΩ‹ÉR?øΩ[?øΩh	*/
	char	Code1[13];			/*  18	?øΩ?øΩi?øΩR?øΩ[?øΩh	*/
	char	Code2[13];			/*  31	?øΩ?øΩ?øΩi?øΩR?øΩ[?øΩh	*/
	char	Code3[8];			/*  44	?øΩl?øΩ?øΩ?øΩR?øΩ[?øΩh	*/
	char	Num[5];				/*  52	?øΩ?øΩ?øΩ?øΩ		*/
} TDATA;

/*****************************************************************************/
/* ?øΩ⁄ìÔøΩ?øΩf?øΩ[?øΩ^																 */
/*****************************************************************************/
typedef struct IDATA_TAG {
	char	Date[8];			/*   0	?øΩ?øΩ?øΩt		*/
	char	Shop[2];			/*  14	?øΩo?øΩ◊ìX?øΩR?øΩ[?øΩh	*/
	char	Shop2[2];			/*  16	?øΩ?øΩ?øΩ?øΩX?øΩR?øΩ[?øΩh	*/
	char	Code1[13];			/*  18	?øΩ?øΩi?øΩR?øΩ[?øΩh	*/
	char	Code2[13];			/*  31	?øΩ?øΩ?øΩi?øΩR?øΩ[?øΩh	*/
	char	Code3[8];			/*  44	?øΩl?øΩ?øΩ?øΩR?øΩ[?øΩh	*/
	char	Num[5];				/*  52	?øΩ?øΩ?øΩ?øΩ		*/
} IDATA;
	/* 	57?øΩo?øΩC?øΩg		*/

/*****************************************************************************/
/* ?øΩd?øΩ?øΩ?øΩf?øΩ[?øΩ^																 */
/*****************************************************************************/
typedef struct SDATA_TAG {
	char	Date[8];			/*   0	?øΩ?øΩ?øΩt		*/
	char	DenNo[8];			/*   0	?øΩ`?øΩ[No		*/
	char	Shop[2];			/*  14	?øΩo?øΩ◊ìX?øΩR?øΩ[?øΩh	*/
	char	Code1[13];			/*  18	?øΩ?øΩi?øΩR?øΩ[?øΩh	*/
	char	Code2[13];			/*  31	?øΩ?øΩ?øΩi?øΩR?øΩ[?øΩh	*/
	char	Code3[8];			/*  44	?øΩl?øΩ?øΩ?øΩR?øΩ[?øΩh	*/
	char	Num[5];				/*  52	?øΩ?øΩ?øΩ?øΩ		*/
} SDATA;
	/* 	57?øΩo?øΩC?øΩg		*/
/*****************************************************************************/
/* ?øΩl?øΩ?øΩ?øΩf?øΩ[?øΩ^																 */
/*****************************************************************************/
typedef struct NDATA_TAG {
	char	Date[8];			/*   0	?øΩ?øΩ?øΩt		*/
	char	Shop[1];			/*  14	?øΩo?øΩ◊ìX?øΩR?øΩ[?øΩh	*/
	char	Code1[13];			/*  18	?øΩ?øΩi?øΩR?øΩ[?øΩh	*/
	char	Code2[13];			/*  31	?øΩ?øΩ?øΩi?øΩR?øΩ[?øΩh	*/
	char	Code3[8];			/*  44	?øΩl?øΩ?øΩ?øΩR?øΩ[?øΩh	*/
	char	Code4[13];			/*  44	?øΩl?øΩ?øΩ?øΩR?øΩ[?øΩh	*/
	char	Num[5];				/*  52	?øΩ?øΩ?øΩ?øΩ		*/
} NDATA;/* 	57?øΩo?øΩC?øΩg		*/

/****************************************************************************/
/* ?øΩe?øΩ?øΩ?øΩ?øΩ
/****************************************************************************/
typedef struct INFO_DATA{
	char	Date[8];			//   0	?øΩ?øΩ?øΩt		
	char	Time[6];
	char	tenpo1[2];			//  ?øΩo?øΩ◊ìX?øΩR?øΩ[?øΩh	
	char	tenpo2[2];			//  ?øΩ?øΩ?øΩ?øΩX?øΩR?øΩ[?øΩh
	char	Bumon[2];
	char	Hinsyu[4];
	char	Code1[13];			//  JAN?øΩR?øΩ[?øΩh
	char 	Joudai[10];
	char	Gedai[10];
	char	tanto[2];
	char	Code2[13];
	char	Code3[13];			//  ?øΩl?øΩ?øΩ
	char	Num[4];
	int	flag;
} INFO;
	
/*****************************************************************************/
/* ?øΩ?øΩ?øΩ?øΩf?øΩ[?øΩ^																 */
/*****************************************************************************/
typedef struct URDATA_TAG {
	char	Date[8];			//   0	?øΩ?øΩ?øΩt		
	char	Time[6];
	char	Shop[2];			//  14	?øΩo?øΩ◊ìX?øΩR?øΩ[?øΩh	
	char	RejiNo[2];
	char	Tanto[2];
	char	Code1[13];			//  18	?øΩ?øΩi?øΩR?øΩ[?øΩh	
	char	Code2[13];			//  31	?øΩ?øΩ?øΩi?øΩR?øΩ[?øΩh	
	char	Code3[8];			//  44	?øΩl?øΩ?øΩ?øΩR?øΩ[?øΩh	
	char	Name[20];
	char	Baika[10];
	char	Num[5];				//  52	?øΩ?øΩ?øΩ?øΩ		
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
/* ?øΩÍéûÔøΩ?øΩ?øΩ?øΩf?øΩ[?øΩ^																 */
/*****************************************************************************/
typedef struct INFOUR_TAG {	
	char	Date[8];			//   0	?øΩ?øΩ?øΩt		
	char	Time[6];
	char	Code1[13];			//  18	?øΩ?øΩi?øΩR?øΩ[?øΩh	
	char	Code2[13];			//  18	?øΩ?øΩi?øΩR?øΩ[?øΩh	
	char	Code3[13];			//  44	?øΩl?øΩ?øΩ?øΩR?øΩ[?øΩh	
	char	Name[20];
	char	Baika[10];
	char	Num[4];				//  52	?øΩ?øΩ?øΩ?øΩ	

} INFOUR;

/*****************************************************************************/
/* ?øΩÍéûÔøΩ?øΩ?øΩ?øΩf?øΩ[?øΩ^																 */
/*****************************************************************************/
typedef struct INFOUR_TAG2 {	
	char	Date[8];			//   0	?øΩ?øΩ?øΩt		
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
	char	tenpo1[2];			/*  14	?øΩo?øΩ◊ìX?øΩR?øΩ[?øΩh	*/
	char	tenpo2[2];
	int	flag;
} TENPO;/* 	5?øΩo?øΩC?øΩg		*/

extern	CONTROLDATA	ctrl;
extern	TDATA		tdata;		/* ?øΩI?øΩ?øΩ?øΩf?øΩ[?øΩ^		*/
extern	IDATA		idata;		/* ?øΩ⁄ìÔøΩ?øΩf?øΩ[?øΩ^		*/
extern	NDATA		ndata;		/* ?øΩl?øΩ?øΩ?øΩf?øΩ[?øΩ^		*/
extern	SDATA		sdata;		/* ?øΩd?øΩ?øΩ?øΩf?øΩ[?øΩ^		*/
extern	BUMST		bumst;		/* ?øΩS?øΩ?øΩ?øΩf?øΩ[?øΩ^		*/
extern	HIMST		himst;		/* ?øΩ?øΩ?øΩ?øΩf?øΩ[?øΩ^		*/
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
extern	int	ctoi(char c);
extern	long	powOfTen(int exp);
extern	void	insComma( long, char[] );
extern	void	printNumberWithComma( long num, int col, int row ) ;
// ?øΩ?øΩ?øΩ?øΩ
extern	URDATA		urdata;	
extern  INFO		info;
extern  INFOUR		infour;
extern  INFOUR2		infour2;

extern	short	BumonFind( char *key );
extern	long	BumonTaxFind( char *key );
extern	long	BumonTaxFindByCode1( char *code1 );
extern	long	HinsyuZeikuFindByCode1( char *code1 );
extern	void	drawLineCrossScreen(long y);