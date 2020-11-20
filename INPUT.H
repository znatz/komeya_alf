#ifndef	INPUT_H
#define	INPUT_H

/*****************************************************************************/
/* ktoutch()関数のKEYｺｰﾄﾞ                                                      */
/*****************************************************************************/
#define		KS_1			0x00000001
#define		KS_2			0x00000002
#define		KS_3			0x00000004
#define		KS_4			0x00000008
#define		KS_5			0x00000010
#define		KS_6			0x00000020
#define		KS_7			0x00000040
#define		KS_8			0x00000080
#define		KS_9			0x00000100
#define		KS_DOT			0x00000200
#define		KS_0			0x00000400
#define		KS_HYPHEN		KS_DOT
#define		KS_CLR			0x00000800
#define		KS_F1			0x00001000
#define		KS_F2			0x00002000
#define		KS_F3			0x00004000
#define		KS_F4			0x00008000
#define		KS_UP			0x00010000
#define		KS_DOWN			0x00020000
#define		KS_ENT			0x00040000
#define		KS_SCAN			0x00080000
#define		KS_TRG			KS_SCAN
#define		KS_TRG_L		0x00100000
#define		KS_TRG_R		0x00200000
#define		KS_PWR			0x00400000
/*****************************************************************************/
/* getch()関数のKEYｺｰﾄﾞ                                                      */
/*****************************************************************************/
#define	TRIG	's'
#define CLR		0x0c
#define DOT		'.'
#define MIN		'.'
#define F1		'a'
#define F2		'b'
#define F3		'c'
#define F4		'd'
#define ENT		0x0d
#define UP		'e'
#define DOWN	'f'
#define	LSIDE	'l'
#define	RSIDE	'r'
#define	PW		'p'
/*****************************************************************************/
/* input関数入力制御スイッチ												 */
/*****************************************************************************/
#define	KEY_FIX			0x00000001L		/* キー入力			0:可変長   1:固定長	 */
#define	KEY_MINUS		0x00000002L		/* マイナス入力		0:不可     1:可能	 */
#define	KEY_DECPOINT	0x00000004L		/* 小数点入力		0:不可     1:可能	 */
#define	KEY_SKIP		0x00000008L		/* 空ＥＮＴ			0:不可     1:可能	 */
#define	KEY_NOTCR		0x00000010L		/* ＥＮＴキー		0:必要     1:不要	 */
#define	KEY_UPDOWN		0x00000020L		/* ▲▼キー			0:不可     1:可能	 */
#define	CUR_BLOCK		0x00000040L		/* カーソルブロック	0:不可     1:可能	 */
#define	KEY_FUNC		0x00000080L		/* ファンクション   0:不可     1:可能	 */
#define	INP_PASS		0x00000100L		/* パスワード入力    0:不可     1:可能	 */
#define	ANK_INPUT		0x00000200L		/* 小ﾌｫﾝﾄ入力	    0:不可     1:可能	 */
#define	KEY_CLR			0x00000400L		/* クリアキー通知   0:不可     1:可能	 */
#define	MIN_ADD			0x00000800L		/* ﾏｲﾅｽを入力長に   0:数える   1:数えない*/
#define	TRIG_ENT		0x00001000L		/* トリガをＥＮＴ   0:不可     1:可能    */
#define	DEC_ONE			0x00002000L		/* 小数点以下１桁 */
#define	DEC_TWO			0x00004000L		/* 小数点以下２桁 */
#define	COMMA_INPUT		0x00004000L		/* カンマ入力 */

#define	INMODEMASK		0x00ff0000L
#define	IN_NUMERIC		0x00000000L		/* 数字のみの入力（デフォルト）	*/
#define	IN_ALPHA		0x00010000L		/* アルファベットのみの入力		*/
#define	IN_HEX			0x00020000L		/* HEXコードのみの入力			*/
#define	IN_ALLCHARS		0x00040000L		/* すべての文字の入力			*/

#define	BCRFLAGMASK		0xff000000L
#define	BCR_NOTCD		0x01000000L		/* Ｃ／Ｄチェック	0:する     1:しない  */
#define	BCR_NOTDRW		0x02000000L		/* 表示				0:する     1:しない	 */
#define	BCR_WPC			0x04000000L		/* WPC  読み取り	0:不可     1:可能	 */
#define	BCR_C39			0x08000000L		/* Ｃ３９読み取り	0:不可     1:可能	 */
#define	BCR_C128		0x10000000L		/* Ｃ１２８読み取り	0:不可     1:可能	 */
#define	BCR_ITF			0x20000000L		/* ＩＴＦ読み取り	0:不可     1:可能	 */
#define	BCR_NW7			0x40000000L		/* ＩＴＦ読み取り	0:不可     1:可能	 */
#define	BCR_PEN			0x80000000L		/* ペンスキャナ仕様 0:不可	   1:可能	 */

/*****************************************************************************/
/* input関数の戻り値														 */
/*****************************************************************************/
#define	SKIP			0			/* 空ＥＮＴされた		 */
#define	ENTRY			1			/* キーで登録された		 */
#define	BARENT			2			/* ＢＣＲで登録された	 */
#define	BSKEY			3			/* BSキーが押された		 */
#define CLRKEY			4			/* CLRｷｰが押された		 */

#define	UPKEY			5			/* ▲キーが押された		 */
#define	DOWNKEY			6			/* ▼キーが押された		 */

#define	SENTRY			7			/* 入力項目先頭でENT(初期値有り) */

#define	F1KEY			8			/* Ｆ１キーが押された	 */
#define	F2KEY			9			/* Ｆ２キーが押された	 */
#define	F3KEY			10			/* Ｆ３キーが押された	 */
#define	F4KEY			11			/* Ｆ４キーが押された	 */
#define	DEC_CLR_KEY		12			/* ・＋ＣＬＲ */
#define	BAR_ERR			97			/* Ｃ／Ｄエラー			 */

#define A1		'A'
#define A2		'B'
#define A3		'C'
#define A4		'D'
#define A5		'E'
#define CD		'Z'

#endif

short	input( short x, short y, char *inpBuf, short len, long prm );
short	isBarcode( char *code, short inpType );
short	calcMod10( char *code, short Weight, short len );
void	cputchar( short x, short y, short ank, short rev, short ch );
void	cputstr( short x, short y, short ank, short rev, char *message );
void	cputchs( short x, short y, short ank, char c, short rev, short len );
void	putspc( short x, short y, short ank, short rev, short len );
void 	PenBcrOpen( void );
void 	PenBcrClose( void );
short 	PenBcrGet( char * );


