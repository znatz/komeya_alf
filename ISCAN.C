#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"

enum
{
	SHOP = 1,
	SHOP2,
	CODE1,
	NUM,
	DENTRY,
	DELETE
};

static short SaveItem;
static char Code[13];

static void meisaiclr(void);
static void Display(short item);
static void entrydata(short sw);

/*****************************************************************************/
static void meisaiclr(void)
{
	memset(idata.Code1, 0x00, sizeof(idata.Code1));
	memset(idata.Code2, 0x00, sizeof(idata.Code2));
	memset(idata.Code3, 0x00, sizeof(idata.Code3));
	memset(idata.Num, 0x00, sizeof(idata.Num));
	memset(Code, 0x00, sizeof(Code));
}

/*****************************************************************************/
/* 			     															 */
/*****************************************************************************/
static void Display(short item)
{
	switch (item)
	{
	case SHOP:
		if (SaveItem != SHOP)
		{
			ClsColor(); /* 0123456789012345*/
			ckputss(0, 0, "<移動入力>   ", False, CLR_ID_TITLE);
			ckputss(0, 5, "出 荷 店:       ", False, CLR_BASE);
			ckputss(0, 10, "受 入 店:       ", False, CLR_BASE);
			ckputss(0, 15, "F1:ﾒﾆｭｰ ENT:確定", False, CLR_BASE);
		}
		memset(tenpo.tenpo1, 0x00, sizeof(tenpo.tenpo1)); // 店舗データ取消
		memset(tenpo.tenpo2, 0x00, sizeof(tenpo.tenpo2)); // 店舗データ取消
		/* 件数 */
		ckprintf(10, 0, False, CLR_ID_TITLE, "%4d件", ctrl.IDataCnt);
		/* 店舗コード */
		if (tenpo.tenpo1[0])
		{
			ckputsn(9, 8, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
			/*
			if( mmst.Code[0] ){
				ckputsn( 2, 7,  mmst.Name, 		14, False, CLR_BASE );
			} else {
				kputspc( 2, 7, 14, CLR_BASE );
			}*/
		}
		else
		{
			kputspc(9, 5, 2, CLR_BASE);
			kputspc(2, 7, 14, CLR_BASE);
		}
		break;
	case SHOP2:
		if (SaveItem != SHOP2)
		{
			ClsColor(); /* 0123456789012345*/
			ckputss(0, 0, "<移動入力>   ", False, CLR_ID_TITLE);
			ckputss(0, 5, "出 荷 店:       ", False, CLR_BASE);
			ckputss(0, 10, "受 入 店:       ", False, CLR_BASE);
			ckputss(0, 15, "F1:戻る ENT:確定", False, CLR_BASE);
		}
		/* 件数 */
		ckprintf(10, 0, False, CLR_ID_TITLE, "%4d件", ctrl.IDataCnt);
		/* 店舗コード */
		// ckprintf( 9, 5, tenpo.tenpo1,  sizeof( tenpo.tenpo1  ), False, CLR_BASE );
		ckprintf(9, 5, False, CLR_BASE, "%s", tenpo.tenpo1);
		// ckputsn( 2, 7,  mmst.Name, 		14, False, CLR_BASE );
		/* 店舗コード2 */
		if (tenpo.tenpo2[0])
		{
			ckputsn(9, 10, tenpo.tenpo2, sizeof(tenpo.tenpo2), False, CLR_BASE);
			/*
			if( mmst2.Code[0] ){

				//ckputsn( 2, 12,  mmst2.Name, 		14, False, CLR_BASE );
				//ckputsn( 3, 9, &mmst.Name[10], 	10, False, CLR_BASE );
			} else {
				kputspc( 2, 12, 14, CLR_BASE );
				//kputspc( 3, 9, 10, CLR_BASE );
			}
		} else {
			kputspc( 9, 10,  1, CLR_BASE );
			kputspc( 2, 12, 14, CLR_BASE );
			//kputspc( 3, 9, 10, CLR_BASE );
			*/
		}
		break;
	case CODE1:
		if (SaveItem != CODE1)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<移動入力>      ", False, CLR_ID_TITLE);
			ckputss(0, 3, "店舗:   → 　　  ", False, CLR_BASE);
			ckputss(0, 6, "▲              ", False, CLR_BASE);
			ckputss(0, 8, "               ", False, CLR_BASE);
			ckputss(0, 11, "                ", False, CLR_BASE);
			ckputss(0, 13, "F1:戻る F4:複数 ", False, CLR_BASE);
			ckputss(0, 15, "        F2:削除 ", False, CLR_BASE);

			ckputsn(5, 3, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
			ckputsn(11, 3, tenpo.tenpo2, sizeof(tenpo.tenpo2), False, CLR_BASE);
		}
		// 件数
		ckprintf(10, 0, False, CLR_ID_TITLE, "%4d件", ctrl.IDataCnt);
		// 上段コード
		if (idata.Code1[0])
		{
			ckputsn(3, 6, idata.Code1, sizeof(idata.Code1), False, CLR_BASE);
			/*// 上段コードの先頭から３桁が501～999の場合
			if( atoin( idata.Code1, 3 ) > 500 && atoin( idata.Code1, 3 ) < 999 ){
				ckputsn( 0, 6, idata.Code1, 3, False, CLR_BASE );
				ckputss( 3, 6, "-", False, CLR_BASE );
				ckputsn( 4, 6, &idata.Code1[3], 4, False, CLR_BASE );
				ckprintf( 11, 6, False, CLR_BASE, "\\%4d", atoln( &idata.Code1[8], 4 ) );
			} else if( !memcmp( idata.Code1, "45", 2 ) || !memcmp( idata.Code1, "49", 2 ) ||
					   !memcmp( idata.Code1, "20", 2 ) ){
				// 上段コードの先頭から２桁が４５、１９、２０の場合
				ckputsn( 0, 6, idata.Code1, sizeof( idata.Code1 ), False, CLR_BASE );
			} else if( !memcmp( idata.Code1, "2289", 4 ) ){
				// 上段コードの先頭から４桁が２２８９の場合
				ckputss( 0, 6, "AT", False, CLR_BASE );
			} else if( !memcmp( idata.Code1, "2299", 4 ) ){
				// 上段コードの先頭から４桁が２２９９の場合
				ckputss( 0, 6, "FJ", False, CLR_BASE );
			}*/
		}
		else if (idata.Code3[0])
		{
			if (idata.Code3[2] == '9')
				ckprintf(3, 6, False, CLR_BASE, "->%2d%OFF", atoin(&idata.Code3[3], 2));
			else
				ckprintf(3, 6, False, CLR_BASE, "->\\%ld", atoln(&idata.Code3[2], 5));
		}
		else
		{
			ckputss(0, 6, "▲              ", False, CLR_BASE);
		}
		break;
	case NUM:
		if (SaveItem != NUM)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<移動入力>      ", False, CLR_ID_TITLE);
			ckputss(0, 3, "店舗:   →　　   ", False, CLR_BASE);
			ckputss(0, 6, "△              ", False, CLR_BASE);
			ckputss(0, 8, "               ", False, CLR_BASE);
			ckputss(0, 11, "数量:           ", False, CLR_BASE);
			ckputss(0, 14, "F1:戻る C :ｸﾘｱ  ", False, CLR_BASE);
			ckputsn(5, 3, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
			ckputsn(11, 3, tenpo.tenpo2, sizeof(tenpo.tenpo2), False, CLR_BASE);
		}
		/* 件数 */
		ckprintf(10, 0, False, CLR_ID_TITLE, "%4d件", ctrl.IDataCnt);
		/* 上段コード */
		if (idata.Code1[0])
		{
			ckputsn(3, 6, idata.Code1, sizeof(idata.Code1), False, CLR_BASE);
		}
		/* 下段コード */
		if (idata.Code2[0])
		{
			ckputsn(3, 8, idata.Code2, sizeof(idata.Code2), False, CLR_BASE);
		}
		/* 数量 */
		if (!idata.Num[0])
		{
			memcpy(idata.Num, "00001", sizeof(idata.Num));
		}
		break;
	case DENTRY:
		if (SaveItem != DENTRY)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<移動入力>      ", False, CLR_ID_TITLE);
			ckputss(0, 3, "店舗:   →　　   ", False, CLR_BASE);
			ckputss(0, 6, "△              ", False, CLR_BASE);
			ckputss(0, 8, "               ", False, CLR_BASE);
			ckputss(0, 11, "数量:           ", False, CLR_BASE);
			ckputss(0, 15, "                ", False, CLR_BASE);
			ckputsn(5, 3, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
			ckputsn(11, 3, tenpo.tenpo2, sizeof(tenpo.tenpo2), False, CLR_BASE);
		}
		/* 件数 */
		ckprintf(10, 0, False, CLR_ID_TITLE, "%4d件", ctrl.IDataCnt);
		/* 上段コード */
		if (idata.Code1[0])
		{
			ckputsn(3, 6, idata.Code1, sizeof(idata.Code1), False, CLR_BASE);
		}
		/* 下段コード */
		if (idata.Code2[0])
		{
			ckputsn(3, 8, idata.Code2, sizeof(idata.Code2), False, CLR_BASE);
		}
		/* 数量 */
		if (!idata.Num[0])
		{
			memcpy(idata.Num, "00001", sizeof(idata.Num));
		}
		ckprintf(5, 11, False, CLR_BASE, "%ld", atoln(idata.Num, sizeof(idata.Num)));
		break;
	case DELETE:
		if (SaveItem != DELETE)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<削除>          ", False, CLR_ID_TITLE);
			ckputss(0, 3, "店舗:   →　　   ", False, CLR_BASE);
			ckputss(0, 6, "△              ", False, CLR_BASE);
			ckputss(0, 8, "               ", False, CLR_BASE);
			ckputss(0, 11, "数量:           ", False, CLR_BASE);
			ckputss(0, 15, "削除?  OK:1 NO:2", False, CLR_BASE);
		}
		ram_read(ctrl.IDataCnt - 1, &idata, IDATAF);
		ckputsn(5, 3, idata.Shop, sizeof(idata.Shop), False, CLR_BASE);
		ckputsn(11, 3, idata.Shop2, sizeof(idata.Shop2), False, CLR_BASE);
		/* 上段コード */
		if (idata.Code1[0])
		{
			ckputsn(3, 6, idata.Code1, 13, False, CLR_BASE);
		}
		/* 下段コード */
		if (idata.Code2[0])
		{
			ckputsn(3, 8, idata.Code2, 13, False, CLR_BASE);
		}
		/* 数量 */
		if (!idata.Num[0])
		{
			memcpy(idata.Num, "00001", sizeof(idata.Num));
		}
		ckprintf(5, 11, False, CLR_BASE, "%ld", atoln(idata.Num, sizeof(idata.Num)));
		break;
	}
	SaveItem = item;
}

/*****************************************************************************/
/* 	明細データ登録 															 */
/*****************************************************************************/
static void entrydata(short sw)
{
	char now[20];
	memcpy(idata.Shop, tenpo.tenpo1, sizeof(idata.Shop));
	memcpy(idata.Shop2, tenpo.tenpo2, sizeof(idata.Shop2));
	if (sw)
	{
		// 新規登録
		getrtc4(now);
		memcpy(idata.Date, now, sizeof(idata.Date)); //+ sizeof(  idata.Time ) );
		ram_write(ctrl.IDataCnt, &idata, IDATAF);
		++ctrl.IDataCnt;
		ram_write(0, &ctrl, CTRLF);
	}
	else
	{
		// 最後に登録したデータを更新
		ram_write(ctrl.IDataCnt - 1, &idata, IDATAF);
	}
	beep(50, 1);
	meisaiclr();
}

/*****************************************************************************/
/* 	移動業務     															 */
/*****************************************************************************/
void Iscan(void)
{
	short item, ret, Func; // Func=1:1点 Func=2:複数　

	memset(&idata, 0x00, sizeof(idata));
	setpict(1);
	item = SHOP;
	Func = 1;
	SaveItem = -1;
	while (item)
	{
		if (MaxCheck(ctrl.IDataCnt, IDATA_MAX))
			return;
		Display(item);
		switch (item)
		{
		case SHOP:
			ret = NumInput(9, 5, tenpo.tenpo1, sizeof(tenpo.tenpo1), 0, 99L,
				       KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK);
			break;
		case SHOP2:
			ret = NumInput(9, 10, tenpo.tenpo2, sizeof(tenpo.tenpo2), 0, 99L,
				       KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK);
			break;
		// case TANTO:
		// ret = NumInput( 9, 5, idata.Tanto, sizeof( idata.Tanto ), 0, 99L,
		//				KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK );
		// break;
		case CODE1:
			ret = CodeInput(3, 6, idata.Code1, sizeof(idata.Code1),
					BCR_NOTDRW | BCR_WPC | KEY_FUNC);
			break;
		case NUM:
			ret = NumInput(5, 11, idata.Num, sizeof(idata.Num), 0, 99999L,
				       IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK);
			break;
		case DENTRY:
			/* 登録 */
			if (!memcmp(idata.Code3, "30", 2))
			{
				entrydata(False);
			}
			else
			{
				entrydata(True);
			}
			item = CODE1;
			Func = 1;
			continue;
		case DELETE:
			ret = getch();
			if (ret == '1')
			{ /* 削除 */
				--ctrl.IDataCnt;
				ram_read(ctrl.IDataCnt, &idata, IDATAF);
				meisaiclr();
				ram_write(0, &ctrl, CTRLF);
				item = CODE1;
			}
			else if (ret == '2')
			{ /* 棚卸 */
				ram_read(ctrl.IDataCnt, &idata, IDATAF);
				meisaiclr();
				item = CODE1;
			}
			Func = 1;
			continue;
		}
		if (ret == ENTRY || ret == SENTRY || ret == SKIP || ret == BARENT)
		{
			/*
			if( item == SHOP ){
				// 店舗マスタ検索
				if( ShopFind( idata.Shop ) == -1 ){
					beep( 10, 2 );
					continue;
				}
				else{
					beepEx( 50, 1 );
				}
			}*/
			if (item == SHOP)
			{
				// コードが01~99でない場合ﾒﾆｭｰへ
				if (atoin(tenpo.tenpo1, 2) >= 1)
				{
					item = SHOP2;
					continue;
				}
				else
				{
					menu();
				}
			}
			else if (item == SHOP2)
			{
				if (atoin(tenpo.tenpo2, 2) >= 1)
				{
					int intChk1 = tenpo.tenpo1[0] - tenpo.tenpo2[0];
					int intChk2 = tenpo.tenpo1[1] - tenpo.tenpo2[1];
					if (intChk1 == 0 && intChk2 == 0)
					{
						item = SHOP;
						memset(tenpo.tenpo2, 0x00, sizeof(tenpo.tenpo2));
						beep(10, 2);
						continue;
					}
					else
					{
						item = CODE1;
						continue;
					}
				}
				else
				{
					menu();
				}
			}

			/*else if( item == TANTO ){
				// 担当マスタ検索
				if( ShopFind( idata.Shop2 ) == -1 ){
					beep( 50, 1 );
					continue;
				}
			}*/
			else if (item == CODE1)
			{

				// ITFの場合
				if ((idata.Code1[0] == '1' || idata.Code1[0] == '2' || idata.Code1[0] == '3') && idata.Code1[27] != ' ')
				{
					/* ITF場合、データ登録へ */
					if (Func == 1)
					{
						item = DENTRY;
					}
					if (Func == 2)
					{
						item = NUM;
					}
					continue;
				}
				// ITFではない
				if (!memcmp(idata.Code1, "45", 2) || !memcmp(idata.Code1, "49", 2) || !memcmp(idata.Code1, "20", 2))
				{

					// 商品存在しない
					if (ItemFind(idata.Code1) == -1)
					{
						beep(10, 2);
						meisaiclr();
						continue;
					}
					// displayStringMsg(tsmst.Name + '\0');
					// 上段コードの先頭から２桁が４５、１９、２０の場合、データ登録へ
					if (Func == 1)
					{
						item = DENTRY;
					}
					if (Func == 2)
					{
						item = NUM;
					}
					continue;
				}
				else if (ctrl.IDataCnt && !memcmp(idata.Code1, "30", 2) && idata.Code1[8] == ' ')
				{
					// 登録済みデータが存在するかつ上段コードの先頭から２桁が３０、かつ８桁の場合
					// 割引率または値下金額を最後の登録データへセットしデータ修正を行う
					memcpy(Code, idata.Code1, sizeof(Code));
					ram_read(ctrl.IDataCnt - 1, &idata, IDATAF);
					memcpy(idata.Code3, Code, sizeof(idata.Code3));
					item = DENTRY;
					continue;
				}
				else
				{
					/* 上記に該当しない場合はエラー */
					beep(10, 2);
					meisaiclr();
					continue;
				}
			}
			else if (item == NUM)
			{
				/* マイナスを含まない場合、入力値が９９９を超えたらエラーとする */
				if (atoln(idata.Num, sizeof(idata.Num)) >= 0 && atoln(idata.Num, sizeof(idata.Num)) > 999)
				{
					beep(50, 1);
					continue;
				}
				/* マイナスを含む場合、入力値が-９９９を超えたらエラーとする */
				if (atoln(idata.Num, sizeof(idata.Num)) < 0 && atoln(idata.Num, sizeof(idata.Num)) <= -1000)
				{
					beep(10, 2);
					continue;
				}
				/* 登録 */
				entrydata(True);
				item = CODE1;
				Func = 1;
				continue;
			}
			++item;
		}
		else if (ret == F2KEY)
		{
			if (ctrl.IDataCnt > 0)
			{
				if (item == CODE1)
				{
					item = DELETE;
				}
			}
			continue;
		}
		else if (ret == F4KEY)
		{
			if (item == CODE1)
			{
				/* 店舗マスタ検索 */
				if (Func == 1)
				{
					Func = 2;
					ckputss(0, 11, "数量:           ", False, CLR_BASE);
					ckputss(0, 13, "F1:戻る F4:1点  ", False, CLR_BASE);
				}
				else if (Func == 2)
				{
					Func = 1;
					ckputss(0, 11, "                ", False, CLR_BASE);
					ckputss(0, 13, "F1:戻る F4:複数 ", False, CLR_BASE);
				}
			}
			continue;
		}
		else if (ret == F1KEY)
		{
			if (item == CODE1)
			{
				memset(tenpo.tenpo1, 0x00, sizeof(tenpo.tenpo1));
				memset(tenpo.tenpo2, 0x00, sizeof(tenpo.tenpo2));
				// item = SHOP;
				// continue;
				menu();
			}
			else if (item == NUM)
			{
				meisaiclr();
				if (item == NUM)
				{
					Func = 1;
					item = CODE1;
					continue;
				}
			}
			--item;
		}
	}
}