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
	JOB_SIIRE,
	JOB_TANA,
	JOB_IDO,
	JOB_URI,
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
	CLR_LABEL,
	CLR_MSGBOX,
	CLR_MAX
};

/* カラーサンプル */
#define CLR_BASE_BACK 	RGB( 200, 255, 200 )		// 基本背景色	RGB(204, 255, 204)			RGB( 173, 216, 230 )
#define CLR_BASE_TEXT 	RGB(   0,   0,   0 )		// 基本文字色				( 黒 )
#define CLR_MSGBOX_BACK RGB( 255, 180, 255 )		// メッセージボックス背景色
#define CLR_MSGBOX_TEXT RGB(   0,   0,   0 )		// メッセージボックス文字色
#define CLR_SCREEN    	RGB( 204, 255, 204 )		// メッセージダイアログ背景色
#define CLR_TITLE_TEXT 	RGB(   0,   0,   0 )		// タイトル文字色			( 黒 )
#define CLR_TITLE_BACK 	RGB( 255, 180,   0 )		// タイトル背景色			255, 213,   0
#define CLR_LABEL_TEXT 	RGB( 255, 255, 255 )		// ラベル文字色				( 黒 )
#define CLR_LABEL_BACK 	RGB(   0,  84, 227 )		// ラベル背景色				( 青 )
#define CLR_REF_TEXT 	RGB(   0,   0,   0 )		// 参照項目文字色			( 黒 )
#define CLR_REF_BACK 	RGB( 255, 255, 128 )		// 参照項目背景色			( 黄 )
#define CLR_NO_TEXT 	RGB( 255, 255, 255 )		// ナンバー文字色			( 白 )
#define CLR_NO_BACK 	RGB(   0,   0,   0 )		// ナンバー背景色			( 黒 )
#define CLR_FUNC_TEXT 	RGB(   0,   0,   0 )		// ファンクション文字色		( 黒 )
#define CLR_FUNC_BACK 	RGB( 255, 255, 255 )		// ファンクション背景色		( 白 )

#define CLR_FUNC_GREEN 	RGB(   0, 255,   0 )		// 緑
#define CLR_FUNC_GREEN1 RGB(   0, 150,   0 )		// 濃い緑
#define CLR_FUNC_GREEN2 RGB( 128, 255, 128 )		// 薄い緑
#define CLR_FUNC_GREEN3 RGB( 200, 255, 200 )		// もっと薄い緑
#define CLR_FUNC_BLUE 	RGB(   0,   0, 255 )		// 青
#define CLR_FUNC_BLUE1 	RGB(   0,  84, 227 )		// 濃い青
#define	CLR_SILVER		RGB( 192, 192, 192 )		// 銀色
#define	CLR_GLAY		RGB(  48,  48,  48 )		// 灰色
#define CLR_YELLOW		RGB( 255, 255,   0 )		// 黄色
#define CLR_LIGHTYELLOW		RGB( 255, 255, 128 )		// 薄黄色
#define CLR_PINK    		RGB( 247, 171, 166 )	// ﾋﾟﾝｸ
#define CLR_ORANGE		RGB( 255, 165,   0 )		// オレンジ色
#define CLR_RED			RGB( 255,   0,   0 )		// 赤
#define CLR_BROWN		RGB( 165,  42,  42 )		// 茶色
#define CLR_LIME		RGB(   0, 255,   0 )		// ライム
#define CLR_GREEN		RGB( 190, 209,  45 )		// 緑色
#define CLR_OLIVE		RGB( 128, 128,   0 )		// オリーブ
#define CLR_CYAN		RGB(   0, 255, 255 )		// シアン
#define CLR_BLUE		RGB(   0,   0, 255 )		// 青色
#define CLR_DARKBLUE	RGB(   0,   0, 160 )		// 濃い青
#define CLR_LIGHTBLUE	RGB( 173, 216, 230 )		// 薄い青
#define CLR_PURPLE		RGB( 167,  87, 168 )		// 紫
#define CLR_FUCHSIA		RGB( 255,   0, 255 )

/*************************************************/
/*  コントロールファイル                         */
/*************************************************/
typedef struct CONTROL_BUFF{
	char	LoadFlag[30];		/*   0	ローディング情報	*/
	short	SDataCnt;		/*  38	仕入データ件数		*/
	short	TDataCnt;		/*  42	棚卸データ件数		*/
	short	IDataCnt;		/*  44	移動データ件数		*/
	short	NDataCnt;		/*  46	値下データ件数		*/
	char	Cursor;			/*  48	カーソルタイプ 		*/
	char	Beep;			/*  49	音設定 			*/
	char	Bright;			/*  50	画面輝度設定		*/
	short	BUMSTCnt;		/*  54	部門マスタ件数   	*/
	short	HIMSTCnt;		/*  58	品種マスタ件数		*/
	short	URDataCnt;		/*  62	売上データ件数		*/
	short	InfoUrCnt;		/*  66	売上データ件数		*/
	long	RecNo;			/*  74	レシートNO		*/
	char	RejiNo[2];		/*  76	レジNO			*/
	char	RecPrint[1];		/*  77	レシート印字		*/
	char	TaxRate[2];		/*  79	システム税率		*/	
	char	filler[50];		/*  59	予備			*/
} CONTROLDATA;				/* 128バイト			*/


/*****************************************************************************/
/* 部門マスタ	2+1+20+1+10+1+2 = 37															 */
/*****************************************************************************/
typedef struct BUMST_TAG {
	char	Code[2];			//   コード	 2
	char	FillerA;			//   カンマ 	 1		
	char	Name[20];			//   部門名	20		
	char	FillerB;			//   カンマ 	 1		
	char	Ryaku[10];			//   略称	10
	char	FillerC;			//   カンマ 	 1		
	char	Zeiritu[2];			//   税率	 2		
} BUMST;

/*****************************************************************************/
/* 品種マスタ	4+1+20+1+10+1+2+1+2+1+2=45															 */
/*****************************************************************************/
typedef struct HIMST_TAG {
	char	Code[4];			//   コード	 4
	char	FillerA;			//   カンマ 	 1		
	char	Name[20];			//   品種名	20		
	char	FillerB;			//   カンマ 	 1		
	char	Ryaku[10];			//   略称	10
	char	FillerC;			//   カンマ 	 1		
	char	Wari[2];			//   割引率	 2	
	char	FillerD;			//   カンマ 	 1		
	char	Zeiku[2];			//   税区	 2		
	char	Filler5;			//   カンマ 	 1		
	char	Tenku[2];			//   点区	 2		
} HIMST;

/*****************************************************************************/
/* 棚卸データ																 */
/*****************************************************************************/
typedef struct TDATA_TAG {
	char	Date[8];			/*   0	日付		*/
	char	Shop[2];			/*  14	店舗コード	*/
	char	Code1[13];			/*  18	上段コード	*/
	char	Code2[13];			/*  31	下段コード	*/
	char	Code3[8];			/*  44	値下コード	*/
	char	Num[5];				/*  52	数量		*/
} TDATA;

/*****************************************************************************/
/* 移動データ																 */
/*****************************************************************************/
typedef struct IDATA_TAG {
	char	Date[8];			/*   0	日付		*/
	char	Shop[2];			/*  14	出荷店コード	*/
	char	Shop2[2];			/*  16	受入店コード	*/
	char	Code1[13];			/*  18	上段コード	*/
	char	Code2[13];			/*  31	下段コード	*/
	char	Code3[8];			/*  44	値下コード	*/
	char	Num[5];				/*  52	数量		*/
} IDATA;
	/* 	57バイト		*/

/*****************************************************************************/
/* 仕入データ																 */
/*****************************************************************************/
typedef struct SDATA_TAG {
	char	Date[8];			/*   0	日付		*/
	char	DenNo[8];			/*   0	伝票No		*/
	char	Shop[2];			/*  14	出荷店コード	*/
	char	Code1[13];			/*  18	上段コード	*/
	char	Code2[13];			/*  31	下段コード	*/
	char	Code3[8];			/*  44	値下コード	*/
	char	Num[5];				/*  52	数量		*/
} SDATA;
	/* 	57バイト		*/
/*****************************************************************************/
/* 値下データ																 */
/*****************************************************************************/
typedef struct NDATA_TAG {
	char	Date[8];			/*   0	日付		*/
	char	Shop[1];			/*  14	出荷店コード	*/
	char	Code1[13];			/*  18	上段コード	*/
	char	Code2[13];			/*  31	下段コード	*/
	char	Code3[8];			/*  44	値下コード	*/
	char	Code4[13];			/*  44	値下コード	*/
	char	Num[5];				/*  52	数量		*/
} NDATA;/* 	57バイト		*/

/****************************************************************************/
/* 各種情報
/****************************************************************************/
typedef struct INFO_DATA{
	char	Date[8];			//   0	日付		
	char	Time[6];
	char	tenpo1[2];			//  出荷店コード	
	char	tenpo2[2];			//  受入店コード
	char	Bumon[2];
	char	Hinsyu[4];
	char	Code1[13];			//  JANコード
	char 	Joudai[10];
	char	Gedai[10];
	char	tanto[2];
	char	Code2[13];
	char	Code3[13];			//  値下
	char	Num[4];
	int	flag;
} INFO;
	
/*****************************************************************************/
/* 売上データ																 */
/*****************************************************************************/
typedef struct URDATA_TAG {
	char	Date[8];			//   0	日付		
	char	Time[6];
	char	Shop[2];			//  14	出荷店コード	
	char	RejiNo[2];
	char	Tanto[2];
	char	Code1[13];			//  18	上段コード	
	char	Code2[13];			//  31	下段コード	
	char	Code3[8];			//  44	値下コード	
	char	Name[20];
	char	Baika[10];
	char	Num[5];				//  52	数量		
	char 	Genkin[6];
	char	Credit[6];
	char 	Kinken[6];
	char	Coupon[6];
	char 	RecNo[6];
	char 	KyakuNo[1];
	char  	SyouhiFlg[1];
	char 	Reason[2];
	long	lngOturi;
} URDATA;

/*****************************************************************************/
/* 一時売上データ																 */
/*****************************************************************************/
typedef struct INFOUR_TAG {	
	char	Date[8];			//   0	日付		
	char	Time[6];
	char	Code1[13];			//  18	上段コード	
	char	Code2[13];			//  18	上段コード	
	char	Code3[13];			//  44	値下コード	
	char	Name[20];
	char	Baika[10];
	char	Num[4];				//  52	数量	

} INFOUR;

/*****************************************************************************/
/* 一時売上データ																 */
/*****************************************************************************/
typedef struct INFOUR_TAG2 {	
	char	Date[8];			//   0	日付		
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
	char	tenpo1[2];			/*  14	出荷店コード	*/
	char	tenpo2[2];
	int	flag;
} TENPO;/* 	5バイト		*/

extern	CONTROLDATA	ctrl;
extern	TDATA		tdata;		/* 棚卸データ		*/
extern	IDATA		idata;		/* 移動データ		*/
extern	NDATA		ndata;		/* 値引データ		*/
extern	SDATA		sdata;		/* 仕入データ		*/
extern	BUMST		bumst;		/* 担当データ		*/
extern	HIMST		himst;		/* 部門データ		*/
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
extern	long 	getJyoudai(char *);
extern	int	ctoi(char c);
extern	long	powOfTen(int exp);
extern	void	insComma( long, char[] );
extern	void	printNumberWithComma( long num, int col, int row ) ;
// 売上
extern	URDATA		urdata;	
extern  INFO		info;
extern  INFOUR		infour;
extern  INFOUR2		infour2;
