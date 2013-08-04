#include "RadianceTool.h"

void eputs(char *str)
{
  fputs(str, stderr);
}

void quit(int errC)
{
  exit(errC);
}

int shash(char  *s)
{
	int  h = 0;

	while (*s)
		h = (h<<1 & 0x7fff) ^ (*s++ & 0xff);
	return(h);
}


int matchid(ID *it, IDMATCH *im)
{
  if (it->name == NULL) {
    if (im->nam != NULL)
      return(0);
    return(it->number >= im->min && it->number <= im->max);
  }
  if (im->nam == NULL)
    return(0);
  return(!strcmp(it->name, im->nam));
}

char *fgetline(char *s, int n, FILE *fp)
{
  char *cp = s;
  int c = EOF;

  while (--n > 0 && (c = getc(fp)) != EOF) {
    if (c == '\r' && (c = getc(fp)) != '\n') {
      ungetc(c, fp); /* must be Apple file */
      c = '\n';
    }
    if (c == '\n' && (cp == s || cp[-1] != '\\'))
      break;
    *cp++ = c;
  }
  if (cp == s && c == EOF)
    return(NULL);
  *cp = '\0';
  return(s);
}

char *savestr(char *str)				/* save a string */
{
	int  hval;
	S_HEAD  *sp;

	if (str == NULL)
		return(NULL);
	hval = hash(str);
	for (sp = stab[hval]; sp != NULL; sp = sp->next)
		if (!strcmp(str, string(sp))) {
			sp->nl++;
			return(string(sp));
		}
	if ((sp = salloc(str)) == NULL) {
		eputs("Out of memory in savestr\n");
		quit(1);
	}
	strcpy(string(sp), str);
	sp->nl = 1;
	sp->next = stab[hval];
	stab[hval] = sp;
	return(string(sp));
}

int qtype(char	*qnm, QLIST	*qlp)
{
	int	i;
	
	for (i = 0; i < qlp->nquals; i++)
  {
		if (!strcmp(qnm, qlp->qual[i]))
			return(i);
  }
	return(-1);
}

double fdot(const FVECT v1, const FVECT v2)
{
	return(DOT(v1,v2));
}


RULEHD *getmapping(char	*file, QLIST	*qlp)
{
	char	*err;
	int	c;
	RULEHD	*mp = NULL;
	int	nrules = 0;
	RULEHD	*rp;
	int	qt;
	char	buf[MAXSTR];
	FILE	*fp;
	
	if ((fp = fopen(file, "r")) == NULL) 
  {
		eputs(file);
		eputs(": cannot open\n");
		quit(1);
	}
							/* get each rule */
	while (fscanf(fp, " %[^ 	;\n]", buf) == 1) {
		if (buf[0] == '#') {			/* comment */
			while ((c = getc(fp)) != EOF && c != '\n')
				;
			continue;
		}
		rp = (RULEHD *)calloc(1, rulsiz(qlp->nquals));
		if (rp == NULL)
			goto memerr;
		rp->mnam = savestr(buf);
		for ( ; ; ) {				/* get conditions */
			while ((c = getc(fp)) != '(')
				if (c == ';' || c == EOF)
					goto endloop;
			if (fscanf(fp, " %s ", buf) != 1) {
				err = "missing variable";
				goto fmterr;
			}
			if ((qt = qtype(buf, qlp)) == -1) {
				err = "unknown variable";
				goto fmterr;
			}
			if (rp->qflg & FL(qt)) {
				err = "variable repeated";
				goto fmterr;
			}
			rp->qflg |= FL(qt);
			c = getc(fp);
			switch (c) {
			case '"':			/* id name */
				if (fscanf(fp, "%[^\"]\" )", buf) != 1) {
					err = "bad string value";
					goto fmterr;
				}
				idm(rp)[qt].nam = savestr(buf);
				break;
			case '[':			/* id range */
				if (fscanf(fp, "%d : %d ] )", &idm(rp)[qt].min,
						&idm(rp)[qt].max) != 2) {
					err = "bad range value";
					goto fmterr;
				}
				if (idm(rp)[qt].min > idm(rp)[qt].max) {
					err = "reverse range value";
					goto fmterr;
				}
				break;
			default:			/* id number? */
				if ((c < '0' || c > '9') && c != '-') {
					err = "unrecognizable value";
					goto fmterr;
				}
				ungetc(c, fp);
				if (fscanf(fp, "%d )", &idm(rp)[qt].min) != 1) {
					err = "bad number id";
					goto fmterr;
				}
				idm(rp)[qt].max = idm(rp)[qt].min;
				break;
			}
		}
	endloop:
		rp->next = mp;
		mp = rp;
		nrules++;
	}
	fclose(fp);
	return(mp);
fmterr:
	sprintf(buf, "%s: %s for rule %d\n", file, err, nrules+1);
	eputs(buf);
	quit(1);
memerr:
	eputs("Out of memory in getmapping\n");
	quit(1);
	return NULL; /* pro forma return */
}

int idcmp(ID *id1, ID *id2)
{
	/* names are greater than numbers */
	if (id1->name == NULL)
		if (id2->name == NULL)
			return(id1->number - id2->number);
		else
			return(-1);
	else
		if (id2->name == NULL)
			return(1);
		else
			return(strcmp(id1->name, id2->name));
}

void fputidlist(IDLIST	*qp, FILE	*fp)
{
	int	fi;
	int	i;
					/* print numbers/ranges */
	fi = 0;
	for (i = 0; i < qp->nids && qp->id[i].name == NULL; i++)
		if (i > 0 && qp->id[i].number > qp->id[i-1].number+1) {
			if (i > fi+1)
				fprintf(fp, "[%d:%d]\n", qp->id[fi].number,
						qp->id[i-1].number);
			else
				fprintf(fp, "%d\n", qp->id[fi].number);
			fi = i;
		}
	if (i-1 > fi)
		fprintf(fp, "[%d:%d]\n", qp->id[fi].number,
				qp->id[i-1].number);
	else if (i > 0)
		fprintf(fp, "%d\n", qp->id[fi].number);
	for ( ; i < qp->nids; i++)
		fprintf(fp, "\"%s\"\n", qp->id[i].name);
}

void write_quals(QLIST *qlp, IDLIST idl[], FILE *fp)
{
	int	i;
	
	for (i = 0; i < qlp->nquals; i++)
		if (idl[i].nids > 0) {
			fprintf(fp, "qualifier %s begin\n", qlp->qual[i]);
			fputidlist(&idl[i], fp);
			fprintf(fp, "end\n");
		}
}

void fputword(char *s, FILE *fp)
{
	int		hasspace = 0;
	int		quote = 0;
	char	*cp;
	
  /* check if quoting needed */
	for (cp = s; *cp; cp++)
		if (isspace(*cp))
			hasspace++;
		else if (*cp == '"')
			quote = '\'';
		else if (*cp == '\'')
			quote = '"';

	if (hasspace || quote) {	/* output with quotes */
		if (!quote) quote = '"';
		fputc(quote, fp);
		fputs(s, fp);
		fputc(quote, fp);
	} else				/* output sans quotes */
		fputs(s, fp);
}

void printargs(int ac, char **av,	FILE *fp)
{
	while (ac-- > 0) {
		fputword(*av++, fp);
		fputc(ac ? ' ' : '\n', fp);
	}
}

char *iskip(char *s)			/* skip integer in string */
{
	while (isspace(*s))
		s++;
	if ((*s == '-') | (*s == '+'))
		s++;
	if (!isdigit(*s))
		return(NULL);
	do
		s++;
	while (isdigit(*s));
	return(s);
}

char *fskip(char *s)			/* skip float in string */
{
	char  *cp;

	while (isspace(*s))
		s++;
	if ((*s == '-') | (*s == '+'))
		s++;
	cp = s;
	while (isdigit(*cp))
		cp++;
	if (*cp == '.') {
		cp++; s++;
		while (isdigit(*cp))
			cp++;
	}
	if (cp == s)
		return(NULL);
	if ((*cp == 'e') | (*cp == 'E'))
		return(isspace(*++cp) ? NULL : iskip(cp));
	return(cp);
}


int isintd(char *s, char *ds)	/* check integer format with delimiter set */
{
	char  *cp;

	cp = iskip(s);
	return(cp != NULL && strchr(ds, *cp) != NULL);
}


int isflt(char *s)			/* check float format */
{
	char  *cp;

	cp = fskip(s);
	return(cp != NULL && *cp == '\0');
}

int isfltd(char *s, char *ds)	/* check integer format with delimiter set */
{
	char  *cp;

	cp = fskip(s);
	return(cp != NULL && strchr(ds, *cp) != NULL);
}


int badarg(int	ac, char **av, char	*fl)
{
	int  i;

	if (fl == NULL)
		fl = "";		/* no arguments? */
	for (i = 1; *fl; i++,av++,fl++) {
		if (i > ac || *av == NULL)
			return(-1);
		switch (*fl) {
		case 's':		/* string */
			if (**av == '\0' || isspace(**av))
				return(i);
			break;
		case 'i':		/* integer */
			if (!isintd(*av, " \t\r\n"))
				return(i);
			break;
		case 'f':		/* float */
			if (!isfltd(*av, " \t\r\n"))
				return(i);
			break;
		default:		/* bad call! */
			return(-1);
		}
	}
	return(0);		/* all's well */
}


int findid(IDLIST	*idl,	ID	*idp, int	insert)
{
	int  upper, lower;
	int  cm, i;
	
  /* binary search */
	lower = 0;
	upper = cm = idl->nids;
	while ((i = (lower + upper) >> 1) != cm) {
		cm = idcmp(idp, &idl->id[i]);
		if (cm > 0)
			lower = i;
		else if (cm < 0)
			upper = i;
		else
			return(i);
		cm = i;
	}
	if (!insert)
    		return(-1);
    	if (idl->nids == 0) {			/* create new list */
    		idl->id = (ID *)malloc(sizeof(ID));
    		if (idl->id == NULL)
    			goto memerr;
    	} else {				/* grow old list */
    		idl->id = (ID *)realloc((void *)idl->id,(idl->nids+1)*sizeof(ID));
    		if (idl->id == NULL)
    			goto memerr;
    		for (i = idl->nids; i > upper; i--) {
    			idl->id[i].number = idl->id[i-1].number;
    			idl->id[i].name = idl->id[i-1].name;
    		}
    	}
    	idl->nids++;				/* insert new element */
	idl->id[i].number = idp->number;
    	if (idp->name == NULL)
    		idl->id[i].name = NULL;
    	else
    		idl->id[i].name = savestr(idp->name);
	return(i);
memerr:
	eputs("Out of memory in findid\n");
	quit(1);
	return -1; /* pro forma return */
}

void fvsum(FVECT vres, const FVECT v0, const FVECT v1, double f)
{
	vres[0] = v0[0] + f*v1[0];
	vres[1] = v0[1] + f*v1[1];
	vres[2] = v0[2] + f*v1[2];
}


double normalize(FVECT v)
{
	double  len, d;
	
	d = DOT(v, v);
	
	if (d == 0.0)
		return(0.0);
	
	if (d <= 1.0+FTINY && d >= 1.0-FTINY) {
		len = 0.5 + 0.5*d;	/* first order approximation */
		d = 2.0 - len;
	} else {
		len = sqrt(d);
		d = 1.0/len;
	}
	v[0] *= d;
	v[1] *= d;
	v[2] *= d;

	return(len);
}

void fcross(FVECT vres, const FVECT v1, const FVECT v2)
{
	vres[0] = v1[1]*v2[2] - v1[2]*v2[1];
	vres[1] = v1[2]*v2[0] - v1[0]*v2[2];
	vres[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

int comp_baryc(BARYCCM *bcm, FVECT v1, FVECT v2, FVECT v3)
{
	RREAL	*vt;
	FVECT	va, vab, vcb;
	double	d;
	int	ax0, ax1;
  int	i;
	
  /* compute major axis */
	VSUB(vab, v1, v2);
	VSUB(vcb, v3, v2);
	VCROSS(va, vab, vcb);
	bcm->ax = ABS(va[0]) > ABS(va[1]) ? 0 : 1;
	bcm->ax = ABS(va[bcm->ax]) > ABS(va[2]) ? bcm->ax : 2;
	if ((ax0 = bcm->ax + 1) >= 3) ax0 -= 3;
	if ((ax1 = ax0 + 1) >= 3) ax1 -= 3;
	for (i = 0; i < 2; i++) {
		vab[0] = v1[ax0] - v2[ax0];
		vcb[0] = v3[ax0] - v2[ax0];
		vab[1] = v1[ax1] - v2[ax1];
		vcb[1] = v3[ax1] - v2[ax1];
		d = vcb[0]*vcb[0] + vcb[1]*vcb[1];
		if (d <= FTINY*FTINY)
			return(-1);
		d = (vcb[0]*vab[0]+vcb[1]*vab[1])/d;
		va[0] = vab[0] - vcb[0]*d;
		va[1] = vab[1] - vcb[1]*d;
		d = va[0]*va[0] + va[1]*va[1];
		if (d <= FTINY*FTINY)
			return(-1);
		d = 1.0/d;
		bcm->tm[i][0] = va[0] *= d;
		bcm->tm[i][1] = va[1] *= d;
		bcm->tm[i][2] = -(v2[ax0]*va[0]+v2[ax1]*va[1]);
					/* rotate vertices */
		vt = v1;
		v1 = v2;
		v2 = v3;
		v3 = vt;
	}
	return(0);
}


int flat_tri(FVECT v1, FVECT v2, FVECT v3, FVECT n1, FVECT n2, FVECT n3)	/* determine if triangle is flat */
{
	double	d1, d2, d3;
	FVECT	vt1, vt2, vn;
		
  /* compute default normal */
	VSUB(vt1, v2, v1);
	VSUB(vt2, v3, v2);
	VCROSS(vn, vt1, vt2);
	if (normalize(vn) == 0.0)
		return(DEGEN);
	
  /* compare to supplied normals */
	d1 = DOT(vn, n1); d2 = DOT(vn, n2); d3 = DOT(vn, n3);
	if (d1 < 0 && d2 < 0 && d3 < 0) {
		if (d1 > -COSTOL || d2 > -COSTOL || d3 > -COSTOL)
			return(RVBENT);
		return(RVFLAT);
	}
	if (d1 < COSTOL || d2 < COSTOL || d3 < COSTOL)
		return(ISBENT);
	return(ISFLAT);
}

void eval_baryc(RREAL wt[3], FVECT p, BARYCCM *bcm)		/* evaluate barycentric weights at p */
{
	double	u, v;
	int	i;
	
	if ((i = bcm->ax + 1) >= 3) i -= 3;
	u = p[i];
	if (++i >= 3) i -= 3;
	v = p[i];
	wt[0] = u*bcm->tm[0][0] + v*bcm->tm[0][1] + bcm->tm[0][2];
	wt[1] = u*bcm->tm[1][0] + v*bcm->tm[1][1] + bcm->tm[1][2];
	wt[2] = 1. - wt[1] - wt[0];
}


int get_baryc(RREAL wt[3], FVECT p, FVECT v1, FVECT v2, FVECT v3)
{
	BARYCCM	bcm;
	
	if (comp_baryc(&bcm, v1, v2, v3) < 0)
		return(-1);
	eval_baryc(wt, p, &bcm);
	return(0);
}


void put_baryc(BARYCCM *bcm, RREAL com[][3], int n)
{
	double	a, b;
	int	i;

	printf("%d\t%d\n", 1+3*n, bcm->ax);
	for (i = 0; i < n; i++) {
		a = com[i][0] - com[i][2];
		b = com[i][1] - com[i][2];
		printf("%14.8f %14.8f %14.8f\n",
			bcm->tm[0][0]*a + bcm->tm[1][0]*b,
			bcm->tm[0][1]*a + bcm->tm[1][1]*b,
			bcm->tm[0][2]*a + bcm->tm[1][2]*b + com[i][2]);
	}
}
// OBJ2RAD

/*
* Convert a Wavefront .obj file to Radiance format.
*
* Currently, we support only polygonal geometry.  Non-planar
* faces are broken rather haphazardly into triangles.
* Also, texture map indices only work for triangles, though
* I'm not sure they work correctly.  (Taken out -- see TEXMAPS defines.)
*/







int OBJ2RAD(int	argc, char	*argv[])
{
	int	donames = 0;
	int	i;

	for (i = 1; i < argc && argv[i][0] == '-'; i++)
		switch (argv[i][1]) {
		case 'o':		/* object name */
			defobj = argv[++i];
			break;
		case 'n':		/* just produce name list */
			donames++;
			break;
		case 'm':		/* use custom mapfile */
			ourmapping = getmapping(argv[++i], &qlist);
			break;
		case 'f':		/* flatten surfaces */
			flatten++;
			break;
		default:
			goto userr;
		}
	if ((i > argc) | (i < argc-1))
		goto userr;
	if (i == argc)
		inpfile = "<stdin>";
	else if (freopen(inpfile=argv[i], "r", stdin) == NULL) {
		fprintf(stderr, "%s: cannot open\n", inpfile);
		exit(1);
	}
	if (donames) {				/* scan for ids */
		getnames(stdin);
		printf("filename \"%s\"\n", inpfile);
		printf("filetype \"Wavefront\"\n");
		write_quals(&qlist, qual, stdout);
		printf("qualifier %s begin\n", qlist.qual[Q_FAC]);
		printf("[%d:%d]\n", 1, faceno);
		printf("end\n");
	} else {				/* translate file */
		printf("# ");
		printargs(argc, argv, stdout);
		convert(stdin);
	}
	if (ndegen)
		printf("# %d degenerate faces\n", ndegen);
	if (n0norm)
		printf("# %d invalid (zero) normals\n", n0norm);
	exit(0);
userr:
	fprintf(stderr, "Usage: %s [-o obj][-m mapping][-n][-f] [file.obj]\n",
			argv[0]);
	exit(1);
}

void getnames(FILE *fp)
{
	char	*argv[MAXARG];
	int	argc;
	ID	tmpid;
	int	i;

	while ( (argc = getstmt(argv, fp)) )
		switch (argv[0][0]) {
		case 'f':				/* face */
			if (!argv[0][1])
				faceno++;
			break;
		case 'u':
			if (!strcmp(argv[0], "usemtl")) {	/* material */
				if (argc < 2)
					break;		/* not fatal */
				tmpid.number = 0;
				tmpid.name = argv[1];
				findid(&qual[Q_MTL], &tmpid, 1);
			} else if (!strcmp(argv[0], "usemap")) {/* map */
				if (argc < 2 || !strcmp(argv[1], "off"))
					break;		/* not fatal */
				tmpid.number = 0;
				tmpid.name = argv[1];
				findid(&qual[Q_MAP], &tmpid, 1);
			}
			break;
		case 'o':		/* object name */
			if (argv[0][1] || argc < 2)
				break;
			tmpid.number = 0;
			tmpid.name = argv[1];
			findid(&qual[Q_OBJ], &tmpid, 1);
			break;
		case 'g':		/* group name(s) */
			if (argv[0][1])
				break;
			tmpid.number = 0;
			for (i = 1; i < argc; i++) {
				tmpid.name = argv[i];
				findid(&qual[Q_GRP], &tmpid, 1);
			}
			break;
		}
}


void convert(FILE	*fp)
{
	char	*argv[MAXARG];
	int	argc;
	int	nstats, nunknown;
	int	i;

	nstats = nunknown = 0;
					/* scan until EOF */
	while ( (argc = getstmt(argv, fp)) ) {
		switch (argv[0][0]) {
		case 'v':		/* vertex */
			switch (argv[0][1]) {
			case '\0':			/* point */
				if (badarg(argc-1,argv+1,"fff"))
					syntax("Bad vertex");
				newv(atof(argv[1]), atof(argv[2]),
						atof(argv[3]));
				break;
			case 'n':			/* normal */
				if (argv[0][2])
					goto unknown;
				if (badarg(argc-1,argv+1,"fff"))
					syntax("Bad normal");
				if (!newvn(atof(argv[1]), atof(argv[2]),
						atof(argv[3])))
					syntax("Zero normal");
				break;
			case 't':			/* texture map */
				if (argv[0][2])
					goto unknown;
				if (badarg(argc-1,argv+1,"ff"))
					goto unknown;
				newvt(atof(argv[1]), atof(argv[2]));
				break;
			default:
				goto unknown;
			}
			break;
		case 'f':				/* face */
			if (argv[0][1])
				goto unknown;
			faceno++;
			switch (argc-1) {
			case 0: case 1: case 2:
				syntax("Too few vertices");
				break;
			case 3:
				if (!puttri(argv[1], argv[2], argv[3]))
					syntax("Bad triangle");
				break;
			default:
				if (!putface(argc-1, argv+1))
					syntax("Bad face");
				break;
			}
			break;
		case 'u':
			if (!strcmp(argv[0], "usemtl")) {	/* material */
				if (argc < 2)
					break;		/* not fatal */
				strcpy(matname, argv[1]);
			} else if (!strcmp(argv[0], "usemap")) {/* map */
				if (argc < 2)
					break;		/* not fatal */
				if (!strcmp(argv[1], "off"))
					mapname[0] = '\0';
				else
					sprintf(mapname, "%s.hdr", argv[1]);
			} else
				goto unknown;
			break;
		case 'o':		/* object name */
			if (argv[0][1])
				goto unknown;
			if (argc < 2)
				break;		/* not fatal */
			strcpy(objname, argv[1]);
			break;
		case 'g':		/* group name(s) */
			if (argv[0][1])
				goto unknown;
			for (i = 1; i < argc; i++)
				strcpy(group[i-1], argv[i]);
			group[i-1][0] = '\0';
			break;
		case '#':		/* comment */
			printargs(argc, argv, stdout);
			break;
		default:;		/* something we don't deal with */
		unknown:
			nunknown++;
			break;
		}
		nstats++;
	}
	printf("\n# Done processing file: %s\n", inpfile);
	printf("# %d lines, %d statements, %d unrecognized\n",
			lineno, nstats, nunknown);
}


int getstmt(char *av[MAXARG], FILE	*fp)
{
	static char	sbuf[MAXARG*16];
	char	*cp;
	int	i;

	do {
		if (fgetline(cp=sbuf, sizeof(sbuf), fp) == NULL)
			return(0);
		i = 0;
		for ( ; ; ) {
			while (isspace(*cp) || *cp == '\\') {
				if (*cp == '\n')
					lineno++;
				*cp++ = '\0';
			}
			if (!*cp)
				break;
			if (i >= MAXARG-1) {
				fprintf(stderr,
			"warning: line %d: too many arguments (limit %d)\n",
					lineno+1, MAXARG-1);
				break;
			}
			av[i++] = cp;
			while (*++cp && !isspace(*cp))
				;
		}
		av[i] = NULL;
		lineno++;
	} while (!i);

	return(i);
}


char *getmtl(void)				/* figure material for this face */
{
	RULEHD	*rp = ourmapping;

	if (rp == NULL) {		/* no rule set */
		if (matname[0])
			return(matname);
		if (group[0][0])
			return(group[0]);
		return(defmat);
	}
					/* check for match */
	do {
		if (matchrule(rp)) {
			if (!strcmp(rp->mnam, VOIDID))
				return(NULL);	/* match is null */
			return(rp->mnam);
		}
		rp = rp->next;
	} while (rp != NULL);
					/* no match found */
	return(NULL);
}


char *getonm(void)				/* invent a good name for object */
{
	static char	name[64];
	register char	*cp1, *cp2;
	register int	i;
					/* check for preset */
	if (objname[0])
		return(objname);
	if (!group[0][0])
		return(defobj);
	cp1 = name;			/* else make name out of groups */
	for (i = 0; group[i][0]; i++) {
		cp2 = group[i];
		if (cp1 > name)
			*cp1++ = '.';
		while ( (*cp1 = *cp2++) )
			if (++cp1 >= name+sizeof(name)-2) {
				*cp1 = '\0';
				return(name);
			}
	}
	return(name);
}


int matchrule(RULEHD	*rp)
{
	ID	tmpid;
	int	gotmatch;
	int	i;

	if (rp->qflg & FL(Q_MTL)) {
		if (!matname[0])
			return(0);
		tmpid.number = 0;
		tmpid.name = matname;
		if (!matchid(&tmpid, &idm(rp)[Q_MTL]))
			return(0);
	}
	if (rp->qflg & FL(Q_MAP)) {
		if (!mapname[0])
			return(0);
		tmpid.number = 0;
		tmpid.name = mapname;
		if (!matchid(&tmpid, &idm(rp)[Q_MAP]))
			return(0);
	}
	if (rp->qflg & FL(Q_GRP)) {
		tmpid.number = 0;
		gotmatch = 0;
		for (i = 0; group[i][0]; i++) {
			tmpid.name = group[i];
			gotmatch |= matchid(&tmpid, &idm(rp)[Q_GRP]);
		}
		if (!gotmatch)
			return(0);
	}
	if (rp->qflg & FL(Q_OBJ)) {
		if (!objname[0])
			return(0);
		tmpid.number = 0;
		tmpid.name = objname;
		if (!matchid(&tmpid, &idm(rp)[Q_OBJ]))
			return(0);
	}
	if (rp->qflg & FL(Q_FAC)) {
		tmpid.name = NULL;
		tmpid.number = faceno;
		if (!matchid(&tmpid, &idm(rp)[Q_FAC]))
			return(0);
	}
	return(1);
}


int cvtndx(VNDX	vi, char	*vs)
{
  /* get point */
	vi[0] = atoi(vs);
	if (vi[0] > 0) {
		if (vi[0]-- > nvs)
			return(0);
	} else if (vi[0] < 0) {
		vi[0] += nvs;
		if (vi[0] < 0)
			return(0);
	} else
	return(0);
	
  /* get map */
	while (*vs)
		if (*vs++ == '/')
			break;
	vi[1] = atoi(vs);
	if (vi[1] > 0) {
		if (vi[1]-- > nvts)
			return(0);
	} else if (vi[1] < 0) {
		vi[1] += nvts;
		if (vi[1] < 0)
			return(0);
	} else
		vi[1] = -1;
					/* get normal */
	while (*vs)
		if (*vs++ == '/')
			break;
	vi[2] = atoi(vs);
	if (vi[2] > 0) {
		if (vi[2]-- > nvns)
			return(0);
	} else if (vi[2] < 0) {
		vi[2] += nvns;
		if (vi[2] < 0)
			return(0);
	} else
		vi[2] = -1;
					/* zero normal is not normal */
	if (vi[2] >= 0 && DOT(vnlist[vi[2]],vnlist[vi[2]]) <= FTINY)
		vi[2] = -1;
	return(1);
}


int nonplanar(int	ac, char	**av)
{
	VNDX	vi;
	RREAL	*p0, *p1;
	FVECT	v1, v2, nsum, newn;
	double	d;
  int	i;

	if (!cvtndx(vi, av[0]))
		return(0);
	if (!flatten && vi[2] >= 0)
		return(1);		/* has interpolated normals */
	if (ac < 4)
		return(0);		/* it's a triangle! */
					/* set up */
	p0 = vlist[vi[0]];
	if (!cvtndx(vi, av[1]))
		return(0);		/* error gets caught later */
	nsum[0] = nsum[1] = nsum[2] = 0.;
	p1 = vlist[vi[0]];
	fvsum(v2, p1, p0, -1.0);
	for (i = 2; i < ac; i++) {
		VCOPY(v1, v2);
		if (!cvtndx(vi, av[i]))
			return(0);
		p1 = vlist[vi[0]];
		fvsum(v2, p1, p0, -1.0);
		fcross(newn, v1, v2);
		if (normalize(newn) == 0.0) {
			if (i < 3)
				return(1);	/* can't deal with this */
			fvsum(nsum, nsum, nsum, 1./(i-2));
			continue;
		}
		d = fdot(newn,nsum);
		if (d >= 0) {
			if (d < (1.0-FTINY)*(i-2))
				return(1);
			fvsum(nsum, nsum, newn, 1.0);
		} else {
			if (d > -(1.0-FTINY)*(i-2))
				return(1);
			fvsum(nsum, nsum, newn, -1.0);
		}
	}
	return(0);
}


int putface(int	ac, char	**av)
{
	VNDX	vi;
	char	*cp;
	int	i;

	if (nonplanar(ac, av)) {	/* break into triangles */
		while (ac > 2) {
			if (!puttri(av[0], av[1], av[2]))
				return(0);
			ac--;		/* remove vertex & rotate */
			cp = av[0];
			for (i = 0; i < ac-1; i++)
				av[i] = av[i+2];
			av[i] = cp;
		}
		return(1);
	}
	if ((cp = getmtl()) == NULL)
		return(-1);
	printf("\n%s polygon %s.%d\n", cp, getonm(), faceno);
	printf("0\n0\n%d\n", 3*ac);
	for (i = 0; i < ac; i++) {
		if (!cvtndx(vi, av[i]))
			return(0);
		pvect(vlist[vi[0]]);
	}
	return(1);
}


int puttri(char *v1, char *v2, char *v3)
{
	char	*mod;
	VNDX	v1i, v2i, v3i;
	BARYCCM	bvecs;
	RREAL	bcoor[3][3];
	int	texOK = 0, patOK;
	int	flatness;
	int	i;

	if ((mod = getmtl()) == NULL)
		return(-1);

	if (!cvtndx(v1i, v1) || !cvtndx(v2i, v2) || !cvtndx(v3i, v3))
		return(0);
					/* compute barycentric coordinates */
	if (v1i[2]>=0 && v2i[2]>=0 && v3i[2]>=0)
		flatness = flat_tri(vlist[v1i[0]], vlist[v2i[0]], vlist[v3i[0]],
				vnlist[v1i[2]], vnlist[v2i[2]], vnlist[v3i[2]]);
	else
		flatness = ISFLAT;

	switch (flatness) {
	case DEGEN:			/* zero area */
		ndegen++;
		return(-1);
	case RVFLAT:			/* reversed normals, but flat */
	case ISFLAT:			/* smoothing unnecessary */
		texOK = 0;
		break;
	case RVBENT:			/* reversed normals with smoothing */
	case ISBENT:			/* proper smoothing */
		texOK = 1;
		break;
	}
	if (flatten)
		texOK = 0;
#ifdef TEXMAPS
	patOK = mapname[0] && (v1i[1]>=0 && v2i[1]>=0 && v3i[1]>=0);
#else
	patOK = 0;
#endif
	if (texOK | patOK)
		if (comp_baryc(&bvecs, vlist[v1i[0]], vlist[v2i[0]],
				vlist[v3i[0]]) < 0)
			texOK = patOK = 0;
					/* put out texture (if any) */
	if (texOK) {
		printf("\n%s texfunc %s\n", mod, TEXNAME);
		mod = TEXNAME;
		printf("4 dx dy dz %s\n", TCALNAME);
		printf("0\n");
		for (i = 0; i < 3; i++) {
			bcoor[i][0] = vnlist[v1i[2]][i];
			bcoor[i][1] = vnlist[v2i[2]][i];
			bcoor[i][2] = vnlist[v3i[2]][i];
		}
		put_baryc(&bvecs, bcoor, 3);
	}
#ifdef TEXMAPS
					/* put out pattern (if any) */
	if (patOK) {
		printf("\n%s colorpict %s\n", mod, PATNAME);
		mod = PATNAME;
		printf("7 noneg noneg noneg %s %s u v\n", mapname, TCALNAME);
		printf("0\n");
		for (i = 0; i < 2; i++) {
			bcoor[i][0] = vtlist[v1i[1]][i];
			bcoor[i][1] = vtlist[v2i[1]][i];
			bcoor[i][2] = vtlist[v3i[1]][i];
		}
		put_baryc(&bvecs, bcoor, 2);
	}
#endif
					/* put out (reversed) triangle */
	printf("\n%s polygon %s.%d\n", mod, getonm(), faceno);
	printf("0\n0\n9\n");
	if (flatness == RVFLAT || flatness == RVBENT) {
		pvect(vlist[v3i[0]]);
		pvect(vlist[v2i[0]]);
		pvect(vlist[v1i[0]]);
	} else {
		pvect(vlist[v1i[0]]);
		pvect(vlist[v2i[0]]);
		pvect(vlist[v3i[0]]);
	}
	return(1);
}


void freeverts(void)			/* free all vertices */
{
	if (nvs) {
		free((void *)vlist);
		nvs = 0;
	}
	if (nvts) {
		free((void *)vtlist);
		nvts = 0;
	}
	if (nvns) {
		free((void *)vnlist);
		nvns = 0;
	}
}


int newv(double	x, double	y, double	z)
{
	if (!(nvs%CHUNKSIZ)) {		/* allocate next block */
		if (nvs == 0)
			vlist = (FVECT *)malloc(CHUNKSIZ*sizeof(FVECT));
		else
			vlist = (FVECT *)realloc((void *)vlist,
					(nvs+CHUNKSIZ)*sizeof(FVECT));
		if (vlist == NULL) {
			fprintf(stderr,
			"Out of memory while allocating vertex %d\n", nvs);
			exit(1);
		}
	}
					/* assign new vertex */
	vlist[nvs][0] = x;
	vlist[nvs][1] = y;
	vlist[nvs][2] = z;
	return(++nvs);
}


int newvn(double	x, double	y, double	z)
{
	if (!(nvns%CHUNKSIZ)) {		/* allocate next block */
		if (nvns == 0)
			vnlist = (FVECT *)malloc(CHUNKSIZ*sizeof(FVECT));
		else
			vnlist = (FVECT *)realloc((void *)vnlist,
					(nvns+CHUNKSIZ)*sizeof(FVECT));
		if (vnlist == NULL) {
			fprintf(stderr,
			"Out of memory while allocating normal %d\n", nvns);
			exit(1);
		}
	}
					/* assign new normal */
	vnlist[nvns][0] = x;
	vnlist[nvns][1] = y;
	vnlist[nvns][2] = z;
	n0norm += (normalize(vnlist[nvns]) == 0.0);
	return(++nvns);
}


int newvt(double	x, double	y)
{
	if (!(nvts%CHUNKSIZ)) {		/* allocate next block */
		if (nvts == 0)
			vtlist = (RREAL (*)[2])malloc(CHUNKSIZ*2*sizeof(RREAL));
		else
			vtlist = (RREAL (*)[2])realloc((void *)vtlist,
					(nvts+CHUNKSIZ)*2*sizeof(RREAL));
		if (vtlist == NULL) {
			fprintf(stderr,
			"Out of memory while allocating texture vertex %d\n",
					nvts);
			exit(1);
		}
	}
					/* assign new vertex */
	vtlist[nvts][0] = x;
	vtlist[nvts][1] = y;
	return(++nvts);
}


void syntax(char	*er)
{
	fprintf(stderr, "%s: Wavefront syntax error near line %d: %s\n",
			inpfile, lineno, er);
	exit(1);
}


