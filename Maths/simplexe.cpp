#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <process.h>
#include <stdlib.h>
#include <conio.h>
#define NMAX 6
#define MMAX 6
#define VARMAX 12
float a,b,c;

void saisie_mat(float[100][100],int*,int*);
void affich_mat(float[100][100],int,int);
void point_col(float[100][100],int,int);
void puiss_carre(float[100][100],float[100][100],int,int);
void transfert(float[100][100],float[100][100],int,int);
void puissance(float[100][100],int,int);
void transp_mat(float[100][100],float[100][100]);
void det_aux(float[100][100],float[100][100],int,int);
float expo(int);
float determinant(float[100][100],int);
void multi_R(float,float[100][100],float[100][100]);
void coffacteur(float[100][100],float[100][100],int);
void inverse(float[100][100],float[100][100],int);
int test(float[100][100],int);
int test_point_col(float[100][100],int,int);
void multi_matrice();

int Aide();
int simplexe();

void Matrice();

int Systeme();

int equation();

int pl_aps_entrant(double a[MMAX][NMAX],int hb[NMAX],int m,int n,int phase);

int pl_aps_sortant(double a[MMAX][NMAX],int m,int k);

void pl_pivotage(double a[MMAX][NMAX],int db[MMAX],int hb[NMAX],
                 int m,int n,int l,int k);

void pl_aps_affich(double a[MMAX][NMAX],int db[MMAX],int hb[NMAX],
                   int m,int n,int phase);

int pl_simplexe_primal(double a[MMAX][NMAX],double sol[VARMAX],
                       int ineq1,int ineq2, int eq,int n);

void main()
{
int x=1,q=0;

do
{
system ("cls");
system ("color 1f");

printf("\\n\\n\\a @@@   @@@  @@@  @@@@@@  @@   @@  @@@@@    @@@  @@@  @@@@@ @@   @@ @@  @@\\n");
    printf("@   @  @@@@@@@@    @@    @@@  @@  @@       @@@@@@@@  @@    @@@  @@ @@  @@\\n");
    printf("@@@@@  @@ @@ @@    @@    @@ @ @@  @@@@@    @@ @@ @@  @@@@@ @@ @ @@ @@  @@\\n");
    printf("@   @  @@    @@    @@    @@  @@@  @@       @@    @@  @@    @@  @@@ @@  @@\\n");
    printf("@   @  @@    @@  @@@@@@  @@   @@  @@@@@    @@    @@  @@@@@ @@   @@  @@@@  \\n");
        printf("\\n              * * * * * * * * * * * * * * * * * * * * * * * * * * *"); 
		printf("\\n              *  Entez votre choix :                              *");
		printf("\\n              *                                                   *");
		printf("\\n              *     1- Methode Simplexe.                          *");
		printf("\\n              *                                                   *");
		printf("\\n              *     2- Calculs Matriciels.                        *");
		printf("\\n              *                                                   *");
		printf("\\n              *     3- Systeme d'Equations Lineaires.             *");
		printf("\\n              *                                                   *");
		printf("\\n              *     4- Resoudre une Equation de second degree.    *");
	    printf("\\n              *                                                   *");
		printf("\\n              *     5- Aide.                                      *");
		printf("\\n              *                                                   *");
		printf("\\n              *     Esc: Quitter.                                 *");
	    printf("\\n              *                                                   *");
		printf("\\n              * * * * * * * * * * * * * * * * * * * * * * * * * * *");
	printf("\\n\\n");
	printf("        Faites votre choix :  ");

switch (getch())
{
case '1':simplexe();break;

case '2':Matrice();break;

case '3':Systeme();break;

case '4':equation();break;

case '5' : Aide();break;
case 27:x=0;break;

default:printf("\\a");
}

    } while(x!=0);
}

resolutionSimplexe()
{

	
system("cls");
 int i,j,ineq1,ineq2,eq,n,err;
 int k=1;
 double a[MMAX][NMAX],sol[VARMAX];

 cout<<"\\n\\t\\t\\t**************************";
 cout<<"\\n\\t\\t\\t    Methode Simplexe";
 cout<<"\\n\\n\\t\\t\\t    Max z = ";
 cout<<"\\n\\t\\t\\t    S.C : <= ";
 cout<<"\\n\\t\\t\\t**************************";
 

 cout<<"\\n\\nDonner le nombre des variables : \\n";
 cin>>n;;

 printf("Donner les elements de la fonction economique : \\n");

 a[0][0]=0;
 for(k=1;k<=n;k++) {
	 cout<<"\\na[0]["<<k<<"] = ";
	 cin>>a[0][k];
 }

 printf("Donner le nombre d'equations en <= : \\n");
 cin>>ineq1;
 printf("Donner les elements l'equations en <= :\\n");
 for(i=1;i<=ineq1;i++) {
	 for(j=0;j<=n;j++) {
	 
	 printf("\\na[%d][%d] =  ",i,j);
	 cin>>a[i][j];
	 }
 }

 ineq2=0;

 eq=0;
 err=pl_simplexe_primal(a,sol,ineq1,ineq2,eq,n);
 if(err==1)printf("Solution infinie\\n");
 else
 if(err==2)printf("Domaine vide\\n");
 else
 {
  printf("\\nSolution optimale:\\n\\n");
  for(i=1;i<=ineq1+ineq2+n;i++)
  printf("\\nx%d =%23.16e\\n",i,sol[i]);
  printf("\\nValeur optimale: z=%23.16e\\n",-a[0][0]);
 }
 
 
 return (0);
}

int pl_aps_entrant(double a[MMAX][NMAX],int hb[NMAX],int m,int n,int phase)
{
 int i,j,k,l;
 double d,s,max;
 k=0;
 max=0.0;
 if(phase==2)l=0;
 else l=m+1;
 for(j=1;j<=n;j++)
 {
  d=a[l][j];
  s=0.0;
  if((d>0)&&(hb[j]!=n+m))
  {
   for(i=1;i<=m;i++)
   s+=fabs(a[i][j]);
   d/=s;
   if(d>max)
   {
    max=d;
    k=j;
   }
  }
 }
 return(k);
}

int pl_aps_sortant(double a[MMAX][NMAX],int m,int k)
{
 int i,l;
 double rap,min;
 min=1e308;
 l=0;
 for(i=1;i<=m;i++)
 if(a[i][k]>0)
 {
  rap=a[i][0]/a[i][k];
  if(rap<min)
  {
   min=rap;
   l=i;
  }
 }
 return(l);
}

void pl_pivotage(double a[MMAX][NMAX],int db[MMAX],int hb[NMAX],
                 int m,int n,int l,int k)
{
 int i,j;
 double pivot,coef;
 pivot=a[l][k];
 for(i=0;i<=m;i++)
 if(i!=l)
 {
  coef=a[i][k]/pivot;
  a[i][k]=-coef;
  for(j=0;j<=n;j++)
  if(j!=k)
  a[i][j]=a[i][j]-coef*a[l][j];
 }
 coef=1/pivot;
 a[l][k]=coef;
 for(j=0;j<=n;j++)
 if(j!=k)
 a[l][j]=coef*a[l][j];
 i=db[l];
 db[l]=hb[k];
 hb[k]=i;
}

void pl_aps_affich(double a[MMAX][NMAX],int db[MMAX],int hb[NMAX],
                   int m,int n,int phase)
{
 int i,j;
 printf("         ");
 for(j=1;j<=n;j++)
 if((phase==1)||(hb[j]!=n+m))
 printf("          x%d",hb[j]);
 printf("\\n");
 if(phase==1)
 {
  printf("z'+");
  for(j=0;j<=n;j++)
  printf("%11.5e ",a[m+1][j]);
  printf("\\n");
 }
 for(i=0;i<=m;i++)
 {
  if(i==0)printf("z+ ");
  else
  if(db[i]!=0)printf("x%d ",db[i]);else printf("   ");
  for(j=0;j<=n;j++)
  if((phase==1)||(hb[j]!=n+m))
  printf("%11.5e ",a[i][j]);
  printf("\\n");
 }
}

int pl_simplexe_primal(double a[MMAX][NMAX],double sol[VARMAX],
                       int ineq1,int ineq2, int eq,int n)
{
 int i,j,k,l,phase,m,m1;
 int db[MMAX],hb[NMAX];
 double min;
 m=ineq1+ineq2+eq;
 for(i=ineq1+1;i<=ineq1+ineq2;i++)
 for(j=0;j<=n;j++)
 a[i][j]=-a[i][j];
 for(i=1;i<=ineq1+ineq2;i++)
 db[i]=n+i;
 for(i=ineq1+ineq2+1;i<=m;i++)
 db[i]=0;
 for(j=1;j<=n;j++)
 hb[j]=j;
 if(eq!=0)
 {

	 for(i=ineq1+ineq2+1;i<=m;i++)
  {
   l=i;
   k=0;
   for(j=1;j<=n;j++)
   if(a[i][j]!=0)k=j;
   if(k==0)
   {
    if(a[i][0]!=0)return(2);
   }
   else
   {

	   printf("var.entrante: x%d\\n",hb[k]); 
    pl_pivotage(a,db,hb,m,n,l,k);
    hb[k]=hb[n];
    for(j=0;j<=m;j++)
    a[j][k]=a[j][n];
    n-=1;
   }
  }
 }
 n+=1;
 m1=m;
 hb[n]=n+m;
 phase=2;
 l=0;
 min=0;
 for(i=1;i<=m;i++)
 if(a[i][0]<min)
 {
  min=a[i][0];
  l=i;
 }
 if(l!=0)phase=1;
 k=1;
 if(phase==1)
 {

	 m1=m+1;
  for(j=0;j<n;j++)
  a[m1][j]=0;
  for(i=1;i<=m;i++)
  if(a[i][0]<0)
  a[i][n]=-1;
  else a[i][n]=0;
  a[0][n]=0;
  a[m1][n]=-1;

  pl_pivotage(a,db,hb,m1,n,l,n);
 }
 while(phase<=2)
 {
  do
  {
   k=pl_aps_entrant(a,hb,m,n,phase);
   if(k!=0)
   {
    l=pl_aps_sortant(a,m,k);
    if(l==0)return(1);

    pl_pivotage(a,db,hb,m1,n,l,k);
   }
  }
  while(k!=0);
  if(phase==1)
  {
   l=0;
   for(i=1;i<=m;i++)
   if(db[i]==n+m)l=i;
   if(l!=0)
   {
    if(fabs(a[l][0])>1e-15)return(2);
    else
    {
     for(j=1;j<=n;j++)
     if(a[l][j]!=0)
     k=j;
     pl_pivotage(a,db,hb,m1,n,l,k);
    }
   }
  }
  phase+=1;
  m1-=1;
 }
 for(i=1;i<m+n;i++)
 sol[i]=0;
 for(i=1;i<=m;i++)
 sol[db[i]]=a[i][0];
 return(0);
}

void saisie_mat(float m[100][100],int *l,int *c)
{ int i,j;
do
{
system ("cls");
printf("\\n\\n\\t\\t\\t** Saisie de la matrice  **\\n\\n");
printf(" Nombre de lignes de la matrice : ");
scanf("%d",l);
if(*l>100){printf("Erreur !");getch();}
}while(*l>100);

do
{
printf(" Nombre de colonnes de la matrice :");
scanf("%d",c);
if(*c>100){printf("Erreur !\\n");getch();}
}while(*c>100);

printf("\\n\\n");

for(i=0;i<*l;i++)
{
    for(j=0;j<*c;j++)
     {
     printf("  a[%d][%d] : ",i+1,j+1);
     scanf("%f",&m[i][j]);
     }
     printf("\\n");
}
}

void affich_mat(float m[100][100],int l,int c)
{int i,j;
printf("\\t\\t\\t**  Affichage de la matrice  **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%.2f",m[i][j]);
     }
     printf("\\n\\n");
}
getch();
}

int test_point_col(float m[100][100],int l,int c)
{
int j,cpt=0;

for(j=1;j<c;j++)
{
if(m[l][j-1]==m[l][j]){cpt++;}
}

if(cpt==c-1)return 0;else return 1;

}

void point_col(float m[100][100],int l,int c)
{int i,j,max_l=0,min_c=0,k,x,t=0;
system ("cls");
printf("\\t\\t\\t**  Points-cols de la matrice  **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {

        for(k=0;k<c;k++)
        {
            if(m[i][j]>m[i][k]){max_l=1;}else{if(m[i][j]<m[i][k]){max_l=0;k=100;}}

        }

        for(x=0;x<l;x++)
        {
                if(m[i][j]<m[x][j]){min_c=1;}else{if(m[i][j]>m[x][j]){min_c=0;x=100;}}

        }

     if((max_l==1)&&(min_c==1)&&(test_point_col(m,i,c)==1)){printf("Ligne : %d ; Colonne : %d ==> Valeur du point-col : %.2f\\n\\n",i,j,m[i][j]);t++;}
     }

}
if(t==0)printf("\\n\\nAucun point-col");getch();
}

void transfert(float t1[100][100],float t2[100][100],int l,int c)
{
int i,j;

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     t2[i][j]=t1[i][j];
     }
}
}

void puiss_carre(float m[100][100],float ca[100][100],int l,int c)
{int i,j,k;
float tot;
for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     tot=0;
        for(k=0;k<c;k++)
        {
        tot=tot+(m[i][k]*m[k][j]);
        }

    ca[i][j]=tot;
     }
}
}

void puissance(float m[100][100],int l,int c)
{
    float ca[100][100],cb[100][100],tot;
    int i,j,k,v,p;

       

printf(" \\n\\nPuissance a laquelle vous desirez elever la matrice : ");
scanf("%d",&p);

if(p==1){affich_mat(m,l,c);}
else
{

puiss_carre(m,ca,l,c);

if(p==2)
{
affich_mat(ca,l,c);
}
else
{

for(v=0;v<p-2;v++)
{
transfert(ca,cb,l,c);

for(i=0;i<l;i++)
{

     for(j=0;j<c;j++)
     {
    tot=0;
         for(k=0;k<c;k++)
        {
        tot=tot+(cb[i][k]*m[k][i]);
        }

    ca[j][i]=tot;
     }
}

}
system("cls");
printf("\\t\\t\\t**  Affichage de la matrice  **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%10.2f",cb[i][j]);
     }
     printf("\\n\\n");
}

printf("\\t\\t\\t**  Affichage de la matrice elevee a %d **\\n\\n\\n",p);

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%10.2f",ca[i][j]);
     }
     printf("\\n\\n");
}
getch();
}
}
}

void transp_mat(float ma[100][100],float mb[100][100])
{
int i,j;

for (i=0;i<100;i++)
{
    for (j=0;j<100;j++)
    {
    mb[j][i]=ma[i][j];
    }
}
}

void det_aux(float ma[100][100],float mb[100][100],int l,int c)
{
int i,j,d,e=0;

    for(i=0;i<100;i++)
    {
     d=0;
        if(i!=l)
        {
        for(j=0;j<100;j++)
                if(j!=c)
                {
            mb[e][d]=ma[i][j];
            d++;
                }
            e++;
        }
    }
}

float expo(int n)
{
if(!(n%2)){return (1);}
return (-1);
}

float determinant(float m[100][100],int l)
{
int i;
float m2[100][100],x=0;

if(l==1){return (m[0][0]);}

for(i=0;i<l;i++)
{
det_aux(m,m2,i,0);
x=x+(expo(i)*m[i][0]*determinant(m2,(l-1)));
}

return (x);
}

void multi_R(float a,float ma[100][100],float mb[100][100])
{
int i,j;

for(i=0;i<100;i++)
{
    for(j=0;j<100;j++)
     {
    mb[i][j]=ma[i][j]*a;
     }
}
}

void coffacteur(float ma[100][100],float mb[100][100],int l)
{
int i,j;
float m2[100][100];

if (l==1)
{
mb[0][0]=1;
}
else
{
for (i=0;i<l;i++)
{
    for (j=0;j<l;j++)
    {
    det_aux(ma,m2,i,j);
    mb[i][j]=expo(i+j)*determinant(m2,(l - 1));
    }
}
}
}

void inverse(float ma[100][100],float mb[100][100],int l)
{
float m1[100][100],m2[100][100],d;

d=(1./determinant(ma,l));
coffacteur(ma,m1,l);
transp_mat(m1,m2);
multi_R(d,m2,mb);
}

int test(float tab[100][100], int l1)
{
if(determinant(tab,l1)){return 1;}
return 0;
}

void Matrice()
{
int l,c,x=1,q=0;
float mat1[100][100],mat2[100][100],r;
int i,j;
float a;
do
{
system ("cls");

printf("\\n\\n\\n\\t\\t\\t**  Les Calculs Matriciels  **\\n\\n\\n");
printf("\\t1- Transposee d'une matrice.\\n\\n");
printf("\\t2- Multiplication d'une matrice par un reel.\\n\\n");
printf("\\t3- Multiplication de deux matrices.\\n\\n");
printf("\\t4- Calcul du determinant d'une matrice carree.\\n\\n");
printf("\\t5- Calcul de l'inverse d'une matrice carree.\\n\\n");
printf("\\tEsc : Quitter.\\n\\n\\n");
printf("\\t\\tFaites votre choix...\\n");

switch (getch())
{
case '1':saisie_mat(mat1,&l,&c);
	printf("\\n\\n\\t\\t\\t**  Affichage de la matrice  **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%.2f",mat1[i][j]);
     }
     printf("\\n\\n");
}
;transp_mat(mat1,mat2);printf("\\n\\n\\t\\t\\t**  Affichage de la matrice transposee **\\n\\n\\n");

for(i=0;i<c;i++)
{
    for(j=0;j<l;j++)
     {
     printf("\\t%.2f",mat2[i][j]);
     }
     printf("\\n\\n");
}
getch();break;

case '2': saisie_mat(mat1,&l,&c);printf("\\n\\tMultiplicateur X : ");scanf("%f",&a);
	printf("\\n\\n\\t\\t\\t**  Affichage de la matrice  **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%.2f",mat1[i][j]);
     }
     printf("\\n\\n");
}
;multi_R(a,mat1,mat2);printf("\\n\\n\\t\\t\\t**  Affichage de la matrice resultat **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%.2f",mat2[i][j]);
     }
     printf("\\n\\n");
}
getch();break;

case '3': multi_matrice();break;

case '4': system("cls");printf("\\n\\n\\t\\t\\t** Saisie de la matrice  **\\n\\n");
printf(" Nombre de lignes et de colonnes de la matrice : ");
scanf("%d",&l);

c=l;

printf("\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("  a[%d][%d] : ",i+1,j+1);
     scanf("%f",&mat1[i][j]);
     }
     printf("\\n");
}

printf("\\n\\n\\t\\t\\t**  Affichage de la matrice  **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%.2f",mat1[i][j]);
     }
     printf("\\n\\n");
}
        
		 r=determinant(mat1,l);
		 printf("\\n\\n\\tLe determinant de la matrice = %f",r);
		 getch();break;

case '5': system("cls");printf("\\n\\n\\t\\t\\t** Saisie de la matrice  **\\n\\n");
printf(" Nombre de lignes et de colonnes de la matrice : ");
scanf("%d",&l);

c=l;

printf("\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("  a[%d][%d] : ",i+1,j+1);
     scanf("%f",&mat1[i][j]);
     }
     printf("\\n");
}

printf("\\n\\n\\t\\t\\t**  Affichage de la matrice  **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%.2f",mat1[i][j]);
     }
     printf("\\n\\n");
}
            
            
            
            if (!test(mat1,l))
            {printf("\\n\\n\\t\\a!! Impossible, le determinant est nul !!");}
         else
         {
         inverse(mat1,mat2,l);
         printf("\\n\\n\\t\\t\\t**  Affichage de la matrice inverse **\\n\\n\\n");

for(i=0;i<l;i++)
{
    for(j=0;j<c;j++)
     {
     printf("\\t%10.4f",mat2[i][j]);
     }
     printf("\\n\\n");
}
getch();
         printf(" Inverse de la matrice \\n\\n");
         }
       
              break;

case 27:x=0;break;

default:printf("\\a");
}

    } while(x!=0);
}

int Systeme(){
char c;
int a=0;
int n;
double e[11][100];
double s[100];

do {
system ("cls");

cout<<"\\n\\t\\t*********************************************";
cout<<"\\n\\n\\t\\t\\t Systemes d'equations lineaires  ";
cout<<"\\n\\n\\t\\t*********************************************";

cout<<"\\n\\nCombien dequations ?\\n\\nN  =  ";
cin>>n;
cout<<"\\n";
for (int i=0;i<n;i++){

    cout<<"Equation "<<i+1<<"\\n";
    for (int p=0;p<n;p++){
    
    
    cout<<"v"<<p+1<<" = ";
    cin>>e[p][i];
    
    }

cout<<"\\n";

cout<<"Equation "<<i+1<<" = ";
cin>>e[n][i];

cout<<"\\n";

}  

int y=0;
double var1=0,var2=0;

double temp;

int a,t;

for(int x=0;x<n-1;x++){     
    
for(a=1+x;a<n;a++){

    
    temp=e[x][a];
    
for (t=x;t<n+1;t++){
    
    e[t][a]=e[t][a]*e[x][x]-e[t][x]*temp;   
    
    }

}
}

int af;

s[n-1]=e[n][n-1]/e[n-1][n-1];

e[n][n-1]=0;
e[n-1][n-1]=0;

for (int ligne=1;ligne<=n;ligne++){

for (int sol=2;sol<=n;sol++){

e[n-ligne][n-sol]*=s[n-ligne];
e[n][n-sol]-=e[n-ligne][n-sol];
e[n-ligne][n-sol]=0;

}

s[n-(ligne+1)]=e[n][n-(ligne+1)]/e[n-(ligne+1)][n-(ligne+1)];

}

for (af=0;af<n;af++){
	
	cout<<"\\t var "<<af+1<<" = "<<s[af]<<" ;";}

cout<<"\\n\\nVoulez vous resoudre un autre systeme d'equations ? (o/n) "; 

cin>>c;
}
while (c=='o');
return (0);
}

int repeatSimplexe()     
{
    char rep;
    printf(" \\nVoulez-vous resoudre un autre programme lineaire ? (o/n) ");
    scanf("%s",&rep);

    return(strcmp(&rep,"n"));
}

int simplexe()
{
    do
    resolutionSimplexe();
    while (repeatSimplexe());
    printf("\\n\\n");
    return(0);
} 

float Discriminant()
{
    do
    {
    printf("\\n Donner la valeur de a (non nulle): ");
    cin>>a;
    }
    while (a==0);
    printf(" Donner la valeur de b      : ");
    cin>>b;
    printf(" Donner la valeur de c      : ");
    cin>>c;;

    return(b*b-4*a*c);
}

void presentation()
{ 

	system ("cls");
    printf("\\n\\t----------------------------------------------------------------\\n");
    printf("  \\t\\tProgramme de resolution d'une equation de second degree  \\n");
    printf("   \\t\\t        2 \\n");
    printf("     \\t\\t    a  X   +  b  X  + c  =  0\\n");
    printf("\\t----------------------------------------------------------------\\n\\n");
}

int resolution()
{
    float delta,x1,x2;
    delta=Discriminant();
    printf("\\n Delta = %f\\n",delta);
    if (delta<0)
     {
     printf(" Delta est negatif, pas de solutions reelles ...\\n");
     printf(" Les solutions complexes sont:\\n");
     x1=(-b/(2*a));
     x2=(sqrt(-delta)/(2*a));
     printf("   X1  = %f +i (%f)\\n",x1,x2);
     printf("   X1  = %f -i (%f)\\n\\n",x1,x2);  
     }
     else
     if (delta==0)
        {
         printf(" Il existe une solution double :\\n");
         x1=(-b)/(2*a);
         printf("   X1 = X2 = %f \\n\\n",x1);
        }
        else
        {
         printf(" Les solutions de l'equation sont :\\n");
         x1=(-b-sqrt(delta))/(2*a);
         x2=(-b+sqrt(delta))/(2*a);
         printf("   X1  = %f\\n",x1);
         printf("   X2  = %f\\n\\n",x2);
        }
		return (0);
}

int repeat()     
{
    char rep;
    printf(" Voulez-vous resoudre une autre equation ? (o/n) ");
    scanf("%s",&rep);

    return(strcmp(&rep,"n"));
}

int equation()
{ 
	do {
    presentation();
   
    resolution();}
    while (repeat());
    printf("\\n\\n");
    return(0);
} 

void multi_matrice()

{

	
float A[100][100]; 
float  B[100][100];
float C[100][100]; 
int n, m, P;   
 int i, j, K;  

  system ("cls");

 printf("\\n\\n\\t\\t*** Multiplication de deux matrices A et B ***\\n");
 printf("\\n<Matrice A>\\n");
 printf("\\nNombre de lignes de   A  : ");
 scanf("%d", &n );
 printf("\\nNombre de colonnes de A  : ");
 scanf("%d", &m );
 for (i=0; i<n; i++)
    for (j=0; j<m; j++)
        {
         printf("\\na[%d][%d] : ",i+1,j+1);
         scanf("%f", &A[i][j]);
        }
 printf("\\n\\n<Matrice B>\\n");
 printf("\\nNombre de lignes de   B : %d\\n", m);
 printf("\\nNombre de colonnes de B  : ");
 scanf("%d", &P );
 for (i=0; i<m; i++)
    for (j=0; j<P; j++)
        {
         printf("\\nb[%d][%d] : ",i+1,j+1);
         scanf("%f", &B[i][j]);
        }

	
printf("\\nMatrice donnee A :\\n");
 for (i=0; i<n; i++)
    {
     for (j=0; j<m; j++)
          printf("%10.2f", A[i][j]);
     printf("\\n");
    }
 printf("\\nMatrice donnee B :\\n");
 for (i=0; i<m; i++)
    {
     for (j=0; j<P; j++)
          printf("%10.2f", B[i][j]);
     printf("\\n");
    }

 
 for (i=0; i<n; i++)
     for (j=0; j<P; j++)
         {
          C[i][j]=0;
          for (K=0; K<m; K++)
               C[i][j] += A[i][K]*B[K][j];
         }

 printf("\\n\\nMatrice resultat C :\\n");
 for (i=0; i<n; i++)
    {
     for (j=0; j<P; j++)
          printf("%10.2f", C[i][j]);
     printf("\\n");
    }
 getch();
}

int Aide()
{
system("cls");
printf("\\n\\n\\n\\tPour obtenir de l'aide veuillez consulter Amine Bouslama .");

getch();
return 0;

}
