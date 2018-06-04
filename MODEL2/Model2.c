#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <time.h>

double reproduccio(int p, int i, int periodes, double xf);
double mortalitat(int p, int i, int periodes, int mmin, int mmax, int var);
double exbenefici(int p, double xb1, double xb2, int extreure, int i);


int main(int argc, char *nom[])
{
    int i,n=60; //5 anys.
    double xf=500, xb1, xb2; //fertilitat, benefici.
    int mmin=1,mmax=3,var=10;
    double poblacioinicial=1000;
    int periodes=4, extreure=1;
    double despesainicial=50000+150000, despesacicleperindividu=6, despesatotal; //despesaincial 50€ per porc(1000x50=50000)+instalacions
    double bpi=40;

    FILE* f = fopen(nom[1], "w");

    if(f==NULL)
    {
        printf("No existeix el fitxer introduit\n");
        return 1;
    }

    printf("Percentatge de benefici lineal: ");
    scanf("%lf",&xb1);

    printf("Percentatge de benefici quadratic: ");
    scanf("%lf",&xb2);

    double p[n+1];

    p[0]=poblacioinicial;

    despesatotal=despesainicial;

    for(i=1;i<=n;i++)
    {
        double r,m,b;
        r=reproduccio(p[i-1],i,periodes,xf);
        m=(mortalitat(p[i-1],i,periodes,mmin,mmax,var));
        b=(exbenefici(p[i-1],xb1, xb2 ,extreure,i));
        p[i]=p[i-1]+r-m-b;

        despesatotal=despesatotal+p[i-1]*despesacicleperindividu+(b*bpi);
    }

    printf("--------------------------------\n");
    printf("En el fitxer %s creat es guarda, per cada any, la població que hi ha a la granja.\n",nom[1]);
    printf("--------------------------------\n");

    fprintf(f, "Mes    Població\n");

    for(i=0;i<=60;i++)
    {
            fprintf(f, "%i:    %g\n", i, p[i]);
    }
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
            //srand(time(NULL));
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
