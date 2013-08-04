/***************************************************************************
                        RadianceTool.h  -  description
                             -------------------
    copyright            : (C) 2012 by Mathieu LAGARDE
    email                : lagarde.mthieu@gmail.com
 ***************************************************************************/
/***************************************************************************
 *   Copyright (C) 2012 by Mathieu LAGARDE                                 *
 *   lagarde.mthieu@gmail.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _CLEAPROJINCLUDE_RADIANCETOOL_HPP
#define _CLEAPROJINCLUDE_RADIANCETOOL_HPP


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAXSTR 128 /* maximum input string length */
#define TCALNAME	"tmesh.cal"	/* the name of our auxiliary file */
#define VOIDID "void" /* null modifier */

typedef int VNDX[3]; /* vertex index (point,map,normal) */

#define  RREAL		float
typedef RREAL  FVECT[3];


typedef struct {
	int	nquals;			/* number of qualifiers */
	char	**qual;			/* qualifier array */
} QLIST;

typedef struct {
	char	*name;			/* string, NULL if number */
	int	number;
} ID;
				/* identifier list */
typedef struct {
	int	nids;			/* number of ids */
	ID	*id;			/* id array */
} IDLIST;

typedef struct {
	char	*nam;			/* string, NULL if range */
	int	min, max;		/* accepted range */
} IDMATCH;

typedef struct rule {
	char	*mnam;			/* material name */
	long	qflg;			/* qualifier condition flags */
	struct rule	*next;		/* next rule in mapping */
	/* followed by the IDMATCH array */
} RULEHD;

#ifndef  NHASH
#define  NHASH		2039		/* hash table size (prime!) */
#endif

#define  FTINY		(1e-6)

typedef struct s_head {
	struct s_head  *next;		/* next in hash list */
	int  nl;			/* links count */
}  S_HEAD;				/* followed by the string itself */

typedef struct {
	int	ax;		/* major axis */
	RREAL	tm[2][3];	/* transformation */
} BARYCCM;

typedef struct 
{ 
  double a,b,c,d; /* longueurs caracteristiques */
  double np; /* indice de refraction */
} TPRISM;

typedef struct {
  int x[4], y[4], z[4], h[4];
} PRISM;


static S_HEAD  *stab[NHASH];

#ifndef lint
static const char	RCSid[] = "$Id: obj2rad.c,v 2.27 2010/09/28 21:28:23 greg Exp $";
#endif

#define PATNAME		"M-pat"		/* mesh pattern name (reused) */
#define TEXNAME		"M-nor"		/* mesh texture name (reused) */
#define DEFOBJ		"unnamed"	/* default object name */
#define DEFMAT		"white"		/* default material name */

#define pvect(v)	printf("%18.12g %18.12g %18.12g\n",(v)[0],(v)[1],(v)[2])

FVECT	*vlist;			/* our vertex list */
int	nvs;			/* number of vertices in our list */
FVECT	*vnlist;		/* vertex normal list */
int	nvns;
RREAL	(*vtlist)[2];		/* map vertex list */
int	nvts;

int	ndegen = 0;		/* count of degenerate faces */
int	n0norm = 0;		/* count of zero normals */

typedef int	VNDX[3];	/* vertex index (point,map,normal) */

#define CHUNKSIZ	1024	/* vertex allocation chunk size */

#define MAXARG		512	/* maximum # arguments in a statement */

				/* qualifiers */
#define Q_MTL		0
#define Q_MAP		1
#define Q_GRP		2
#define Q_OBJ		3
#define Q_FAC		4
#define NQUALS		5

char	*qname[NQUALS] = {
	"Material",
	"Map",
	"Group",
	"Object",
	"Face",
};

QLIST	qlist = {NQUALS, qname};
				/* valid qualifier ids */
IDLIST	qual[NQUALS];
				/* mapping rules */
RULEHD	*ourmapping = NULL;

char	*defmat = DEFMAT;	/* default (starting) material name */
char	*defobj = DEFOBJ;	/* default (starting) object name */

int	flatten = 0;		/* discard surface normal information */

char	mapname[128];		/* current picture file */
char	matname[64];		/* current material name */
char	group[16][32];		/* current group names */
char	objname[128];		/* current object name */
char	*inpfile;		/* input file name */
int	lineno;			/* current line number */
int	faceno;			/* current face number */






int shash(char *s);

#define hash(s) (shash(s)%NHASH)
#define FL(qn) (1L<<(qn))

#define string(sp) ((char *)((sp)+1))
#define salloc(str) (S_HEAD *)malloc(sizeof(S_HEAD)+1+strlen(str))
#define idm(rp) ((IDMATCH *)((rp)+1))
#define VCOPY(v1,v2) ((v1)[0]=(v2)[0],(v1)[1]=(v2)[1],(v1)[2]=(v2)[2])

#define DOT(v1,v2)	((v1)[0]*(v2)[0]+(v1)[1]*(v2)[1]+(v1)[2]*(v2)[2])
#define VLEN(v)	sqrt(DOT(v,v))
#define VADD(vr,v1,v2)	((vr)[0]=(v1)[0]+(v2)[0], \
				(vr)[1]=(v1)[1]+(v2)[1], \
				(vr)[2]=(v1)[2]+(v2)[2])
#define VSUB(vr,v1,v2)	((vr)[0]=(v1)[0]-(v2)[0], \
				(vr)[1]=(v1)[1]-(v2)[1], \
				(vr)[2]=(v1)[2]-(v2)[2])
#define VSUM(vr,v1,v2,f)	((vr)[0]=(v1)[0]+(f)*(v2)[0], \
				(vr)[1]=(v1)[1]+(f)*(v2)[1], \
				(vr)[2]=(v1)[2]+(f)*(v2)[2])
#define VCROSS(vr,v1,v2) \
			((vr)[0]=(v1)[1]*(v2)[2]-(v1)[2]*(v2)[1], \
			(vr)[1]=(v1)[2]*(v2)[0]-(v1)[0]*(v2)[2], \
			(vr)[2]=(v1)[0]*(v2)[1]-(v1)[1]*(v2)[0])
#define ABS(x) ((x) >= 0 ? (x) : -(x))


#ifndef COSTOL
#define COSTOL		0.999995	/* cosine of tolerance for smoothing */
#endif

				/* flat_tri() return values */
#define ISBENT		0		/* is not flat */
#define ISFLAT		1		/* is flat */
#define RVBENT		2		/* reversed and not flat */
#define RVFLAT		3		/* reversed and flat */
#define DEGEN		-1		/* degenerate (zero area) */


int	flat_tri(FVECT v1, FVECT v2, FVECT v3, FVECT n1, FVECT n2, FVECT n3);
int	comp_baryc(BARYCCM *bcm,  FVECT v1, FVECT v2, FVECT v3);
void eval_baryc(RREAL wt[3], FVECT p, BARYCCM *bcm);
int	get_baryc(RREAL wt[3], FVECT p, FVECT v1, FVECT v2, FVECT v3);
void put_baryc(BARYCCM *bcm, RREAL com[][3], int n);


double fdot(const FVECT v1, const FVECT v2);

#define rulsiz(nq) (sizeof(RULEHD)+(nq)*sizeof(IDMATCH))

void eputs(char *str);
void quit(int errC);
char *fgetline(char *s, int n, FILE *fp);

int qtype(char *qnm, QLIST *qlp);
int puttri(char *v1, char *v2, char *v3);
int nonplanar(int ac, char **av);
int newv(double x, double y, double z);
int newvt(double x, double y);
int newvn(double x, double y, double z);
int matchrule(RULEHD *rp);
void getnames(FILE *fp);
void convert(FILE *fp);
int getstmt(char *av[MAXARG], FILE *fp);
char * getmtl(void);
char * getonm(void);
int cvtndx(VNDX vi, char *vs);
int nonplanar(int ac, char **av);
int putface(int ac, char **av);
void freeverts(void);


char *iskip(char *s);
char *fskip(char *s);
int isintd(char *, char *);
int isflt(char *);
int	isfltd(char *s, char *ds);

void putface(char *m, char *bn, char *fn, PRISM *p, int a, int b, int c, int d, FILE *fp);

char *savestr();
void fputidlist(IDLIST *qp, FILE *fp);
void write_quals(QLIST *qlp, IDLIST idl[], FILE *fp);
void fputword(char s, FILE *fp);
void printargs(int ac, char **av, FILE *fp);
int badarg(int ac, char **av, char *fl);
void syntax(char *err);

int idcmp(ID *id1, ID *id2);
int matchid(ID *it, IDMATCH *im);


int findid(IDLIST *idl, ID *idp, int insert);

void fvsum(FVECT vres, const FVECT v0, const FVECT v1, double f);
double normalize(FVECT v);
void fcross(FVECT vres, const FVECT v1, const FVECT v2);

RULEHD	*getmapping(char *file, QLIST *qlp);


int OBJ2RAD(int	argc, char	*argv[]);

#endif