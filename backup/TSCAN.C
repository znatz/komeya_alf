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
	CODE1,
	CODE2,
	NUM,
	DENTRY,
	DELETE
};

static short SaveItem;
static char Code[13];

static void meisaiclr(void);
static void Display(short item, short Func);
static void entrydata(short sw);
/*****************************************************************************/
/* 	明細データ初期化														 */
/*****************************************************************************/
static void meisaiclr(void)
{
	memset(tdata.Code1, 0x00, sizeof(tdata.Code1));
	memset(tdata.Code2, 0x00, sizeof(tdata.Code2));
	memset(tdata.Code3, 0x00, sizeof(tdata.Code3));
	memset(tdata.Num, 0x00, sizeof(tdata.Num));
	memset(Code, 0x00, sizeof(Code));
}

/*****************************************************************************/
/* 			     															 */
/*****************************************************************************/
static void Display(short item, short Func)
{
	switch (item)
	{
	case SHOP:
		if (SaveItem != SHOP)
		{
			ClsColor(); /* 0123456789012345*/
			ckputss(0, 0, "<棚卸入力>      ", False, CLR_ZA_TITLE);
			ckputss(0, 5, "    店舗:       ", False, CLR_BASE);
			ckputss(0, 15, "F1:ﾒﾆｭｰ ENT:確定", False, CLR_BASE);
		}
		memset(tenpo.tenpo1, 0x00, sizeof(tenpo.tenpo1)); // 店舗データ取消
		/* 件数 */
		ckprintf(10, 0, False, CLR_ZA_TITLE, "%4d件", ctrl.TDataCnt);
		/* 店舗コード */
		if (tenpo.tenpo1[0])
		{
			ckputsn(9, 5, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
		}
		else
		{
			kputspc(9, 5, 2, CLR_BASE);
			kputspc(2, 7, 10, CLR_BASE);
		}
		break;
	case CODE1:
		if (SaveItem != CODE1)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<棚卸入力>      ", False, CLR_ZA_TITLE);
			ckputss(0, 3, "店舗:           ", False, CLR_BASE);
			ckputss(0, 6, "▲              ", False, CLR_BASE);
			ckputss(0, 8, "▽              ", False, CLR_BASE);
			ckputss(0, 11, "                ", False, CLR_BASE);
			ckputss(0, 13, "F1:戻る F4:複数 ", False, CLR_BASE);
			ckputss(0, 15, "        F2:削除 ", False, CLR_BASE);

			ckputsn(5, 3, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
		}
		/* 件数 */
		ckprintf(10, 0, False, CLR_ZA_TITLE, "%4d件", ctrl.TDataCnt);
		/* 上段コード */
		if (tdata.Code1[0])
		{
			ckputsn(3, 6, tdata.Code1, 13, False, CLR_BASE);
		}
		else
		{
			ckputss(0, 6, "▲              ", False, CLR_BASE);
		}
		break;
	case CODE2:
		if (SaveItem != CODE2)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<棚卸入力>      ", False, CLR_ZA_TITLE);
			ckputss(0, 3, "店舗:           ", False, CLR_BASE);
			ckputss(0, 6, "△              ", False, CLR_BASE);
			ckputss(0, 8, "▼              ", False, CLR_BASE);
			ckputss(0, 11, "                ", False, CLR_BASE);
			ckputss(0, 13, "F1:戻る         ", False, CLR_BASE);
			ckputss(0, 15, "                ", False, CLR_BASE);

			ckputsn(5, 3, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
		}
		/* 件数 */
		ckprintf(10, 0, False, CLR_ZA_TITLE, "%4d件", ctrl.TDataCnt);
		/* 上段コード */
		if (tdata.Code1[0])
		{
			ckputsn(3, 6, tdata.Code1, 13, False, CLR_BASE);
		}
		else
		{
			ckputss(0, 6, "△              ", False, CLR_BASE);
		}
		/* 下段コード */
		if (tdata.Code2[0])
		{
			ckputsn(3, 8, tdata.Code2, 13, False, CLR_BASE);
		}
		else
		{
			ckputss(0, 8, "▼              ", False, CLR_BASE);
		}
		break;
	case NUM:
		if (SaveItem != NUM)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<棚卸入力>      ", False, CLR_ZA_TITLE);
			ckputss(0, 3, "店舗:           ", False, CLR_BASE);
			ckputss(0, 6, "△              ", False, CLR_BASE);
			ckputss(0, 8, "▽              ", False, CLR_BASE);
			ckputss(0, 11, "数量:           ", False, CLR_BASE);
			ckputss(0, 13, "F1:戻る C :ｸﾘｱ  ", False, CLR_BASE);
			ckputss(0, 15, "                ", False, CLR_BASE);

			ckputsn(5, 3, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
		}
		/* 件数 */
		ckprintf(10, 0, False, CLR_ZA_TITLE, "%4d件", ctrl.TDataCnt);
		/* 上段コード */
		if (tdata.Code1[0])
		{
			ckputsn(3, 6, tdata.Code1, 13, False, CLR_BASE);
		}
		else
		{
			ckputss(0, 6, "△             ", False, CLR_BASE);
		}
		/* 下段コード */
		if (tdata.Code2[0])
		{
			ckputsn(3, 8, tdata.Code2, 13, False, CLR_BASE);
		}
		else
		{
			ckputss(0, 8, "▽             ", False, CLR_BASE);
		}
		/* 数量 */
		if (!tdata.Num[0])
		{
			memcpy(tdata.Num, "00001", sizeof(tdata.Num));
		}
		break;
	case DENTRY:
		if (SaveItem != DENTRY)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<棚卸入力>      ", False, CLR_ZA_TITLE);
			ckputss(0, 3, "店舗:           ", False, CLR_BASE);
			ckputss(0, 6, "△              ", False, CLR_BASE);
			ckputss(0, 8, "▽              ", False, CLR_BASE);
			ckputss(0, 11, "数量:           ", False, CLR_BASE);
			ckputss(0, 15, "                ", False, CLR_BASE);

			ckputsn(5, 3, tenpo.tenpo1, sizeof(tenpo.tenpo1), False, CLR_BASE);
		}
		/* 件数 */
		ckprintf(10, 0, False, CLR_ZA_TITLE, "%4d件", ctrl.TDataCnt);
		/* 上段コード */
		if (tdata.Code1[0])
		{
			ckputsn(3, 6, tdata.Code1, 13, False, CLR_BASE);
		}
		/* 下段コード */
		if (tdata.Code2[0])
		{
			ckputsn(3, 8, tdata.Code2, 13, False, CLR_BASE);
		}
		/* 数量 */
		if (!tdata.Num[0])
		{
			memcpy(tdata.Num, "00001", sizeof(tdata.Num));
		}
		ckprintf(5, 11, False, CLR_BASE, "%ld", atoln(tdata.Num, sizeof(tdata.Num)));
		break;
	case DELETE:
		if (SaveItem != DELETE)
		{
			ClsColor(); /*   0123456789012345*/
			ckputss(0, 0, "<削除>          ", False, CLR_ZA_TITLE);
			ckputss(0, 3, "店舗:           ", False, CLR_BASE);
			ckputss(0, 6, "△              ", False, CLR_BASE);
			ckputss(0, 8, "▽              ", False, CLR_BASE);
			ckputss(0, 11, "数量:           ", False, CLR_BASE);
			ckputss(0, 15, "削除?  OK:1 NO:2", False, CLR_BASE);
		}

		ram_read(ctrl.TDataCnt - 1, &tdata, TDATAF);
		ckputsn(5, 3, tdata.Shop, sizeof(tdata.Shop), False, CLR_BASE);
		/* 上段コード */
		if (tdata.Code1[0])
		{
			ckputsn(3, 6, tdata.Code1, 13, False, CLR_BASE);
		}
		/* 下段コード */
		if (tdata.Code2[0])
		{
			ckputsn(3, 8, tdata.Code2, 13, False, CLR_BASE);
		}
		/* 数量 */
		if (!tdata.Num[0])
		{
			memcpy(tdata.Num, "00001", sizeof(tdata.Num));
		}
		ckprintf(5, 11, False, CLR_BASE, "%ld", atoln(tdata.Num, sizeof(tdata.Num)));
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
	memcpy(tdata.Shop, tenpo.tenpo1, sizeof(tdata.Shop));
	if (sw)
	{
		/* 新規登録 */
		getrtc4(now);
		memcpy(tdata.Date, now, sizeof(tdata.Date));
		//+ sizeof(  tdata.Time )
		ram_write(ctrl.TDataCnt, &tdata, TDATAF);
		++ctrl.TDataCnt;
		ram_write(0, &ctrl, CTRLF);
	}
	else
	{
		// 最後に登録したデータを更新
		ram_write(ctrl.TDataCnt - 1, &tdata, TDATAF);
	}
	beep(50, 1);
	meisaiclr();
}

/*****************************************************************************/
/* 	棚卸業務     							     */
/*****************************************************************************/
void Tscan(void)
{
	short item, ret, Func; // Func=1:1点 Func=2:複数　

	memset(&tdata, 0x00, sizeof(tdata));
	setpict(1);
	item = SHOP;
	Func = 1;
	SaveItem = -1;
	while (item)
	{
		if (MaxCheck(ctrl.TDataCnt, TDATA_MAX))
			return;
		Display(item, Func);
		switch (item)
		{
		case SHOP:
			ret = NumInput(9, 5, tenpo.tenpo1, sizeof(tenpo.tenpo1), 0, 99L,
				       KEY_FIX | IN_NUMERIC | KEY_FUNC, TYPE_CHAR, NO_CHECK);
			break;
		case CODE1:
			ret = CodeInput(3, 6, tdata.Code1, sizeof(tdata.Code1),
					BCR_NOTDRW | BCR_WPC | KEY_FUNC);
			break;
		case CODE2:
			ret = CodeInput(3, 8, tdata.Code2, sizeof(tdata.Code2),
					BCR_NOTDRW | BCR_WPC | KEY_FUNC);
			break;
		case NUM:
			ret = NumInput(5, 11, tdata.Num, sizeof(tdata.Num), 0, 99999L,
				       IN_NUMERIC | KEY_MINUS | KEY_FUNC, TYPE_NUM, NO_CHECK);
			break;
		case DENTRY:
			/* 登録 */
			if (!memcmp(tdata.Code3, "30", 2))
			{
				entrydata(False);
			}
			else
			{
				entrydata(True);
			}
			Func = 1;
			item = CODE1;
			continue;
		case DELETE:
			ret = getch();
			if (ret == '1')
			{ /* 削除 */
				--ctrl.TDataCnt;
				ram_read(ctrl.TDataCnt, &tdata, TDATAF);
				meisaiclr();
				ram_write(0, &ctrl, CTRLF);
				item = CODE1;
			}
			else if (ret == '2')
			{ /* 棚卸 */
				ram_read(ctrl.TDataCnt, &tdata, TDATAF);
				meisaiclr();
				item = CODE1;
			}
			Func = 1;
			continue;
		}
		if (ret == ENTRY || ret == SENTRY || ret == SKIP || ret == BARENT)
		{
			if (item == SHOP)
			{
				// コードが01~99でない場合ﾒﾆｭｰへ
				if (atoin(tenpo.tenpo1, 2) >= 1)
				{
					item = CODE1;
					continue;
				}
				else
				{
					menu();
				}
			}
			else if (item == CODE1)
			{
				// ITFの場合
				if ((tdata.Code1[0] == '1' || tdata.Code1[0] == '2' || tdata.Code1[0] == '3') && tdata.Code1[27] != ' ')
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
				// 3桁が501～999の場合
				if (atoin(tdata.Code1, 3) > 500 && atoin(tdata.Code1, 3) < 999)
				{
					item = CODE2;
					continue;
				}
				else if (!memcmp(tdata.Code1, "45", 2) || !memcmp(tdata.Code1, "49", 2) ||
					 !memcmp(tdata.Code1, "20", 2))
				{
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
				else if (!memcmp(tdata.Code1, "2289", 4) || !memcmp(tdata.Code1, "2299", 4))
				{
					item = CODE2;
					continue;
				}
				else if (ctrl.TDataCnt && !memcmp(tdata.Code1, "30", 2) && tdata.Code1[8] == ' ')
				{
					/* 登録済みデータが存在するかつ上段コードの先頭から２桁が３０、かつ８桁の場合 */
					/* 割引率または値下金額を最後の登録データへセットしデータ修正を行う			*/
					memcpy(Code, tdata.Code1, sizeof(Code));
					ram_read(ctrl.TDataCnt - 1, &tdata, TDATAF);
					memcpy(tdata.Code3, Code, sizeof(tdata.Code3));
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
			else if (item == CODE2)
			{
				// ITFの場合
				if ((tdata.Code1[0] == '1' || tdata.Code1[0] == '2' || tdata.Code1[0] == '3') && tdata.Code1[27] != ' ')
				{
					/* 上記に該当しない場合はエラー */
					memset(tdata.Code2, 0x00, sizeof(tdata.Code2));
					beep(10, 2);
					continue;
				}
				// ITFではない
				/* 上段コードの先頭から３桁が501～999の場合、0999、1、3から始まるコード以外不可 */
				if (atoin(tdata.Code1, 3) > 500 && atoin(tdata.Code1, 3) < 999)
				{
					if ((tdata.Code2[0] == '0' || tdata.Code2[0] == '1' || tdata.Code2[0] == '2' || tdata.Code2[0] == '3') && tdata.Code2[12] != ' ')
					{
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
					else
					{
						/* 上記に該当しない場合はエラー */
						memset(tdata.Code2, 0x00, sizeof(tdata.Code2));
						beep(10, 2);
						continue;
					}
				}
				else if (!memcmp(tdata.Code1, "2289", 4) || !memcmp(tdata.Code1, "2299", 4))
				{
					if (!memcmp(tdata.Code2, "2500", 4))
					{
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
					else
					{
						/* 上記に該当しない場合はエラー */
						memset(tdata.Code2, 0x00, sizeof(tdata.Code2));
						beep(10, 2);
						continue;
					}
				}
				else
				{
					/* 上記に該当しない場合はエラー */
					memset(tdata.Code2, 0x00, sizeof(tdata.Code2));
					beep(10, 2);
					continue;
				}
			}
			else if (item == NUM)
			{
				/* マイナスを含まない場合、入力値が９９９を超えたらエラーとする */
				if (atoln(tdata.Num, sizeof(tdata.Num)) >= 0 && atoln(tdata.Num, sizeof(tdata.Num)) > 999)
				{
					beep(10, 2);
					continue;
				}
				/* マイナスを含む場合、入力値が-９９９を超えたらエラーとする */
				if (atoln(tdata.Num, sizeof(tdata.Num)) < 0 && atoln(tdata.Num, sizeof(tdata.Num)) <= -1000)
				{
					beep(10, 2);
					continue;
				}
				/* 登録 */
				// entrydata( False );
				// item = CODE1;
				item = DENTRY;
				Func = 1;
				continue;
			}
			else if (item == DELETE)
			{
				continue;
			}
			++item;
		}
		else if (ret == F2KEY)
		{
			if (ctrl.TDataCnt > 0)
			{
				if (item == CODE1)
				{
					item = DELETE;
				}
				continue;
			}
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
			if (item == CODE2 || item == NUM)
			{
				meisaiclr();
				Func = 1;
				item = CODE1;
				continue;
			}
			menu();
		}
	}
}