#include "Tool.hpp"

const GLfloat Tool::PI = static_cast<GLfloat>(3.14159265358979323846);

void Tool::printShaderError(GLuint shaderId, GLint logLength, const char *source)
{
    // Allocation d'une chaine de caractère suffisement grande pour contenir le log  
    char *log(reinterpret_cast<char*>(malloc(sizeof(char) * (logLength + 1))));

    if (!log)
    {
      return ;
    }
    glGetShaderInfoLog(shaderId, logLength, 0, log);
    fprintf(stderr, 
	    "%s:%d, Vertex Shader error: %s\n%s", 
	    VPLOGL_PRETTY_FUNCTION,
	    __LINE__,
	    log,
	    source);
    delete log;
}


const char* Tool::dummyReadFile(const char* filePath, size_t sizeMax) 
{    
  char buffer[1024];
  GetCurrentDirectory(1024, buffer);

  FILE *file(filePath ? fopen(filePath, "r") : NULL);
  char *source(NULL);
  size_t sizeToRead(0);
  bool fileTooBig(false);
  
  if (!file) 
  {
    return 0;
  }

  // used calloc to have a preallocated buffer
  source = reinterpret_cast<char*>(calloc(sizeof(*source), sizeMax + 1));
  if (!source)
  {
    fprintf(stderr, "%s%d, calloc failed\n", VPLOGL_PRETTY_FUNCTION, __LINE__);
    return 0;
  }
  
  do
  {
    sizeToRead = fread(source, sizeMax, sizeof(*source), file);

    if (!sizeToRead && fileTooBig)
    {
      break;
    }
    else if (fileTooBig)
    {
      fprintf(stderr, 
	      "%s%d, can't read more data, file is too big(%lu)\n", 
	      VPLOGL_PRETTY_FUNCTION, 
	      __LINE__,
	      sizeMax);
      return 0;
    }
    fileTooBig = true;
  } while (sizeToRead == sizeMax);  
  fclose(file);

  return source;
}


GLuint Tool::loadShader(const char *vertexShaderFile, const char *fragmentShaderFile)
{
  const char* vertexShaderSource(dummyReadFile(vertexShaderFile));
  const char* fragmentShaderSource(NULL);
  GLuint vertexShader(0);
  GLuint fragmentShader(0);
  GLint compileStatus(0);
  GLuint program(0);
  GLint logLength(0);
  GLint linkStatus(0);

  if(!vertexShaderSource) 
  {
    fprintf(stderr, 
	    "%s%d, Unable to load vertexShaderFile: %s\n", 
	    VPLOGL_PRETTY_FUNCTION, 
	    __LINE__,
	    vertexShaderFile);
    return 0;
  }
  fragmentShaderSource = dummyReadFile(fragmentShaderFile);  

  if(!fragmentShaderSource) 
  {
    fprintf(stderr, 
	    "%s%d, Unable to load fragmentShaderFile: %s\n", 
	    VPLOGL_PRETTY_FUNCTION, 
	    __LINE__,
	    fragmentShaderFile);

    return 0;
  }

  // create vertexShader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);

  // vertexShader code spec                                      
  glShaderSource(vertexShader, 1, &vertexShaderSource, 0);
  // vertexShader compiling
  glCompileShader(vertexShader);
  /// Check Compilation
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);

  if(compileStatus == GL_FALSE) 
  {
    // Si echec, récupération de la taille du log de compilation 
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
    Tool::printShaderError(vertexShader, logLength, vertexShaderSource);
    return 0;
  }
  
  // Creation d'un Fragment Shader                                        
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  
  // Spécification du code source
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, 0);
  
  // Compilation du shader
  glCompileShader(fragmentShader);
  
  /// Vérification que la compilation a bien fonctionnée (très important !) 
  // Récupération du status de compilation 
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
  
  if(compileStatus == GL_FALSE) 
  {
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
    Tool::printShaderError(fragmentShader, logLength, fragmentShaderSource);
    return 0;
  }
  
  
  // Creation d'un programme
  program = glCreateProgram();

  // Attachement des shaders au programme
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  
  // Désallocation des shaders: ils ne seront réellement supprimés que lorsque le programme sera supprimé
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  
  // Edition de lien
  glLinkProgram(program);
  
  /// Vérification que l'édition de liens a bien fonctionnée (très important aussi !)
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  if(linkStatus == GL_FALSE) 
  {
    // Si echec, récupération de la taille du log de link
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    
    // Allocation d'une chaine de caractère suffisement grande pour contenir le log                                                          
    Tool::printShaderError(program, logLength, "");
    return 0;
  }
  
  delete [] vertexShaderSource;
  delete [] fragmentShaderSource;
  
  return program;
}






int Tool::getNextLine(char *incomingBuffer, char **bufferToFill, int maxLineSize)
{
  int count(-1);
  char c;

  while (++count < maxLineSize)
  {
    c = incomingBuffer[count];
    if (!c || c == '\n')
    {
      (*bufferToFill)[count] = 0;
      return count;
    }
    (*bufferToFill)[count] = c;
  }
  return count ? 0 : -1;
}



int Tool::strPos(char *stack, char *needle)
{
  char *p = strstr(stack, needle);
   if (p)
      return p - stack;
   return -1;
}

glm::vec3 Tool::pt3DFromMat4Vec4Mult(const glm::mat4& m,const glm::vec4& v) {
  glm::vec4 pt = m*v;
  return glm::vec3(pt.x,pt.y,pt.z)/pt.w;
}

glm::vec3 Tool::pt3DFromMat4Vec3Mult(const glm::mat4& m,const glm::vec3& v) {
  glm::vec4 pt = m*glm::vec4(v,1.0f);
  return glm::vec3(pt.x,pt.y,pt.z)/pt.w;
}

void Tool::computeDepthMatrixFromBBox(const glm::vec3 cornerFrustum[8],const glm::vec3 lightInvDir,
                                      glm::mat4& dViewMat,glm::mat4& dProjMat) {

  glm::vec3 frustumcentroid(0.0);
  glm::vec3 frustumCornersLS[8];
  for (int i = 0 ; i < 8 ; ++i) 
    frustumcentroid += cornerFrustum[i];
  
  frustumcentroid /= 8.0f;

  float distFromCentroid = 5.0f;
  glm::vec3 pos = frustumcentroid + (lightInvDir * distFromCentroid);

  glm::vec3 target = frustumcentroid;
  dViewMat = glm::lookAt(pos, target, glm::vec3(0.0,1.0,0.0));

  for (int i = 0 ; i < 8 ; ++i) frustumCornersLS[i]= pt3DFromMat4Vec3Mult(dViewMat,cornerFrustum[i]);

  glm::vec3 mins = frustumCornersLS[0];
  glm::vec3 maxs = frustumCornersLS[0];
  for (int i = 1; i < 8 ; ++i)
  {
    maxs = glm::max(frustumCornersLS[i],maxs);
    mins = glm::min(frustumCornersLS[i],mins);
  }

  dProjMat = glm::ortho<float>(mins.x, maxs.x, mins.y, maxs.y  , -maxs.z , -mins.z ); 
}



















// OBJ2MESH
//
//#ifndef lint
//static const char RCSid[] = "$Id: obj2mesh.c,v 2.13 2008/11/12 03:54:31 greg Exp $";
//#endif
///*
// *  Main program to compile a Wavefront .OBJ file into a Radiance mesh
// */
//
//#include "copyright.h"
//#include "paths.h"
//#include "platform.h"
//#include "standard.h"
//#include "resolu.h"
//#include "cvmesh.h"
//#include "otypes.h"
//
//extern int	o_face(); /* XXX should go to a header file */
//
//int	o_default() { return(O_MISS); }
//
//FUN  ofun[NUMOTYPE] = INIT_OTYPE;	/* needed for link resolution */
//
//char  *progname;			/* argv[0] */
//
//int  nowarn = 0;			/* supress warnings? */
//
//int  objlim = 9;			/* # of objects before split */
//
//int  resolu = 16384;			/* octree resolution limit */
//
//double	mincusize;			/* minimum cube size from resolu */
//
//static void addface(CUBE  *cu, OBJECT	obj);
//static void add2full(CUBE  *cu, OBJECT	obj);
//
//
//int
//main(		/* compile a .OBJ file into a mesh */
//	int  argc,
//	char  *argv[]
//)
//{
//	int  nmatf = 0;
//	char  pathnames[12800];
//	char  *pns = pathnames;
//	char  *matinp[128];
//	char  *cp;
//	int  i, j;
//
//	progname = argv[0];
//	ofun[OBJ_FACE].funp = o_face;
//
//	for (i = 1; i < argc && argv[i][0] == '-'; i++)
//		switch (argv[i][1]) {
//		case 'n':				/* set limit */
//			objlim = atoi(argv[++i]);
//			break;
//		case 'r':				/* resolution limit */
//			resolu = atoi(argv[++i]);
//			break;
//		case 'a':				/* material file */
//			matinp[nmatf++] = argv[++i];
//			break;
//		case 'l':				/* library material */
//			cp = getpath(argv[++i], getrlibpath(), R_OK);
//			if (cp == NULL) {
//				sprintf(errmsg,
//					"cannot find library material: '%s'",
//						argv[i]);
//				error(USER, errmsg);
//			}
//			matinp[nmatf++] = strcpy(pns, cp);
//			while (*pns++)
//				;
//			break;
//		case 'w':				/* supress warnings */
//			nowarn = 1;
//			break;
//		default:
//			sprintf(errmsg, "unknown option: '%s'", argv[i]);
//			error(USER, errmsg);
//			break;
//		}
//
//	if (i < argc-2)
//		error(USER, "too many file arguments");
//					/* initialize mesh */
//	cvinit(i==argc-2 ? argv[i+1] : "<stdout>");
//					/* load material input */
//	for (j = 0; j < nmatf; j++)
//		readobj(matinp[j]);
//					/* read .OBJ file into triangles */
//	if (i == argc)
//		wfreadobj(NULL);
//	else
//		wfreadobj(argv[i]);
//	
//	cvmeshbounds();			/* set octree boundaries */
//
//	if (i == argc-2)		/* open output file */
//		if (freopen(argv[i+1], "w", stdout) == NULL)
//			error(SYSTEM, "cannot open output file");
//	SET_FILE_BINARY(stdout);
//	newheader("RADIANCE", stdout);	/* new binary file header */
//	printargs(i<argc ? i+1 : argc, argv, stdout);
//	fputformat(MESHFMT, stdout);
//	fputc('\n', stdout);
//
//	mincusize = ourmesh->mcube.cusize / resolu - FTINY;
//
//	for (i = 0; i < nobjects; i++)	/* add triangles to octree */
//		if (objptr(i)->otype == OBJ_FACE)
//			addface(&ourmesh->mcube, i);
//
//					/* optimize octree */
//	ourmesh->mcube.cutree = combine(ourmesh->mcube.cutree);
//
//	if (ourmesh->mcube.cutree == EMPTY)
//		error(WARNING, "mesh is empty");
//	
//	cvmesh();			/* convert mesh and leaf nodes */
//
//	writemesh(ourmesh, stdout);	/* write mesh to output */
//	
//	/* printmeshstats(ourmesh, stderr); */
//
//	quit(0);
//	return 0; /* pro forma return */
//}
//
//
//void
//quit(				/* exit program */
//	int  code
//)
//{
//	exit(code);
//}
//
//
//void
//cputs(void)					/* interactive error */
//{
//	/* referenced, but not used */
//}
//
//
//void
//wputs(				/* warning message */
//	char  *s
//)
//{
//	if (!nowarn)
//		eputs(s);
//}
//
//
//void
//eputs(				/* put string to stderr */
//	register char  *s
//)
//{
//	static int  inln = 0;
//
//	if (!inln++) {
//		fputs(progname, stderr);
//		fputs(": ", stderr);
//	}
//	fputs(s, stderr);
//	if (*s && s[strlen(s)-1] == '\n')
//		inln = 0;
//}
//
//
//static void
//addface(			/* add a face to a cube */
//	register CUBE  *cu,
//	OBJECT	obj
//)
//{
//
//	if (o_face(objptr(obj), cu) == O_MISS)
//		return;
//
//	if (istree(cu->cutree)) {
//		CUBE  cukid;			/* do children */
//		int  i, j;
//		cukid.cusize = cu->cusize * 0.5;
//		for (i = 0; i < 8; i++) {
//			cukid.cutree = octkid(cu->cutree, i);
//			for (j = 0; j < 3; j++) {
//				cukid.cuorg[j] = cu->cuorg[j];
//				if ((1<<j) & i)
//					cukid.cuorg[j] += cukid.cusize;
//			}
//			addface(&cukid, obj);
//			octkid(cu->cutree, i) = cukid.cutree;
//		}
//		return;
//	}
//	if (isempty(cu->cutree)) {
//		OBJECT	oset[2];		/* singular set */
//		oset[0] = 1; oset[1] = obj;
//		cu->cutree = fullnode(oset);
//		return;
//	}
//					/* add to full node */
//	add2full(cu, obj);
//}
//
//
//static void
//add2full(			/* add object to full node */
//	register CUBE  *cu,
//	OBJECT	obj
//)
//{
//	OCTREE	ot;
//	OBJECT	oset[MAXSET+1];
//	CUBE  cukid;
//	register int  i, j;
//
//	objset(oset, cu->cutree);
//	cukid.cusize = cu->cusize * 0.5;
//
//	if (oset[0] < objlim || cukid.cusize <
//			(oset[0] < MAXSET ? mincusize : mincusize/256.0)) {
//						/* add to set */
//		if (oset[0] >= MAXSET) {
//			sprintf(errmsg, "set overflow in addobject (%s)",
//					objptr(obj)->oname);
//			error(INTERNAL, errmsg);
//		}
//		insertelem(oset, obj);
//		cu->cutree = fullnode(oset);
//		return;
//	}
//					/* subdivide cube */
//	if ((ot = octalloc()) == EMPTY)
//		error(SYSTEM, "out of octree space");
//					/* assign subcubes */
//	for (i = 0; i < 8; i++) {
//		cukid.cutree = EMPTY;
//		for (j = 0; j < 3; j++) {
//			cukid.cuorg[j] = cu->cuorg[j];
//			if ((1<<j) & i)
//				cukid.cuorg[j] += cukid.cusize;
//		}
//		for (j = 1; j <= oset[0]; j++)
//			addface(&cukid, oset[j]);
//		addface(&cukid, obj);
//					/* returned node */
//		octkid(ot, i) = cukid.cutree;
//	}
//	cu->cutree = ot;
//}




