#include <float.h>
#include <math.h>
#include "cquadpak.h"

double dqng(double f(),double a,double b,double epsabs,
	double epsrel,double *abserr,int *neval,int *ier)
{
	static long double x1[5] = {
		0.97390652851717172008,
		0.86506336668898451073,
		0.67940956829902440623,
		0.43339539412924719080,
		0.14887433898163121088};
	static long double w10[5] = {
		0.06667134430868813759,
		0.14945134915058059315,
		0.21908636251598204400,
		0.26926671930999635509,
		0.29552422471475287017};
	static long double x2[5] ={
		0.99565716302580808074,
		0.93015749135570822600,
		0.78081772658641689706,
		0.56275713466860468334,
		0.29439286270146019813};
	static long double w21a[5] = {
		0.03255816230796472748,
		0.07503967481091995277,
		0.10938715880229764190,
		0.13470921731147332593,
		0.14773910490133849137};
	static long double w21b[6] = {
		0.01169463886737187428,
		0.05475589657435199603,
		0.09312545458369760554,
		0.12349197626206585108,
		0.14277593857706008080,
		0.14944555400291690566};
	static long double x3[11] = {
		0.99933336090193208139,
		0.98743340290808886980,
		0.95480793481426629926,
		0.90014869574832829363,
		0.82519831498311415085,
		0.73214838898930498261,
		0.62284797053772523864,
		0.49947957407105649995,
		0.36490166134658076804,
		0.22225491977660129650,
		0.07465061746138332204};				
	static long double w43a[10] = {
		0.01629673428966656492,
		0.03752287612086950146,
		0.05469490205825544215,
		0.06735541460947808608,
		0.07387019963239395343,
		0.00576855605976979618,
		0.02737189059324884208,
		0.04656082691042883074,
		0.06174499520144256450,
		0.07138726726869339777};
	static long double w43b[12] = {
		0.00184447764021241410,
		0.01079868958589165174,
		0.02189536386779542810,
		0.03259746397534568944,
		0.04216313793519181185,
		0.05074193960018457778,
		0.05837939554261924838,
		0.06474640495144588554,
		0.06956619791235648453,
		0.07282444147183320815,
		0.07450775101417511827,
		0.07472214751740300559};
	static long double x4[22] = {
		0.99990297726272923449,
		0.99798989598667874543,
		0.99217549786068722281,
		0.98135816357271277357,
		0.96505762385838461913,
		0.94316761313367059682,
		0.91580641468550720959,
		0.88322165777131650137,
		0.84571074846241566661,
		0.80355765803523098279,
		0.75700573068549555833,
		0.70627320978732181982,
		0.65158946650117792253,
		0.59322337405796108888,
		0.53149360597083193229,
		0.46676362304202284487,
		0.39942484785921880473,
		0.32987487710618828827,
		0.25850355920216155180,
		0.18569539656834665202,
		0.11184221317990746817,
		0.03735212339461987081};
	static long double w87a[21] = {
		0.00814837738414917290,
		0.01876143820156282224,
		0.02734745105005228616,
		0.03367770731163793005,
		0.03693509982042790761,
		0.00288487243021153050,
		0.01368594602271270189,
		0.02328041350288831112,
		0.03087249761171335868,
		0.03569363363941877072,
		0.00091528334520224136,
		0.00539928021930047137,
		0.01094767960111893113,
		0.01629873169678733526,
		0.02108156888920383511,
		0.02537096976925382724,
		0.02918969775647575250,
		0.03237320246720278969,
		0.03478309895036514275,
		0.03641222073135178756,
		0.03725387550304770854};
	static long double w87b[23] = {
		0.00027414556376207235,
		0.00180712415505794295,
		0.00409686928275916486,
		0.00675829005184737870,
		0.00954995767220164654,
		0.01232944765224485369,
		0.01501044734638895238,
		0.01754896798624319110,
		0.01993803778644088820,
		0.02219493596101228680,
		0.02433914712600080547,
		0.02637450541483920724,
		0.02828691078877120066,
		0.03005258112809269532,
		0.03164675137143992940,
		0.03305041341997850329,
		0.03425509970422606179,
		0.03526241266015668103,
		0.03607698962288870119,
		0.03669860449845609450,
		0.03712054926983257611,
		0.03733422875193504032,
		0.03736107376267902341};
	double fv1[5],fv2[5],fv3[5],fv4[5],savfun[21];
	double absc,centr,dhlgth,dmax1,dmin1;
	double fcentr,fsum,fval,fval1,fval2,hlgth;
	double result,res10,res21,res43,res87;
	double resabs,resasc,reskh;
	int ipx,k,l;
	
	result = 0.0;
	*abserr = 0.0;
	*neval = 0;
	*ier = 6;
	if ((epsabs < 0.0) && (epsrel < 0.0)) return result;
	hlgth = 0.5 * (b - a);
    dhlgth = fabs(hlgth);
	centr = 0.5 * (a + b);
	fcentr=(*f)(centr);
	*neval = 21;
	*ier = 1;

	for (l = 1; l <=3; l++) {
		switch (l) {
			case 1:
				res10 = 0.0;
				res21 = w21b[5] * fcentr;
                resabs = w21b[5] * fabs(fcentr);
				for (k = 0;k < 5; k++) {
					absc = hlgth * x1[k];
					fval1 = (*f)(centr+absc);
					fval2 = (*f)(centr-absc);
					fval = fval1 + fval2;
					res10 += (w10[k] * fval);
					res21 += (w21a[k] * fval);
					resabs += (w21a[k] *
                        (fabs(fval1) + fabs(fval2)));
					savfun[k] = fval;
					fv1[k] = fval1;
					fv2[k] = fval2;
				}
				ipx = 4;
				for (k = 0; k < 5; k++) {
					ipx++;
					absc = hlgth * x2[k];
					fval1 = (*f)(centr + absc);
					fval2 = (*f)(centr - absc);
					fval = fval1 + fval2;
					res21 += (w21b[k] * fval);
					resabs += (w21b[k] *
                        (fabs(fval1) + fabs(fval2)));
					savfun[ipx] = fval;
					fv3[k] = fval1;
					fv4[k] = fval2;
				}
				result = res21 * hlgth;
				resabs *= dhlgth;
				reskh = 0.5 * res21;
                resasc = w21b[5] * fabs(fcentr - reskh);
				for (k = 0; k < 5; k++)
                 resasc += (w21a[k] * (fabs(fv1[k] -reskh) +
                  fabs(fv2[k] - reskh)) + w21b[k] *
                  (fabs(fv3[k] - reskh) + fabs(fv4[k]-reskh)));
                *abserr = fabs((res21 - res10) * hlgth);
				resasc *=dhlgth;
				break;
			case 2:
				res43 = w43b[11] * fcentr;
				*neval = 43;
				for (k = 0; k < 10; k++)
				 res43 += (savfun[k] * w43a[k]);
				for (k = 0; k < 11; k++) {
					ipx++;
					absc = hlgth * x3[k];
					fval = (*f)(centr+absc) + (*f)(centr-absc);
					res43 += (fval * w43b[k]);
					savfun[ipx] = fval;
				}
				result = res43 * hlgth;
                *abserr = fabs((res43-res21) * hlgth);
				break;
			case 3:
				res87 = w87b[22] * fcentr;
				*neval = 87;
				for (k = 0; k < 21; k++)
					res87 += (savfun[k] * w87a[k]);
				for (k = 0; k < 22; k++) {
					absc = hlgth * x4[k];
					res87 += w87b[k] * 
						((*f)(centr+absc) + (*f)(centr-absc));
				}
				result = res87 * hlgth;
                *abserr = fabs((res87 - res43) * hlgth);
		}
		if ((resasc != 0.0) &&(*abserr != 0.0))
			*abserr = resasc * min(1.0,
                pow(200.0 * (*abserr)/resasc,1.5));
		if (resabs > uflow/(50.0 * epmach)) *abserr =
			max((epmach * 50.0) * resabs,*abserr);
        if (*abserr <= max(epsabs,epsrel * fabs(result))) *ier = 0;
		if (*ier == 0) break;
	}
	return result;
}
