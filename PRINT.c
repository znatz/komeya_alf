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

int	PrintMain_NEX_M230(char* _addr, bool _crc, int _mode , short _Flag ) {
	bt_conf_t conf;
	int ret;
	int retry_count;
	unsigned char addr[13];
	unsigned char name[41];
	char buf[256];//�]���p�o�b�t�@
	//�e��X�e�[�^�X�𒲂ׂ邽�߂̃R�}���h(DLE EOT n)
	// char printer_status[] = { 0x10, 0x04, 0x01 };
	char printer_status[] = { 0x10, 0x1b, 0x76, 0x01 };
	char offline_status[] = { 0x10, 0x04, 0x02 };
	char error_status[] = { 0x10, 0x04, 0x03 };
	char sensor_status[] = { 0x10, 0x04, 0x04 };
	
restart:
	cls();
	setcolor(COLOR_WHITE, COLOR_BLUE);
	putsc("  Printer Test  ");
	setcolor(COLOR_BLACK, COLOR_WHITE);
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
	
	ret = EzBtSelect(EZ_BT_MODE_MENU, addr, name);
	if (ret != 0) {
		sprintf(buf, "EzBtSelect %d", ret);
		goto exit;
	}

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
		
	//��ʕ\��
	curp(0,3);
	putsc("[ENT] test print");
	curp(0,5);
	putsc("[C]   end");

	
	while (1) {
		ret = getch();//���͑҂�
		if (ret == CH_CLR) {//�ر�������ꂽ�ꍇ
			sprintf(buf, "Abort");
			break;
		} else if (ret != CH_ENT) {//Ent�ȊO�������ꂽ�ꍇ
			continue;
		} else {
		}
		
		///////////////�󎚕���//////////////////
		sprintf(buf, "test\n");
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)buf, strlen(buf));
		
		/////////////////////////////////////////
		//short rputs(int port, unsigned char *ptr, short size)
		//�ʐM�|�[�gport�̑��M�o�b�t�@��ptr�Ŏw�肵���o�b�t�@����size���̕������������ށB
		/////////////////////////////////////////
		
		//�v�����^�[�X�e�[�^�X
		rxbclr(PORT_BLUETOOTH);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)printer_status, sizeof(printer_status));
		ret = rgetc(PORT_BLUETOOTH);
		curp(0, 9);
		if (ret & RSTAT_BIT_RXDATA) {
			sprintf(buf, "printer %02x", ret & 0xFF);
		} else {
			sprintf(buf, "rgetc   %02x", (ret >> 8) & 0xFF);
		}
		putsc(buf);

		//�I�t���C���v���X�e�[�^�X
		rxbclr(PORT_BLUETOOTH);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)offline_status, sizeof(offline_status));
		ret = rgetc(PORT_BLUETOOTH);
		curp(0, 11);
		if (ret & RSTAT_BIT_RXDATA) {
			sprintf(buf, "offline %02x", ret & 0xFF);
		} else {
			sprintf(buf, "rgetc   %02x", (ret >> 8) & 0xFF);
		}
		putsc(buf);

		//�G���[�v���X�e�[�^�X
		rxbclr(PORT_BLUETOOTH);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)error_status, sizeof(error_status));
		ret = rgetc(PORT_BLUETOOTH);
		curp(0, 13);
		if (ret & RSTAT_BIT_RXDATA) {
			sprintf(buf, "error  %02x", ret & 0xFF);
		} else {
			sprintf(buf, "rgetc  %02x", (ret >> 8) & 0xFF);
		}
		putsc(buf);

		//�A���p�����o��X�e�[�^�X
		rxbclr(PORT_BLUETOOTH);
		ret = rputs(PORT_BLUETOOTH, (unsigned char *)sensor_status, sizeof(sensor_status));
		ret = rgetc(PORT_BLUETOOTH);
		curp(0, 15);
		if (ret & RSTAT_BIT_RXDATA) {
			sprintf(buf, "sensor %02x", ret & 0xFF);
		} else {
			sprintf(buf, "rgetc  %02x", (ret >> 8) & 0xFF);
		}
		putsc(buf);
	}
	
exit:
	rclose(PORT_BLUETOOTH);
	cls();
	setcolor(COLOR_WHITE, COLOR_BLUE);
	putsc("  Printer Test  ");
	setcolor(COLOR_BLACK, COLOR_WHITE);
	curp(0,3);
	putsc("Test End");
	curp(0,9);
	putsc(buf);
	
	getch();
	goto restart;
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
