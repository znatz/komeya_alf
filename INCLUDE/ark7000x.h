#ifndef	__ARK7000X_H_
#define	__ARK7000X_H_

#include "ark7000x_key.h"

#define		ON						(1)
#define		OFF						(0)

#define		ENABLE					(1)
#define		DISABLE					(0)

#define		YES						(1)
#define		NO						(0)

#define		RESULT_OK				0
#define		ERROR_PARAM				-1
#define		ERROR_USED				-2
#define		ERROR_NOT_CONNECT		-3
#define		ERROR_SYSTEM			-4
#define		ERROR_TIMEOUT			-5
#define		ERROR_NOT_OPEN			-6
#define		ERROR_CONNECTED			-7
#define		ERROR_NO_DEVICE			-8
#define		ERROR_ABORT				-9

#define		CURSWITCH_OFF			0
#define		CURSWITCH_SLOW			1
#define		CURSWITCH_HIGH			2
#define		CURSWITCH_ON			3

#define		CORMODE_FULL			0
#define		CORMODE_HALF			1

#define		SETPICT_DISABLE			0
#define		SETPICT_ENABLE			1
#define		SETPICT_ENABLE_BOTTOM	2

#define		LCD_INPUT_PICT_OFF		0
#define		LCD_INPUT_PICT_NUMBER	1
#define		LCD_INPUT_PICT_ALPHA	2

#define		LCD_MSGBOX_NOICON		0
#define		LCD_MSGBOX_ERROR		1
#define		LCD_MSGBOX_WARNING		2
#define		LCD_MSGBOX_INFORMATION	3
#define		LCD_MSGBOX_QUESTION		4
#define		LCD_MSGBOX_USER			5

#define		LCD_MSGBOX_DEF_BUTTON2	0x00000100

#define		LCD_POPUP_LEFT			0
#define		LCD_POPUP_RIGHT			1
#define		LCD_POPUP_CENTER		2

#define		LCD_PROGRESS_USE_BAR		0x00000001
#define		LCD_PROGRESS_USE_COUNTER	0x00000002
#define		LCD_PROGRESS_USE_ANIMATION	0x00000004

#define		RGB(r, g, b)	(((r >> 3) << (5 + 6)) | ((g >> 2) << 5) | (b >> 3))

#define		COLOR_WHITE				RGB(0xFF, 0xFF, 0xFF)
#define		COLOR_SILVER			RGB(0xC0, 0xC0, 0xC0)
#define		COLOR_GLAY				RGB(0x40, 0x40, 0x40)
#define 	COLOR_BLACK				RGB(0x00, 0x00, 0x00)
#define 	COLOR_YELLOW			RGB(0xFF, 0xFF, 0x00)
#define 	COLOR_LIGHTYELLOW		RGB(0xFF, 0xFF, 0x80)
#define 	COLOR_ORANGE			RGB(0xFF, 0xA5, 0x00)
#define 	COLOR_RED				RGB(0xFF, 0x00, 0x00)
#define 	COLOR_BROWN				RGB(0xA5, 0x2A, 0x2A)
#define 	COLOR_LIME				RGB(0x00, 0xFF, 0x00)
#define 	COLOR_GREEN				RGB(0x00, 0x80, 0x00)
#define 	COLOR_OLIVE				RGB(0x80, 0x80, 0x00)
#define 	COLOR_CYAN				RGB(0x00, 0xFF, 0xFF)
#define 	COLOR_BLUE				RGB(0x00, 0x00, 0xFF)
#define 	COLOR_DARKBLUE			RGB(0x00, 0x00, 0xA0)
#define 	COLOR_LIGHTBLUE			RGB(0xAD, 0xD8, 0xE6)
#define 	COLOR_PURPLE			RGB(0x80, 0x00, 0x80)
#define 	COLOR_FUCHSIA			RGB(0xFF, 0x00, 0xFF)

#define		PORT_USB				0
#define		PORT_BLUETOOTH			1
#define		PORT_MAX				2

#define		UUID_SPP				1101
#define		UUID_DUN				1103

#define		BT_INIT					0
#define		BT_GET					1
#define		BT_SET					2

#define		SSP_DISP_ONLY			0
#define		SSP_DISP_YESNO			1
#define		SSP_KEY_ONLY			2
#define		SSP_NO_IN_OUT			3

#define		BT_SSP_CONF				10
#define		BT_SSP_DISP				11
#define		BT_SSP_INPUT			12

#define		BT_OTHER_ERROR			-100
#define		BT_FUNC_ERROR			-101
#define		BT_AT_ERROR				-102
#define		BT_AT_NO_CARRIER		-103
#define		BT_AT_NO_ANSWER			-104
#define		BT_DISCAVERY_FAIL		-105
#define		BT_SERVICE_NOT_SUPPORT	-106
#define		BT_DEVICE_ADDR_TAIL		-107

#define		RSTAT_BIT_TIMEOUT		0x8000
#define		RSTAT_BIT_BUFOVER		0x4000
#define		RSTAT_BIT_NOT_CONNECT	0x2000
#define		RSTAT_BIT_USBERR		0x1000
#define		RSTAT_BIT_BTERR			0x0800
#define		RSTAT_BIT_TXEND			0x0400
#define		RSTAT_BIT_RXDATA		0x0200
#define		RSTAT_BIT_TXENABLE		0x0100

#define		AUTOFF_RESUME_OFF		1
#define		AUTOFF_RESUME_ON		2
#define		AUTOFF_RESUME_SET		3

#define		OFFKEY_ENABLE			1
#define		OFFKEY_DISABLE			0

#define		AUTOFF_RESUME_OFF		1
#define		AUTOFF_RESUME_ON		2
#define		AUTOFF_RESUME_SET		3

#define		BATTERY_EMPTY			0
#define		BATTERY_LOW				1
#define		BATTERY_MIDDLE			2
#define		BATTERY_HIGH			3
#define		BATTERY_CHARGING		4

#define		LED_OFF					0
#define		LED_GREEN				1
#define		LED_RED					2
#define		LED_ORANGE				3

#define		VOL_SMALL				1
#define		VOL_MEDIUM				2
#define		VOL_LARGE				3

#define		BLBRIGHT_LOW			0
#define		BLBRIGHT_MIDDLE			1
#define		BLBRIGHT_HIGH			2

#define		WLAN_ERR_SOCKET_FULL		-100
#define		WLAN_ERR_INVALID_SOCKET		-101
#define		WLAN_ERR_BUF_OVER			-102
#define		WLAN_ERR_SOCKET_OVER		-200
#define		WLAN_ERR_ARP_REQ_FAILED		-201
#define		WLAN_ERR_SERVER_NON_EXIST	-202
#define		WLAN_ERR_SOCKET_PARAM		-203
#define		WLAN_ERR_SOCKET_OPEN		-204
#define		WLAN_ERR_SOCKET_BUSY		-205
#define		WLAN_ERR_DNS_NOT_FOUND		-300
#define		WLAN_ERR_DNS_CLASS			-301
#define		WLAN_ERR_DNS_COUNT			-302
#define		WLAN_ERR_DNS_RECODE			-303
#define		WLAN_ERR_DNS_OPCODE			-304
#define		WLAN_ERR_DNS_ID				-305
#define		WLAN_ERR_DNS_VALUE			-306
#define		WLAN_ERR_DNS_TIMEOUT		-307
#define		WLAN_ERR_SEND_BUSY			-400
#define		WLAN_ERR_DEVICE_BUSY		-401
#define		WLAN_ERR_DEVICE_FAIL		-402
#define		WLAN_ERR_UNKNOWN			-500

#define		SOCKET_BIT_ERR_PARAM		0x8000
#define		SOCKET_BIT_BUFFOVER			0x4000
#define		SOCKET_BIT_TIMEOUT			0x2000
#define		SOCKET_BIT_RXDATA			0x0002
#define		SOCKET_BIT_CONNECTED		0x0001

#define		WLAN_MAX_DOMAIN_NAME	42

#define		FTP_REMAINDER			1
#define		FTP_ERR_BAD_RESPONSE	-1000

#define		FTP_CONNECT_ACTIVE		0
#define		FTP_CONNECT_PASSIVE		1

#define		FTP_MODE_SEND			0
#define		FTP_MODE_APPEND			1

#define		FTP_TYPE_ASCII			0
#define		FTP_TYPE_BINARY			1
#define		FTP_TYPE_EBCDIC   		2

#define		FTP_LIST_NAME			0
#define		FTP_LIST_PATH			1
#define		FTP_LIST_ALL			2

#define 	FTP_MAX_PATH			256

#define 	NTP_ERR_BAD_RESPONSE	-1000

#define		EZ_BT_MODE_MENU			0
#define 	EZ_BT_MODE_LIST			1
#define 	EZ_BT_MODE_SEARCH		2

#define 	INPUT_MAX_SIZE			256

#define 	INPUT_MODE_NUMBER		0
#define 	INPUT_MODE_ASCII		1
#define 	INPUT_MODE_TABLE		2

#define 	INPUT_MODE_PICT_NUM		0x00000004
#define 	INPUT_MODE_PICT_ALPHA	0x00000008

#define		INPUT_MODE_INSERT		0x00000010

#define 	INPUT_MODE_FIX_LEN		0x00000100
#define 	INPUT_MODE_ZERO_LEN		0x00000200

#define 	INPUT_MODE_SCAN_ENABLE	0x00001000

#define 	INPUT_MODE_DOT_ENT		0x00002000

#define 	INPUT_MODE_F1_CANCEL	0x00010000
#define 	INPUT_MODE_F2_CANCEL	0x00020000
#define 	INPUT_MODE_F3_CANCEL	0x00040000
#define 	INPUT_MODE_F4_CANCEL	0x00080000
#define 	INPUT_MODE_F1_ENT		0x00100000
#define 	INPUT_MODE_F2_ENT		0x00200000
#define 	INPUT_MODE_F3_ENT		0x00400000
#define 	INPUT_MODE_F4_ENT		0x00800000

/****************************************************************************/

typedef struct {
	unsigned short vram[20480];
	unsigned short gram[20480];	
} screen_t;

typedef struct {
	unsigned char width;
	unsigned char hight;
	unsigned short *image;
	unsigned char num;
} animation_t;


typedef struct {
	char devName[32];	// 31 byte + NULL
	char localPIN[17];	// 16 byte + NULL
	char timeout;		// 1-30
	char MITM_Enable;	// ENABLE(1) / DISABLE(0)
	char SSP_Mode;		// SSP_DISP_ONLY(0) / SSP_DISP_YESNO(1) / SSP_KEY_ONLY(2) / SSP_NO_IN_OUT(3)
} bt_conf_t;

typedef struct {
	unsigned char band;
	unsigned char channel;
	unsigned char ssid[32];
	unsigned char securityType;
	unsigned char psk[63];
	unsigned short listenInterval;
	unsigned char powerLevel;
} wlan_conf_t;


typedef struct {
	unsigned char dhcpMode;
	unsigned char ipaddr[4];
	unsigned char netmask[4];
	unsigned char gateway[4];
	unsigned char dnsip[4];
} ip_conf_t;;


typedef struct {
	unsigned short port;
	unsigned char ipaddr[4];
} socket_t;

typedef struct {
	unsigned char ipaddr[4];		/* ＦＴＰサーバアドレス						 */
	unsigned char user[32];			/* ＦＴＰユーザＩＤ							 */
	unsigned char pass[32];			/* ＦＴＰパスワード							 */
	unsigned char passive;			/* 接続方式									 */
	unsigned char abortkey;			/* 強制終了キーコード                        */
} ftp_t;

typedef void (* ftpcb)(void);

typedef struct {
	unsigned char key[11][16];
} key_tbl_t;

/* APIインターフェース		*/

/* キー制御					*/
extern unsigned short getcc(void);
extern int getch(void);
extern int getche(void);
extern void kbclr(void);
extern int kbhit(void);
extern void kclk(int sw);
extern unsigned int ktouch(void);
extern int setk(int key, int code, int ch);
extern int ungetch(char ch);


/* 表示制御					*/
extern void cls(void);
extern void cls_char(void);
extern void cls_line(void);
extern void curonf(int sw, int mode);
extern void curp(int x, int y);
extern void gline(int x1, int y1, int x2, int y2, int sw);
extern void gpset(int x, int y, int sw);
extern void home(void);
extern void putch(int ch);
extern void putchs(int ch);
extern void putsc(char *string);
extern void putss(char *string);
extern void revonf(int sw);
extern void setpict(int pict);
extern int getpict(void);
extern void setgaiji(int fontsize, unsigned short charcode, unsigned char *data);
extern void bitimage(int x1, int y1, int x2, int y2, unsigned short *data);
extern void BackupScreen(screen_t *buf);
extern void RestoreScreen(screen_t *buf);
extern void setcolor(unsigned short front, unsigned short back);
extern void inputpict(int sw);
extern void popup(unsigned long type, char *msg, int time);
extern int msgbox(unsigned long type, unsigned short *icon, char *messag1, char *messag2,
					char *button1, char *button2);
extern void progress_start(unsigned long type, char *msg);
extern void progress_stop(void);
extern void progress_update(char *msg, char *count, int percent);
extern void animation_start(int x, int y, animation_t *info);
extern void animation_stop(int sw);
extern void progress_bar(int x, int y, unsigned long count, unsigned long max, unsigned short color);
extern void screen_lock(void);
extern void screen_unlock(void);

/* 通信制御					*/
extern int ropen(int port);
extern unsigned short rgetc(int port);
extern unsigned short rputc(int port, char ch);
extern void rsettime(int port, int time);
extern unsigned short rstat(int port);
extern void rxbclr(int port);
extern unsigned short rgets(int port, unsigned char *ptr, short maxsize, short *recsize);
extern unsigned short rputs(int port, unsigned char *ptr, short size);
extern void rclose(int port);

/* Bluetooth制御			*/
extern int BtRts(int mode);
extern int BtCtsStat(void);
extern int BtConfig(int mode, bt_conf_t* conf);
extern int BtLocalAddr(unsigned char *addr);
extern int BtRemoteName(unsigned char *addr, unsigned char *name);
extern int BtRemoteService(unsigned char *addr, int uuid);
extern int BtInquiry(int *count);
extern int BtBondList(int *count);
extern int BtNextAddr(unsigned char *addr);
extern int BtDeleteBondListItem(unsigned char *addr);
extern int BtDeleteBondListAll(void);
extern int BtConnect(unsigned char *addr, int uuid);
extern int BtDisconnect(void);
extern int BtListen(unsigned int time);
extern int BtRemoteAddr(unsigned char *addr);
extern int BtSSPConf(int accept);
extern int BtSSPPin(unsigned char *pin);
extern int BtGetSSPPin(unsigned char *pin);
extern int BtSSPWaitConnect(void);
extern void BtGetLastAddr(unsigned char *addr);

/* レーザースキャナ制御		*/
extern int bmode(unsigned char *commands);
extern void bmodeget(unsigned char *commands);
extern void bmd_clr(void);
extern int bgets(char *string);
extern int bgets3(char *string);


/* タイマ制御				*/
extern void delay(unsigned int time);
extern int tcheck(void);
extern void tstart(unsigned int count);
extern void tstop(void);
extern unsigned long tgetcount(void);


/* リアルタイムクロック制御	*/
extern void setrtc(char *string);
extern void getrtc(char *string);
extern int setwakeup(int sw, char *string);
extern int wakeupstat(void);


/* 電源制御					*/
extern void setautoff(int idletime, int offtime);
extern void getautoff(int *idletime, int *offtime);
extern void offmode(int sw);
extern void offkey(int sw);
extern void pwroff(void);
extern int vstat(void);


/* その他					*/
/* (RAM制御)			*/
extern int disksize(void);
extern int mread(int offset, char *ptr, int count);
extern int mwrite(int offset, char *ptr, int count);
extern void mflush(void);
extern int tempsize(void);
extern int tempread(int offset, char *ptr, int count);
extern int tempwrite(int offset, char *ptr, int count);


/* (ブザー制御)			*/
extern void buzzer(int Hz);
extern void volume(int level);
/* (LED 制御)			*/
extern void ledonf(int sw);
/* (バックライト制御)	*/
extern void blbright(int level);

/* (バイブレータ制御)	*/
extern void vibonf(int time);
/* (その他)				*/
extern int ldstat(void);
extern void aplupdt(int startaddr, int datacnt);
extern void biosver(char *ptr);
extern void MemSW(char*);

/* 無線ＬＡＮ制御 */
extern int wconfig(int mode, wlan_conf_t *conf);
extern int wsetip(ip_conf_t *conf);
extern int wgetip(ip_conf_t *conf);
extern int wopen(void);
extern void wclose(void);
extern int wstat(void);
extern int wsocket(void);
extern int wconnect(int sockfd, socket_t *sock, unsigned long timeout);
extern int waccept(int sockfd, socket_t *sock, unsigned long timeout);
extern void wclosesocket(int sockfd);
extern int wsend(int sockfd, unsigned char *buf, unsigned long size);
extern int wrecv(int sockfd, unsigned char *buf, unsigned short size, unsigned short *read);
extern void wsettime(int sockfd, unsigned long time);
extern void wbclr(int sockfd);
extern int wipaddrbyname(unsigned char *name, unsigned char *ipaddr);
extern unsigned short wport(void);
extern unsigned short wsockstat(int sockfd);

/* ユーティリティ */
extern int EzBtSelect(int mode, unsigned char *addr, unsigned char *name);
extern int EzBtConnect(unsigned char *addr, int uuid);
extern int EzBtListen(unsigned int time);
extern int KeyInputString(int x, int y, int dispsize, unsigned char *buf, int bufsize,
					int *curidx, unsigned long mode, key_tbl_t *keytbl);

/* ＦＴＰ */
extern int FtpConnect(ftp_t *ftpinfo);
extern void FtpDisconnect(void);
extern int FtpPutFileOpen(char *filepath, unsigned long mode, unsigned long type);
extern void FtpPutFileClose(void);
extern int FtpWrite(unsigned char *buff, unsigned long len);
extern int FtpGetFile(char *filepath, unsigned long type, ftpcb callback);
extern int FtpGetList(char *dirpath, unsigned long list, ftpcb callback);
extern int FtpRead(unsigned char *buff, unsigned short bufsize, unsigned short *read);
extern int FtpGetFileSize(char *filepath, unsigned long *size);
extern int FtpGetFileInfo(char *filepath, unsigned char *buff, unsigned short size);
extern int FtpRename(char *from, char *to);
extern int FtpDelete(char *filepath);

/* ＮＴＰ */
extern int ntpdate(unsigned char *ipaddr);

#endif	/* __ARK7000X_H_ */
