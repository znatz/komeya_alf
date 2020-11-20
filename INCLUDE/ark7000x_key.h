#ifndef	_ARK7000X_KEY_H_
#define	_ARK7000X_KEY_H_


/****************************************************************************/
/*	特殊キー																*/
/****************************************************************************/
#define		SP_PWR			1
#define		SP_SCAN			2
#define		SP_TRG_L		3
#define		SP_TRG_R		4

/****************************************************************************/
/*	キーキャラクタ定義														*/
/****************************************************************************/
#define		CH_0			'0'
#define		CH_1			'1'
#define		CH_2			'2'
#define		CH_3			'3'
#define		CH_4			'4'
#define		CH_5			'5'
#define		CH_6			'6'
#define		CH_7			'7'
#define		CH_8			'8'
#define		CH_9			'9'
#define		CH_DOT			'.'
#define		CH_HYPHEN		CH_DOT
#define		CH_CLR			'\014'
#define		CH_F1			'a'
#define		CH_F2			'b'
#define		CH_F3			'c'
#define		CH_F4			'd'
#define		CH_UP			'e'
#define		CH_DOWN			'f'
#define		CH_ENT			'\015'
#define		CH_SCAN			's'
#define		CH_TRG			CH_SCAN
#define		CH_TRG_L		'l'
#define		CH_TRG_R		'r'
#define		CH_PW			'p'

/****************************************************************************/
/*	キーコード定義															*/
/****************************************************************************/
#define		KN_1			 1
#define		KN_2			 2
#define		KN_3			 3
#define		KN_4			 4
#define		KN_5			 5
#define		KN_6			 6
#define		KN_7			 7
#define		KN_8			 8
#define		KN_9			 9
#define		KN_DOT			10
#define		KN_HYPHEN		KN_DOT
#define		KN_0			11
#define		KN_CLR			12
#define		KN_F1			13
#define		KN_F2			14
#define		KN_F3			15
#define		KN_F4			16
#define		KN_UP			17
#define		KN_DOWN			18
#define		KN_ENT			19
#define		KN_SCAN			20
#define		KN_TRG			KN_SCAN
#define		KN_TRG_L		21
#define		KN_TRG_R		22
#define		KN_PWR			23


/****************************************************************************/
/*	キー入力状態ビット定義													*/
/****************************************************************************/
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

#endif /* _ARK7000X_KEY_H_ */

