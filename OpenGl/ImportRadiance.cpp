#include "ImportRadiance.hpp"

void ImportRadiance::Import(const char *filename, std::vector<Vector3f>& vertex, std::vector<int>& indice)
{
  std::ifstream fileContent(filename, std::ios::in);
  std::string s(filename);
  std::string contenu;
  int ind; 
  int oldInd; 
  int nbIndices = 0;

  if(fileContent == NULL)
  {
    return ;
  }
            
  while(std::getline(fileContent, contenu))
  {
    size_t searchScene = contenu.find("scene");
    size_t searchComment;
    size_t searchSpace;
	 
    if(searchScene != std::string::npos)
    {
      searchScene = contenu.find("=");
      if (searchScene != std::string::npos)
      {
        ind =(int) searchScene;
        oldInd = ind;
        ind += 2;
			  
        std::string text;
        text = VPLOGL_DATA_PATH;
        text += contenu.substr(ind, contenu.length() - ind);	  
        const char* c = text.c_str();
        std::ifstream f(c, std::ios::in);
			 
        if (f)
        {
          std::string scripText;

          while (std::getline(f, scripText))
          {
            char buf[20];
            sprintf(buf, "%s \n", scripText);


            searchComment = scripText.find("#");
            if (!(searchComment != std::string::npos))
            {
              if(scripText.length() > 0)
              {
	              size_t foundShapes = scripText.find("polygon");
	              searchSpace = scripText.find(" ");
	              if (foundShapes != std::string::npos)
	              {
		               if(int(searchSpace) != -1)
		              {
			              HandleShape(f, scripText, vertex, indice, nbIndices);
		              }
	              }
              }
             }
            }
          }			 
        }
      }  
   }
}

int ImportRadiance::Intersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) 
{	
  double x12 = x2 - x1;
  double y12 = y2 - y1;
  double x34 = x4 - x3;
  double y34 = y4 - y3;
  double x13 = x3 - x1;
  double y13 = y3 - y1;
  double t;
  double u;
		
  double area = (x12 * y34 - x34 * y12);
		
  if(0 == area)
  {
	  return -1;
  }
	
  t = x13 * y34 - x34 * y13;
  if (((t / area) <= 0) || ((t / area) >= 1))
  {
	  return 1;
  }
	
  u = x13 * y12 - x12 * y13;
  if(((u / area) <= 0) || ((u / area) >= 1))
  {
	  return 1;
  }
	
  return 0;
}

//Nb appel différents
float ImportRadiance::Area(Vector3dVector &contour)
{
	int n = contour.size();
	float a = 0.0f;
	for (int p = n - 1, q = 0; q < n; p = q++)
	{
		a += contour[p]._x * contour[q]._y - contour[q]._x * contour[p]._y;
	}
				
	return a * 0.5f;
}

bool ImportRadiance::InsideTriangle(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Px, float Py)
{
	float ax;
	float ay;
	float bx;
	float by;
	float cx;
	float cy;
	float apx;
	float apy;
	float bpx;
	float bpy;
	float cpx;
	float cpy;
	
	float cCROSSap;
	float bCROSScp;
	float aCROSSbp;
	
	ax = Cx - Bx;
	ay = Cy - By;
	bx = Ax - Cx;
	by = Ay - Cy;
	cx = Bx - Ax;
	cy = By - Ay;
	apx = Px - Ax;
	apy = Py - Ay;
	bpx = Px - Bx;
	bpy = Py - By;
	cpx = Px - Cx;
	cpy = Py - Cy;
	
	aCROSSbp = ax * bpy - ay * bpx;
	cCROSSap = cx * apy - cy * apx;
	bCROSScp = bx * cpy - by * cpx;
	if ((aCROSSbp >= 0.0f) && (bCROSScp >= 0.0f) && (cCROSSap >= 0.0f))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void ImportRadiance::Reorder(Vector3dVector& in , Vector3dVector& out,  VectorInt& in_out)
{
  double area = Area(in);
  bool invert = false;
  bool intersect = true;
  VectorInt pts;
  std::list<seg*> a, b;

  if (area < 0)
  {
	  invert = true;
	  for (int i = 0 ; i < (int)in.size(); ++i)
	  {
		  in[i].Set(in[i]._x, -in[i]._y);
	  }
  }
	
  int sze = in.size();
  for (int i = 0 ; i < sze ; ++i)
  {
	  int ref = i;
	  for (int j = 0 ; j < i ; ++j)
	  {
		  if (( (in[i]._x == in[j]._x) && (in[i]._y == in[j]._y)) && (ref == i))
		  {
			  ref = j;
		  }
	  }
	  pts.push_back(ref);
  }
				

  for (int i = 0 ; i < sze; ++i)
  {
	  seg* s = new seg();
	  s->_a = pts[i];
	  s->_b = pts[(i + 1) % sze];
	  s->_angle = atan2(in[s->_b]._y - in[s->_a]._y, in[s->_b]._x - in[s->_a]._x);
	  if( i > 0)
	  {
		  a.push_back(s);
	  }
	  else
	  {
		  b.push_back(s);
	  }
  }
	

		
  while(intersect)
  {
	  intersect = false;
	  for (std::list<seg*>::iterator it = a.begin(), itEnd(a.end()); it != itEnd; ++it)
	  {
		  for (std::list<seg*>::iterator it2 = it, it2End(a.end()); it2 != it2End; ++it2)
		  {
			  if(false == intersect && 
           it != it2)
			  {
				  if ((*it)->_a == (*it2)->_b && 
              (*it2)->_a == (*it)->_b)
				  {
					  bool found = false;
					  int compteur = 0;

					  for (std::list<seg*>::iterator it3 = a.begin(); it3 != a.end(); ++it3)
					  {
							
						  compteur++;							
						  if ((!found) && 
                  (*it)->_a != (*it3)->_a && 
                  (*it)->_b != (*it3)->_a && 
                  (*it)->_a != (*it3)->_b && 
                  (*it)->_b != (*it3)->_b && 

						      !Intersect(in[(*it)->_a]._x, 
                                  in[(*it)->_a]._y,
                                  in[(*it)->_b]._x, 
                                  in[(*it)->_b]._y,
                                  in[(*it3)->_a]._x, 
                                  in[(*it3)->_a]._y,
                                  in[(*it3)->_b]._x, 
                                  in[(*it3)->_b]._y))
								    {
									    found = true;
								    } 
					  }
					  if(found)
					  {
						  std::vector<int> posspnts;

						  for (int i = 0 ; i < (int) in.size(); ++i)
						  {
							  posspnts.push_back(0);
						  }
														
						  posspnts[(*it)->_a] = -1;
						  posspnts[(*it)->_b] = 1;
							
						  int compt = 0;
							
						  for (int i = 0; i < (int) posspnts.size() ; ++i)
						  {
							  if(posspnts[i] != 0)
							  {
								  compt++;
							  }
						  }
										

						  std::list<seg*> c;
							
						  for (std::list<seg*>::iterator it3 = a.begin(), it3End(a.end()); it3 != it3End; ++it3)
						  {
							  c.push_back((*it3));
						  }
							
              int nbCall4 = 0;

						  while(c.size() > 0)
						  {
							  for (std::list<seg*>::iterator it3 = c.begin(); it3 != c.end(); ++it3)
							  {
								  if(posspnts[(*it3)->_a] != 0)
								  {
									  posspnts[(*it3)->_b] = posspnts[(*it3)->_a];

									  it3 = c.erase(it3);

                    if(it3 == c.end())
                    {
                        break;
                    }
								  }
								  else if(posspnts[(*it3)->_b] != 0)
								  {
									  posspnts[(*it3)->_a] = posspnts[(*it3)->_b];
									  it3 = c.erase(it3);

                    if(it3 == c.end())
                    {
                        break;
                    }
								  }
							  }

						  }
							
						  double dist = 0.0;
						  int from = -1;
						  int to = -1;
						  for (int i = 0 ; i < (int) in.size() ; ++i)
						  {
							  for (int j = 0 ; j < (int) in.size(); ++j)
							  {
								  if(i != j && 
                     posspnts[i] == -1 && 
                     posspnts[j] == 1)
								  {
									  double ndist = (in[j]._x - in[i]._x) * 
                                   (in[j]._x - in[i]._x) + 
                                   (in[j]._y - in[i]._y) * 
                                   (in[j]._y - in[i]._y);
										
									  if(from == -1 || 
                       (ndist > 0.0 && ndist < dist))
									  {
										  bool OK = true;
										  for (std::list<seg*>::iterator it3 = a.begin() ; it3 != a.end(); ++it3)
										  {
											  if ((OK) && 
                            i != (*it3)->_a && 
                            j != ((*it3)->_a) && 
                            i != ((*it3)->_b) && 
                            j != ((*it3)->_b) && 
											      0 == Intersect(in[i]._x, 
                                           in[i]._y, 
                                           in[j]._x, 
                                           in[j]._y, 
                                           in[(*it3)->_a]._x, 
                                           in[(*it3)->_a]._y, 
                                           in[(*it3)->_b]._x, 
                                           in[(*it3)->_b]._y))
											  {
                          OK = false;
                          break;
											  }
										  }
										  if(OK)
										  {
											  dist = ndist;
											  from = i;
											  to = j;
										  }
									  }
								  }
							  }
						  }
							
						  (*it)->_a = from;
						  (*it)->_b = to;
						  (*it2)->_a = to;
						  (*it2)->_b = from;
							
						  intersect = true;
					  }
				  }
			  }
		  }
	  } 
  }
		
  bool dout = false;
	
  while(a.size() > 0) 
  {	
		int ID = (*(b.rbegin()))->_b;
		int IDF = (*(b.rbegin()))->_a;
		double cang = (*(b.rbegin()))->_angle;
		seg* res = NULL;
		double best = -4.0; // Anything smaller than -PI
		bool reverse = false;

    for(std::list<seg*>::iterator it = a.begin(); it != a.end(); ++it) 
	  {
		  if ((*it)->_a == ID && 
          (*it)->_b != IDF) 
		  {
        #ifdef _WIN32
          double nval = Remainder(((*it)->_angle) - cang, 2 * 3.14);
        #else
          double nval = remainder(((*it)->angle) - cang, 2*M_PI);
        #endif

			  if (nval > best) 
			  { 
				  best = nval; 
				  res = *it; 
				  reverse = false; 
			  }
			 } 
			 else if ((*it)->_b == ID && 
                (*it)->_a != IDF) 
			 {
          #ifdef _WIN32
            double nval = Remainder(((*it)->_angle) + 3.14f - cang, 2 * 3.14);
          #else
				    double nval = remainder(((*it)->angle) + M_PI-cang, 2*M_PI);
          #endif

				  if (nval > best) 
				  { 
					  best=nval; 
					  res=*it; 
					  reverse=true; 
				  }
			} 
		}
		
		dout = false;
		if (res==NULL) 
		{
		  fprintf(stderr, "Error in function Reorder\n" );
		  exit(-1);
		}
		b.push_back(res);
		if (reverse) 
		{ 
			int tmp = res->_a; 
			res->_a = res->_b; 
			res->_b = tmp; 
			res->_angle += M_PI; 
		}
		for(std::list<seg*>::iterator it=a.begin(); it!=a.end(); ++it) 
		{
		  if (*it == res) 
		  {
			  *it= *(a.begin());
		  }
		}
		a.pop_front();	
	}

  for (std::list<seg*>::iterator it = b.begin(); it != b.end(); ++it)
  {
	  out.push_back(Vector3f(in[(*it)->_a]._x, in[(*it)->_a]._y, 0.0f));
	  in_out.push_back((*it)->_a);
    delete *it;
  }
		
  if(invert)
  {
	  for (int i = 0; i < (int) in.size() ; ++i)
	  {
		  in[i].Set(in[i]._x, -in[i]._y);
	  }
  }

}



bool ImportRadiance::Project(Vector3f* in , int n , Vector3f* out, Vector3f& u, Vector3f& v, Vector3f& w)
{
  float best = 0.0f;
  float nrm = 0.0f;   
  Vector3f bw;
	float s;

  if (n < 3)
  {
	  fprintf(stderr, " WARNING : Polygon hass less than 3 points (no triangle needed)");
	  return false;
  }
	  
  u._x = in[1]._x - in[0]._x;
  u._y = in[1]._y - in[0]._y;
  u._z = in[1]._z - in[0]._z;
	 
  for (int i = 0 ; i < n ; ++i)
  {
	  for (int j = i + 1; j < n ; ++j)
	  {
		  for (int k = j + 1; k < n ; ++k)
		  {
			  bw._x = (in[j]._y - in[i]._y) * (in[k]._z - in[i]._z) - (in[j]._z - in[i]._z) * (in[k]._y - in[i]._y);
			  bw._y = (in[j]._z - in[i]._z) * (in[k]._x - in[i]._x) - (in[j]._x - in[i]._x) * (in[k]._z - in[i]._z);
			  bw._z = (in[j]._x - in[i]._x) * (in[k]._y - in[i]._y) - (in[j]._y - in[i]._y) * (in[k]._x - in[i]._x);
			  
			  s = bw._x * bw._x + bw._y * bw._y + bw._z * bw._z;
				  
			  if(s > best)
			  {
				  w = bw;
				  best = s;
			  }
		  }
	  }
  }
	  
  v._x = w._y * u._z - u._y * w._z;
  v._y = w._z * u._x - u._z * w._x;
  v._z = w._x * u._y - u._x * w._y;
	  
  nrm = sqrt(u._x * u._x + u._y * u._y + u._z * u._z);
	  
  if (nrm == 0)
  {
	  return false;
  }
	  
  u._x /= nrm;
  u._y /= nrm;
  u._z /= nrm;
	  
  nrm = sqrt(v._x * v._x + v._y * v._y + v._z * v._z);
	  		
  if(nrm == 0)
  {
	  return false;
  }
	  
  v._x /= nrm;
  v._y /= nrm;
  v._z /= nrm;
	  
  nrm = sqrt(w._x * w._x + w._y * w._y + w._z * w._z);
  if(nrm == 0)
  {
	  return false;
  }
	  
  w._x /= nrm;
  w._y /= nrm;
  w._z /= nrm;
	  	  
  for (int i = 0 ; i < n ; ++i)
  {
		  
	  out[i]._x = (in[i]._x - in[0]._x) * u._x + (in[i]._y - in[0]._y) * u._y + (in[i]._z - in[0]._z) * u._z;
	  out[i]._y = ((in[i]._x - in[0]._x) * v._x + (in[i]._y - in[0]._y) * v._y + (in[i]._z - in[0]._z) * v._z);
	  out[i]._z =(in[i]._x - in[0]._x) * w._x + (in[i]._y - in[0]._y) * w._y + (in[i]._z - in[0]._z) * w._z;
		  
  }
		
  return true;
}
  		 
int ImportRadiance::PolygoneTriangulation(int nbPoint, Vector3f *tab, Vector3f *pXYZ, ITRIANGLE *pITriangle)
{
	Vector3f *projectVector;
	Vector3f u;
	Vector3f v;
	Vector3f w;
	int count = 0;	  
  int nbTri(0);
	projectVector = new Vector3f[nbPoint];
	  
	if(Project(pXYZ, nbPoint, projectVector, u, v,w))
	{
		int tcount = 0;
		float area;
		Vector3dVector a;
		Vector3dVector b;
		VectorInt in_out;
		for (int i = 0 ; i < nbPoint; ++i)
		{  
			a.push_back(Vector3f(projectVector[i]._x, projectVector[i]._y, projectVector[i]._z)); 
		}
		  
		area = Area(a);
		VectorInt result;
		  		  
		Reorder(a, b, in_out);		
		Process(b, result);
		 
		tcount = result.size() / 3;

		for (int i = 0 ; i < tcount; ++i)
		{
			float s = 0.0f;
			float abcw = 0.0f;

			Vector3f va = pXYZ[in_out[result[i * 3]]];
			Vector3f vb = pXYZ[in_out[result[i * 3 + 1]]];
			Vector3f vc = pXYZ[in_out[result[i * 3 + 2]]];

			Vector3f ab = va.Dist(vb);
			Vector3f bc = vb.Dist(vc);
			Vector3f abc = ab.Cross(bc);
			  
			abcw = abc.Dot(w);

			s = area * abcw;
			  
      pITriangle[i].p1 = in_out[result[i * 3]];
			if (s >= 0.0f)
			{		  				
				pITriangle[i].p2 = in_out[result[i * 3 + 1]];
				pITriangle[i].p3 = in_out[result[i * 3 + 2]];
			}
			else
			{				
				pITriangle[i].p2 = in_out[result[i * 3 + 2]];
				pITriangle[i].p3 = in_out[result[i * 3 + 1]];
			}

			++nbTri;
		}
		++count;			
	}
  delete [] projectVector;

  return nbTri;
}
 

# ifdef _WIN32
bool ImportRadiance::Snip(Vector3dVector &contour, int u, int v, int w , int n , int *V)
{	
	int p; 
	float Ax; 
  float Ay; 
  float Bx; 
  float By;
  float Cx;
  float Cy; 
  float Px;
  float Py;

	Ax = contour[V[u]]._x;
	Ay = contour[V[u]]._y;
	Bx = contour[V[v]]._x;
	By = contour[V[v]]._y;
	Cx = contour[V[w]]._x;
	Cy = contour[V[w]]._y;
	
	if( EPSILON > (Bx - Ax) * (Cy - Ay) - (By - Ay) * ( Cx -Ax))
	{
		return false;
	}

	for (p = 0 ; p < n ; ++p)
	{
		Px = contour[V[p]]._x;
		Py = contour[V[p]]._y;
		
		if(p == u || 
       p == v || 
       p == w)
		{
			continue;
		}

    
    if(Absolute(Px, Ax) < 10E-6 && 
       Absolute(Py, Ay) < 10E-6)
		{
			continue;
		}

    if(Absolute(Px , Bx) < 10E-6 && 
       Absolute(Py , By) < 10E-6)
		{
			continue;
		}

    if (Absolute(Px, Cx) < 10E-6 && 
        Absolute(Py, Cy) < 10E-6)
    {
			continue;
		}

		if (InsideTriangle(Ax, Ay, Bx, By, Cx , Cy, Px, Py))
		{
			return false;
		}

	}
		
	return true;
}

#else

bool ImportRadiance::Snip(Vector3dVector &contour, int u, int v, int w , int n , int *V)
{	
	int p; 
	float Ax; 
  float Ay; 
  float Bx; 
  float By;
  float Cx;
  float Cy; 
  float Px;
  float Py;

	Ax = contour[V[u]]._x;
	Ay = contour[V[u]]._y;
	Bx = contour[V[v]]._x;
	By = contour[V[v]]._y;
	Cx = contour[V[w]]._x;
	Cy = contour[V[w]]._y;
	
	if( EPSILON > ( ( ( Bx - Ax) * (Cy - Ay) ) - ( (By - Ay) * ( Cx -Ax) ) ))
	{
		return false;
	}

	for (p = 0 ; p < n ; ++p)
	{
		Px = contour[V[p]]._x;
		Py = contour[V[p]]._y;
		
		if( (p == u) || ( p ==v) || (p == w))
		{
			continue;
		}

    
    if((abs(Px - Ax) < 10E-6) && (abs(Py - Ay) < 10E-6))
    {
			continue;
		}

    if( (abs(Px - Bx) < 10E-6) && (abs(Py - By) < 10E-6))
    {
			continue;
		}

    if ( (abs(Px-Cx) < 10E-6) && (abs(Py-Cy) < 10E-6))
    {
			continue;
		}

		if (InsideTriangle(Ax, Ay, Bx, By, Cx , Cy, Px, Py))
		{
			return false;
		}
	}
		
	return true;
}
#endif

bool ImportRadiance::Process(Vector3dVector &contour, VectorInt & result)
{
	
	int n = contour.size();
	int nbTriangle = 0;

	if( n < 3)
	{
		return false;
	}

	int *V = new int[n];

	if (0.0f < Area(contour))
	{
		for (int v = 0 ; v < n ; ++v)
		{
			V[v] = v;
		}
	}
	else
	{
		for (int v = 0; v < n ; ++v)
		{
			V[v] = (n-1) - v;
		}
	}
	
	int nv = n;
	int count = 2 * nv;
	
	for (int m = 0 , v = nv - 1 ; nv >2;)
	{		
		if ( 0 >= count--)
		{
			return false;
		}

		int u = v ; 
							
		if (nv <= u)
		{
			u = 0;     /* previous */
		} 

		v = u + 1;
		

		if (nv <= v)
		{
			v = 0;     /* new v  */  
		} 
			
		int w = v + 1; 
						
		if (nv <= w) 
		{
			w = 0;     /* next   */  
		}

		if (Snip(contour, u, v, w, nv, V))
		{
			nbTriangle++;
			int a;
			int b;
			int c;
			int s;
			int t;
			a = V[u];
			b = V[v];
			c = V[w];

			result.push_back(a);
			result.push_back(b);
			result.push_back(c);
			
			for (s = v , t = v + 1; t < nv ; ++s, ++t)
			{
				V[s] = V[t];
			}

			nv--;
			count = 2 * nv;

		}
	}
	
	delete [] V;
	return true;
}

void ImportRadiance::HandleShape(std::ifstream& f, std::string shapes, std::vector<Vector3f>& vertex, std::vector<int>& indice, int &nbIndices)
{
	std::vector<Vector3f> positions;
	std::vector<Vector3f> triangles;
	std::vector<Vector3f> normals;
  size_t searchSpace;
	Vector3f ** tabNormal = new Vector3f*[3];
  std::string strArgument;
  int nbTri(0);
	int ** tabIndices = new int*[3];
	int tabnbShapes[3];
  int IndIncide(0);
	std::string tab[3];
	float ** tabargs = new float*[3];
	Vector3f * pXYZ;
	Vector3f* tabXYZ;
	ITRIANGLE *pITriangle;
	int ind(0);
	int oldInd(0);

	for (int i = 0 ; i < 3 ; ++i)
	{
		searchSpace = shapes.find(" " , ind);
		ind = (int) searchSpace;
		strArgument = shapes.substr(oldInd, ind - oldInd);
		oldInd = ind;
		ind++;
		tab[i] = strArgument;
	}

	for (int i = 0 ; i < 3 ; ++i)
	{
		int nbShapes;
    int thirdNbShape;
		int nbCarac;
		int nbVal = 0;
		ind = 0;
		oldInd = 0;

		getline(f,shapes);
		
		searchSpace = shapes.find(" ",ind);
		ind = (int) searchSpace;

		strArgument = shapes.substr(oldInd, ind - oldInd);

		nbShapes = atoi(strArgument.c_str());    
		if (nbShapes == 0)
		{
      continue;
    }

    thirdNbShape = nbShapes / 3;
		oldInd = ind;
		++ind;

		tabnbShapes[i] = nbShapes;
		tabIndices[i] = new int[thirdNbShape];
		tabNormal[i] = new Vector3f[thirdNbShape];

		for (int j = 0 ; j < nbShapes / 3 ; ++j)
		{
			tabNormal[i][j] = Vector3f(0.0f, 0.0f, 0.0f);
		}

		tabargs[i] = new float[nbShapes];		
		std::vector<int> compteIndice;
		compteIndice.clear();

		for (int j = 0 ; j < thirdNbShape ; ++j)
		{
			getline(f,shapes);

			ind = 1;
			oldInd = 0;
			nbCarac = 0;

			tabIndices[i][IndIncide] = IndIncide;
			IndIncide++;

			while(nbCarac < 3)
			{
				searchSpace = shapes.find(" " , ind);
				ind = (int) searchSpace;
				strArgument = shapes.substr(oldInd, ind - oldInd);
				if (strArgument.compare(" ") != 0)
				{
					float v = (float) atof(strArgument.c_str());
					tabargs[i][nbVal++] =v;
					nbCarac++;
				}

				oldInd = ind;
				++ind;
			}
		}

		pXYZ = new Vector3f[thirdNbShape];
		pITriangle = new ITRIANGLE[nbShapes];
		tabXYZ = new Vector3f[thirdNbShape];
		int ind = 0;

		for (int i = 0 ; i < nbShapes; i +=3)
		{
			pXYZ[ind]._x = tabXYZ[ind]._x = tabargs[2][i];
			pXYZ[ind]._y = tabXYZ[ind]._y = tabargs[2][i + 1];
			pXYZ[ind]._z = tabXYZ[ind]._z = tabargs[2][i + 2];
			ind++;
		}


		nbTri = PolygoneTriangulation(thirdNbShape, tabXYZ, pXYZ, pITriangle);
		for (int i = 0 ; i < thirdNbShape ; ++i)
		{
			vertex.push_back(Vector3f(pXYZ[i]._x, pXYZ[i]._y, pXYZ[i]._z));
		}
							
		for (int i = 0 ; i < nbTri ; ++i)
		{
							
			indice.push_back(pITriangle[i].p1 + nbIndices);
			indice.push_back(pITriangle[i].p2 + nbIndices);
			indice.push_back(pITriangle[i].p3 + nbIndices);					
		}
						
		nbIndices += thirdNbShape;
		delete [] tabIndices;
		delete [] tabNormal;			
	}
	return;

}
