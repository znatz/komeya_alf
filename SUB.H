#ifndef	SUB_H
#define	SUB_H

#define BEEP_SEC 100
#define ERR_SEC  100

#endif

/*********************************************************/
/* ÉuÉUÅ[ä÷åW										 	 */
/*********************************************************/
extern	void	beep( short tim, short loop );
extern	void	beeb( short tim, short loop, short wait );
extern	void	playHz( short hz, short tim, short loop );

/*********************************************************/
/* âÊñ ï\é¶												 */
/*********************************************************/
extern	void	ckputss( short x, short y, char *str, char revflag, short clr );
extern	void	caputss( short x, short y, char *str, char revflag, short clr );
extern	void	ckputsn( short x, short y, char *str, short len, char revflag, short clr );
extern	void	caputsn( short x, short y, char *str, short len, char revflag, short clr );
extern	void	kputspc( short x, short y, short len, short clr );
extern	void	aputspc( short x, short y, short len, short clr );
extern	void	kputchar( short x, short y, char c, short len, short clr );
extern	void	aputchar( short x, short y, char c, short len, short clr );
extern	void	ckprintf( short x, short y, short rev, short clr, char *format, ... );
extern	void	caprintf( short x, short y, short rev, short clr, char *format, ... );
extern	void	ckputssEx( short x, short y, char *str, char revflag, short clr );
extern	void	caputssEx( short x, short y, char *str, char revflag, short clr );
extern  void	ReDrowDisplay( short clr );
extern  void	ClsColor( void );

/*********************************************************/
/* ÉÅÉÇÉää÷åW											 */
/*********************************************************/
extern	long	atoln( char *buf, short len );
extern	short	atoin( char *buf, short len );
extern	double	atofn( char *buf, short len );
extern	void	memncpy( char *out, char *in, short len );
extern	void	addDot( char *in, short Fixlen );

/*********************************************************/
/* ì¸óÕä÷åW												 */
/*********************************************************/
extern	short		okng( void );
extern	short		okngEx( short Yes, short No );

/*********************************************************/
/* ì˙ïtä÷åW												 */
/*********************************************************/
extern	short	getweek(short year, short month, short day);
extern	void	getrtc4( char *s );
extern  void    getrtcHourMinute( char *s );
extern  void    getrtcHourMinuteSecond( char *s );
extern	void	addYear( char *outYear, char *inYear );
extern	void	addDay( char *yymmdd, short addCnt );
extern	short	datechk( char *date, char *now, short check );