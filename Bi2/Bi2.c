#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double reproduccio(int p, int i, int periodes, double xf);
double mortalitat(int p, int i, int periodes, int mmin, int mmax, int var);
double exbenefici(int p, double xb1, double xb2, int extreure, int i);


int main(int argc, char *nom[])
{
    FILE* f = fopen(nom[1], "w");

    if(f==NULL)
    {
        printf("No existeix el fitxer introduit\n");
        return 1;
    }

    double beneficimax;
    int inter=1;
    int i,n=60; //5 anys.
    double xf=500; //fertilitat, benefici.
    int mmin=1,mmax=3,var=10;
    double poblacioinicial=1000;
    int periodes=4, extreure=1;
    double despesainicial=-50000-150000, despesacicleperindividu=6, despesatotal; //despesaincial 50€ per porc(1000x50=50000)+instalacions.
    double bpi=40;
    double xb0, xb1;
    double bimax;
    double p[n+1];
    double xb0max=0,xb1max, beneficimaxmax=0;

    fprintf(f,"B0       B1       BENEFICI TOTAL\n");
    for(xb0=0; xb0<=50; xb0+=0.1)
    {
        bimax=-0.1;
        beneficimax=-1e6;
        for(xb1=-0.1; xb1<=0.1; xb1+=0.0001)
        {
            inter=1;
            poblacioinicial=1000;
            despesainicial=-50000-150000; //despesaincial 50€ per porc(1000x50=50000)+instalacions

            p[0]=poblacioinicial;

            despesatotal=despesainicial;

            double r,m,b;

            for(i=1;i<=n;i++)
            {
                r=reproduccio(p[i-1],i,periodes,xf);
                m=(mortalitat(p[i-1],i,periodes,mmin,mmax,var));
                b=(exbenefici(p[i-1],xb0, xb1 ,extreure,i));
                p[i]=p[i-1]+r-m-b;

                despesatotal=despesatotal-p[i-1]*despesacicleperindividu;
                despesatotal=despesatotal+(b*bpi);

                if(p[i]>1090)
                {
                    inter=0;
                    break;
                }
                if(p[i]<0)
                {
                    inter = 0;
                    break;
                }
            }

            if(inter==1)
            {
                if(despesatotal>beneficimax)
                {
                    beneficimax=despesatotal;
                    bimax=xb1;
                }
            }
        }

        if(beneficimax>0) fprintf(f,"%lf %lf %lf\n", xb0, bimax, beneficimax);
        else fprintf(f,"%lf %lf %lf\n", xb0, bimax, 0.);

        if(beneficimax>beneficimaxmax)
        {
            beneficimaxmax=beneficimax;
            xb1max=bimax;
            xb0max=xb0;
        }
    }
    printf("Per obtenir el benefici màxim, el B0 òptim és %lf amb B1 = %lf.\nEl benefici que s'obté en aquest cas és: %lf€\n", xb0max, xb1max, beneficimaxmax);

    fclose(f);
    return 0;

}


double reproduccio(int p, int i, int periodes, double xf) //nombre de individus, any/mes/dia exacte, cada quant es reprodueixen.
{
    if(i % periodes==0)
    {
        int r;
        r=(int)(p*(xf/100));
            if(p*(xf/100)-r<0.5)
            {
                return r;
            }
            else
            {
                return r+1;
            }
    }
    else
    {
        return 0;
    }
}

double mortalitat(int p, int i, int periodes, int mmin, int mmax, int var) //nombre de individus, any/mes/dia exacte, cada quant es reprodueixen.
{
    if(mmin!=mmax)
    {
        double r;
        if(i % periodes==1)
        {
            int j;
            //srand(time(NULL));
            r=(rand() % (mmax-mmin+1))+mmin+var;
            j=(int)(p*(r/100));
            if(((p*(r/100))-j)<0.5)
            {
                return j;
            }
            else
            {
                return j+1;
            }
        }
        else
        {
            int j;
            r=(rand() % (mmax-mmin+1))+mmin;
            j=(int)(p*(r/100));
            if(((p*(r/100))-j)<0.5)
            {
                return j;
            }
            else
            {
                return j+1;
            }
        }
    }
    else
    {
        int j;
        double mmaxdouble;
        mmaxdouble=(double)mmax;
        return p*(mmaxdouble/100);
        j=(int)(p*(mmaxdouble/100));
            if(((p*(mmaxdouble/100))-j)<0.5)
            {
                return j;
            }
            else
            {
                return j+1;
            }
    }
}

double exbenefici(int p, double xb1, double xb2, int extreure, int i) //nombre de individus, any/mes/dia exacte, cada quant es reprodueixen.
{
    if(i % extreure==0)
    {
        int b;
        b=(int)(p*((xb1)/100.))+(p*p*((xb2)/100.));       //modificar aixo (xb+B1*p)
        if(((p*((xb1)/100.))+(p*p*((xb2)/100.))-b)<0.5)
        {
            return b;
        }
        else
        {
            return b+1;
        }
    }
    else
    {
        return 0;
    }
}
