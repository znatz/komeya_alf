#ifndef ALF7000X_P_H
#define ALF7000X_P_H

#define ALF7000X_H /* 暗黙的にこのライブラリを使用する場合、旧ALFCOMは使用できないようにする */

#if !defined( TRUE )
#if !defined( True )
#define True  1
#define False 0
#endif
#define TRUE  True
#define FALSE False
#endif

#if ! defined( ALFCOMPLUS_FLG )
/* 【警告】通常はここのフラグは変更しない
     新旧両方のALCOM手順で通信をする場合(ごく稀なケース)
     このフラグをオフにする */
#define ALFCOMPLUS_FLG 1 
#endif

/* 戻り値 */
#define COM_EOF             2   /* 送信件反転 */
#define COM_RECIEVE         1   /* 送信件反転 */
#define COM_OK              0   /* 正常終了 */
#define COM_ABORT           -1  /* 強制終了 */
#define COM_TIMEOUT         -2  /* タイムアウト */
#define COM_ERROR           -3  /* 通信エラー */
#define COM_MODEM_ERR       -4  /* モデムエラー */
#define COM_BCC_ERR         -5  /* BCCエラー */
#define COM_FAILURE         -6  /* 関数の使い方が違う */
#define COM_RESP_ERR        -7  /* レスポンス受信エラー(ERROR) */
#define COM_DIALTONE        -8  /* ﾀﾞｲｱﾙﾄｰﾝ(NO DIALTONE)を検出できない */
#define COM_BUSY            -9  /* ﾋﾞｼﾞﾄｰﾝ(BUSY)を検出した */
#define COM_ANSWER          -10 /* ATD@のとき5秒間の無音状態を検出できない */
#define COM_SEQ_ERR         -13 /* 連番エラー */
#define COM_DUPLEX          -22 /* 二重オープン Bluetoothのみ*/

/* BlueTooth **************************************************** */
#define COM_BT_ARGERR       -31 /* 引数エラー */
#define COM_BT_OPENERR      -32 /* オープンエラー */
#define COM_BT_CONNECT_ERR  -33 /* 接続エラー */ /* このエラーのときは自動的にエラーダイアログが出た後の為、アプリでエラー表示を出す必要はない */
/* ************************************************************** */

/* 制御情報テーブル設定用 */
#define USB                  0 /* USB */
#define BT_SPP               1 /* BlueTooth (SPP)*/
#define BT_DUN               2 /* BlueTooth (DUN)*/
#define MASK              0xff /* 受信用マスクキャラクタ */

/* ALF標準通信手順 制御情報テーブル */
typedef struct ALFCOMINF {
    int    port;           /* 通信ポート   0:USB / 1:Bluetooth SPP / 2:Bluetooth DUN */
    int    abort;          /* 強制終了キー (キーコード）*/
    int    sendLen;        /* 送信データ長 */
    int    recvLen;        /* 受信データ長 */
    int    revflag;        /* 送受信反転   0: しない 1: する */
    char   remoteAddr[13]; /* Bluetoothリモートデバイスアドレス(12バイト + NULL) */
} ALFCOMINF ;

typedef struct {
    char    AlfMark       [  5 ];
    char    TermNo        [ 20 ];
    char    Kubun         [  1 ];
    char    BlockLen      [  4 ];
    char    FileName      [  6 ];
    char    FileSize      [  9 ];
    char    RecordCount   [  8 ];
    char    Filler        [ 11 ];
} RecAlfAlphaConnectDef; /* 64 */

#if ALFCOMPLUS_FLG
extern int ALFCOMPLUS_alfOpen(ALFCOMINF *comEnvUser);
extern int ALFCOMPLUS_alfSend(char *block);
extern int ALFCOMPLUS_alfRecv(char *block);
extern int ALFCOMPLUS_alfClose(void);
extern int ALFCOMPLUS_alfOnline(char *atbuf, int length, int time);
#else
extern int alfOpen(ALFCOMINF *comEnvUser);
extern int alfSend(char *block);
extern int alfRecv(char *block);
extern int alfClose(void);
extern int alfOnline(char *atbuf, int length, int time);
#endif

#if ALFCOMPLUS_FLG
#define alfOpen   ALFCOMPLUS_alfOpen
#define alfSend   ALFCOMPLUS_alfSend
#define alfRecv   ALFCOMPLUS_alfRecv
#define alfClose  ALFCOMPLUS_alfClose
#define alfOnline ALFCOMPLUS_alfOnline
#endif

extern char alfOpen_PrefixSend(ALFCOMINF *comEnvUser,
                               char *TermNoBuf,
                               char *KubunBuf,
                               char *BlockLenBuf,
                               char *FileNameBuf,
                               char *FileSizeBuf,
                               char *RecordCountBuf);

extern char alfOpen_PrefixSend_Length(ALFCOMINF *comEnvUser,
                                      char *TermNoBuf,      short int TermNoLen,
                                      char *KubunBuf,       short int KubunLen,
                                      char *BlockLenBuf,    short int BlockLenLen,
                                      char *FileNameBuf,    short int FileNameLen,
                                      char *FileSizeBuf,    short int FileSizeLen,
                                      char *RecordCountBuf, short int RecordCountLen);

extern char alfOpen_Rev_PrefixSend_Length(ALFCOMINF *comEnvUser,
                                          char *TermNoBuf,      short int TermNoLen,
                                          char *KubunBuf,       short int KubunLen,
                                          char *BlockLenBuf,    short int BlockLenLen,
                                          char *FileNameBuf,    short int FileNameLen,
                                          char *FileSizeBuf,    short int FileSizeLen,
                                          char *RecordCountBuf, short int RecordCountLen,
                                          char *BlockLenBuf2,   short int BlockLenLen2,
                                          char *FileNameBuf2,   short int FileNameLen2,
                                          char *FileSizeBuf2,   short int FileSizeLen2,
                                          char *RecordCountBuf2,short int RecordCountLen2);

extern char alfOpen_Rev_PrefixSend(ALFCOMINF *comEnvUser,
                                   char *TermNoBuf,
                                   char *KubunBuf,
                                   char *BlockLenBuf,
                                   char *FileNameBuf,
                                   char *FileSizeBuf,
                                   char *RecordCountBuf,
                                   char *BlockLenBuf2,
                                   char *FileNameBuf2,
                                   char *FileSizeBuf2,
                                   char *RecordCountBuf2);

extern char alfOpen_PrefixRecSend(ALFCOMINF *comEnvUser, RecAlfAlphaConnectDef *prefixRec);

extern char alfPrefixConnect(RecAlfAlphaConnectDef *src, RecAlfAlphaConnectDef *dst);

extern char alfClose_Rev_PrefixReceive(void);

extern RecAlfAlphaConnectDef alfGetPrefix(void);

extern void alfRecvAbort(void);

extern void alfGetVer(char* ptr);

extern void alfOpenArgClr(void);

extern char alfOpen_Rev_Next(void);

extern char  alfOpen_Rev_Next_PrefixSend(ALFCOMINF *comEnvUser,
                                         char *TermNoBuf,
                                         char *KubunBuf,
                                         char *BlockLenBuf,
                                         char *FileNameBuf,
                                         char *FileSizeBuf,
                                         char *RecordCountBuf);

extern char alfOpen_Rev_Next_PrefixSendEx(ALFCOMINF *comEnvUser,
                                          RecAlfAlphaConnectDef *Def);

extern char alf_Rev_Close(void);
extern char alf_Send_End(void);

#endif
