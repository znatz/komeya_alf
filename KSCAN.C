#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"


enum {
	CODE1=1,
	CODE2,
	NUM,
	DENTRY,
	DELETE
};

static short	SaveItem;
static char		Code[13];

static void		meisaiclr( void );
static void		Display( short item,short Func);

/*****************************************************************************/
/* 	明細データ初期化														 */
/*****************************************************************************/
static void meisaiclr( void )
{
	memset( tdata.Code1, 0x00, sizeof( tdata.Code1 ) );
	memset( tdata.Code2, 0x00, sizeof( tdata.Code2 ) );
	memset( tdata.Code3, 0x00, sizeof( tdata.Code3 ) );
	memset( tdata.Num, 	 0x00, sizeof( tdata.Num   ) );
	memset( Code,		 0x00, sizeof( Code ) );
}

/*****************************************************************************/
/* 			     															 */
/*****************************************************************************/
static void Display( short item , short Func )
{
	switch( item ){
		case CODE1:
			if( SaveItem != CODE1)
			{
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<検品入力>      ", False, CLR_KP_TITLE );
				ckputss( 0,  4, "▲              ", False, CLR_BASE );
				ckputss( 0,  6, "▽              ", False, CLR_BASE );
				ckputss( 0,  9, "                ", False, CLR_BASE );
				ckputss( 0, 11, "                ", False, CLR_BASE );	
				ckputss( 0, 14, "F1:戻る         ", False, CLR_BASE );
			}
			/* 上段コード */
			if( tdata.Code1[0] ){
				ckputsn( 3, 4, tdata.Code1, 13, False, CLR_BASE );
			} else {
				ckputss( 0, 4, "▲              ", False, CLR_BASE );
			}
			break;
		case CODE2:
			if( SaveItem != CODE2){
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<検品入力>      ", False, CLR_KP_TITLE );
				ckputss( 0,  4, "△              ", False, CLR_BASE );
				ckputss( 0,  6, "▼              ", False, CLR_BASE );
				ckputss( 0,  9, "                ", False, CLR_BASE );
				ckputss( 0, 11, "                ", False, CLR_BASE );	
				ckputss( 0, 14, "F1:戻る         ", False, CLR_BASE );
			}

			/* 上段コード */
			if( tdata.Code1[0] ){
				ckputsn( 3,  4, tdata.Code1, 13, False, CLR_BASE );
			} else {
				ckputss( 0,  4, "△              ", False, CLR_BASE );
			}
			/* 下段コード */
			if( tdata.Code2[0] ){
				ckputsn( 3,  6, tdata.Code2, 13, False, CLR_BASE );
			} else {
				ckputss( 0,  6, "▼              ", False, CLR_BASE );
			}
			break;
		case DENTRY:
			if(SaveItem != DENTRY )
			{
				ClsColor();	/*   0123456789012345*/
				ckputss( 0,  0, "<検品入力>      ", False, CLR_KP_TITLE );
				ckputss( 0,  4, "△              ", False, CLR_BASE );
				ckputss( 0,  6, "▽              ", False, CLR_BASE );
				ckputss( 0,  9, "上代:           ", False, CLR_BASE );
				ckputss( 0, 11, "下代:           ", False, CLR_BASE );	
				ckputss( 0, 14, "F1:戻る         ", False, CLR_BASE );			
			}
			/* 上段コード */
			if( tdata.Code1[0] ){
				//displayMsg(getJyoudai(&tdata.Code1));			
				ckputsn( 3, 4, tdata.Code1, 13, False, CLR_BASE );
				printNumberWithComma( getJyoudai(&tdata.Code1), 6, 9);
			}
			/* 下段コード */
			if( tdata.Code2[0] ){
				//displayMsg(getGedai(&tdata.Code2));			
				ckputsn( 3, 6, tdata.Code2, 13, False, CLR_BASE );
				printNumberWithComma( getGedai(&tdata.Code2), 6, 11);

			/* TEST用 
				displayMsg(BumonTaxFindByCode1(&tdata.Code1));
				displayMsg(HinsyuZeikuFindByCode1(&tdata.Code1));
			*/
			}

			break;
	}
	SaveItem = item;
}

/*****************************************************************************/
/* 	検品業務     							     */
/*****************************************************************************/
void Kscan( void )
{
	short	item, ret, Func;//Func=1:1点 Func=2:複数　

	memset( &tdata, 0x00, sizeof( tdata ) );
	setpict( 1 );
	item = CODE1;
	Func = 1;
	SaveItem = -1;
	while( item ){
		if( MaxCheck( ctrl.TDataCnt, TDATA_MAX ) ) return;
		Display( item,Func );
		switch( item ){
			case CODE1:
				ret = CodeInput( 3, 4, tdata.Code1, sizeof( tdata.Code1 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;
			case CODE2:
				ret = CodeInput( 3, 6, tdata.Code2, sizeof( tdata.Code2 ), 
								 BCR_NOTDRW | BCR_WPC | KEY_FUNC );
				break;
			case DENTRY:
				//Func=1;
				//item = CODE1;
				//continue;
				ret = getch();
				//displayMsg(ret);
				meisaiclr();
				item = CODE1;
				break;
		}
		if( ret == ENTRY || ret == SENTRY || ret == SKIP || ret == BARENT ){
			if( item == CODE1 || item == DENTRY ){
				//3桁が501〜999の場合
				if( atoin( tdata.Code1, 3 ) > 500 && atoin( tdata.Code1, 3 ) < 999 ){
					item = CODE2;					
					continue;
				} else {
					/* 上記に該当しない場合はエラー */
					beep( 10, 2 );
					meisaiclr();
					continue;
				}
			} else if( item == CODE2 ){
				/* 上段コードの先頭から３桁が501〜999の場合、0999、1、3から始まるコード以外不可 */
				if( atoin( tdata.Code1, 3 ) > 500 && atoin( tdata.Code1, 3 ) < 999 ){
					if( (tdata.Code2[0] == '0' || tdata.Code2[0] == '1' || tdata.Code2[0] == '2' || tdata.Code2[0] == '3') && tdata.Code2[12] != ' ' ){
						if(Func == 1){
							item = DENTRY;
						}
						continue;
					} else {
						/* 上記に該当しない場合はエラー */
						memset( tdata.Code2, 0x00, sizeof( tdata.Code2 ) );
						beep( 10, 2 );
						continue;
					}

				} else {
					/* 上記に該当しない場合はエラー */
					memset( tdata.Code2, 0x00, sizeof( tdata.Code2 ) );
					beep( 10, 2 );
					continue;
				}
			}
			++item;
		}else if( ret == F2KEY ){
			if ( ctrl.TDataCnt > 0 ) {
				if(item == CODE1){
					item = DELETE;
				}
				continue;
			}
		}else if( ret == F1KEY ){
			if( item == CODE2 || item == DENTRY ){
				meisaiclr();
				Func=1;
				item = CODE1;
				continue;
			}
			menu();
		}
	}
}