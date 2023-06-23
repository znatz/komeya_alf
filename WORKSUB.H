#ifndef WORKSUB_H

#define WORKSUB_H

#define	TYPE_NUM	True
#define	TYPE_CHAR	False

#define	ZORO_CHECK	True
#define	NO_CHECK	False

#define	NO_CHAR		'-'
#endif

extern	short 	CodeInput( short, short, char *, short, unsigned long );
extern	short 	NumInput( short, short, char *, short, long, long, unsigned long, short, short );
extern	short 	selectInput( short x, short y, char *s, char *item, short size, short cnt );
extern	short	yyyymmddInput( short x, short y, char *s, char *now, unsigned long prm, short check );
extern	short	yymmddInput( short x, short y, char *s, char *now, unsigned long prm, short check );
extern	short	MaxCheck( short cnt, short max );
extern	short	ShopFind( char *key );
extern	short	ItemFind( char *key );
extern	short	TantoFind( char *key );
extern  short	RemasFind( char *key );
extern	long	GetAllCnt( void );
extern	void	playHzEx( short hz, short tim, short loop );
extern	void	beebEx( short tim, short loop, short wait );
extern	void	beepEx( short tim, short loop );
extern int gf_GetKingaku2(long wkKingaku, double Zeiritu);
extern long extractZeikomiFromITF(const char *str);
