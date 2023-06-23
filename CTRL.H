#ifndef	CTRL_H
#define	CTRL_H

#ifndef	True
#define True 1
#endif
#ifndef False
#define False 0
#endif

#include <capture.h>

#define	VER	"Ver1.00"
#define	VER2	"POSCO  Ver1.00\n"
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

#define CLR_BASE_BACK 	RGB( 200, 255, 200 )
#define CLR_BASE_TEXT 	RGB(   0,   0,   0 )
#define CLR_MSGBOX_BACK RGB( 255, 180, 255 )
#define CLR_MSGBOX_TEXT RGB(   0,   0,   0 )
#define CLR_SCREEN    	RGB( 204, 255, 204 )
#define CLR_TITLE_TEXT 	RGB(   0,   0,   0 )
#define CLR_TITLE_BACK 	RGB( 255, 180,   0 )
#define CLR_LABEL_TEXT 	RGB( 255, 255, 255 )
#define CLR_LABEL_BACK 	RGB(   0,  84, 227 )
#define CLR_REF_TEXT 	RGB(   0,   0,   0 )
#define CLR_REF_BACK 	RGB( 255, 255, 128 )
#define CLR_NO_TEXT 	RGB( 255, 255, 255 )
#define CLR_NO_BACK 	RGB(   0,   0,   0 )
#define CLR_FUNC_TEXT 	RGB(   0,   0,   0 )
#define CLR_FUNC_BACK 	RGB( 255, 255, 255 )

#define CLR_FUNC_GREEN 	RGB(   0, 255,   0 )
#define CLR_FUNC_GREEN1 RGB(   0, 150,   0 )
#define CLR_FUNC_GREEN2 RGB( 128, 255, 128 )
#define CLR_FUNC_GREEN3 RGB( 200, 255, 200 )
#define CLR_FUNC_BLUE 	RGB(   0,   0, 255 )
#define CLR_FUNC_BLUE1 	RGB(   0,  84, 227 )
#define	CLR_SILVER		RGB( 192, 192, 192 )
#define	CLR_GLAY		RGB(  48,  48,  48 )
#define CLR_YELLOW		RGB( 255, 255,   0 )
#define CLR_ORANGE		RGB( 255, 165,   0 )
#define CLR_RED			RGB( 255,   0,   0 )
#define CLR_BROWN		RGB( 165,  42,  42 )
#define CLR_LIME		RGB(   0, 255,   0 )
#define CLR_GREEN		RGB( 190, 209,  45 )
#define CLR_OLIVE		RGB( 128, 128,   0 )
#define CLR_CYAN		RGB(   0, 255, 255 )
#define CLR_BLUE		RGB(   0,   0, 255 )
#define CLR_DARKBLUE	RGB(   0,   0, 160 )
#define CLR_LIGHTBLUE	RGB( 173, 216, 230 )
#define CLR_PURPLE		RGB( 167,  87, 168 )
#define CLR_FUCHSIA		RGB( 255,   0, 255 )
#define CLR_LIGHTYELLOW		RGB( 255, 255, 128 )
#define CLR_PINK    		RGB( 247, 171, 166 )

typedef struct CONTROL_BUFF{
	char	LoadFlag[30];
	short	SDataCnt;
	short	TDataCnt;
	short	IDataCnt;
	short	NDataCnt;
	char	Cursor;
	char	Beep;
	char	Bright;
	short	BUMSTCnt;
	short	HIMSTCnt;
	short	TAMSTCnt;
	short	REMSTCnt;
	short	TSMSTCnt;
	short	URDataCnt;
	short	InfoUrCnt;
	long	RecNo;
	char	ShopNo[2];
	char	RejiNo[2];
	char	RecPrint[1];
	char	TaxRate[2];
	char    TaxType[1];
	char	filler[39];
} CONTROLDATA;


/*****************************************************************************/
/* 部門マスタ	2+1+20+1+10+1+2 = 37										   */
/*****************************************************************************/
typedef struct BUMST_TAG {
	char	Code[2];
	char	FillerA;
	char	Name[20];
	char	FillerB;
	char	Ryaku[10];
	char	FillerC;
	char	Zeiritu[2];
} BUMST;

/*****************************************************************************/
/* 品種マスタ 4+1+20+1+10+1+2+1+2+1+2=45									   */
/*****************************************************************************/
typedef struct HIMST_TAG {
	char	Code[4];
	char	FillerA;
	char	Name[20];
	char	FillerB;
	char	Ryaku[10];
	char	FillerC;
	char	Wari[2];
	char	FillerD;
	char	Zeiku[2];
	char	Filler5;
	char	Tenku[2];
} HIMST;

/*****************************************************************************/
/* 担当マスタ 2+1+20=23														   */
/*****************************************************************************/
typedef struct TAMST_TAG {
	char	Code[2];
	char	FillerA;
	char	Name[20];
} TAMST;

/*****************************************************************************/
/* レシートマスタ 2+1+30=33														   */
/*****************************************************************************/
typedef struct REMST_TAG {
	char	Code[2];
	char	FillerA;
	char	Name[30];
} REMST;

/*****************************************************************************/
/* 商品マスタ 13+1+20+1+2+1+2+1+10+1+10=62  */
/*****************************************************************************/
typedef struct TSMST_TAG {
	char	Code[13];
	char	FillerA;
	char	Name[20];
	char	FillerB;
	char	Bumon[2];
	char	FillerC;
	char	Hinsyu[2];
	char	FillerD;
	char	Zeinuki[10];
	char	FillerE;
	char	Zeikomi[10];
} TSMST;

typedef struct TDATA_TAG {
	char	Date[8];
	char	Shop[2];
	char	Code1[28];
	char	Code2[13];
	char	Code3[8];
	char	Num[5];
} TDATA;

typedef struct IDATA_TAG {
	char	Date[8];
	char	Shop[2];
	char	Shop2[2];
	char	Code1[28];
	char	Code2[13];
	char	Code3[8];
	char	Num[5];
} IDATA;

typedef struct SDATA_TAG {
	char	Date[8];
	char	DenNo[8];
	char	Shop[2];
	char	Code1[28];
	char	Code2[13];
	char	Code3[8];
	char	Num[5];
} SDATA;

typedef struct NDATA_TAG {
	char	Date[8];
	char	Shop[1];
	char	Code1[28];
	char	Code2[13];
	char	Code3[8];
	char	Code4[13];
	char	Num[5];
} NDATA;

typedef struct INFO_DATA{
	char	Date[8];
	char	Time[6];
	char	tenpo1[2];
	char	tenpo2[2];
	char	Bumon[2];
	char	Hinsyu[4];
	char	Code1[28];
	char 	Joudai[10];
	char 	Baika[10];
	char	Gedai[10];
	char	tanto[2];
	char	Code2[13];
	char	Code3[8];
	char	Num[4];
	long	lngZeiku;
	long	lngFinalTaxRate;
	long	lngFinalTaxRateTax;
	long    lngBumonTaxRateTax;
	long    lngSystemTaxRate;
	long    lngSystemTaxRateTax;
	long	lngHinsyuZeiku;
	long	lngSystemZeiku;
	int	flag;
} INFO;
	
typedef struct URDATA_TAG {
	char	Date[8];
	char	Time[6];
	char	Shop[2];
	char	RejiNo[2];
	char	Tanto[2];
	char	Code1[28];
	char	Code2[13];
	char	Code3[8];
	char	Name[20];
	char	Joudai[10];
	char	Baika[10];
	char	Num[5];	
	char 	Genkin[6];
	char	Credit[6];
	char 	Kinken[6];
	char	Coupon[6];
	char 	RecNo[6];
	char 	KyakuNo[1];
	char  	SyouhiFlg[1];
	char 	Reason[2];
	long	lngOturi;
	long	lngZeiku;
	long	lngFinalTaxRate;
	long	lngFinalTaxRateTax;
	long    lngBumonTaxRateTax;
	long    lngSystemTaxRate;
	long    lngSystemTaxRateTax;
} URDATA;

typedef struct INFOUR_TAG {	
	char	Date[8];
	char	Time[6];
	char	Code1[28];
	char	Code2[13];
	char	Code3[8];
	char	Name[20];
	char	Joudai[10];
	char	Baika[10];
	char	Num[4];	
	long	lngZeiku;
	long	lngFinalTaxRate;
	long	lngFinalTaxRateTax;
	long    lngBumonTaxRateTax;
	long    lngSystemTaxRate;
	long    lngSystemTaxRateTax;
} INFOUR;

typedef struct INFOUR_TAG2 {	
	char	Date[8];
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
	char	tenpo1[2];
	char	tenpo2[2];
	int	flag;
} TENPO;

extern	CONTROLDATA	ctrl;
extern	TDATA		tdata;
extern	IDATA		idata;
extern	NDATA		ndata;
extern	SDATA		sdata;
extern	BUMST		bumst;
extern	HIMST		himst;
extern	TAMST		tamst;
extern	REMST		remst;
extern	TSMST		tsmst;
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
extern	int		ctoi(char c);
extern	long	powOfTen(int exp);
extern	void	insComma( long, char[] );
extern	void	printNumberWithComma( long num, int col, int row ) ;
extern	URDATA		urdata;	
// * 売上一行の一時データ
extern  INFO		info;
// * レシート一行の一時データ
extern  INFOUR		infour;
extern  INFOUR2		infour2;
// * 実装 WORKSUB.C
extern	short	BumonFind( char *key );
extern	long	BumonCodeFindByCode1( char *code1 );
extern	long	BumonTaxFind( char *key );
extern	long	BumonTaxFindByCode1( char *code1 );
// * 上段上代は税抜きでセットされるため、品種の税区実際見ない予備よ、以下の関数が0(外税しか返さない)に固定している
extern	long	HinsyuZeikuFindByCode1( char *code1 );
extern	void	drawLineCrossScreen(long y);
extern	void	convertToString( long num, char str[MAXLEN] );
extern	long	HinsyuFindByCode1( char *code1 );
extern	int		PrintMain_NEX_M230(char* addr, bool crc, int mode , short Flag );

// * menu.c
extern	void	print_receipt_header();