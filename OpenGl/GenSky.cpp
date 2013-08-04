#include "GenSky.hpp"

const int GenSky::CLEAR = 1;
const int GenSky::OVER = 2;
const int GenSky::UNIF = 3;
const int GenSky::INTER = 4;

const float GenSky::SUNEFFICACY = 208.0f; 	/* standard illuminant D65 */
const float GenSky::SKYEFFICACY = 203.0f; /* skylight (should be 110) */

double GenSky::_smeridian = 0.0;
//20.944 meridien de base

zone GenSky::tzone[] = {
	{"YST", 135}, {"YDT", 120},
	{"PST", 120}, {"PDT", 105},
	{"MST", 105}, {"MDT", 90},
	{"CST", 90}, {"CDT", 75},
	{"EST", 75}, {"EDT", 60},
	{"AST", 60}, {"ADT", 45},
	{"NST", 52.5}, {"NDT", 37.5},
	{"GMT", 0}, {"BST", -15},
	{"CET", -15}, {"CEST", -30},
	{"EET", -30}, {"EEST", -45},
	{"AST", -45}, {"ADT", -60},
	{"GST", -60}, {"GDT", -75},
	{"IST", -82.5}, {"IDT", -97.5},
	{"JST", -135}, {"NDT", -150},
	{"NZST", -180}, {"NZDT", -195},
	{"", 0}
};

//EDT : Eastern Daylight Time ( UTC - 04 : 00)
//EST : Eastern Standard Time
//AST : Atlantic Standard Time ( UTC - 04 : 00)
//MST : Mountain Standard Time ( UTC - 07 : 00)
//BST : british summer time ( UTC + 01:00)
//PST : Pacific standard Time ( UTC - 08 : 00)
//CST : Center Stardard Time (UTC - 06 : 00)
//CET : Central European Time ( UTC + 01 : 00)
//CEST : Central European Summer Time ( UTC + 02 : 00)

//gensky 7 4 14:30EDT +s −a 42 −o 89
//gensky −ang 23 −40 −s

GenSky::GenSky()
  : 
// private var
    _latitude(0.0),
    _longitude(0.0),
    _meridian(0.0),
    _groundbr(0.0),
    _f2(0.0),
    _solarbr(0.0),
    _uSolar(0),
    _progname(0),
    // satic array
    //_errmsg(""),
    _month(0),
    _day(0),
    _hour(0.0),
    _solar(0),
    _altitude(0.0),
    _azimuth(0.0),
    _skytype(CLEAR),
    _dosun(0),
    _zenithbr(0.0),
    _uzenith(0),
    _turbidity(0.45f),
    _gprefl(0.015f),
    // c++0x
    //_sundir({0, 0, 0}),
    _sundirVec(glm::vec3(0.0f, 0.0f, 0.0f))
{

  int oldInd = 0;
  int ind = 0;

  std::string text;
  std::string s;
  std::string data;

  text = VPLOGL_DATA_PATH;
  text += "sky.txt";

  std::ifstream f(text.c_str(), std::ios::in);

  if(f)
  {
     while (std::getline(f, s))
     {
       size_t search = s.find("GenSky");
       
       if (search != std::string::npos)
       {
         ind = (int) search;
         oldInd = (int) search;

         search = s.find(" ", ind);

         if (search != std::string::npos)
         {
           ind = (int) search;
         }

         oldInd = ind;

         ind++;

         //Extraction du mois

         search = s.find(" ", ind);

         if (search != std::string::npos)
         {
           ind = (int) search;

           data = s.substr(oldInd, ind - oldInd);

           _month = atoi(data.c_str());

           if (_month < 0 || _month > 12)
           {
             exit(1);
           }
         }

         oldInd = ind;
         ind++;

         //extraction du jour

        search = s.find(" ", ind);

         if (search != std::string::npos)
         {
           ind = (int) search;

           data = s.substr(oldInd, ind - oldInd);

           _day = atoi(data.c_str());

           if (_day < 0 || _day > 31)
           {
             exit(1);
           }
         }

         oldInd = ind;
         ind++;

         search = s.find(" ", ind);

         if (search != std::string::npos)
         {
           ind = (int) search;

           data = s.substr(oldInd, ind - oldInd);

           size_t min = s.find(":", oldInd);

           if(min != std::string::npos)
           {
             int minInd = 0;
             std::string h = s.substr(oldInd, (int)min - oldInd);
             minInd = (int) min;
             min = s.find(" ",minInd);
             std::string m = s.substr((minInd + 1), (int)min - (minInd + 1) );
             _hour = static_cast<float>(atoi(h.c_str()) + atoi(m.c_str()) / 60.0);
           }
           else
           {
             _hour = static_cast<float>(atof(data.c_str()));
           }

         }

         //Extraction du méridien
         oldInd = ind;
         ind++;

         search = s.find(" ", ind);
         if(search != std::string::npos)
         {
           int ind = (int) search;
           std::string sub = s.substr((oldInd + 1), ind - (oldInd + 1));
           setMeridian(sub);
         }
         
         int length = s.length();

         while (ind < length && ind > 0)
         {
            oldInd = ind;
            ind++;

            search = s.find(" ", ind);

            std::string s2;

           if(search != std::string::npos)
           {
             ind = (int) search;
             std::string sub = s.substr((oldInd + 1) , ind - (oldInd + 1));

             if (sub.find("-") || sub.find("+"))
             {
               std::string sub2 = s.substr((oldInd + 2), ind - (oldInd + 2));

               if (sub2.compare("s") == 0)
               {
                 _skytype = CLEAR;
                 std::string arg = sub.substr(0,1); 
                 if(arg.compare("-") == 0)
                 {
                   _dosun = 0;
                 }  
                 else if(arg.compare("+") == 0)
                 {
                   _dosun = 1;
                 }
               }
               else if (sub2.compare("c") == 0)
               {
                  _skytype = OVER;

               }
               else if (sub2.compare("u") == 0)
               {
                  _skytype = UNIF;
               }
               else if (sub2.compare("i") == 0)
               {
                  _skytype = INTER;
               }
               else if (sub2.compare("t") == 0)
               {
                  size_t val = s.find(" " , (int) ind + 1);

                  s2 = s.substr(ind, (int)val -ind);

                  _turbidity = static_cast<float>(atof(s2.c_str())) ;

                  ind = oldInd  = (int) val;
               }
               else if ( sub2.compare("g") == 0)
               {
                 size_t val = s.find(" " , (int) ind + 1);

                  s2 = s.substr(ind, (int)val -ind);

                  _gprefl = static_cast<float>(atof(s2.c_str())) ;

                  ind = oldInd  = (int) val;

               }
               else if(sub2.compare("a") == 0)
                 {

                   size_t val = s.find(" " , (int) ind + 1);

                   s2 = s.substr(ind, (int)val -ind);

                   _latitude = static_cast<float>(atof(s2.c_str()) * (PI / 180));

                   ind = oldInd  = (int) val;

                 }
                 else if (sub2.compare("o") == 0)
                 {

                   size_t val = s.find(" " , (int) ind + 1);

                   s2 = s.substr(ind, (int)val -ind);

                   _longitude = static_cast<float>(atof(s2.c_str()) * (PI / 180)) ;

                   ind = oldInd  = (int) val;
                 }
               }
           }
         }
       }

     }
  }

 computeSky();
}

GenSky::~GenSky()
{

}

//Fonction pour simuler le déplcement du soleil au cour d'une journée
void GenSky::updateSky()
{
  if(_hour < 24.0)
  {
     _hour += 0.05f;
     //std::cout << " hour " << _hour << std::endl;
     computeSky();
  }
}

void GenSky::setMeridian(std::string s)
{
  for (int i = 0 ; i < 30 ; ++i)
  {
    if(s.compare(tzone[i].zname) == 0)
    {
      _meridian = tzone[i].zmer * ( PI / 180.0f);
    }
  }
}

/*
Calcul des parametres ciel
*/

void GenSky::computeSky(void)
{
  float normFactor;

  //Calcul de la direction du soleil
  if(_month) 
  {
    int jd;
    float sd;
    float st;

    jd = jdate(_month, _day); // Jour julien , fonction non définie
    sd = sdec(jd);  //declinaison solaire
    if (_solar) //Position solaire en fonction de l'heure
    {
      st = _hour;
    }
    else
    {
      st = _hour + stadj(jd);
    }

    _altitude = salt(sd, st); 
    _azimuth = sazi (sd , st);

    //printf(" # Local solar time %.2f \n" , st);
    //printf(" # Solar altitude and azimuth : %.1f %.&f \n", 180 / (PI * _altitude) , 180 / ( PI * _azimuth));

    if(!isOvertcast() && _altitude > 87. * PI/ 180.)
    {
		  printf("# warning - sun too close to zenith, reducing altitude to 87 degrees\n");
      _altitude = 87.0f *PI/180.0f;
    }

    _sundir[0] = -sin(_azimuth) * cos(_altitude);
    _sundir[1] = -cos(_azimuth) * cos(_altitude);
    _sundir[2] = sin(_altitude);

    //Calcul du facteur "normal"....
    switch(_skytype)
    {
      case UNIF:
        normFactor = 1.0f;
        break;

      case OVER:
        normFactor = 0.777778f;
        break;

      case CLEAR:
        //Fonction d'origine de Radiance à conserver
        // _f2 = 0.274 * ( 0.91 + 10.0 * exp(-3.0 * (PI / (2.0 - _altitude)) + 0.45 * _sundir[2] * _sundir[2]));
        _f2 = 0.274f * ( 0.91f + 10.0f * exp(-3.0f * (PI/2.0f - _altitude)) + 0.45f * _sundir[2]*_sundir[2]);

        normFactor = normsc() / _f2 / PI;
        break;

      case INTER:
        _f2 = (2.739f + .9291f * sin (.3119f + 2.6f * _altitude)) * exp (-(PI/ 2.0f - _altitude) * (.4441f + 1.48f * _altitude));
        normFactor = normsc() / _f2 / PI;
        break;
    }

    //calcul de la luminosité du zenith
    if (_uzenith == -1)
    {
      _zenithbr /= normFactor / PI;
    }
    else if (_uzenith == 0)
    {
      if(isOvertcast())
      {
        _zenithbr = 8.6f * _sundir[2] + .123f;
      }
      else
      {
        _zenithbr = 1.876f * tan(_altitude) + 0.38f;
        //Fonction original de Radiance à conserver
        //_zenithbr = (1.376 * _turbidity - 1.81) * tan(_altitude) + 0.38;        
      }

      if(_skytype == INTER)
      {
        _zenithbr = (_zenithbr + 8.6f * _sundir[2] + .123f) / 2.0f;
      }
      if (_zenithbr < 0.0)
      {
        _zenithbr = 0.0f;
      }
      else
      {
        _zenithbr *= 1000.0f / SKYEFFICACY;
      }
    }

    _groundbr = _zenithbr * normFactor;

    //printf("# ground ambient level : %.1f \n" , _groundbr);

    if(!isOvertcast() && _sundir[2] > 0.0 && (!_uSolar || _solarbr > 0.0))
    {
      if(_uSolar == -1)
      {
        _solarbr /= 6e-5f * _sundir[2];
      }
      else if (_uSolar == 0)
      {
        //Fonction original de Radiance
       // _solarbr = 1.5e9 / SUNEFFICACY * (1.147 - .147/(_sundir[2] > .16 ? _sundir[2]:.16));
        _solarbr = 1.5e9f / SUNEFFICACY * (1.147f - .147f/(_sundir[2] > .16f ? _sundir[2]:.16f));
        if (_skytype == INTER)
        {
          _solarbr *= 0.15f;
        }
      }

      float d = 6e-5f / PI * _solarbr * _sundir[2];

      _groundbr += 6e-5f / PI * _solarbr * _sundir[2];
    }
    else
    {
      _dosun = 0;
    }

    _groundbr *= _gprefl;

    /*if(_groundbr > 1.3f)
    {
       _groundbr = 1.3f;
    }*/
  }
}

bool GenSky::isOvertcast()
{
  return ( (_skytype == OVER) || (_skytype == UNIF));

}

/* Calcul du facteur de "normalisation" ( E0 * F2 / LO) */

float GenSky::normsc(void)
{
  static float nfc[2][5] = 
  {
    //Ciel dégagé approximation
    {2.766521f, 0.547665f, -0.369832f, 0.009237f, 0.059229f},
    //Ciel couvert approximation
    		{3.5556f, -2.7152f, -1.3081f, 1.0660f, 0.60227f},
  };

  float *nf;
  float x;
  float nsc;
  int i;

  nf = nfc[_skytype == INTER];
  x = (_altitude - PI / 4.0f) / (PI / 4.0f);
  nsc = nf[i = 4];
  while (i --)
  {
    nsc = nsc * x + nf[i];
  }

  return nsc;
}

/*Fonction tirés de la sun.c */

//jour Julien
int GenSky::jdate(int month , int day)
{
  static short mo_da[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

  return (mo_da[month - 1] + day);
}

//Déclinaison angulaire du soleil en fonction du jour Julien
float GenSky::sdec(int jd)
{
  return (0.4093f * sin( 2.0f * (PI / 368.0f) * ( jd - 81.0f))); 
}

//Ajustement temporel solaire en fonction du jour Julien
float GenSky::stadj(int jd)
{
   return( 0.170f * sin( (4.0f * PI / 373.0f) * (jd - 80.0f) ) -
		0.129f * sin( (2.0f * PI/ 355.0f) * (jd - 8.0f) ) +
		12.0f * (_meridian - _longitude) / PI );
}

//Altitude solaire en fonction l'inclinaison solaire et du temps
float GenSky::salt(float sd, float st)
{
  float d =  (asin( sin(_latitude) * sin(sd) - cos(_latitude) * cos(sd) * cos(st * (PI/12))));

  return (asin( sin(_latitude) * sin(sd) - cos(_latitude) * cos(sd) * cos(st * (PI/12.0f))));
}

//Azimuth solaire en fonction de l'inclinaion solaire et du temps
float GenSky::sazi(float sd, float st)
{
  	return( -atan2( cos(sd)*sin(st*(PI/12)),
 			-cos(_latitude)*sin(sd) -
 			sin(_latitude)*cos(sd)*cos(st*(PI/12)) ) );
}

int GenSky::getMonth()
{
  return _month;
}

float GenSky::getHour()
{
  return _hour;
}

float GenSky::getGroundBr()
{
  return _groundbr;
}

float GenSky::getSundir(int indice)
{
  return _sundir[indice];
}

glm::vec3 GenSky::getVecSundir()
{
  return glm::vec3(_sundir[0],_sundir[1],_sundir[2]);
}
