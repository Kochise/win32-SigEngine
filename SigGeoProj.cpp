// SigGeoProj.cpp: implementation of the CSigGeoProj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SigGeoProj.h"
#include "ConversApi.h" // Pour CSigGeoProj::SelectCurrentSysGeo::o_rnUnit et o_rnMerid

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSigGeoProj::CSigGeoProj()
{
  mp_asProjecBuffer.CreateBuffer(sizeof(CSigContainer::sSigProjection), 800); // Buffer de 800 sSigProjection
  mp_asStringBuffer.CreateBuffer(1<<16); // Buffer de 64 Ko
  mp_asConvBuffer.CreateBuffer(1<<12); // Buffer de 4 Ko

  mp_asConvBuffer.AddStringTail(""); // String 0, projection CONVERS par d�faut

  _InitCurrentSysGeo(); // Ajout � la base de donn�e tous les syst�mes g�od�siques 'connus'
}

CSigGeoProj::~CSigGeoProj()
{

}

/////////////////////////////////////////////////////////////////////////////

void CSigGeoProj::_InitCurrentSysGeo(void)
{
  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Longitude / Latitude");

  _AddCurrentSysGeo(NULL, "Longitude / Latitude", 1, 0);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (AGD 66)", 1, 12);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (AGD 84)", 1, 13);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (Belgique)", 1, 110);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (Corrego Alegre)", 1, 24);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (DHDN)", 1, 1000);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (DLx)", 1, 113);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (D73)", 1, 114);
  _AddCurrentSysGeo("ED50", "Longitude / Latitude (ED 50)", 1, 28);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (Ireland 65)", 1, 42);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (NAD 27 for US)", 1, 62);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (NAD 27 for Canada)", 1, 66);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (NAD 83)", 1, 74);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (Netherlands)", 1, 109);
  _AddCurrentSysGeo("NTF", "Longitude / Latitude (NTF)", 1, 1002);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (OSGB 36)", 1, 79);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (Pulkovo 1942)", 1, 1001);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (Rome 1940)", 1, 87);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (RT 90)", 1, 112);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (SAD 69)", 1, 92);
  _AddCurrentSysGeo(NULL, "Longitude / Latitude (Tokyo)", 1, 97);
  _AddCurrentSysGeo("WGS84", "Longitude / Latitude (WGS 84)", 1, 104);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Non-Terrestre");

  _AddCurrentSysGeo(NULL, "Non-Terrestre (inches)", 0, 2);
  _AddCurrentSysGeo(NULL, "Non-Terrestre (feet)", 0, 3);
  _AddCurrentSysGeo(NULL, "Non-Terrestre (yards)", 0, 4);
  _AddCurrentSysGeo(NULL, "Non-Terrestre arth (miles)", 0, 0);
  _AddCurrentSysGeo(NULL, "Non-Terrestre (millim�tres)", 0, 5);
  _AddCurrentSysGeo(NULL, "Non-Terrestre (centim�tres)", 0, 6);
  _AddCurrentSysGeo(NULL, "Non-Terrestre (m�tres)", 0, 7);
  _AddCurrentSysGeo(NULL, "Non-Terrestre (kilom�tres)", 0, 1);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Projections du Monde");

  _AddCurrentSysGeo(NULL, "Behrmann Cylindrique Surfaces �gales", 2, 62, 7, 0, 30);
  _AddCurrentSysGeo(NULL, "Eckert IV (Surfaces �gales)", 14, 62, 7, 0);
  _AddCurrentSysGeo(NULL, "Eckert VI (Surfaces �gales)", 15, 62, 7, 0);
  _AddCurrentSysGeo(NULL, "Gall", 17, 62, 7, 0);
  _AddCurrentSysGeo(NULL, "Mercator", 10, 62, 7, 0);
  _AddCurrentSysGeo(NULL, "Miller Cylindrique", 11, 62, 7, 0);
  _AddCurrentSysGeo(NULL, "Mollweide (Surfaces �gales)", 13, 62, 7, 0);
  _AddCurrentSysGeo(NULL, "Robinson", 12, 62, 7, 0);
  _AddCurrentSysGeo(NULL, "Sinuso�dale (Surfaces �gales)", 16, 62, 7, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Projections d'une H�misph�re");

  _AddCurrentSysGeo(NULL, "Azimuthale �quidistante (P�le Nord)", 5, 62, 7, 0, 90, 90);
  _AddCurrentSysGeo(NULL, "Azimuthale �quidistante (P�le Sud)", 5, 62, 7, 0, -90, 90);
  _AddCurrentSysGeo(NULL, "Lambert Azimuthale Surfaces �gales (P�le Nord)", 4, 62, 7, 0, 90, 90);
  _AddCurrentSysGeo(NULL, "Lambert Azimuthale Surfaces �gales (P�le Sud)", 4, 62, 7, 0, -90, 90);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Projections R�gionales Conformes");

  _AddCurrentSysGeo(NULL, "Projection Conforme (Afrique)", 10, 0, 0, 20);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Asie)", 3, 0, 0, 95, 40, 20, 60, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Australie)", 3, 0, 0, 132.5, -10, -21.5, -33.5, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Am�rique Centrale)", 10, 0, 0, -85.5);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Chine)", 3, 0, 0, 110, 10, 25, 40, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Europe)", 3, 0, 0, 25, 35, 40, 65, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Inde)", 8, 0, 0, 78, 0, 1.0, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Middle East)", 3, 0, 0, 47.5, 25, 15, 35, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Am�rique du Nord)", 3, 62, 7, -96, 23, 20, 60, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Conforme (Am�rique du Sud)", 10, 0, 0, -60);
  _AddCurrentSysGeo(NULL, "Projection Conforme (�tats Unis)", 3, 62, 7, -96, 23, 33, 45, 0, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Projections R�gionales Surfaces �gales");

  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Afrique)", 2, 0, 0, 20, 0);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Asie)", 9, 0, 0, 95, 40, 20, 60, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Australia)", 9, 0, 0, 132.5, -10, -21.5, -33.5, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Am�rique Centrale)", 2, 0, 0, -85.5, 13);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Chine)", 9, 0, 0, 110, 10, 25, 40, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Europe)", 9, 0, 0, 25, 35, 40, 65, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Inde)", 9, 0, 0, 78, 23, 22, 33, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Middle East)", 9, 0, 0, 47.5, 25, 15, 35, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Am�rique du Nord)", 9, 62, 7, -96, 23, 20, 60, 0, 0);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (Am�rique du Sud)", 14, 0, 0, -60);
  _AddCurrentSysGeo(NULL, "Projection Surfaces �gales (�tats Unis)", 9, 62, 7, -96, 23, 29.5, 45.5, 0, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Australian Map Grid (AGD 66)");

  _AddCurrentSysGeo(NULL, "AMG Zone 47" /* (AGD 66)*/, 8, 12, 7, 99, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 48" /* (AGD 66)\\p20248"*/, 8, 12, 7, 105, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 49" /* (AGD 66)\\p20249"*/, 8, 12, 7, 111, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 50" /* (AGD 66)\\p20250"*/, 8, 12, 7, 117, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 51" /* (AGD 66)\\p20251"*/, 8, 12, 7, 123, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 52" /* (AGD 66)\\p20252"*/, 8, 12, 7, 129, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 53" /* (AGD 66)\\p20253"*/, 8, 12, 7, 135, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 54" /* (AGD 66)\\p20254"*/, 8, 12, 7, 141, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 55" /* (AGD 66)\\p20255"*/, 8, 12, 7, 147, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 56" /* (AGD 66)\\p20256"*/, 8, 12, 7, 153, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 57" /* (AGD 66)\\p20257"*/, 8, 12, 7, 159, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 58" /* (AGD 66)\\p20258"*/, 8, 12, 7, 165, 0, 0.9996, 500000, 10000000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Australian Map Grid (AGD 84)");

  _AddCurrentSysGeo(NULL, "AMG Zone 47" /* (AGD 84)"*/, 8, 13, 7, 99, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 48" /* (AGD 84)\\p20348"*/, 8, 13, 7, 105, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 49" /* (AGD 84)\\p20349"*/, 8, 13, 7, 111, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 50" /* (AGD 84)\\p20350"*/, 8, 13, 7, 117, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 51" /* (AGD 84)\\p20351"*/, 8, 13, 7, 123, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 52" /* (AGD 84)\\p20352"*/, 8, 13, 7, 129, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 53" /* (AGD 84)\\p20353"*/, 8, 13, 7, 135, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 54" /* (AGD 84)\\p20354"*/, 8, 13, 7, 141, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 55" /* (AGD 84)\\p20355"*/, 8, 13, 7, 147, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 56" /* (AGD 84)\\p20356"*/, 8, 13, 7, 153, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 57" /* (AGD 84)\\p20357"*/, 8, 13, 7, 159, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "AMG Zone 58" /* (AGD 84)\\p20358"*/, 8, 13, 7, 165, 0, 0.9996, 500000, 10000000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Syst�mes de Coordonn�es Belges");

  _AddCurrentSysGeo(NULL, "Syst�me National Belge (1950)" /*\\p21500"*/, 3, 110, 7, 4.367975, 90, 49.8333333333, 51.1666666667, 150000, 5400000);
  _AddCurrentSysGeo(NULL, "Syst�me National Belge (1972)" /*\\p31300"*/, 19, 110, 7, 4.3569397222, 90, 49.8333333333, 51.1666666667, 150000.01256, 5400088.4378);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("British Coordinate Systems");

  _AddCurrentSysGeo(NULL, "British National Grid" /*\\p27700"*/, 8, 79, 7, -2, 49, 0.9996012717, 400000, -100000);
  _AddCurrentSysGeo(NULL, "British OSGRS80 Grid", 8, 33, 7, -2, 49, 0.9996012717, 400000, -100000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Danish Coordinate Systems");

  _AddCurrentSysGeo(NULL, "Danish System 34 Jylland-Fyn", 21, 28, 7, 9, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "Danish System 34 Sjaelland", 22, 28, 7, 9, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "Danish System 45 Bornholm", 23, 28, 7, 15, 0, 0.9996, 500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Finnish Coordinate Systems");

  _AddCurrentSysGeo(NULL, "Finnish KKJ Zone 1", 24, 28, 7, 21, 0, 1, 1500000, 0);
  _AddCurrentSysGeo(NULL, "Finnish KKJ Zone 2", 24, 28, 7, 24, 0, 1, 2500000, 0);
  _AddCurrentSysGeo(NULL, "Finnish KKJ Zone 3", 24, 28, 7, 27, 0, 1, 3500000, 0);
  _AddCurrentSysGeo(NULL, "Finnish KKJ Zone 4", 24, 28, 7, 30, 0, 1, 4500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Syst�mes Fran�ais M�ridien de Paris");

  _AddCurrentSysGeo("LT1", "Lambert I Carto - Paris" /*\\p27581"*/,3,1002,7,0, 49.5, 48.598522847174, 50.395911631678,600000,1200000);
  _AddCurrentSysGeo("LT2", "Lambert II Carto - Paris" /*\\p27582"*/,3,1002,7,0, 46.8, 45.898918964419, 47.696014502038,600000,2200000);
  _AddCurrentSysGeo("LT3", "Lambert III Carto - Paris" /*\\p27583"*/,3,1002,7,0, 44.1, 43.199291275544, 44.996093814511,600000,3200000);
  _AddCurrentSysGeo("LT4", "Lambert IV Carto - Paris",3,1002,7,0, 42.165, 41.560387840948, 42.76766346965, 234.358, 4185861.369);
  _AddCurrentSysGeo("LT1", "Lambert I Nord - Paris" /*\\p27591"*/,3,1002,7,0, 49.5, 48.598522847174, 50.395911631678,600000,200000);
  _AddCurrentSysGeo("LT2E", "Lambert II Centre - Paris" /*\\p27592"*/,3,1002,7,0, 46.8, 45.898918964419, 47.696014502038,600000,200000);
  _AddCurrentSysGeo("LT3", "Lambert III Sud - Paris" /*\\p27593"*/,3,1002,7,0, 44.1, 43.199291275544, 44.996093814511,600000,200000);
  _AddCurrentSysGeo("LT4", "Lambert IV Corse - Paris",3,1002,7,0, 42.165, 41.560387840948, 42.76766346965, 234.358, 185861.369);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Syst�me fran�ais RGF93 M�ridien de greenwich IGN");

  _AddCurrentSysGeo("LT93", "RGF - Lambert93", 3, 33, /*0, 0, 0, 0,*/ 7, 3, 46.5, 44.0, 49.0, 700000, 6600000);
  _AddCurrentSysGeo("LT93", "RGF - Lambert93", 3, 999, /*0, 0, 0, 0,*/ 7, 3, 46.5, 44.0, 49.0, 700000, 6600000);

  _AddCurrentSysGeo("LT93", "RGF - Lambert93", 3, 9999, /*0, 0, 0, 0,*/ 7, 3, 46.5, 44.0, 49.0, 700000, 6600000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("DOM-TOM-IGN");

  // Suisse Autriche : SWISS=SWISS1,31,CH1903,2,0,0,0,0,7.26225�,46.570866�,6e5,2e5
  
  // Reunion : UTMSPDN=UTM / Piton des Neiges,1,Piton des Neiges,4,0,0,0,0,1
  _AddCurrentSysGeo("UTM40S", "R�union, RGR 92 - UTM 40 sud", 8, 104, 7, 57, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo("GSLBR", "R�union, PDN - Gauss Laborde", 8, 9999, 4, 789.524, -626.486, -89.904, -0.6006,-76.7946, 10.5788, -32.3241, 0); //, 7, 55.53333333333, -21.11666666667, 1, 160000, 50000);
  _AddCurrentSysGeo("UTM20", "Guadeloupe-Martinique, WGS84 - UTM 20 nord", 8, 104, 7, -63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo("UTMSTAN", "Guadeloupe, Ste Anne IGN48 - UTM 20 nord", 8, 9999, 4, -472.29, -5.63, -304.12, -0.4362, 0.8374, -0.2563, 1.8984, 0); //, 7, -63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo("UTMFTMT", "Guadeloupe, Fort Marigot IGN49 - UTM 20 nord", 8, 999, 4, 136.596, 248.148, -429.789, 7,-63, 0, 0.9996, 500000); //, 0);
  // Antilles : UTMFTDX=UTM / Fort Desaix,1,Fort Desaix,4,0,0,0,0
  _AddCurrentSysGeo("UTM20FTDX", "Martinique, Fort Desaix IGN52 - UTM 20 nord", 8,9999, 4, 126.93, 547.94, 130.41, 2.7867,-5.1612, 0.8584,13.8227, 0); //, 7, -63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "Guyane, RGFG95 - UTM 22 nord", 8, 104, 7, -51, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "Guyane, RGFG95 - UTM 21 nord", 8, 104, 7, -57, 0, 0.9996, 500000, 0);
  // Guyane : UTMCGS67=UTM / CGS67,1,CGS67,4,0,0,0,0
  _AddCurrentSysGeo("UTM22CGS67", "Guyane, CSG67- UTM 22 nord", 8, 9999, 4, -193.066, 236.993, 105.447, -0.4814, 0.8074, -0.1276, 1.5649, 0); //, 7, -51, 0, 0.9996, 500000, 0 );
  _AddCurrentSysGeo(NULL, "Guyane, CSG67- UTM 21 nord", 8, 9999, 4, -193.066, 236.993, 105.447, -0.4814, 0.8074, -0.1276, 1.5649, 0); //, 7, -57, 0, 0.9996, 500000, 0 );
  _AddCurrentSysGeo(NULL, "Nouvelle Cal�donie, Ouv�a WGS84 - UTM 58 sud", 8, 104, 7, 165, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "Nouvelle Cal�donie, Grande Terre IGN72 - UTM 58 sud", 8, 9999, 4, 97.295, -263.237, 310.866, 1.599931, -0.838952, -3.141180, 13.325864, 0); //, 7, 165, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "Nouvelle Cal�donie, Mar� IGN53 - UTM 58 sud", 8, 9999, 4, -533.321, 238.077, -224.713, -2.381004, 7.580876, 2.346668, -124.243204, 0); //, 7, 165, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "Nouvelle Cal�donie, Lifou IGN56 - UTM 58 sud", 8, 9999, 4, 137.098, 131.678, 91.473, 1.943943, 11.599469, 4.331745, -7.480528, 0); //, 7, 165, 0, 0.9996, 500000, 10000000 );
  _AddCurrentSysGeo(NULL, "Mayotte, Combani1950 - UTM 38 sud", 8, 999, 4, -382.34, -59.14, -262.41, 7, 45, 0, 0.9996, 500000); //, 10000000);
  _AddCurrentSysGeo(NULL, "St Pierre et Miquelon, IGN1950 - UTM 21 nord", 8, 999, 7, 30, 430, 368, 7, -57, 0, 0.9996, 500000); //, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Syst�mes Fran�ais MapInfo <= 3");

  _AddCurrentSysGeo("LT1", "Lambert I Carto ", 3, 999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 49.5, 48.60258453428, 50.39741546572, 600000, 1200000);
  _AddCurrentSysGeo("LT2", "Lambert II Carto", 3, 999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 46.8, 45.90287723937, 47.69712276063, 600000, 2200000);
  _AddCurrentSysGeo("LT3", "Lambert III Carto", 3, 999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 44.1, 43.20317004, 44.99682996, 600000, 3200000);
  _AddCurrentSysGeo("LT4", "Lambert IV Carto", 3, 999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 42.165, 41.56249184952, 42.76750815048, 234.358, 4185861.369);
  _AddCurrentSysGeo("LT1", "Lambert I Nord ", 3, 999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 49.5, 48.60258453428, 50.39741546572, 600000, 200000);
  _AddCurrentSysGeo("LT2", "Lambert II Centre ", 3, 999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 46.8, 45.90287723937, 47.69712276063, 600000, 200000);
  _AddCurrentSysGeo("LT3", "Lambert III Sud", 3, 999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 44.1, 43.20317004, 44.99682996, 600000, 200000);
  _AddCurrentSysGeo("LT4", "Lambert IV Corse", 3, 999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 42.165, 41.56249184952, 42.76750815048, 234.358, 185861.369);

  _AddCurrentSysGeo("LT1", "Lambert I Carto ", 3, 9999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 49.5, 48.60258453428, 50.39741546572, 600000, 1200000);
  _AddCurrentSysGeo("LT2", "Lambert II Carto", 3, 9999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 46.8, 45.90287723937, 47.69712276063, 600000, 2200000);
  _AddCurrentSysGeo("LT3", "Lambert III Carto", 3, 9999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 44.1, 43.20317004, 44.99682996, 600000, 3200000);
  _AddCurrentSysGeo("LT4", "Lambert IV Carto", 3, 9999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 42.165, 41.56249184952, 42.76750815048, 234.358, 4185861.369);
  _AddCurrentSysGeo("LT1", "Lambert I Nord ", 3, 9999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 49.5, 48.60258453428, 50.39741546572, 600000, 200000);
  _AddCurrentSysGeo("LT2", "Lambert II Centre ", 3, 9999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 46.8, 45.90287723937, 47.69712276063, 600000, 200000);
  _AddCurrentSysGeo("LT3", "Lambert III Sud", 3, 9999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 44.1, 43.20317004, 44.99682996, 600000, 200000);
  _AddCurrentSysGeo("LT4", "Lambert IV Corse", 3, 9999, /*6, -168, -60, 320,*/ 7, 2.337229104484, 42.165, 41.56249184952, 42.76750815048, 234.358, 185861.369);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Gauss-Kruger (DHDN)");

  _AddCurrentSysGeo(NULL, "GK Zone 1" /* (DHDN)\\p31491"*/, 8, 1000, 7, 3, 0, 1, 1500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 2" /* (DHDN)\\p31492"*/, 8, 1000, 7, 6, 0, 1, 2500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 3" /* (DHDN)\\p31493"*/, 8, 1000, 7, 9, 0, 1, 3500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 4" /* (DHDN)\\p31494"*/, 8, 1000, 7, 12, 0, 1, 4500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 5" /* (DHDN)\\p31495"*/, 8, 1000, 7, 15, 0, 1, 5500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Gauss-Kruger (Pulkovo 1942)");

  _AddCurrentSysGeo(NULL, "GK Zone 1" /* (Pulkovo 1942)"*/, 8, 1001, 7, 3, 0, 1, 1500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 2" /* (Pulkovo 1942)"*/, 8, 1001, 7, 9, 0, 1, 2500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 3" /* (Pulkovo 1942)"*/, 8, 1001, 7, 15, 0, 1, 3500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 4" /* (Pulkovo 1942)\\p28404"*/, 8, 1001, 7, 21, 0, 1, 4500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 5" /* (Pulkovo 1942)\\p28405"*/, 8, 1001, 7, 27, 0, 1, 5500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 6" /* (Pulkovo 1942)\\p28406"*/, 8, 1001, 7, 33, 0, 1, 6500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 7" /* (Pulkovo 1942)\\p28407"*/, 8, 1001, 7, 39, 0, 1, 7500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 8" /* (Pulkovo 1942)\\p28408"*/, 8, 1001, 7, 45, 0, 1, 8500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 9" /* (Pulkovo 1942)\\p28409"*/, 8, 1001, 7, 51, 0, 1, 9500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 10" /* (Pulkovo 1942)\\p28410"*/, 8, 1001, 7, 57, 0, 1, 10500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 11" /* (Pulkovo 1942)\\p28411"*/, 8, 1001, 7, 63, 0, 1, 11500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 12" /* (Pulkovo 1942)\\p28412"*/, 8, 1001, 7, 69, 0, 1, 12500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 13" /* (Pulkovo 1942)\\p28413"*/, 8, 1001, 7, 75, 0, 1, 13500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 14" /* (Pulkovo 1942)\\p28414"*/, 8, 1001, 7, 81, 0, 1, 14500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 15" /* (Pulkovo 1942)\\p28415"*/, 8, 1001, 7, 87, 0, 1, 15500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 16" /* (Pulkovo 1942)\\p28416"*/, 8, 1001, 7, 93, 0, 1, 16500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 17" /* (Pulkovo 1942)\\p28417"*/, 8, 1001, 7, 99, 0, 1, 17500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 18" /* (Pulkovo 1942)\\p28418"*/, 8, 1001, 7, 105, 0, 1, 18500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 19" /* (Pulkovo 1942)\\p28419"*/, 8, 1001, 7, 111, 0, 1, 19500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 20" /* (Pulkovo 1942)\\p28420"*/, 8, 1001, 7, 117, 0, 1, 20500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 21" /* (Pulkovo 1942)\\p28421"*/, 8, 1001, 7, 123, 0, 1, 21500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 22" /* (Pulkovo 1942)\\p28422"*/, 8, 1001, 7, 129, 0, 1, 22500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 23" /* (Pulkovo 1942)\\p28423"*/, 8, 1001, 7, 135, 0, 1, 23500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 24" /* (Pulkovo 1942)\\p28424"*/, 8, 1001, 7, 141, 0, 1, 24500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 25" /* (Pulkovo 1942)\\p28425"*/, 8, 1001, 7, 147, 0, 1, 25500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 26" /* (Pulkovo 1942)\\p28426"*/, 8, 1001, 7, 153, 0, 1, 26500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 27" /* (Pulkovo 1942)\\p28427"*/, 8, 1001, 7, 159, 0, 1, 27500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 28" /* (Pulkovo 1942)\\p28428"*/, 8, 1001, 7, 165, 0, 1, 28500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 29" /* (Pulkovo 1942)\\p28429"*/, 8, 1001, 7, 171, 0, 1, 29500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 30" /* (Pulkovo 1942)\\p28430"*/, 8, 1001, 7, 177, 0, 1, 30500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 31" /* (Pulkovo 1942)\\p28431"*/, 8, 1001, 7, -177, 0, 1, 31500000, 0);
  _AddCurrentSysGeo(NULL, "GK Zone 32" /* (Pulkovo 1942)\\p28432"*/, 8, 1001, 7, -171, 0, 1, 32500000, 0);
            
  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Irish Coordinate Systems");

  _AddCurrentSysGeo(NULL, "Irish Transverse Mercator Grid" /*\\p29900"*/, 8, 42, 7, -8, 53.5, 1.000035, 200000, 250000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Italian Coordinate Systems");

  _AddCurrentSysGeo(NULL, "Italian National System (Gauss-Boaga), Zone 1 (West)" /*\\p26591"*/, 8, 87, 7, 9, 0, 0.9996, 1500000, 0);
  _AddCurrentSysGeo(NULL, "Italian National System (Gauss-Boaga), Zone 2 (East)" /*\\p26592"*/, 8, 87, 7, 15, 0, 0.9996, 2520000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Japanese Coordinate Systems");

  _AddCurrentSysGeo(NULL, "Japanese Zone I", 8, 97, 7, 129.5, 33, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone II", 8, 97, 7, 131, 33, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone III", 8, 97, 7, 132.166666, 36, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone IV", 8, 97, 7, 133.5, 33, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone V", 8, 97, 7, 134.333333, 36, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone VI", 8, 97, 7, 136, 36, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone VII", 8, 97, 7, 137.166666, 36, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone VIII", 8, 97, 7, 138.5, 36, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone IX", 8, 97, 7, 139.833333, 36, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone X", 8, 97, 7, 140.833333, 40, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XI", 8, 97, 7, 140.25, 44, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XII", 8, 97, 7, 142.25, 44, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XIII", 8, 97, 7, 144.25, 44, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XIV", 8, 97, 7, 142, 26, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XV", 8, 97, 7, 127.5, 26, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XVI", 8, 97, 7, 124, 26, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XVII", 8, 97, 7, 131, 26, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XVIII", 8, 97, 7, 136, 20, 0.99999, 0, 0);
  _AddCurrentSysGeo(NULL, "Japanese Zone XIX", 8, 97, 7, 154, 26, 0.99999, 0, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Netherlands Coordinate Systems");

  _AddCurrentSysGeo(NULL, "Netherlands National System" /*\\p28992"*/, 20, 109, 7, 5.387638889, 52.156160556, 0.9999079, 155000, 463000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("New Zealand Coordinate Systems");

  _AddCurrentSysGeo(NULL, "New Zealand Map Grid" /*\\p27200"*/, 18, 31, 7, 173, -41, 2510000, 6023150);
  _AddCurrentSysGeo(NULL, "National Yard Grid (North Island)" /*\\p27291"*/, 8, 31, 4, 175.5, -39, 1, 300000, 400000);
  _AddCurrentSysGeo(NULL, "National Yard Grid (South Island)" /*\\p27292"*/, 8, 31, 4, 171.5, -44, 1, 500000, 500000);
  _AddCurrentSysGeo(NULL, "Amuri Circuit", 8, 31, 7, 173.010133389, -42.689116583, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Bay of Plenty Circuit", 8, 31, 7, 176.466197250, -37.761249806, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Bluff Circuit", 8, 31, 7, 168.342872000, -46.600009611, 1, 300002.66, 699999.58);
  _AddCurrentSysGeo(NULL, "Buller Circuit", 8, 31, 7, 171.581260056, -41.810802861, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Collingwood Circuit", 8, 31, 7, 172.672046500, -40.714759056, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Gawler Circuit", 8, 31, 7, 171.360748472, -43.748711556, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Grey Circuit", 8, 31, 7, 171.549771306, -42.333694278, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Hawkes Bay Circuit", 8, 31, 7, 176.673680528, -39.650929306, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Hokitika Circuit", 8, 31, 7, 170.979993500, -42.886322361, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Jacksons Bay Circuit", 8, 31, 7, 168.606267000, -43.977802889, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Karamea Circuit", 8, 31, 7, 172.109028194, -41.289911528, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Lindis Peak Circuit", 8, 31, 7, 169.467755083, -44.735267972, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Marlborough Circuit", 8, 31, 7, 173.802074111, -41.544486667, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Mt. Eden Circuit", 8, 31, 7, 174.764339361, -36.879865278, 0.9999, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Mt. Nicholas Circuit", 8, 31, 7, 168.398641194, -45.132902583, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Mt. Pleasant Circuit", 8, 31, 7, 172.727193583, -43.590637583, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Mt. York Circuit", 8, 31, 7, 167.738861778, -45.563726167, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Nelson Circuit", 8, 31, 7, 173.299316806, -41.274544722, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "North Taieri Circuit", 8, 31, 7, 170.282589111, -45.861513361, 0.99996, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Observation Pt. Circuit", 8, 31, 7, 170.628595167, -45.816196611, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Okarito Circuit", 8, 31, 7, 170.260925833, -43.110128139, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Poverty Bay Circuit", 8, 31, 7, 177.885636278, -38.624702778, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Taranaki Circuit", 8, 31, 7, 174.228011750, -39.135758306, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Timaru Circuit", 8, 31, 7, 171.057250833, -44.402220361, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Tuhirangi Circuit", 8, 31, 7, 175.640036806, -39.512470389, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Wairarapa Circuit", 8, 31, 7, 175.647349667, -40.925532639, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Wanganui Circuit", 8, 31, 7, 175.488099611, -40.241947139, 1, 300000, 700000);
  _AddCurrentSysGeo(NULL, "Wellington Circuit", 8, 31, 7, 174.776623111, -41.301319639, 1, 300000, 700000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Portuguese Coordinate Systems");

  _AddCurrentSysGeo(NULL, "Portuguese National System (DLx)", 8, 113, 7, -8.1319061111, 39.6666666667, 1, 0, 0);
  _AddCurrentSysGeo(NULL, "Portuguese National System (D73)", 8, 114, 7, -8.1319061111, 39.6666666667, 1, 180.598, -86.99);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Quebec Modified Transverse Mercator (NAD 27)");

  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 2" /* (NAD 27)"*/, 8, 66, 7, -55.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 3" /* (NAD 27)"*/, 8, 66, 7, -58.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 4" /* (NAD 27)"*/, 8, 66, 7, -61.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 5" /* (NAD 27)"*/, 8, 66, 7, -64.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 6" /* (NAD 27)"*/, 8, 66, 7, -67.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 7" /* (NAD 27)"*/, 8, 66, 7, -70.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 8" /* (NAD 27)"*/, 8, 66, 7, -73.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 9" /* (NAD 27)"*/, 8, 66, 7, -76.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 10" /* (NAD 27)"*/, 8, 66, 7, -79.5, 0, 0.9999, 304800, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Quebec Modified Transverse Mercator (NAD 83)");

  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 2" /* (NAD 83)"*/, 8, 74, 7, -55.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 3" /* (NAD 83)"*/, 8, 74, 7, -58.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 4" /* (NAD 83)"*/, 8, 74, 7, -61.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 5" /* (NAD 83)"*/, 8, 74, 7, -64.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 6" /* (NAD 83)"*/, 8, 74, 7, -67.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 7" /* (NAD 83)"*/, 8, 74, 7, -70.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 8" /* (NAD 83)"*/, 8, 74, 7, -73.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 9" /* (NAD 83)"*/, 8, 74, 7, -76.5, 0, 0.9999, 304800, 0);
  _AddCurrentSysGeo(NULL, "Quebec MTM Zone 10" /* (NAD 83)"*/, 8, 74, 7, -79.5, 0, 0.9999, 304800, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Swedish Coordinate Systems (RT 90)");

  _AddCurrentSysGeo(NULL, "Swedish National System", 8, 112, 7, 15.8082777778, 0, 1, 1500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("St�r�ographique Polaire Universelle (UPS)");

  _AddCurrentSysGeo(NULL, "UPS Zone Nord", 20, 104, 7, 0, 90, 0.994, 2000000, 2000000);
  _AddCurrentSysGeo(NULL, "UPS Zone Sud", 20, 104, 7, 0, -90, 0.994, 2000000, 2000000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Universal Transverse Mercator (Corrego Alegre)");

  _AddCurrentSysGeo(NULL, "UTM Zone 18, H�misph�re Sud" /* (Corrego Alegre)"*/, 8, 24, 7, -75, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 19, H�misph�re Nord" /* (Corrego Alegre)"*/, 8, 24, 7, -69, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 19, H�misph�re Sud" /* (Corrego Alegre)"*/, 8, 24, 7, -69, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 20, H�misph�re Nord" /* (Corrego Alegre)"*/, 8, 24, 7, -63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 20, H�misph�re Sud" /* (Corrego Alegre)"*/, 8, 24, 7, -63, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 21, H�misph�re Nord" /* (Corrego Alegre)"*/, 8, 24, 7, -57, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 21, H�misph�re Sud" /* (Corrego Alegre)"*/, 8, 24, 7, -57, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 22, H�misph�re Nord" /* (Corrego Alegre)"*/, 8, 24, 7, -51, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 22, H�misph�re Sud" /* (Corrego Alegre)"*/, 8, 24, 7, -51, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 23, H�misph�re Sud" /* (Corrego Alegre)"\\p22523"*/, 8, 24, 7, -45, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 24, H�misph�re Sud" /* (Corrego Alegre)"\\p22524"*/, 8, 24, 7, -39, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 25, H�misph�re Sud" /* (Corrego Alegre)"*/, 8, 24, 7, -33, 0, 0.9996, 500000, 10000000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Universal Transverse Mercator (ED 50)");

  _AddCurrentSysGeo(NULL, "UTM Zone 28" /* (ED 50)\\p23028"*/, 8, 28, 7, -15, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 29" /* (ED 50)\\p23029"*/, 8, 28, 7, -9, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo("UTM30ED50", "UTM Zone 30" /* (ED 50)\\p23030"*/, 8, 28, 7, -3, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 31" /* (ED 50)\\p23031"*/, 8, 28, 7, 3, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 32" /* (ED 50)\\p23032"*/, 8, 28, 7, 9, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 33" /* (ED 50)\\p23033"*/, 8, 28, 7, 15, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 34" /* (ED 50)\\p23034"*/, 8, 28, 7, 21, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 35" /* (ED 50)\\p23035"*/, 8, 28, 7, 27, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 36" /* (ED 50)\\p23036"*/, 8, 28, 7, 33, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 37" /* (ED 50)\\p23037"*/, 8, 28, 7, 39, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 38" /* (ED 50)\\p23038"*/, 8, 28, 7, 45, 0, 0.9996, 500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Universal Transverse Mercator (NAD 27 for Canada)");

  _AddCurrentSysGeo(NULL, "UTM Zone 7" /* (NAD 27 for Canada)"*/, 8, 66, 7, -141, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 8" /* (NAD 27 for Canada)"*/, 8, 66, 7, -135, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 9" /* (NAD 27 for Canada)"*/, 8, 66, 7, -129, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 10" /* (NAD 27 for Canada)"*/, 8, 66, 7, -123, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 11" /* (NAD 27 for Canada)"*/, 8, 66, 7, -117, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 12" /* (NAD 27 for Canada)"*/, 8, 66, 7, -111, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 13" /* (NAD 27 for Canada)"*/, 8, 66, 7, -105, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 14" /* (NAD 27 for Canada)"*/, 8, 66, 7, -99, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 15" /* (NAD 27 for Canada)"*/, 8, 66, 7, -93, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 16" /* (NAD 27 for Canada)"*/, 8, 66, 7, -87, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 17" /* (NAD 27 for Canada)"*/, 8, 66, 7, -81, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 18" /* (NAD 27 for Canada)"*/, 8, 66, 7, -75, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 19" /* (NAD 27 for Canada)"*/, 8, 66, 7, -69, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 20" /* (NAD 27 for Canada)"*/, 8, 66, 7, -63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 21" /* (NAD 27 for Canada)"*/, 8, 66, 7, -57, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 22" /* (NAD 27 for Canada)"*/, 8, 66, 7, -51, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 23" /* (NAD 27 for Canada)"*/, 8, 66, 7, -45, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 24" /* (NAD 27 for Canada)"*/, 8, 66, 7, -39, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 25" /* (NAD 27 for Canada)"*/, 8, 66, 7, -33, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 26" /* (NAD 27 for Canada)"*/, 8, 66, 7, -27, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 27" /* (NAD 27 for Canada)"*/, 8, 66, 7, -21, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 28" /* (NAD 27 for Canada)"*/, 8, 66, 7, -15, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 29" /* (NAD 27 for Canada)"*/, 8, 66, 7, -9, 0, 0.9996, 500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Universal Transverse Mercator (NAD 27 for US)");

  _AddCurrentSysGeo(NULL, "UTM Zone 1 (NAD 27 for Alaska)" /*\\p26701"*/, 8, 63, 7, -177, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 2 (NAD 27 for Alaska)" /*\\p26702"*/, 8, 63, 7, -171, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 3 (NAD 27 for Alaska)" /*\\p26703"*/, 8, 63, 7, -165, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 4 (NAD 27 for Alaska)" /*\\p26704"*/, 8, 63, 7, -159, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 5 (NAD 27 for Alaska)" /*\\p26705"*/, 8, 63, 7, -153, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 6 (NAD 27 for Alaska)" /*\\p26706"*/, 8, 63, 7, -147, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 7 (NAD 27 for Alaska)" /*\\p26707"*/, 8, 63, 7, -141, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 8 (NAD 27 for Alaska)" /*\\p26708"*/, 8, 63, 7, -135, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 9 (NAD 27 for Alaska)" /*\\p26709"*/, 8, 63, 7, -129, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 10" /* (NAD 27 for US)\\p26710"*/, 8, 62, 7, -123, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 11" /* (NAD 27 for US)\\p26711"*/, 8, 62, 7, -117, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 12" /* (NAD 27 for US)\\p26712"*/, 8, 62, 7, -111, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 13" /* (NAD 27 for US)\\p26713"*/, 8, 62, 7, -105, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 14" /* (NAD 27 for US)\\p26714"*/, 8, 62, 7, -99, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 15" /* (NAD 27 for US)\\p26715"*/, 8, 62, 7, -93, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 16" /* (NAD 27 for US)\\p26716"*/, 8, 62, 7, -87, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 17" /* (NAD 27 for US)\\p26717"*/, 8, 62, 7, -81, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 18" /* (NAD 27 for US)\\p26718"*/, 8, 62, 7, -75, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 19" /* (NAD 27 for US)\\p26719"*/, 8, 62, 7, -69, 0, 0.9996, 500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Universal Transverse Mercator (NAD 83)");

  _AddCurrentSysGeo(NULL, "UTM Zone 1" /* (NAD 83)"*/, 8, 74, 7, -177, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 2" /* (NAD 83)"*/, 8, 74, 7, -171, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 3" /* (NAD 83)\\p26903"*/, 8, 74, 7, -165, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 4" /* (NAD 83)\\p26904"*/, 8, 74, 7, -159, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 5" /* (NAD 83)\\p26905"*/, 8, 74, 7, -153, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 6" /* (NAD 83)\\p26906"*/, 8, 74, 7, -147, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 7" /* (NAD 83)\\p26907"*/, 8, 74, 7, -141, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 8" /* (NAD 83)\\p26908"*/, 8, 74, 7, -135, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 9" /* (NAD 83)\\p26909"*/, 8, 74, 7, -129, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 10" /* (NAD 83)\\p26910"*/, 8, 74, 7, -123, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 11" /* (NAD 83)\\p26911"*/, 8, 74, 7, -117, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 12" /* (NAD 83)\\p26912"*/, 8, 74, 7, -111, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 13" /* (NAD 83)\\p26913"*/, 8, 74, 7, -105, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 14" /* (NAD 83)\\p26914"*/, 8, 74, 7, -99, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 15" /* (NAD 83)\\p26915"*/, 8, 74, 7, -93, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 16" /* (NAD 83)\\p26916"*/, 8, 74, 7, -87, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 17" /* (NAD 83)\\p26917"*/, 8, 74, 7, -81, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 18" /* (NAD 83)\\p26918"*/, 8, 74, 7, -75, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 19" /* (NAD 83)\\p26919"*/, 8, 74, 7, -69, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 20" /* (NAD 83)\\p26920"*/, 8, 74, 7, -63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 21" /* (NAD 83)\\p26921"*/, 8, 74, 7, -57, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 22" /* (NAD 83)\\p26922"*/, 8, 74, 7, -51, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 23" /* (NAD 83)\\p26923"*/, 8, 74, 7, -45, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 24" /* (NAD 83)\\p26924"*/, 8, 74, 7, -39, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 25" /* (NAD 83)\\p26925"*/, 8, 74, 7, -33, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 26" /* (NAD 83)\\p26926"*/, 8, 74, 7, -27, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 27" /* (NAD 83)\\p26927"*/, 8, 74, 7, -21, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 28" /* (NAD 83)\\p26928"*/, 8, 74, 7, -15, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 29" /* (NAD 83)"*/, 8, 74, 7, -9, 0, 0.9996, 500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Universal Transverse Mercator (SAD 69)");

  _AddCurrentSysGeo(NULL, "UTM Zone 17, H�misph�re Nord" /* (SAD 69)"*/, 8, 92, 7, -81, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 17, H�misph�re Sud" /* (SAD 69)\\p29177"*/, 8, 92, 7, -81, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 18, H�misph�re Nord" /* (SAD 69)\\p29118"*/, 8, 92, 7, -75, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 18, H�misph�re Sud" /* (SAD 69)\\p29178"*/, 8, 92, 7, -75, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 19, H�misph�re Nord" /* (SAD 69)\\p29119"*/, 8, 92, 7, -69, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 19, H�misph�re Sud" /* (SAD 69)\\p29179"*/, 8, 92, 7, -69, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 20, H�misph�re Nord" /* (SAD 69)\\p29120"*/, 8, 92, 7, -63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 20, H�misph�re Sud" /* (SAD 69)\\p29180"*/, 8, 92, 7, -63, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 21, H�misph�re Nord" /* (SAD 69)\\p29121"*/, 8, 92, 7, -57, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 21, H�misph�re Sud" /* (SAD 69)\\p29181"*/, 8, 92, 7, -57, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 22, H�misph�re Nord" /* (SAD 69)\\p29122"*/, 8, 92, 7, -51, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 22, H�misph�re Sud" /* (SAD 69)\\p29182"*/, 8, 92, 7, -51, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 23, H�misph�re Sud" /* (SAD 69)\\p29183"*/, 8, 92, 7, -45, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 24, H�misph�re Sud" /* (SAD 69)\\p29184"*/, 8, 92, 7, -39, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 25, H�misph�re Sud" /* (SAD 69)\\p29185"*/, 8, 92, 7, -33, 0, 0.9996, 500000, 10000000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Universal Transverse Mercator (Tokyo Datum)");

  _AddCurrentSysGeo(NULL, "UTM Zone 51" /* (Tokyo)"*/, 8, 97, 7, 123, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 52" /* (Tokyo)"*/, 8, 97, 7, 129, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 53" /* (Tokyo)"*/, 8, 97, 7, 135, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 54" /* (Tokyo)"*/, 8, 97, 7, 141, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 55" /* (Tokyo)"*/, 8, 97, 7, 147, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 56" /* (Tokyo)"*/, 8, 97, 7, 153, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 57" /* (Tokyo)"*/, 8, 97, 7, 159, 0, 0.9996, 500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("Universal Transverse Mercator (WGS 84)");

  _AddCurrentSysGeo(NULL, "UTM Zone 1, H�misph�re Nord" /* (WGS 84)\\p32601"*/, 8, 104, 7, -177, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 1, H�misph�re Sud" /* (WGS 84)\\p32701"*/, 8, 104, 7, -177, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 2, H�misph�re Nord" /* (WGS 84)\\p32602"*/, 8, 104, 7, -171, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 2, H�misph�re Sud" /* (WGS 84)\\p32702"*/, 8, 104, 7, -171, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 3, H�misph�re Nord" /* (WGS 84)\\p32603"*/, 8, 104, 7, -165, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 3, H�misph�re Sud" /* (WGS 84)\\p32703"*/, 8, 104, 7, -165, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 4, H�misph�re Nord" /* (WGS 84)\\p32604"*/, 8, 104, 7, -159, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 4, H�misph�re Sud" /* (WGS 84)\\p32704"*/, 8, 104, 7, -159, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 5, H�misph�re Nord" /* (WGS 84)\\p32605"*/, 8, 104, 7, -153, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 5, H�misph�re Sud" /* (WGS 84)\\p32705"*/, 8, 104, 7, -153, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 6, H�misph�re Nord" /* (WGS 84)\\p32606"*/, 8, 104, 7, -147, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 6, H�misph�re Sud" /* (WGS 84)\\p32706"*/, 8, 104, 7, -147, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 7, H�misph�re Nord" /* (WGS 84)\\p32607"*/, 8, 104, 7, -141, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 7, H�misph�re Sud" /* (WGS 84)\\p32707"*/, 8, 104, 7, -141, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 8, H�misph�re Nord" /* (WGS 84)\\p32608"*/, 8, 104, 7, -135, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 8, H�misph�re Sud" /* (WGS 84)\\p32708"*/, 8, 104, 7, -135, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 9, H�misph�re Nord" /* (WGS 84)\\p32609"*/, 8, 104, 7, -129, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 9, H�misph�re Sud" /* (WGS 84)\\p32709"*/, 8, 104, 7, -129, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 10, H�misph�re Nord" /* (WGS 84)\\p32610"*/, 8, 104, 7, -123, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 10, H�misph�re Sud" /* (WGS 84)\\p32710"*/, 8, 104, 7, -123, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 11, H�misph�re Nord" /* (WGS 84)\\p32611"*/, 8, 104, 7, -117, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 11, H�misph�re Sud" /* (WGS 84)\\p32711"*/, 8, 104, 7, -117, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 12, H�misph�re Nord" /* (WGS 84)\\p32612"*/, 8, 104, 7, -111, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 12, H�misph�re Sud" /* (WGS 84)\\p32712"*/, 8, 104, 7, -111, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 13, H�misph�re Nord" /* (WGS 84)\\p32613"*/, 8, 104, 7, -105, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 13, H�misph�re Sud" /* (WGS 84)\\p32713"*/, 8, 104, 7, -105, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 14, H�misph�re Nord" /* (WGS 84)\\p32614"*/, 8, 104, 7, -99, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 14, H�misph�re Sud" /* (WGS 84)\\p32714"*/, 8, 104, 7, -99, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 15, H�misph�re Nord" /* (WGS 84)\\p32615"*/, 8, 104, 7, -93, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 15, H�misph�re Sud" /* (WGS 84)\\p32715"*/, 8, 104, 7, -93, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 16, H�misph�re Nord" /* (WGS 84)\\p32616"*/, 8, 104, 7, -87, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 16, H�misph�re Sud" /* (WGS 84)\\p32716"*/, 8, 104, 7, -87, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 17, H�misph�re Nord" /* (WGS 84)\\p32617"*/, 8, 104, 7, -81, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 17, H�misph�re Sud" /* (WGS 84)\\p32717"*/, 8, 104, 7, -81, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 18, H�misph�re Nord" /* (WGS 84)\\p32618"*/, 8, 104, 7, -75, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 18, H�misph�re Sud" /* (WGS 84)\\p32718"*/, 8, 104, 7, -75, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 19, H�misph�re Nord" /* (WGS 84)\\p32619"*/, 8, 104, 7, -69, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 19, H�misph�re Sud" /* (WGS 84)\\p32719"*/, 8, 104, 7, -69, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo("UTM20", "UTM Zone 20, H�misph�re Nord" /* (WGS 84)\\p32620"*/, 8, 104, 7, -63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 20, H�misph�re Sud" /* (WGS 84)\\p32720"*/, 8, 104, 7, -63, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 21, H�misph�re Nord" /* (WGS 84)\\p32621"*/, 8, 104, 7, -57, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 21, H�misph�re Sud" /* (WGS 84)\\p32721"*/, 8, 104, 7, -57, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo("UTM22", "UTM Zone 22, H�misph�re Nord" /* (WGS 84)\\p32622"*/, 8, 104, 7, -51, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 22, H�misph�re Sud" /* (WGS 84)\\p32722"*/, 8, 104, 7, -51, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 23, H�misph�re Nord" /* (WGS 84)\\p32623"*/, 8, 104, 7, -45, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 23, H�misph�re Sud" /* (WGS 84)\\p32723"*/, 8, 104, 7, -45, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 24, H�misph�re Nord" /* (WGS 84)\\p32624"*/, 8, 104, 7, -39, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 24, H�misph�re Sud" /* (WGS 84)\\p32724"*/, 8, 104, 7, -39, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 25, H�misph�re Nord" /* (WGS 84)\\p32625"*/, 8, 104, 7, -33, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 25, H�misph�re Sud" /* (WGS 84)\\p32725"*/, 8, 104, 7, -33, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 26, H�misph�re Nord" /* (WGS 84)\\p32626"*/, 8, 104, 7, -27, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 26, H�misph�re Sud" /* (WGS 84)\\p32726"*/, 8, 104, 7, -27, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 27, H�misph�re Nord" /* (WGS 84)\\p32627"*/, 8, 104, 7, -21, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 27, H�misph�re Sud" /* (WGS 84)\\p32727"*/, 8, 104, 7, -21, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo("UTM28", "UTM Zone 28, H�misph�re Nord" /* (WGS 84)\\p32628"*/, 8, 104, 7, -15, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 28, H�misph�re Sud" /* (WGS 84)\\p32728"*/, 8, 104, 7, -15, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 29, H�misph�re Nord" /* (WGS 84)\\p32629"*/, 8, 104, 7, -9, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 29, H�misph�re Sud" /* (WGS 84)\\p32729"*/, 8, 104, 7, -9, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo("UTM30", "UTM Zone 30, H�misph�re Nord" /* (WGS 84)\\p32630"*/, 8, 104, 7, -3, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 30, H�misph�re Sud" /* (WGS 84)\\p32730"*/, 8, 104, 7, -3, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo("UTM31WGS84", "UTM Zone 31, H�misph�re Nord" /* (WGS 84)\\p32631"*/, 8, 104, 7, 3, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 31, H�misph�re Sud" /* (WGS 84)\\p32731"*/, 8, 104, 7, 3, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo("UTM32WGS84", "UTM Zone 32, H�misph�re Nord" /* (WGS 84)\\p32632"*/, 8, 104, 7, 9, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 32, H�misph�re Sud" /* (WGS 84)\\p32732"*/, 8, 104, 7, 9, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo("UTM33", "UTM Zone 33, H�misph�re Nord" /* (WGS 84)\\p32633"*/, 8, 104, 7, 15, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 33, H�misph�re Sud" /* (WGS 84)\\p32733"*/, 8, 104, 7, 15, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 34, H�misph�re Nord" /* (WGS 84)\\p32634"*/, 8, 104, 7, 21, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 34, H�misph�re Sud" /* (WGS 84)\\p32734"*/, 8, 104, 7, 21, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 35, H�misph�re Nord" /* (WGS 84)\\p32635"*/, 8, 104, 7, 27, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 35, H�misph�re Sud" /* (WGS 84)\\p32735"*/, 8, 104, 7, 27, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 36, H�misph�re Nord" /* (WGS 84)\\p32636"*/, 8, 104, 7, 33, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 36, H�misph�re Sud" /* (WGS 84)\\p32736"*/, 8, 104, 7, 33, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 37, H�misph�re Nord" /* (WGS 84)\\p32637"*/, 8, 104, 7, 39, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 37, H�misph�re Sud" /* (WGS 84)\\p32737"*/, 8, 104, 7, 39, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 38, H�misph�re Nord" /* (WGS 84)\\p32638"*/, 8, 104, 7, 45, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 38, H�misph�re Sud" /* (WGS 84)\\p32738"*/, 8, 104, 7, 45, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 39, H�misph�re Nord" /* (WGS 84)\\p32639"*/, 8, 104, 7, 51, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 39, H�misph�re Sud" /* (WGS 84)\\p32739"*/, 8, 104, 7, 51, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 40, H�misph�re Nord" /* (WGS 84)\\p32640"*/, 8, 104, 7, 57, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 40, H�misph�re Sud" /* (WGS 84)\\p32740"*/, 8, 104, 7, 57, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 41, H�misph�re Nord" /* (WGS 84)\\p32641"*/, 8, 104, 7, 63, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 41, H�misph�re Sud" /* (WGS 84)\\p32741"*/, 8, 104, 7, 63, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 42, H�misph�re Nord" /* (WGS 84)\\p32642"*/, 8, 104, 7, 69, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 42, H�misph�re Sud" /* (WGS 84)\\p32742"*/, 8, 104, 7, 69, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 43, H�misph�re Nord" /* (WGS 84)\\p32643"*/, 8, 104, 7, 75, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 43, H�misph�re Sud" /* (WGS 84)\\p32743"*/, 8, 104, 7, 75, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 44, H�misph�re Nord" /* (WGS 84)\\p32644"*/, 8, 104, 7, 81, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 44, H�misph�re Sud" /* (WGS 84)\\p32744"*/, 8, 104, 7, 81, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 45, H�misph�re Nord" /* (WGS 84)\\p32645"*/, 8, 104, 7, 87, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 45, H�misph�re Sud" /* (WGS 84)\\p32745"*/, 8, 104, 7, 87, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 46, H�misph�re Nord" /* (WGS 84)\\p32646"*/, 8, 104, 7, 93, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 46, H�misph�re Sud" /* (WGS 84)\\p32746"*/, 8, 104, 7, 93, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 47, H�misph�re Nord" /* (WGS 84)\\p32647"*/, 8, 104, 7, 99, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 47, H�misph�re Sud" /* (WGS 84)\\p32747"*/, 8, 104, 7, 99, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 48, H�misph�re Nord" /* (WGS 84)\\p32648"*/, 8, 104, 7, 105, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 48, H�misph�re Sud" /* (WGS 84)\\p32748"*/, 8, 104, 7, 105, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 49, H�misph�re Nord" /* (WGS 84)\\p32649"*/, 8, 104, 7, 111, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 49, H�misph�re Sud" /* (WGS 84)\\p32749"*/, 8, 104, 7, 111, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 50, H�misph�re Nord" /* (WGS 84)\\p32650"*/, 8, 104, 7, 117, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 50, H�misph�re Sud" /* (WGS 84)\\p32750"*/, 8, 104, 7, 117, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 51, H�misph�re Nord" /* (WGS 84)\\p32651"*/, 8, 104, 7, 123, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 51, H�misph�re Sud" /* (WGS 84)\\p32751"*/, 8, 104, 7, 123, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 52, H�misph�re Nord" /* (WGS 84)\\p32652"*/, 8, 104, 7, 129, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 52, H�misph�re Sud" /* (WGS 84)\\p32752"*/, 8, 104, 7, 129, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 53, H�misph�re Nord" /* (WGS 84)\\p32653"*/, 8, 104, 7, 135, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 53, H�misph�re Sud" /* (WGS 84)\\p32753"*/, 8, 104, 7, 135, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 54, H�misph�re Nord" /* (WGS 84)\\p32654"*/, 8, 104, 7, 141, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 54, H�misph�re Sud" /* (WGS 84)\\p32754"*/, 8, 104, 7, 141, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 55, H�misph�re Nord" /* (WGS 84)\\p32655"*/, 8, 104, 7, 147, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 55, H�misph�re Sud" /* (WGS 84)\\p32755"*/, 8, 104, 7, 147, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 56, H�misph�re Nord" /* (WGS 84)\\p32656"*/, 8, 104, 7, 153, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 56, H�misph�re Sud" /* (WGS 84)\\p32756"*/, 8, 104, 7, 153, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 57, H�misph�re Nord" /* (WGS 84)\\p32657"*/, 8, 104, 7, 159, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 57, H�misph�re Sud" /* (WGS 84)\\p32757"*/, 8, 104, 7, 159, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 58, H�misph�re Nord" /* (WGS 84)\\p32658"*/, 8, 104, 7, 165, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 58, H�misph�re Sud" /* (WGS 84)\\p32758"*/, 8, 104, 7, 165, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 59, H�misph�re Nord" /* (WGS 84)\\p32659"*/, 8, 104, 7, 171, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 59, H�misph�re Sud" /* (WGS 84)\\p32759"*/, 8, 104, 7, 171, 0, 0.9996, 500000, 10000000);
  _AddCurrentSysGeo(NULL, "UTM Zone 60, H�misph�re Nord" /* (WGS 84)\\p32660"*/, 8, 104, 7, 177, 0, 0.9996, 500000, 0);
  _AddCurrentSysGeo(NULL, "UTM Zone 60, H�misph�re Sud" /* (WGS 84)\\p32760"*/, 8, 104, 7, 177, 0, 0.9996, 500000, 10000000);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("US State Plane Coordinate Systems (1927)");

  _AddCurrentSysGeo(NULL, "Alabama, Eastern Zone" /* (1927)\\p26729"*/, 8, 62, 8, -85.8333333333, 30.5, 0.99996, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alabama, Western Zone" /* (1927)\\p26730"*/, 8, 62, 8, -87.5, 30, 0.9999333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 1" /* (1927)\\p26731"*/, 7, 63, 8, -133.6666666667, 57, -36.8698976458, 0.9999, 16404166.6667, -16404166.6667);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 2" /* (1927)\\p26732"*/, 8, 63, 8, -142, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 3" /* (1927)\\p26733"*/, 8, 63, 8, -146, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 4" /* (1927)\\p26734"*/, 8, 63, 8, -150, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 5" /* (1927)\\p26735"*/, 8, 63, 8, -154, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 6" /* (1927)\\p26736"*/, 8, 63, 8, -158, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 7" /* (1927)\\p26737"*/, 8, 63, 8, -162, 54, 0.9999, 700000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 8" /* (1927)\\p26738"*/, 8, 63, 8, -166, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 9" /* (1927)\\p26739"*/, 8, 63, 8, -170, 54, 0.9999, 600000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 10" /* (1927)\\p26740"*/, 3, 63, 8, -176, 51, 51.8333333333, 53.8333333333, 3000000, 0);
  _AddCurrentSysGeo(NULL, "Arizona, Eastern Zone" /* (1927)\\p26748"*/, 8, 62, 8, -110.1666666667, 31, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Arizona, Central Zone" /* (1927)\\p26749"*/, 8, 62, 8, -111.9166666667, 31, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Arizona, Western Zone" /* (1927)\\p26750"*/, 8, 62, 8, -113.75, 31, 0.9999333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Arkansas, Northern Zone" /* (1927)\\p26751"*/, 3, 62, 8, -92, 34.3333333333, 34.9333333333, 36.2333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Arkansas, Southern Zone" /* (1927)\\p26752"*/, 3, 62, 8, -92, 32.6666666667, 33.3, 34.7666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "California, Zone I" /* (1927)\\p26741"*/, 3, 62, 8, -122, 39.3333333333, 40, 41.6666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "California, Zone II" /* (1927)\\p26742"*/, 3, 62, 8, -122, 37.6666666667, 38.3333333333, 39.8333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "California, Zone III" /* (1927)\\p26743"*/, 3, 62, 8, -120.5, 36.5, 37.0666666667, 38.4333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "California, Zone IV" /* (1927)\\p26744"*/, 3, 62, 8, -119, 35.3333333333, 36, 37.25, 2000000, 0);
  _AddCurrentSysGeo(NULL, "California, Zone V" /* (1927)\\p26745"*/, 3, 62, 8, -118, 33.5, 34.0333333333, 35.4666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "California, Zone VI" /* (1927)\\p26746"*/, 3, 62, 8, -116.25, 32.1666666667, 32.7833333333, 33.8833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "California, Zone VII" /* (1927)\\p26747"*/, 3, 62, 8, -118.3333333333, 34.1333333333, 33.8666666667, 34.4166666667, 4186692.58, 4160926.74);
  _AddCurrentSysGeo(NULL, "Colorado, Northern Zone" /* (1927)\\p26753"*/, 3, 62, 8, -105.5, 39.3333333333, 39.7166666667, 40.7833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Colorado, Central Zone" /* (1927)\\p26754"*/, 3, 62, 8, -105.5, 37.8333333333, 38.45, 39.75, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Colorado, Southern Zone" /* (1927)\\p26755"*/, 3, 62, 8, -105.5, 36.6666666667, 37.2333333333, 38.4333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Connecticut" /* (1927)\\p26756"*/, 3, 62, 8, -72.75, 40.8333333333, 41.2, 41.8666666667, 600000, 0);
  _AddCurrentSysGeo(NULL, "Delaware" /* (1927)\\p26757"*/, 8, 62, 8, -75.4166666667, 38, 0.999995, 500000, 0);
  _AddCurrentSysGeo(NULL, "Florida, Northern Zone" /* (1927)\\p26760"*/, 3, 62, 8, -84.5, 29, 29.5833333333, 30.75, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Florida, Eastern Zone" /* (1927)\\p26758"*/, 8, 62, 8, -81, 24.3333333333, 0.9999411765, 500000, 0);
  _AddCurrentSysGeo(NULL, "Florida, Western Zone" /* (1927)\\p26759"*/, 8, 62, 8, -82, 24.3333333333, 0.9999411765, 500000, 0);
  _AddCurrentSysGeo(NULL, "Georgia, Eastern Zone" /* (1927)\\p26766"*/, 8, 62, 8, -82.1666666667, 30, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Georgia, Western Zone" /* (1927)\\p26767"*/, 8, 62, 8, -84.1666666667, 30, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 1" /* (1927)\\p26761"*/, 8, 62, 8, -155.5, 18.8333333333, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 2" /* (1927)\\p26762"*/, 8, 62, 8, -156.6666666667, 20.3333333333, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 3" /* (1927)\\p26763"*/, 8, 62, 8, -158, 21.1666666667, 0.99999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 4" /* (1927)\\p26764"*/, 8, 62, 8, -159.5, 21.8333333333, 0.99999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 5" /* (1927)\\p26765"*/, 8, 62, 8, -160.1666666667, 21.6666666667, 1, 500000, 0);
  _AddCurrentSysGeo(NULL, "Idaho, Eastern Zone" /* (1927)\\p26768"*/, 8, 62, 8, -112.1666666667, 41.6666666667, 0.9999473684, 500000, 0);
  _AddCurrentSysGeo(NULL, "Idaho, Central Zone" /* (1927)\\p26769"*/, 8, 62, 8, -114, 41.6666666667, 0.9999473684, 500000, 0);
  _AddCurrentSysGeo(NULL, "Idaho, Western Zone" /* (1927)\\p26770"*/, 8, 62, 8, -115.75, 41.6666666667, 0.9999333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Illinois, Eastern Zone" /* (1927)\\p26771"*/, 8, 62, 8, -88.3333333333, 36.6666666667, 0.999975, 500000, 0);
  _AddCurrentSysGeo(NULL, "Illinois, Western Zone" /* (1927)\\p26772"*/, 8, 62, 8, -90.1666666667, 36.6666666667, 0.9999411765, 500000, 0);
  _AddCurrentSysGeo(NULL, "Indiana, Eastern Zone" /* (1927)\\p26773"*/, 8, 62, 8, -85.6666666667, 37.5, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "Indiana, Western Zone" /* (1927)\\p26774"*/, 8, 62, 8, -87.0833333333, 37.5, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "Iowa, Northern Zone" /* (1927)\\p26775"*/, 3, 62, 8, -93.5, 41.5, 42.0666666667, 43.2666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Iowa, Southern Zone" /* (1927)\\p26776"*/, 3, 62, 8, -93.5, 40, 40.6166666667, 41.7833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Kansas, Northern Zone" /* (1927)\\p26777"*/, 3, 62, 8, -98, 38.3333333333, 38.7166666667, 39.7833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Kansas, Southern Zone" /* (1927)\\p26778"*/, 3, 62, 8, -98.5, 36.6666666667, 38.5666666667, 37.2666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Kentucky, Northern Zone" /* (1927)\\p26779"*/, 3, 62, 8, -84.25, 37.5, 37.9666666667, 38.9666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Kentucky, Southern Zone" /* (1927)\\p26780"*/, 3, 62, 8, -85.75, 36.3333333333, 36.7333333333, 37.9333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Louisiana, Northern Zone" /* (1927)\\p26781"*/, 3, 62, 8, -92.5, 30.6666666667, 31.1666666667, 32.6666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Louisiana, Southern Zone" /* (1927)\\p26782"*/, 3, 62, 8, -91.3333333333, 28.6666666667, 29.3, 30.67, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Louisiana, Offshore Zone (1927)", 3, 62, 8, -91.3333333333, 25.6666666667, 26.1666666667, 27.8333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Maine, Eastern Zone" /* (1927)\\p26783"*/, 8, 62, 8, -68.5, 43.8333333333, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Maine, Western Zone" /* (1927)\\p26784"*/, 8, 62, 8, -70.1666666667, 42.8333333333, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "Maryland" /* (1927)\\p26785"*/, 3, 62, 8, -77, 37.8333333333, 38.3, 39.45, 800000, 0);
  _AddCurrentSysGeo(NULL, "Massachusetts, Mainland Zone" /* (1927)\\p26786"*/, 3, 62, 8, -71.5, 41, 41.7166666667, 42.6833333333, 600000, 0);
  _AddCurrentSysGeo(NULL, "Massachusetts, Island Zone" /* (1927)\\p26787"*/, 3, 62, 8, -70.5, 41, 41.2833333333, 41.4833333333, 800000, 0);
  _AddCurrentSysGeo(NULL, "Michigan, Northern Zone" /* (1927)\\p26788"*/, 3, 73, 8, -87, 44.7833333333, 45.4833333333, 47.0833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Michigan, Central Zone" /* (1927)\\p26789"*/, 3, 73, 8, -84.3333333333, 43.3166666667, 44.1833333333, 45.7, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Michigan, Southern Zone" /* (1927)\\p26790"*/, 3, 73, 8, -84.3333333333, 41.5, 42.1, 43.6666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Minnesota, Northern Zone" /* (1927)\\p26791"*/, 3, 62, 8, -93.1, 46.5, 47.0333333333, 48.6333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Minnesota, Central Zone" /* (1927)\\p26792"*/, 3, 62, 8, -94.25, 45, 45.6166666667, 47.05, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Minnesota, South Zone" /* (1927)\\p26793"*/, 3, 62, 8, -94, 43, 43.7833333333, 45.2166666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Mississippi, Eastern Zone" /* (1927)\\p26794"*/, 8, 62, 8, -88.8333333333, 29.6666666667, 0.999996, 500000, 0);
  _AddCurrentSysGeo(NULL, "Mississippi, Western Zone" /* (1927)\\p26795"*/, 8, 62, 8, -90.3333333333, 30.5, 0.9999411765, 500000, 0);
  _AddCurrentSysGeo(NULL, "Missouri, Eastern Zone" /* (1927)\\p26796"*/, 8, 62, 8, -90.5, 35.8333333333, 0.9999333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Missouri, Central Zone" /* (1927)\\p26797"*/, 8, 62, 8, -92.5, 35.8333333333, 0.9999333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Missouri, Western Zone" /* (1927)\\p26798"*/, 8, 62, 8, -94.5, 36.1666666667, 0.9999411765, 500000, 0);
  _AddCurrentSysGeo(NULL, "Montana, Northern Zone" /* (1927)\\p32001"*/, 3, 62, 8, -109.5, 47, 47.85, 48.7166666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Montana, Central Zone" /* (1927)\\p32002"*/, 3, 62, 8, -109.5, 45.8333333333, 46.45, 47.8833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Montana, Southern Zone" /* (1927)\\p32003"*/, 3, 62, 8, -109.5, 44, 44.8666666667, 46.4, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Nebraska, Northern Zone" /* (1927)\\p32005"*/, 3, 62, 8, -100, 41.3333333333, 41.85, 42.8166666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Nebraska, Southern Zone" /* (1927)\\p32006"*/, 3, 62, 8, -99.5, 39.6666666667, 40.2833333333, 41.7166666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Nevada, Eastern Zone" /* (1927)\\p32007"*/, 8, 62, 8, -115.5833333333, 34.75, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Nevada, Central Zone" /* (1927)\\p32008"*/, 8, 62, 8, -116.6666666667, 34.75, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Nevada, Western Zone" /* (1927)\\p32009"*/, 8, 62, 8, -118.5833333333, 34.75, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "New Hampshire" /* (1927)\\p32010"*/, 8, 62, 8, -71.6666666667, 42.5, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "New Jersey" /* (1927)\\p32011"*/, 8, 62, 8, -74.6666666667, 38.8333333333, 0.999975, 2000000, 0);
  _AddCurrentSysGeo(NULL, "New Mexico, Eastern Zone" /* (1927)\\p32012"*/, 8, 62, 8, -104.3333333333, 31, 0.9999090909, 500000, 0);
  _AddCurrentSysGeo(NULL, "New Mexico, Central Zone" /* (1927)\\p32013"*/, 8, 62, 8, -106.25, 31, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "New Mexico, Western Zone" /* (1927)\\p32014"*/, 8, 62, 8, -107.8333333333, 31, 0.9999166667, 500000, 0);
  _AddCurrentSysGeo(NULL, "New York, Eastern Zone" /* (1927)\\p32015"*/, 8, 62, 8, -74.3333333333, 40, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "New York, Central Zone" /* (1927)\\p32016"*/, 8, 62, 8, -76.5833333333, 40, 0.9999375, 500000, 0);
  _AddCurrentSysGeo(NULL, "New York, Western Zone" /* (1927)\\p32017"*/, 8, 62, 8, -78.5833333333, 40, 0.9999375, 500000, 0);
  _AddCurrentSysGeo(NULL, "New York, Long Island" /* (1927)\\p32018"*/, 3, 62, 8, -74, 40.5, 40.6666666667, 41.0333333333, 2000000, 100000);
  _AddCurrentSysGeo(NULL, "North Carolina" /* (1927)\\p32019"*/, 3, 62, 8, -79, 33.75, 34.3333333333, 36.1666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "North Dakota, Northern Zone" /* (1927)\\p32020"*/, 3, 62, 8, -100.5, 47, 47.4333333333, 48.7333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "North Dakota, Southern Zone" /* (1927)\\p32021"*/, 3, 62, 8, -100.5, 45.6666666667, 46.1833333333, 47.4833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Ohio, Northern Zone" /* (1927)\\p32022"*/, 3, 62, 8, -82.5, 39.6666666667, 40.4333333333, 41.7, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Ohio, Southern Zone" /* (1927)\\p32023"*/, 3, 62, 8, -82.5, 38, 38.7333333333, 40.0333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Oklahoma, Northern Zone" /* (1927)\\p32024"*/, 3, 62, 8, -98, 35, 35.5666666667, 36.7666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Oklahoma, Southern Zone" /* (1927)\\p32025"*/, 3, 62, 8, -98, 33.3333333333, 33.9333333333, 35.2333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Oregon, Northern Zone" /* (1927)\\p32026"*/, 3, 62, 8, -120.5, 43.6666666667, 44.3333333333, 46, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Oregon, Southern Zone" /* (1927)\\p32027"*/, 3, 62, 8, -120.5, 41.6666666667, 42.3333333333, 44, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Pennsylvania, Northern Zone" /* (1927)\\p32028"*/, 3, 62, 8, -77.75, 40.1666666667, 40.8833333333, 41.95, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Pennsylvania, Southern Zone" /* (1927)\\p32029"*/, 3, 62, 8, -77.75, 39.3333333333, 39.9333333333, 40.9666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Puerto Rico and Virgin Islands, Zone 1" /* (1927)\\p32059"*/, 3, 62, 8, -66.4333333333, 18.4333333333, 18.0333333333, 18.4333333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Puerto Rico, St. Croix, Virgin Islands, Zone 2" /* (1927)\\p32060"*/, 3, 62, 8, -66.4333333333, 18.4333333333, 18.0333333333, 18.4333333333, 500000, 100000);
  _AddCurrentSysGeo(NULL, "Rhode Island" /* (1927)\\p32030"*/, 8, 62, 8, -71.5, 41.0833333333, 0.99999375, 500000, 0);
  _AddCurrentSysGeo(NULL, "South Carolina, Northern Zone" /* (1927)\\p32031"*/, 3, 62, 8, -81, 33, 33.7666666667, 34.9666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "South Carolina, Southern Zone" /* (1927)\\p32033"*/, 3, 62, 8, -81, 31.8333333333, 32.3333333333, 33.6666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "South Dakota, Northern Zone" /* (1927)\\p32034"*/, 3, 62, 8, -100, 43.8333333333, 44.4166666667, 45.6833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "South Dakota, Southern Zone" /* (1927)\\p32035"*/, 3, 62, 8, -100.3333333333, 42.3333333333, 42.8333333333, 44.4, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Tennessee" /* (1927)\\p32036"*/, 3, 62, 8, -86, 34.6666666667, 35.25, 36.4166666667, 2000000, 100000);
  _AddCurrentSysGeo(NULL, "Texas, Northern Zone" /* (1927)\\p32037"*/, 3, 62, 8, -101.5, 34, 34.65, 36.1833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Texas, North Central Zone" /* (1927)\\p32038"*/, 3, 62, 8, -97.5, 31.6666666667, 32.1333333333, 33.9666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Texas, Central Zone" /* (1927)\\p32039"*/, 3, 62, 8, -100.3333333333, 29.6666666667, 30.1166666667, 31.8833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Texas, South Central Zone" /* (1927)\\p32040"*/, 3, 62, 8, -99, 27.8333333333, 28.3833333333, 30.2833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Texas, Southern Zone" /* (1927)\\p32041"*/, 3, 62, 8, -98.5, 25.6666666667, 26.1666666667, 27.8333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Utah, Northern Zone" /* (1927)\\p32042"*/, 3, 62, 8, -111.5, 40.3333333333, 40.7166666667, 41.7833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Utah, Central Zone" /* (1927)\\p32043"*/, 3, 62, 8, -111.5, 38.3333333333, 39.0166666667, 40.65, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Utah, Southern Zone" /* (1927)\\p32044"*/, 3, 62, 8, -111.5, 36.6666666667, 37.2166666667, 38.35, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Vermont" /* (1927)\\p32045"*/, 8, 62, 8, -72.5, 42.5, 0.9999642857, 500000, 0);
  _AddCurrentSysGeo(NULL, "Virginia, Northern Zone" /* (1927)\\p32046"*/, 3, 62, 8, -78.5, 37.6666666667, 38.0333333333, 39.2, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Virginia, Southern Zone" /* (1927)\\p32047"*/, 3, 62, 8, -78.5, 36.3333333333, 36.7666666667, 37.9666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Washington, Northern Zone" /* (1927)\\p32048"*/, 3, 62, 8, -120.8333333333, 47, 47.5, 48.7333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Washington, Southern Zone" /* (1927)\\p32049"*/, 3, 62, 8, -120.5, 45.3333333333, 45.8333333333, 47.3333333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "West Virginia, Northern Zone" /* (1927)\\p32050"*/, 3, 62, 8, -79.5, 38.5, 39, 40.25, 2000000, 0);
  _AddCurrentSysGeo(NULL, "West Virginia, Southern Zone" /* (1927)\\p32051"*/, 3, 62, 8, -81, 37, 37.4833333333, 38.8833333333, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Wisconsin, Northern Zone" /* (1927)\\p32052"*/, 3, 62, 8, -90, 45.1666666667, 45.5666666667, 46.7666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Wisconsin, Central Zone" /* (1927)\\p32053"*/, 3, 62, 8, -90, 43.8333333333, 44.25, 45.5, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Wisconsin, Southern Zone" /* (1927)\\p32054"*/, 3, 62, 8, -90, 42, 42.7333333333, 44.0666666667, 2000000, 0);
  _AddCurrentSysGeo(NULL, "Wyoming, Eastern Zone" /* (1927)\\p32055"*/, 8, 62, 8, -105.1666666667, 40.6666666667, 0.9999411765, 500000, 0);
  _AddCurrentSysGeo(NULL, "Wyoming, East Central Zone" /* (1927)\\p32056"*/, 8, 62, 8, -107.3333333333, 40.6666666667, 0.9999411765, 500000, 0);
  _AddCurrentSysGeo(NULL, "Wyoming, West Central Zone" /* (1927)\\p32057"*/, 8, 62, 8, -108.75, 40.6666666667, 0.9999411765, 500000, 0);
  _AddCurrentSysGeo(NULL, "Wyoming, Western Zone" /* (1927)\\p32058"*/, 8, 62, 8, -110.0833333333, 40.6666666667, 0.9999411765, 500000, 0);

  mp_nIndexFam = mp_asStringBuffer.AddStringTail("US State Plane Coordinate Systems (1983)");

  _AddCurrentSysGeo(NULL, "Alabama, Eastern Zone" /* (1983)\\p26929"*/, 8, 74, 7, -85.8333333333, 30.5, 0.99996, 200000, 0);
  _AddCurrentSysGeo(NULL, "Alabama, Western Zone" /* (1983)\\p26930"*/, 8, 74, 7, -87.5, 30.0, 0.9999333333, 600000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 1" /* (1983)\\p26931"*/, 7, 74, 7, -133.6666666667, 57, -36.8698976458, 0.9999, 5000000, -5000000);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 2" /* (1983)\\p26932"*/, 8, 74, 7, -142, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 3" /* (1983)\\p26933"*/, 8, 74, 7, -146, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 4" /* (1983)\\p26934"*/, 8, 74, 7, -150, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 5" /* (1983)\\p26935"*/, 8, 74, 7, -154, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 6" /* (1983)\\p26936"*/, 8, 74, 7, -158, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 7" /* (1983)\\p26937"*/, 8, 74, 7, -162, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 8" /* (1983)\\p26938"*/, 8, 74, 7, -166, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 9" /* (1983)\\p26939"*/, 8, 74, 7, -170, 54, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Alaska, Zone 10" /* (1983)\\p26940"*/, 3, 74, 7, -176,51, 51.8333333333, 53.8333333333, 1000000, 0);
  _AddCurrentSysGeo(NULL, "Arizona, Eastern Zone" /* (1983)\\p26948"*/, 8, 74, 7, -110.1666666667, 31, 0.9999, 213360, 0);
  _AddCurrentSysGeo(NULL, "Arizona, Central Zone" /* (1983)\\p26949"*/, 8, 74, 7, -111.9166666667, 31, 0.9999, 213360, 0);
  _AddCurrentSysGeo(NULL, "Arizona, Western Zone" /* (1983)\\p26950"*/, 8, 74, 7, -113.75, 31, 0.9999333333, 213360, 0);
  _AddCurrentSysGeo(NULL, "Arkansas, Northern Zone" /* (1983)\\p26951"*/, 3, 74, 7, -92, 34.3333333333, 34.9333333333, 36.2333333333, 400000, 0);
  _AddCurrentSysGeo(NULL, "Arkansas, Southern Zone" /* (1983)\\p26952"*/, 3, 74, 7, -92, 32.6666666667, 33.3, 34.7666666667, 400000, 400000);
  _AddCurrentSysGeo(NULL, "California, Zone I" /* (1983)\\p26941"*/, 3, 74, 7, -122, 39.3333333333, 40, 41.6666666667, 2000000, 500000);
  _AddCurrentSysGeo(NULL, "California, Zone II" /* (1983)\\p26942"*/, 3, 74, 7, -122, 37.6666666667, 38.3333333333, 39.8333333333, 2000000, 500000);
  _AddCurrentSysGeo(NULL, "California, Zone III" /* (1983)\\p26943"*/, 3, 74, 7, -120.5, 36.5, 37.0666666667, 38.4333333333, 2000000, 500000);
  _AddCurrentSysGeo(NULL, "California, Zone IV" /* (1983)\\p26944"*/, 3, 74, 7, -119, 35.3333333333, 36, 37.25, 2000000, 500000);
  _AddCurrentSysGeo(NULL, "California, Zone V" /* (1983)\\p26945"*/, 3, 74, 7, -118, 33.5, 34.0333333333, 35.4666666667, 2000000, 500000);
  _AddCurrentSysGeo(NULL, "California, Zone VI" /* (1983)\\p26946"*/, 3, 74, 7, -116.25, 32.1666666667, 32.7833333333, 33.8833333333, 2000000, 500000);
  _AddCurrentSysGeo(NULL, "Colorado, Northern Zone" /* (1983)\\p26953"*/, 3, 74, 7, -105.5, 39.3333333333, 39.7166666667, 40.7833333333, 914401.8289, 304800.6096);
  _AddCurrentSysGeo(NULL, "Colorado, Central Zone" /* (1983)\\p26954"*/, 3, 74, 7, -105.5, 37.8333333333, 38.45, 39.75, 914401.8289, 304800.6096);
  _AddCurrentSysGeo(NULL, "Colorado, Southern Zone" /* (1983)\\p26955"*/, 3, 74, 7, -105.5, 36.6666666667, 37.2333333333, 38.4333333333, 914401.8289, 304800.6096);
  _AddCurrentSysGeo(NULL, "Connecticut" /* (1983)\\p26956"*/, 3, 74, 7, -72.75, 40.8333333333, 41.2, 41.8666666667, 304800.6096, 152400.3048);
  _AddCurrentSysGeo(NULL, "Delaware" /* (1983)\\p26957"*/, 8, 74, 7, -75.4166666667, 38, 0.999995, 200000, 0);
  _AddCurrentSysGeo(NULL, "Florida, Northern Zone" /* (1983)\\p26960"*/, 3, 74, 7, -84.5, 29, 29.5833333333, 30.75, 600000, 0);
  _AddCurrentSysGeo(NULL, "Florida, Eastern Zone" /* (1983)\\p26958"*/, 8, 74, 7, -81, 24.3333333333, 0.9999411765, 200000, 0);
  _AddCurrentSysGeo(NULL, "Florida, Western Zone" /* (1983)\\p26959"*/, 8, 74, 7, -82, 24.3333333333, 0.9999411765, 200000, 0);
  _AddCurrentSysGeo(NULL, "Georgia, Eastern Zone" /* (1983)\\p26966"*/, 8, 74, 7, -82.1666666667, 30, 0.9999, 200000, 0);
  _AddCurrentSysGeo(NULL, "Georgia, Western Zone" /* (1983)\\p26967"*/, 8, 74, 7, -84.1666666667, 30, 0.9999, 700000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 1" /* (1983)\\p26961"*/, 8, 74, 7, -155.5, 18.8333333333, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 2" /* (1983)\\p26962"*/, 8, 74, 7, -156.6666666667, 20.3333333333, 0.9999666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 3" /* (1983)\\p26963"*/, 8, 74, 7, -158, 21.1666666667, 0.99999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 4" /* (1983)\\p26964"*/, 8, 74, 7, -159.5, 21.8333333333, 0.99999, 500000, 0);
  _AddCurrentSysGeo(NULL, "Hawaii, Zone 5" /* (1983)\\p26965"*/, 8, 74, 7, -160.1666666667, 21.6666666667, 1, 500000, 0);
  _AddCurrentSysGeo(NULL, "Idaho, Eastern Zone" /* (1983)\\p26968"*/, 8, 74, 7, -112.1666666667, 41.6666666667, 0.9999473684, 200000, 0);
  _AddCurrentSysGeo(NULL, "Idaho, Central Zone" /* (1983)\\p26969"*/, 8, 74, 7, -114, 41.6666666667, 0.9999473684, 500000, 0);
  _AddCurrentSysGeo(NULL, "Idaho, Western Zone" /* (1983)\\p26970"*/, 8, 74, 7, -115.75, 41.6666666667, 0.9999333333, 800000, 0);
  _AddCurrentSysGeo(NULL, "Illinois, Eastern Zone" /* (1983)\\p26971"*/, 8, 74, 7, -88.3333333333, 36.6666666667, 0.999975, 300000, 0);
  _AddCurrentSysGeo(NULL, "Illinois, Western Zone" /* (1983)\\p26972"*/, 8, 74, 7, -90.1666666667, 36.6666666667, 0.9999411765, 700000, 0);
  _AddCurrentSysGeo(NULL, "Indiana, Eastern Zone" /* (1983)\\p26973"*/, 8, 74, 7, -85.6666666667, 37.5, 0.9999666667, 100000, 250000);
  _AddCurrentSysGeo(NULL, "Indiana, Western Zone" /* (1983)\\p26974"*/, 8, 74, 7, -87.0833333333, 37.5, 0.9999666667, 900000, 250000);
  _AddCurrentSysGeo(NULL, "Iowa, Northern Zone" /* (1983)\\p26975"*/, 3, 74, 7, -93.5, 41.5, 42.0666666667, 43.2666666667, 1500000, 1000000);
  _AddCurrentSysGeo(NULL, "Iowa, Southern Zone" /* (1983)\\p26976"*/, 3, 74, 7, -93.5, 40, 40.6166666667, 41.7833333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Kansas, Northern Zone" /* (1983)\\p26977"*/, 3, 74, 7, -98, 38.3333333333, 38.7166666667, 39.7833333333, 400000, 0);
  _AddCurrentSysGeo(NULL, "Kansas, Southern Zone" /* (1983)\\p26978"*/, 3, 74, 7, -98.5, 36.6666666667, 38.5666666667, 37.2666666667, 400000, 400000);
  _AddCurrentSysGeo(NULL, "Kentucky, Northern Zone" /* (1983)\\p26979"*/, 3, 74, 7, -84.25, 37.5, 37.9666666667, 38.9666666667, 500000, 0);
  _AddCurrentSysGeo(NULL, "Kentucky, Southern Zone" /* (1983)\\p26980"*/, 3, 74, 7, -85.75, 36.3333333333, 36.7333333333, 37.9333333333, 500000, 500000);
  _AddCurrentSysGeo(NULL, "Louisiana, Northern Zone" /* (1983)\\p26981"*/, 3, 74, 7, -92.5, 30.5, 31.1666666667, 32.6666666667, 1000000, 0);
  _AddCurrentSysGeo(NULL, "Louisiana, Southern Zone" /* (1983)\\p26982"*/, 3, 74, 7, -91.3333333333, 28.5, 29.3, 30.7, 1000000, 0);
  _AddCurrentSysGeo(NULL, "Louisiana, Offshore Zone (1983)", 3, 74, 7, -91.3333333333, 25.5, 26.1666666667, 27.8333333333, 1000000, 0);
  _AddCurrentSysGeo(NULL, "Maine, Eastern Zone" /* (1983)\\p26983"*/, 8, 74, 7, -68.5, 43.6666666667, 0.9999, 300000, 0);
  _AddCurrentSysGeo(NULL, "Maine, Western Zone" /* (1983)\\p26984"*/, 8, 74, 7, -70.1666666667, 42.8333333333, 0.9999666667, 900000, 0);
  _AddCurrentSysGeo(NULL, "Maryland" /* (1983)\\p26985"*/, 3, 74, 7, -77, 37.6666666667, 38.3, 39.45, 400000, 0);
  _AddCurrentSysGeo(NULL, "Massachusetts, Mainland Zone" /* (1983)\\p26986"*/, 3, 74, 7, -71.5, 41, 41.7166666667, 42.6833333333, 200000, 750000);
  _AddCurrentSysGeo(NULL, "Massachusetts, Island Zone" /* (1983)\\p26987"*/, 3, 74, 7, -70.5, 41, 41.2833333333, 41.4833333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Michigan, Northern Zone" /* (1983)\\p26988"*/, 3, 74, 7, -87, 44.7833333333, 45.4833333333, 47.0833333333, 8000000, 0);
  _AddCurrentSysGeo(NULL, "Michigan, Central Zone" /* (1983)\\p26989"*/, 3, 74, 7, -84.3666666667, 43.3166666667, 44.1833333333, 45.7, 6000000, 0);
  _AddCurrentSysGeo(NULL, "Michigan, Southern Zone" /* (1983)\\p26990"*/, 3, 74, 7, -84.3666666667, 41.5, 42.1, 43.6666666667, 4000000, 0);
  _AddCurrentSysGeo(NULL, "Minnesota, Northern Zone" /* (1983)\\p26991"*/, 3, 74, 7, -93.1, 46.5, 47.0333333333, 48.6333333333, 800000, 100000);
  _AddCurrentSysGeo(NULL, "Minnesota, Central Zone" /* (1983)\\p26992"*/, 3, 74, 7, -94.25, 45, 45.6166666667, 47.05, 800000, 100000);
  _AddCurrentSysGeo(NULL, "Minnesota, South Zone" /* (1983)\\p26993"*/, 3, 74, 7, -94, 43, 43.7833333333, 45.2166666667, 800000, 100000);
  _AddCurrentSysGeo(NULL, "Mississippi, Eastern Zone" /* (1983)\\p26994"*/, 8, 74, 7, -88.8333333333, 29.5, 0.99995, 300000, 0);
  _AddCurrentSysGeo(NULL, "Mississippi, Western Zone" /* (1983)\\p26995"*/, 8, 74, 7, -90.3333333333, 29.5, 0.99995, 700000, 0);
  _AddCurrentSysGeo(NULL, "Missouri, Eastern Zone" /* (1983)\\p26996"*/, 8, 74, 7, -90.5, 35.8333333333, 0.9999333333, 250000, 0);
  _AddCurrentSysGeo(NULL, "Missouri, Central Zone" /* (1983)\\p26997"*/, 8, 74, 7, -92.5, 35.8333333333, 0.9999333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Missouri, Western Zone" /* (1983)\\p26998"*/, 8, 74, 7, -94.5, 36.1666666667, 0.9999411765, 850000, 0);
  _AddCurrentSysGeo(NULL, "Montana" /* (1983)\\p32100"*/, 3, 74, 7, -109.5, 44.25, 45, 49, 600000, 0);
  _AddCurrentSysGeo(NULL, "Nebraska" /* (1983)\\p32104"*/, 3, 74, 7, -100, 39.8333333333, 40, 43, 500000, 0);
  _AddCurrentSysGeo(NULL, "Nevada, Eastern Zone" /* (1983)\\p32107"*/, 8, 74, 7, -115.5833333333, 34.75, 0.9999, 200000, 8000000);
  _AddCurrentSysGeo(NULL, "Nevada, Central Zone" /* (1983)\\p32108"*/, 8, 74, 7, -116.6666666667, 34.75, 0.9999, 500000, 6000000);
  _AddCurrentSysGeo(NULL, "Nevada, Western Zone" /* (1983)\\p32109"*/, 8, 74, 7, -118.5833333333, 34.75, 0.9999, 800000, 4000000);
  _AddCurrentSysGeo(NULL, "New Hampshire" /* (1983)\\p32110"*/, 8, 74, 7, -71.6666666667, 42.5, 0.9999666667, 300000, 0);
  _AddCurrentSysGeo(NULL, "New Jersey" /* (1983)\\p32111"*/, 8, 74, 7, -74.5, 38.8333333334, 0.9999, 150000, 0);
  _AddCurrentSysGeo(NULL, "New Mexico, Eastern Zone" /* (1983)\\p32112"*/, 8, 74, 7, -104.3333333333, 31, 0.9999090909, 165000, 0);
  _AddCurrentSysGeo(NULL, "New Mexico, Central Zone" /* (1983)\\p32113"*/, 8, 74, 7, -106.25, 31, 0.9999, 500000, 0);
  _AddCurrentSysGeo(NULL, "New Mexico, Western Zone" /* (1983)\\p32114"*/, 8, 74, 7, -107.8333333333, 31, 0.9999166667, 830000, 0);
  _AddCurrentSysGeo(NULL, "New York, Eastern Zone" /* (1983)\\p32115"*/, 8, 74, 7, -74.5, 38.8333333333, 0.9999, 150000, 0);
  _AddCurrentSysGeo(NULL, "New York, Central Zone" /* (1983)\\p32116"*/, 8, 74, 7, -76.5833333333, 40, 0.9999375, 250000, 0);
  _AddCurrentSysGeo(NULL, "New York, Western Zone" /* (1983)\\p32117"*/, 8, 74, 7, -78.5833333333, 40, 0.9999375, 350000, 0);
  _AddCurrentSysGeo(NULL, "New York, Long Island Zone" /* (1983)\\p32118"*/, 3, 74, 7, -74, 40.1666666667, 40.6666666667, 41.0333333333, 300000, 0);
  _AddCurrentSysGeo(NULL, "North Carolina" /* (1983)\\p32119"*/, 3, 74, 7, -79, 33.75, 34.3333333333, 36.1666666667, 609601.22, 0);
  _AddCurrentSysGeo(NULL, "North Dakota, Northern Zone" /* (1983)\\p32120"*/, 3, 74, 7, -100.5, 47, 47.4333333333, 48.7333333333, 600000, 0);
  _AddCurrentSysGeo(NULL, "North Dakota, Southern Zone" /* (1983)\\p32121"*/, 3, 74, 7, -100.5, 45.6666666667, 46.1833333333, 47.4833333333, 600000, 0);
  _AddCurrentSysGeo(NULL, "Ohio, Northern Zone" /* (1983)\\p32122"*/, 3, 74, 7, -82.5, 39.6666666667, 40.4333333333, 41.7, 600000, 0);
  _AddCurrentSysGeo(NULL, "Ohio, Southern Zone" /* (1983)\\p32123"*/, 3, 74, 7, -82.5, 38, 38.7333333333, 40.0333333333, 600000, 0);
  _AddCurrentSysGeo(NULL, "Oklahoma, Northern Zone" /* (1983)\\p32124"*/, 3, 74, 7, -98, 35, 35.5666666667, 36.7666666667, 600000, 0);
  _AddCurrentSysGeo(NULL, "Oklahoma, Southern Zone" /* (1983)\\p32125"*/, 3, 74, 7, -98, 33.3333333333, 33.9333333333, 35.2333333333, 600000, 0);
  _AddCurrentSysGeo(NULL, "Oregon, Northern Zone" /* (1983)\\p32126"*/, 3, 74, 7, -120.5, 43.6666666667, 44.3333333333, 46, 2500000, 0);
  _AddCurrentSysGeo(NULL, "Oregon, Southern Zone" /* (1983)\\p32127"*/, 3, 74, 7, -120.5, 41.6666666667, 42.3333333333, 44, 1500000, 0);
  _AddCurrentSysGeo(NULL, "Pennsylvania, Northern Zone" /* (1983)\\p32128"*/, 3, 74, 7, -77.75, 40.1666666667, 40.8833333333, 41.95, 600000, 0);
  _AddCurrentSysGeo(NULL, "Pennsylvania, Southern Zone" /* (1983)\\p32129"*/, 3, 74, 7, -77.75, 39.3333333333, 39.9333333333, 40.9666666667, 600000, 0);
  _AddCurrentSysGeo(NULL, "Puerto Rico and Virgin Islands" /* (1983)\\p32161"*/, 3, 74, 7, -66.4333333333, 17.8333333333, 18.0333333333, 18.4333333333, 200000, 200000);
  _AddCurrentSysGeo(NULL, "Rhode Island" /* (1983)\\p32130"*/, 8, 74, 7, -71.5, 41.0833333333, 0.99999375, 100000, 0);
  _AddCurrentSysGeo(NULL, "South Carolina" /* (1983)\\p32133"*/, 3, 74, 7, -81, 31.8333333333, 32.5, 34.8333333333, 609600, 0);
  _AddCurrentSysGeo(NULL, "South Dakota, Northern Zone" /* (1983)\\p32134"*/, 3, 74, 7, -100, 43.8333333333, 44.4166666667, 45.6833333333, 600000, 0);
  _AddCurrentSysGeo(NULL, "South Dakota, Southern Zone" /* (1983)\\p32135"*/, 3, 74, 7, -100.3333333333, 42.3333333333, 42.8333333333, 44.4, 600000, 0);
  _AddCurrentSysGeo(NULL, "Tennessee" /* (1983)\\p32136"*/, 3, 74, 7, -86, 34.3333333333, 35.25, 36.4166666667, 600000, 0);
  _AddCurrentSysGeo(NULL, "Texas, Northern Zone" /* (1983)\\p32137"*/, 3, 74, 7, -101.5, 34, 34.65, 36.1833333333, 200000, 1000000);
  _AddCurrentSysGeo(NULL, "Texas, North Central Zone" /* (1983)\\p32138"*/, 3, 74, 7, -98.5, 31.6666666667, 32.1333333333, 33.9666666667, 600000, 2000000);
  _AddCurrentSysGeo(NULL, "Texas, Central Zone" /* (1983)\\p32139"*/, 3, 74, 7, -100.3333333333, 29.6666666667, 30.1166666667, 31.8833333333, 700000, 3000000);
  _AddCurrentSysGeo(NULL, "Texas, South Central Zone" /* (1983)\\p32140"*/, 3, 74, 7, -99, 27.8333333333, 28.3833333333, 30.2833333333, 600000, 4000000);
  _AddCurrentSysGeo(NULL, "Texas, Southern Zone" /* (1983)\\p32141"*/, 3, 74, 7, -98.5, 25.6666666667, 26.1666666667, 27.8333333333, 300000, 5000000);
  _AddCurrentSysGeo(NULL, "Utah, Northern Zone" /* (1983)\\p32142"*/, 3, 74, 7, -111.5, 40.3333333333, 40.7166666667, 41.7833333333, 500000, 1000000);
  _AddCurrentSysGeo(NULL, "Utah, Central Zone" /* (1983)\\p32143"*/, 3, 74, 7, -111.5, 38.3333333333, 39.0166666667, 40.65, 500000, 2000000);
  _AddCurrentSysGeo(NULL, "Utah, Southern Zone" /* (1983)\\p32144"*/, 3, 74, 7, -111.5, 36.6666666667, 37.2166666667, 38.35, 500000, 3000000);
  _AddCurrentSysGeo(NULL, "Vermont" /* (1983)\\p32145"*/, 8, 74, 7, -72.5, 42.5, 0.9999642857, 500000, 0);
  _AddCurrentSysGeo(NULL, "Virginia, Northern Zone" /* (1983)\\p32146"*/, 3, 74, 7, -78.5, 37.6666666667, 38.0333333333, 39.2, 3500000, 2000000);
  _AddCurrentSysGeo(NULL, "Virginia, Southern Zone" /* (1983)\\p32147"*/, 3, 74, 7, -78.5, 36.3333333333, 36.7666666667, 37.9666666667, 3500000, 1000000);
  _AddCurrentSysGeo(NULL, "Washington, Northern Zone" /* (1983)\\p32148"*/, 3, 74, 7, -120.8333333333, 47, 47.5, 48.7333333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "Washington, Southern Zone" /* (1983)\\p32149"*/, 3, 74, 7, -120.5, 45.3333333333, 45.8333333333, 47.3333333333, 500000, 0);
  _AddCurrentSysGeo(NULL, "West Virginia, Northern Zone" /* (1983)\\p32150"*/, 3, 74, 7, -79.5, 38.5, 39, 40.25, 600000, 0);
  _AddCurrentSysGeo(NULL, "West Virginia, Southern Zone" /* (1983)\\p32151"*/, 3, 74, 7, -81, 37, 37.4833333333, 38.8833333333, 600000, 0);
  _AddCurrentSysGeo(NULL, "Wisconsin, Northern Zone" /* (1983)\\p32152"*/, 3, 74, 7, -90, 45.1666666667, 45.5666666667, 46.7666666667, 600000, 0);
  _AddCurrentSysGeo(NULL, "Wisconsin, Central Zone" /* (1983)\\p32153"*/, 3, 74, 7, -90, 43.8333333333, 44.25, 45.5, 600000, 0);
  _AddCurrentSysGeo(NULL, "Wisconsin, Southern Zone" /* (1983)\\p32154"*/, 3, 74, 7, -90, 42, 42.7333333333, 44.0666666667, 600000, 0);
  _AddCurrentSysGeo(NULL, "Wyoming, Eastern Zone" /* (1983)\\p32155"*/, 8, 74, 7, -105.1666666667, 40.5, 0.9999375, 200000, 0);
  _AddCurrentSysGeo(NULL, "Wyoming, East Central Zone" /* (1983)\\p32156"*/, 8, 74, 7, -107.3333333333, 40.5, 0.9999375, 400000, 100000);
  _AddCurrentSysGeo(NULL, "Wyoming, West Central Zone" /* (1983)\\p32157"*/, 8, 74, 7, -108.75, 40.5, 0.9999375, 600000, 0);
  _AddCurrentSysGeo(NULL, "Wyoming, Western Zone" /* (1983)\\p32158"*/, 8, 74, 7, -110.0833333333, 40.5, 0.9999375, 800000, 100000);
}

void CSigGeoProj::_AddCurrentSysGeo
( char*  i_panProjection // Equivalent CONVERS (si on connais)
, char*  i_panSyntax
, int    i_nTypeOrTableName
, int    i_nDatumOrWindowID
, int    i_nStuffing          // = 0 // <- Sert � ne pas prendre en compte un des param�tres
, double i_nOriginLon         // = 0.0
, double i_nOriginLat         // = 0.0
, double i_nStandartParallel1 // = 0.0
, double i_nStandartParallel2 // = 0.0
, double i_nAzimut            // = 0.0
, double i_nScaleFactor       // = 0.0
, double i_nFalseEasting      // = 0.0
, double i_nFalseNorthing     // = 0.0
, double i_nRange             // = 0.0
)
{
  int   l_nNodeIndex;
  int   l_nStringIndex;
  int   l_nConvIndex;

  // ON CASTE SUR DES INTS POUR :
  // 1 : S'AFFRANCHIR D'UN EXCES DE PRECISION INUTILE
  // 2 : ETRE SUR D'AVOIR STRICTEMENT LES MEMES NOMBRES POUR LE TEST CRC

  int   l_nTempo[9] = 
  { i_nOriginLon         * 1000
  , i_nOriginLat         * 1000
  , i_nStandartParallel1 * 1000
  , i_nStandartParallel2 * 1000
  , i_nAzimut            * 1000
  , i_nScaleFactor       * 1000
  , i_nFalseEasting      * 1000
  , i_nFalseNorthing     * 1000
  , i_nRange             * 1000
  };

  mp_sSigProjCast.nSyntax            = 0;
  mp_sSigProjCast.nTypeOrTableName   = i_nTypeOrTableName;
  mp_sSigProjCast.nDatumOrWindowID   = i_nDatumOrWindowID;
  *((hyper*) &mp_sSigProjCast.anUnitName[0]) = 0;
  mp_sSigProjCast.nOriginLon         = l_nTempo[0]; // CASTATION
  mp_sSigProjCast.nOriginLat         = l_nTempo[1];
  mp_sSigProjCast.nStandartParallel1 = l_nTempo[2];
  mp_sSigProjCast.nStandartParallel2 = l_nTempo[3];
  mp_sSigProjCast.nAzimut            = l_nTempo[4];
  mp_sSigProjCast.nScaleFactor       = l_nTempo[5];
  mp_sSigProjCast.nFalseEasting      = l_nTempo[6];
  mp_sSigProjCast.nFalseNorthing     = l_nTempo[7];
  mp_sSigProjCast.nRange             = l_nTempo[8];

  // Ajoute la structure
  l_nNodeIndex   = mp_asProjecBuffer.AddNodeCRC(&mp_sSigProjCast); // (void*)
  l_nStringIndex = mp_asStringBuffer.AddStringTail(i_panSyntax);

  // S�lectionne la famille en cours
  mp_asProjecBuffer.SetNodeData
  ( l_nNodeIndex
  , l_nStringIndex
  );

  if(i_panProjection != NULL)
  { // Si projection CONVERS
    l_nConvIndex = mp_asConvBuffer.AddStringTail(i_panProjection);

    mp_asConvBuffer.SetStringData
    ( l_nConvIndex
    , l_nNodeIndex
    );
  }
  else
  {
    l_nConvIndex = 0;
  }

  // Ajoute le nom de la structure
  mp_asStringBuffer.SetStringData
  ( l_nStringIndex
  , (l_nConvIndex << 16) | mp_nIndexFam
  );
}

void CSigGeoProj::SelectCurrentSysGeo
( CString& o_roStrProjConvers
, CString& o_roStrCurSysGeo
, CString& o_roStrCurFamGeo
, int&     o_rnUnit
, int&     o_rnMerid
, CSigContainer::sSigProjection* i_psSigProj // = NULL
)
{
  if(i_psSigProj != NULL)
  { // On copie la structure de projection
    m_sSigProj = *i_psSigProj;
  }
  else{}

  // ON CASTE SUR DES INTS POUR :
  // 1 : S'AFFRANCHIR D'UN EXCES DE PRECISION INUTILE
  // 2 : ETRE SUR D'AVOIR STRICTEMENT LES MEMES NOMBRES POUR LE TEST CRC

  int   l_nNodeIndex;
  int   l_nStringIndex;
  int   l_nConvIndex;
  int   l_nTempo[9] = 
  { m_sSigProj.nOriginLon         * 1000
  , m_sSigProj.nOriginLat         * 1000
  , m_sSigProj.nStandartParallel1 * 1000
  , m_sSigProj.nStandartParallel2 * 1000
  , m_sSigProj.nAzimut            * 1000
  , m_sSigProj.nScaleFactor       * 1000
  , m_sSigProj.nFalseEasting      * 1000
  , m_sSigProj.nFalseNorthing     * 1000
  , m_sSigProj.nRange             * 1000
  };

  mp_sSigProjCast.nSyntax            = 0;
  mp_sSigProjCast.nTypeOrTableName   = m_sSigProj.nTypeOrTableName;
  mp_sSigProjCast.nDatumOrWindowID   = m_sSigProj.nDatumOrWindowID;
  *((hyper*) &mp_sSigProjCast.anUnitName[0]) = 0; // Le casting qui TUE !!! Comment initialiser une chaine de 8 caract�res � 0 :)
  mp_sSigProjCast.nOriginLon         = 0.0;
  mp_sSigProjCast.nOriginLat         = 0.0;
  mp_sSigProjCast.nStandartParallel1 = 0.0;
  mp_sSigProjCast.nStandartParallel2 = 0.0;
  mp_sSigProjCast.nAzimut            = 0.0;
  mp_sSigProjCast.nScaleFactor       = 0.0;
  mp_sSigProjCast.nFalseEasting      = 0.0;
  mp_sSigProjCast.nFalseNorthing     = 0.0;
  mp_sSigProjCast.nRange             = 0.0;

  switch(mp_sSigProjCast.nTypeOrTableName)
  { // Limitation du nombre de param�tres � prendre en compte pour la recherche CRC
    case 3 :
    case 7 :
    case 9 :
    case 19 :
      mp_sSigProjCast.nFalseEasting      = l_nTempo[6];
    case 8 :
    case 20 :
    case 21 :
    case 22 :
    case 23 :
    case 24 :
      mp_sSigProjCast.nScaleFactor       = l_nTempo[5];
    case 18 :
      mp_sSigProjCast.nAzimut            = l_nTempo[4];
    case 4 :
    case 5 :
      mp_sSigProjCast.nStandartParallel2 = l_nTempo[3];
    case 2 :
      mp_sSigProjCast.nStandartParallel1 = l_nTempo[2];
    case 10 :
    case 11 :
    case 12 :
    case 13 :
    case 14 :
    case 15 :
    case 16 :
    case 17 :
      mp_sSigProjCast.nOriginLat         = l_nTempo[1];
      mp_sSigProjCast.nOriginLon         = l_nTempo[0];
    case 0 :
    case 1 :
    default :
      break;
  }

/*
  // AUTRE METHODE D'INITIALISATION, PAS DE CASTING POSSIBLE...
  mp_sSigProjCast = m_sSigProj;

  switch(mp_sSigProjCast.nTypeOrTableName)
  { // Limitation du nombre de param�tres � prendre en compte pour la recherche CRC
    case 0 :
    case 1 :
      mp_sSigProjCast.nOriginLon         = 0.0;
      mp_sSigProjCast.nOriginLat         = 0.0;
    case 10 :
    case 11 :
    case 12 :
    case 13 :
    case 14 :
    case 15 :
    case 16 :
    case 17 :
      mp_sSigProjCast.nStandartParallel1 = 0.0;
    case 2 :
      mp_sSigProjCast.nStandartParallel2 = 0.0;
    case 4 :
    case 5 :
      mp_sSigProjCast.nAzimut            = 0.0;
    case 18 :
      mp_sSigProjCast.nScaleFactor       = 0.0;
    case 8 :
    case 20 :
    case 21 :
    case 22 :
    case 23 :
    case 24 :
      mp_sSigProjCast.nFalseEasting      = 0.0;
    case 3 :
    case 7 :
    case 9 :
    case 19 :
      mp_sSigProjCast.nFalseNorthing     = 0.0;
      mp_sSigProjCast.nRange             = 0.0;
    default :
      mp_sSigProjCast.nSyntax = 0;
      *((hyper*) &mp_sSigProjCast.anUnitName[0]) = 0;
  }
*/

  // Recherche la structure connue
  l_nNodeIndex   = mp_asProjecBuffer.GetNodeIndexCRC(&mp_sSigProjCast); // (void*)
  l_nStringIndex = mp_asProjecBuffer.GetNodeData(l_nNodeIndex);
  l_nConvIndex   = mp_asStringBuffer.GetStringData(l_nStringIndex) >> 16;

  o_roStrProjConvers = mp_asConvBuffer[l_nConvIndex];
	o_roStrCurSysGeo   = mp_asStringBuffer[l_nStringIndex];
	o_roStrCurFamGeo   = mp_asStringBuffer[mp_asStringBuffer.GetStringData(l_nStringIndex) & 0xFFFF];

  o_rnUnit  = METRES; // DEGMINSEC=0, DEGMIN=1, DEGDEC=2, GRADES=3, RADIANS=4, METRES=5, KM=6
  o_rnMerid = 0;      // GREENWICH=1, PARIS=2
}
