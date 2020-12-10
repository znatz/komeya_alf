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

// * Japanese Kanji
const char bJP[5] = {0x1C, 0x26, 0x1C, 0x43, 0x1};
const char bDoulbeStrike[3] = {0x1B, 0x47, 0x1};
const char bNonDoulbeStrik[3] = {0x1B, 0x47, 0x0};

// *  Alignment
const char bAlignLeft[3] = {0x1B, 0x61, 0x0};
const char bAlignCenter[3] = {0x1B, 0x61, 0x1};
const char bAlignRight[3] = {0x1B, 0x61, 0x2};

// *  Size
// const char  bSizeNor[] = {0x1D, 0x21, Convert.ToInt32("00000000", 2)};
const char  bSizeNor[3] = {0x1D, 0x21, 0};

// const char  bSizeDbl[] = {0x1D, 0x21, Convert.ToInt32("00010001", 2)};
const char  bSizeDbl[3] = {0x1D, 0x21, 17};

// const char  bSizeTri[] = {0x1D, 0x21, Convert.ToInt32("00001010", 2)};
const char  bSizeTri[3] = {0x1D, 0x21, 10};

// const char  bSizeHDb[] = {0x1D, 0x21, Convert.ToInt32("00000001", 2)};
const char  bSizeHDb[3] = {0x1D, 0x21, 1};

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

int	PrintMain_NEX_M230(char* _addr, bool _crc, int _mode , short _Flag ) {
	bt_conf_t conf;
	int ret;
	int retry_count;
	unsigned char addr[13];
	unsigned char name[41];
	char buf[256];//�]���p�o�b�t�@
	//�e��X�e�[�^�X�𒲂ׂ邽�߂̃R�}���h(DLE EOT n)
	char printer_status[] = { 0x10, 0x1b, 0x76, 0x01 };

	progress_start(LCD_PROGRESS_USE_ANIMATION, "�����");
	progress_update(NULL, NULL, 0);
	
restart:
	ret = BtConfig(BT_GET, &conf);
	if (ret != 0) {
		sprintf(buf, "BtConfig %d", ret);
		goto exit;
	}
	//PIN���ސݒ�
	strncpy(conf.localPIN, "0000000000000000", sizeof(conf.localPIN));
	conf.timeout = 30;
	conf.MITM_Enable = DISABLE;
	
	ret = BtConfig(BT_SET, &conf);
	if (ret != 0) {
		sprintf(buf, "BtConfig %d", ret);
		goto exit;
	}
	
	// * ---------------------------------------------------- 
	// * 	�ڑ��e�X�g�̏ꍇ�A���L�̌܍s��L������
	// * ---------------------------------------------------- 
	// ret = EzBtSelect(EZ_BT_MODE_MENU, addr, name);
	// if (ret != 0) {
	// 	sprintf(buf, "EzBtSelect %d", ret);
	// 	goto exit;
	// }

	// * ---------------------------------------------------- 
	// * 	�ڑ��e�X�g�ł͂Ȃ��ꍇ�A�O��ڑ������̃A�h���X�Őڑ�
	// * ---------------------------------------------------- 
	BtGetLastAddr(addr);

	// �ڑ��������ƂȂ邱�Ƃ��������̂Ń��g���C����
	retry_count = 0;
retry:	
	ret = EzBtConnect(addr, UUID_SPP);
	if (ret == BT_AT_NO_ANSWER) {
		retry_count++;
		if (retry_count > 2) {
			goto exit;
		}
		goto retry;
	} else if (ret != 0) {
		sprintf(buf, "EzBtConnect %d", ret);
		goto exit;
	} else {
	}

	rsettime(PORT_BLUETOOTH, 3);
		
	// * ------------------------  �󎚍\�z�����J�n

	// * -- ���{��
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignCenter, sizeof(bAlignCenter));

	// * -- �^�C�g��
	memset(buf,0x0, sizeof(buf));
	sprintf(buf, "�� �� �� ��\n");
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));

	// * -- ���[�}��
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bSizeNor, sizeof(bSizeNor));
	sprintf(buf, VER2);
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));

	// * -- ������
	char now[8];
	memset(now,0x0, sizeof(now));
	getrtc4( now );

	memset(buf,0x0,11);
	memcpy(buf,now,4);
	memcpy(buf+4,"/",1);
	memcpy(buf+5,now+4,2);
	memcpy(buf+7,"/",1);
	memcpy(buf+8,now+6,2);
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));

	// * -- ���s
	ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- ���s

	// * -- ���{��
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bJP, sizeof(bJP));
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bAlignLeft, sizeof(bAlignLeft));

	// * -- �ݒ��
	memset(buf,0x0,sizeof(buf));
	snprintf(buf, 10, "%s%s", "���WNO:", ctrl.RejiNo);
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
	ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- ���s

	memset(buf,0x0,sizeof(buf));
	snprintf(buf, 10, "%s%s", "  �ŗ�:", ctrl.TaxRate);
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
	ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- ���s

	memset(buf,0x0,sizeof(buf));
	snprintf(buf, 9, "%s%s", "  �ŋ�:", ctrl.TaxType);
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
	ret = rputs(PORT_BLUETOOTH, "\n", sizeof("\n"));  // ----------- ���s

	// * -- Printer & Line Feed
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)nFeedLine, sizeof(nFeedLine));

	// * -- �J�b�g
	ret = rputs(PORT_BLUETOOTH, (unsigned char *)bCut, sizeof(bCut));

	// * ------------------------  �󎚍\�z�����I��
	/////////////////////////////////////////
	//short rputs(int port, unsigned char *ptr, short size)
	//�ʐM�|�[�gport�̑��M�o�b�t�@��ptr�Ŏw�肵���o�b�t�@����size���̕������������ށB
	/////////////////////////////////////////
	
	//�v�����^�[�X�e�[�^�X
	// rxbclr(PORT_BLUETOOTH);
	// ret = rputs(PORT_BLUETOOTH, (unsigned char *)printer_status, sizeof(printer_status));
	// ret = rgetc(PORT_BLUETOOTH);
	// curp(0, 9);
	// if (ret & RSTAT_BIT_RXDATA) {
	// 	sprintf(buf, "printer %02x", ret & 0xFF);
	// } else {
	// 	// �u��� 20�Ȃ� OK�v
	// 	sprintf(buf, "  ���   %02x", (ret >> 8) & 0xFF);
	// }
	// putsc(buf);
	
exit:
	rclose(PORT_BLUETOOTH);
	cls();
	return 0;
}

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
