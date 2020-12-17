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
// *    NEX-M230 ����
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
 * �̎������
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

// 	char title[24] =  "     ���� �� �� �� ����\r";    	// |
// 	char kyaku[29] =  "                        �l  \r";	// |
// 	char aline[29] =  "----------------------------\r";	// |
// 	char empty[29] =  "                            \r";	// |

// 	insComma( infour2.Goukei , strKingaku );
// 	char total[29];
// 	sprintf(&total[ 0 ], "             ��%7s-   ", strKingaku );


// 	progress_start(LCD_PROGRESS_USE_ANIMATION, "�����");
// 	progress_update(NULL, NULL, 0);

// 	//if( Flag == 0 ){//#01 ���v�\
// 	ret = LapinBTOpen(addr, crc, mode);
// 	if (ret != LAPIN_OK) {
// 		goto end;
// 	}
// 	//}

// 	// �������R�}���h�̓v�����^�R�}���h�\�����ɓK�X�ݒ肷�邱�ƁB
// 	if ( mode == COMMAND_MODE_SBPL ) {

// 		memset(buf, 0x00, sizeof(buf));
// 		len = 0;
// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// �f�[�^���o�J�n
// 		memcpy( &buf[ len ], "A", 1 );					len += 1;

// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// �v�����^����ݒ�
// 		memcpy( &buf[ len ], "PG", 2);					len += 2;
// 		memcpy( &buf[ len ], "3", 1);					len += 1;	// �󎚑��x 50mm/sec
// 		memcpy( &buf[ len ], "2", 1);					len += 1;	// �󎚔Z�x���x�� ����
// 		memcpy( &buf[ len ], "A", 1);					len += 1;	// �󎚔Z�x A
// 		memcpy( &buf[ len ], "3", 1);					len += 1;	// �o�b�N�t�B�[�h����
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// �[���X���b�V������
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// �����R�[�h �V�t�gJIS
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// �����t�B�[�h����
// 		memcpy( &buf[ len ], "0", 1);					len += 1;	// �v���|�[�V���i���s�b�`����
// 		memcpy( &buf[ len ], "2400", 4);				len += 4;	// �c���x���T�C�Y
// 		memcpy( &buf[ len ], "0832", 4);				len += 4;	// �����x���T�C�Y
// 		memcpy( &buf[ len ], "+000", 4);				len += 4;	// �c��_�␳�l
// 		memcpy( &buf[ len ], "+000", 4);				len += 4;	// ����_�␳�l
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// �p���s�b�`�I�t�Z�b�g
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// �e�B�A�I�t�I�t�Z�b�g
// 		memcpy( &buf[ len ], "+00", 3);					len += 3;	// �n�N���I�t�Z�b�g
// 		memcpy( &buf[ len ], "000", 3);					len += 3;	// �I�[�g�p���[�I�t����
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// �W���[�i�����s���[�h
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// ���샂�[�h �A��
// 		memcpy( &buf[ len ], "1", 1);					len += 1;	// �Z���T��w�� ���ߎ��Z���T

// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	// �f�[�^���o�I��
// 		memcpy( &buf[ len ], "Z", 1 );					len += 1;

// 	} else {
// 		/*�v�����^�ݒ�*/
// 		memset(buf, 0x00, sizeof(buf));
// 		/*�f�[�^���M�J�n�w��*/
// 		len = 0;
// 		memcpy( &buf[ len ], "\x1B", 1 );				len += 1;	/* �f�[�^���o�J�n				*/
// 		memcpy( &buf[ len ], "A", 1 );					len += 1;
// 		/*�������s�ݒ�*/
// 		/*�����t�B�[���h�w��*/
// 		buf[len++] = 0x1b;
// 		//abcdefghiiiijjjjkkkkllllmmmnnnooo
// 		memcpy( &buf[len], "PG43A3010012800384+000+000+00+00+00", 35 );
// 		len += 35;
// 		memcpy( &buf[len], "000", 3);
// 		len += 3;
// 		memcpy( &buf[len], "111", 3 );					//	�W���[�i�����s���[�h
// 		len += 3;

// 		/*�f�[�^���M�I���w��*/
// 		buf[len++] = 0x1b;
// 		buf[len++] = 'Z';
// 	}

// 	// �������R�}���h���M

// 	ret = LapinBTSend(buf, len);
// 	if (ret != LAPIN_OK) {
// 		goto end;
// 	}


// 	// �󎚃R�}���h�͈󎚓��e�ɂ���ēK�X�ݒ肷�邱��
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

// 		//	<< �̎��� >>
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


// 		// 	�l
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


// 	// �f�[�^���o�I��
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
// 			sprintf(buf, "%s", "������̓d�����m�F���ĉ�����" );
// 		} else if ( ret == -24 ) {
// 			sprintf(buf, "%s\n%s", "�p���`�F�b�N", "������̍ċN��" );
// 		} else if ( ret == -21 ) {
// 			sprintf(buf, "%s", "������̶�ް���m�F���ĉ�����" );
// 		} else {
// 			sprintf(buf, "[%d]", ret);
// 		}
// 		intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "�G���[\n", buf, "�Ĉ��", "�I��");
// 	} else {
// 		ret = LapinBTDisconnect();
// 		progress_stop();

// 		if (ret != LAPIN_OK) {
// 			memset(buf, 0x00, sizeof(buf));
// 			sprintf(buf, "Disconnect %d", ret);
// 			intMsg = msgbox(LCD_MSGBOX_ERROR, 0, "�G���[", buf, "�Ĉ��", "�I��");
// 		} else {
// 			msgbox(LCD_MSGBOX_INFORMATION, 0, "����I��", NULL, NULL, NULL);
// 			intMsg = 1;
// 		}
// 	}
// 	return intMsg;
return 0;
}
