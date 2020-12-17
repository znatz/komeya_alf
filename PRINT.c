#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ark7000x.h"
#include "ctrl.h"
#include "input.h"
#include "ram_io.h"
#include "sub.h"
#include "worksub.h"
#include "CTRL.H"
#include "SUB.H"
#include "PRINT.h"
#include <Math.h>

// * ---------------------------
// *    NEX-M230 命令
// * --------------------------- 

// * Romaji
const char bRomaji[2] = {0x1B, 0x40};

// * Quadruple Japanese Kanji
const char bJPx4Release[3] = {0x1C, 0x57, 0x0};
const char bJPx4Set[3] = {0x1C, 0x57, 0x1};

// * Japanese Kanji
const char bJP[5] = {0x1C, 0x26, 0x1C, 0x43, 0x1};
const char bDoulbeStrike[3] = {0x1B, 0x47, 0x1};
const char bNonDoulbeStrik[3] = {0x1B, 0x47, 0x0};

// * Japanese Release
const char bJPRelease[2] = {0x1C, 0x2E};

// *  Alignment
const char bAlignLeft[3] = {0x1B, 0x61, 0x0};
const char bAlignCenter[3] = {0x1B, 0x61, 0x1};
const char bAlignRight[3] = {0x1B, 0x61, 0x2};

// *  Size
// const char  bSizeNor[] = {0x1D, 0x21, Convert.ToInt32("00000000", 2)};
// * Normal Size
const char  bSizeNor[3] = {0x1D, 0x21, 0};

// const char  bSizeDbl[] = {0x1D, 0x21, Convert.ToInt32("00010000", 2)};
// * Double Width
const char  bSizeDbl[3] = {0x1D, 0x21, 0x10};

// const char  bSizeHDb[] = {0x1D, 0x21, Convert.ToInt32("00000001", 2)};
// * Double Height
const char  bSizeHDb[3] = {0x1D, 0x21, 0x1};

// const char  bSizeTri[] = {0x1D, 0x21, Convert.ToInt32("00001010", 2)};
// * Double Width and Height
const char  bSizeTri[3] = {0x1D, 0x21, 0x11};

// *  Feed 4 lines
const char  nFeedLine[3] = {0x1B, 0x64, 0x4};

// *  Cut Paper
const char  bCut[2] = {0x1B, 0x69};

// *  Underline
const char  bUnderline0[3] = {0x1C, 0x2D, 0x0};
const char  bUnderline1[3] = {0x1C, 0x2D, 0x1};
const char  bUnderline2[3] = {0x1C, 0x2D, 0x2};

// *  Register Image
const char  startRegisterImage0[3] = {0x1D, 0x54, 0x0};
const char  startRegisterImage1[3] = {0x1D, 0x54, 0x1};
const char  startRegisterImage2[3] = {0x1D, 0x54, 0x2};
const char  finishRegisterImage[3] = {0x1D, 0x54, 0xFF};

// *  Print Register Image
const char  printRegisterImage0[3] = {0x1D, 0x50, 0x0};
const char  printRegisterImage1[3] = {0x1D, 0x50, 0x1};
const char  printRegisterImage2[3] = {0x1D, 0x50, 0x2};

// *  HRI Character
const char  hriNon[3] = {0x1D, 0x48, 0x0};
const char  hriAbove[3] = {0x1D, 0x48, 0x1};
const char  hriBelow[3] = {0x1D, 0x48, 0x2};
const char  hriBoth[3] = {0x1D, 0x48, 0x3};

// *  Print Barcode
const char  printJAN13[3] = {0x1D, 0x6B, 0x2};


    // Public Sub bufferRegisterImage(ByRef filename As String, ByVal number As Int32)

    //         Dim bBufWork As [Byte]() = New [Byte]() {}
    //         Dim bmp As Bitmap = New Bitmap(filename)

    //         Dim n1 As Byte = (bmp.Width + (8 - (bmp.Width Mod 8))) / 8
    //         Dim n2 As Byte = bmp.Height

    //         Select Case number
    //             Case 0
    //                 Commands.startRegisterImage0.CopyTo(bufferToPrint, currentLength)
    //                 currentLength = currentLength + Commands.startRegisterImage0.Length
    //             Case 1
    //                 Commands.startRegisterImage1.CopyTo(bufferToPrint, currentLength)
    //                 currentLength = currentLength + Commands.startRegisterImage0.Length
    //             Case 2
    //                 Commands.startRegisterImage2.CopyTo(bufferToPrint, currentLength)
    //                 currentLength = currentLength + Commands.startRegisterImage0.Length
    //         End Select

    //         bBufWork = New Byte() {&H1B, &H62, n1, n2, &H0}
    //         bBufWork.CopyTo(bufferToPrint, currentLength)
    //         currentLength = currentLength + bBufWork.Length

    //         For y As Integer = 0 To bmp.Height - 1
    //             Dim line(bmp.Width - 1) As Char
    //             For x As Integer = 0 To bmp.Width - 1
    //                 If bmp.GetPixel(x, y).R <> &H0 Then
    //                     line(x) = "0"
    //                 Else
    //                     line(x) = "1"
    //                 End If
    //             Next

    //             Dim every8bit() As String = SplitString(padLineTo8Bit(line), 8)

    //             For Each eightBit In every8bit
    //                 bBufWork = New Byte() {Convert.ToInt32(eightBit, 2)}
    //                 bBufWork.CopyTo(bufferToPrint, currentLength)
    //                 currentLength = currentLength + bBufWork.Length
    //             Next
    //         Next

    //         bBufWork = New Byte() {&H1B, &H4A, &H0}
    //         bBufWork.CopyTo(bufferToPrint, currentLength)
    //         currentLength = currentLength + bBufWork.Length

    //         Commands.finishRegisterImage.CopyTo(bufferToPrint, currentLength)
    //         currentLength = currentLength + Commands.finishRegisterImage.Length
    //     End Sub



/********************************************************
 * 領収書印刷
 ********************************************************/
int PrintRyousyu(char* addr, bool crc, int mode , short Flag )
{
// 	int ret;
// 	char buf [2048];
// 	short len;
// 	int i;
// 	int intMsg;
// 	char now[14], Year[4], Month[2], Day[2];
// 	char Hour[2], Min[2];
// 	char Name[20];
// 	char RecNo[6];
// 	char strKingaku[MAXLEN];
// 	long lngNum = 0, lngBaika = 0, lngTensuu = 0;
// 	long lngKingaku = 0 , lngZei = 0 , lngZeiGoukei = 0;
// 	double dblZei = 0 , dblTaxRate = 0 , dblNum = 0 , dblNo = 0 ;

// 	long adrs;

// 	char title[24] =  "     ＜＜ 領 収 書 ＞＞\r";    	// |
// 	char kyaku[29] =  "                        様  \r";	// |
// 	char aline[29] =  "----------------------------\r";	// |
// 	char empty[29] =  "                            \r";	// |

// 	insComma( infour2.Goukei , strKingaku );
// 	char total[29];
// 	sprintf(&total[ 0 ], "             ￥%7s-   ", strKingaku );


// 	progress_start(LCD_PROGRESS_USE_ANIMATION, "印刷中");
// 	progress_update(NULL, NULL, 0);

// 	//if( Flag == 0 ){//#01 日計表
// 	ret = LapinBTOpen(addr, crc, mode);
// 	if (ret != LAPIN_OK) {
// 		goto end;
// 	}
// 	//}

// 	// 初期化コマンドはプリンタコマンド表を元に適宜設定すること。
// 	if ( mode == COMMAND_MODE_SBPL ) {

// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// データ送出開始
// 		memcpy( &buf[ len ], "A", 1 );					len += 1;

// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// プリンタ動作設定
// 		memcpy( &buf[ len ], "PG", 2);					len += 2;
// 		memcpy( &buf[ len ], "3", 1);					len += 1;	// 印字速度 50mm/sec
// 		memcpy( &buf[ len ], "2", 1);					len += 1;	// 印字濃度レベル 中薄
// 		memcpy( &buf[ len ], "A", 1);					len += 1;	// 印字濃度 A
// 		memcpy( &buf[ len ], "3", 1);					len += 1;	// バックフィード無し
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// ゼロスラッシュ無効
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// 漢字コード シフトJIS
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// 初期フィード無効
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// プロポーショナルピッチ解除
// 		memcpy( &buf[ len ], "2400", 4);				len += 4;	// 縦ラベルサイズ
// 		memcpy( &buf[ len ], "0832", 4);				len += 4;	// 横ラベルサイズ
// 		memcpy( &buf[ len ], "+000", 4);				len += 4;	// 縦基点補正値
// 		memcpy( &buf[ len ], "+000", 4);				len += 4;	// 横基点補正値
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// 用紙ピッチオフセット
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// ティアオフオフセット
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// ハクリオフセット
// 		memcpy( &buf[ len ], "000", 3);					len += 3;	// オートパワーオフ無し
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// ジャーナル発行モード
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// 動作モード 連続
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// センサ種指定 透過式センサ

// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// データ送出終了
// 		memcpy( &buf[ len ], "Z", 1 );					len += 1;

// 	} else {
// 		/*プリンタ設定*/
// 		memset(buf, 0x00, sizeof(buf));
// 		/*データ送信開始指定*/
// 		len = 0;
// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	/* データ送出開始				*/
// 		memcpy( &buf[ len ], "A", 1 );					len += 1;
// 		/*自動改行設定*/
// 		/*文字フィールド指定*/
// 		buf[len++] = 0x1b;
// 		//abcdefghiiiijjjjkkkkllllmmmnnnooo
// 		memcpy( &buf[len], "PG43A3010012800384+000+000+00+00+00", 35 );
// 		len += 35;
// 		memcpy( &buf[len], "000", 3);
// 		len += 3;
// 		memcpy( &buf[len], "111", 3 );					//	ジャーナル発行モード
// 		len += 3;

// 		/*データ送信終了指定*/
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';
// 	}

// 	// 初期化コマンド送信

// 	ret = LapinBTSend(buf, len);
// 	if (ret != LAPIN_OK) {
// 		goto end;
// 	}


// 	// 印字コマンドは印字内容によって適宜設定すること
// 	if (mode == COMMAND_MODE_SBPL) {
// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'A';
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E2", 2 );					len += 2;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "K9B", 3 );				len += 3;

// 	} else {
// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'A';
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E001", 4 );				len += 4;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E2", 2 );					len += 2;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "L0001000108111000", 17 );	len += 17;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'D';
// 	}


// 	if ( Flag == 0 ) {

// 		//	<< 領収書 >>
// 		ret = PrintDoubleHeight(title, 24);


// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'A';
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E2", 2 );					len += 2;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "K9B", 3 );				len += 3;

// 		memcpy( &buf[ len ], "\r", 1 ); len += 1;
// 		memcpy( &buf[ len ], "\r", 1 ); len += 1;

// 		getrtc4( now );
// 		memcpy( Year,  &now[0], 4 );
// 		memcpy( Month, &now[4], 2 );
// 		memcpy( Day,   &now[6], 2 );
// 		memcpy( Hour,  &now[8], 2 );
// 		memcpy( Min,   &now[10], 2 );

// 		// 	Year/Month/Day Hour:Min Tenpo-Reji-Tanto
// 		sprintf(&buf[len], "%s", Year ); len += sizeof( Year );
// 		memcpy( &buf[ len ], "/", 1 ); len += 1;
// 		sprintf(&buf[len], "%s", Month); len += sizeof( Month );
// 		memcpy( &buf[ len ], "/", 1 ); len += 1;
// 		sprintf(&buf[len], "%s", Day  ); len += sizeof( Day  );
// 		memcpy( &buf[ len ], " ", 1 ); len += 1;
// 		sprintf(&buf[len], "%s", Hour ); len += sizeof( Hour );
// 		memcpy( &buf[ len ], ":", 1 ); len += 1;
// 		sprintf(&buf[len], "%s", Min  ); len += sizeof( Min  );

// 		memcpy( &buf[ len ], "  ", 2 ); len += 2;

// 		sprintf(&buf[ len ], "%s", info.tenpo1 ); len += sizeof( info.tenpo1 );
// 		memcpy( &buf[ len ], "-", 1 ); len += 1;
// 		sprintf(&buf[ len ], "%s", ctrl.RejiNo ); len += sizeof( ctrl.RejiNo );
// 		memcpy( &buf[ len ], "-", 1 ); len += 1;
// 		sprintf(&buf[ len ], "%s", info.tanto ); len += sizeof( info.tanto );


// 		memcpy( &buf[ len ], "\r", 1 ); len += 1;
// 		memcpy( &buf[ len ], "\r", 1 ); len += 1;

// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';
// 		ret = LapinBTSend(buf, len);


// 		// 	様
// 		ret = PrintNormal(empty, 29);
// 		ret = PrintDoubleHeight(kyaku, 29);
// 		ret = PrintDoubleHeight(aline, 29);


// 		ret = PrintDoubleHeight(total, 29);


// 		ret = PrintDoubleHeight(aline, 29);




// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'A';
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "PC0", 3 );				len += 3;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "E2", 2 );					len += 2;
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "K9B", 3 );				len += 3;


// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;
// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;

// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;
// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;
// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;
// 		memcpy( &buf[ len ], " \r", 2 ); len += 2;

// 	}

// 	buf[len++] = 0x1b;
// 	memcpy( &buf[ len ], "RF010002", 4 );
// 	len += 4;


// 	// データ送出終了
// 	if (mode == COMMAND_MODE_SBPL) {
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';

// 		ret = LapinBTSend(buf, len);
// 		if (ret != LAPIN_OK) {
// 			goto end;
// 		}
// 	} else {
// 		buf[len++] = 0x1b;
// 		memcpy( &buf[ len ], "Q001", 4 );					len += 4;
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';

// 		ret = LapinBTSend(buf, len);
// 		if (ret != LAPIN_OK) {
// 			goto end;
// 		}
// 	}

// // ERROR HANDLING
// end:
// 	if (ret != LAPIN_OK) {
// 		LapinBTDisconnect();
// 		progress_stop();

// 		memset(buf, 0x00, sizeof(buf));
// 		if ( ret == -104) {
// 			sprintf(buf, "%s", "ﾌﾟﾘﾝﾀの電源を確認して下さい" );
// 		} else if ( ret == -24 ) {
// 			sprintf(buf, "%s\n%s", "用紙チェック", "ﾌﾟﾘﾝﾀの再起動" );
// 		} else if ( ret == -21 ) {
// 			sprintf(buf, "%s", "ﾌﾟﾘﾝﾀのｶﾊﾞｰを確認して下さい" );
// 		} else {
// 			sprintf(buf, "[%d]", ret);
// 		}
// 		intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "エラー\n", buf, "再印刷", "終了");
// 	} else {
// 		ret = LapinBTDisconnect();
// 		progress_stop();

// 		if (ret != LAPIN_OK) {
// 			memset(buf, 0x00, sizeof(buf));
// 			sprintf(buf, "Disconnect %d", ret);
// 			intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "エラー", buf, "再印刷", "終了");
// 		} else {
// 			msgbox(LCD_MSGBOX_INFORMATION, 0, "正常終了", NULL, NULL, NULL);
// 			intMsg = 1;
// 		}
// 	}
// 	return intMsg;
return 0;
}
