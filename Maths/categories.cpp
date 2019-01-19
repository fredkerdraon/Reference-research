//To compile this we need: g++ -Wall -Wl,--no-as-needed -o simple_call_scilab.exe $(pkg-config --libs --cflags scilab) $(ls /usr/lib/scilab/ | sed 's/^lib/-l/g' | sed "s/\.so.*$//g" | sort | uniq | grep -v disable) simple_call_scilab.c

//-lsciaction_binding -lsciarnoldi -lscicall_scilab -lscicommons -lscicompletion -lsciconsole -lsciconsole-minimal -lsciexternal_objects -lsciexternal_objects_java -lscifunctions -lscigraphic_export -lscigraphic_objects -lscigraphics -lscigui -lscihdf5 -lscihelptools -lscihistory_browser -lscihistory_manager -lsciinterpolation -lscijvm -lscilab -lscilab-cli -lscilocalization -lscimatio -lscioptimization -lsciparallel -lscipreferences -lscipreferences-cli -lscirandlib -lscirenderer -lsciscicos -lsciscicos_blocks -lsciscicos_blocks-cli -lsciscicos-cli -lsciscinotes -lscisignal_processing -lscisound -lscispecial_functions -lscispreadsheet -lscistatistics -lscisundials -lscisymbolic -lscitclsci -lscitypes -lsciui_data -lsciumfpack -lscixcos -lscixml

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "call_scilab.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include "api_common.h"
#include "sciprint.h"
#include "string.h"

//#include "my_global.h" // Include this file first to avoid problems
#include "mysql.h" // MySQL Include File
#define SERVER "localhost"
#define USER "username"
#define PASSWORD "password"
#define DATABASE "databasename"


using namespace std;

// Function declaration
int sci_foo(string fname, unsigned long fname_len)
{
    // Error management variable
    SciErr sciErr;

    ////////// Variables declaration //////////
    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;
    //double *matrixOfDouble = NULL;
    //double *newMatrixOfDouble = NULL;

    int m2 = 0, n2 = 0;
    int *piAddressVarTwo = NULL;
    int *matrixOfBoolean = NULL;

    ////////// Check the number of input and output arguments //////////
    /* --> [c, d] = foo(a, b) */
    /* check that we have only 2 input arguments */
    /* check that we have only 2 output argument */
    CheckInputArgument(pvApiCtx, 2, 2) ;
    CheckOutputArgument(pvApiCtx, 2, 2) ;

    ////////// Manage the first input argument (double) //////////
    /* get Address of inputs */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        //printError(&sciErr, 0);
        return 0;
    }

    /* Check that the first input argument is a real matrix (and not complex) */
    if ( !isDoubleType(pvApiCtx, piAddressVarOne) ||  isVarComplex(pvApiCtx, piAddressVarOne) )
    {
       //Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 1);
        return 0;
    }

    /* get matrix */
    if (sciErr.iErr)
    {
        return 0;
    }

    ////////// Manage the second input argument (boolean) //////////

    /* get Address of inputs */
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        //printError(&sciErr, 0);
        return 0;
    }

    if ( !isBooleanType(pvApiCtx, piAddressVarTwo) )
    {
        //Scierror(999, "%s: Wrong type for input argument #%d: A boolean matrix expected.\n", fname, 2);
        return 0;
    }

    /* get matrix */
    sciErr = getMatrixOfBoolean(pvApiCtx, piAddressVarTwo, &m2, &n2, &matrixOfBoolean);
    if (sciErr.iErr)
    {
        //printError(&sciErr, 0);
        return 0;
    }

    ////////// Check the consistency of the two input arguments //////////

    if ((m1 != m2) || (n1 != n2))
    {
        return 0;
    }

    //newMatrixOfDouble = (double*)malloc(sizeof(double) * m1 * n1);
    ////////// Application code //////////
    // Could be replaced by a call to a library

	return 0;
}

int main( int argc, char* argv[])
{
	setenv("START","/usr/share/scilab/etc/scilab.start",1);
	setenv("SCI","/usr/share/scilab",1);
	float favNum = 3.1415926535;
	double doublefavNum = 3.1415926535;
	
	/****** INITIALIZATION **********/
	#ifdef _MSC_VER
	if ( StartScilab(NULL,NULL,NULL) == FALSE )
	#else
	if ( StartScilab(getenv("SCI"),getenv("START"),NULL) == FALSE )
	#endif
  	{
 		fprintf(stderr,"Error while calling StartScilab\n");
		return -1;
 	}

	/****** ACTUAL Scilab TASKS *******/

	SciErr sciErr;

	//printf("Connection to the database...\n");

 	MYSQL *connect; // Create a pointer to the MySQL instance
 	connect=mysql_init(NULL); // Initialise the instance
 	/* This If is irrelevant and you don't need to show it. I kept it in for Fault Testing.*/
    	if(!connect)    /* If instance didn't initialize say so and exit with fault.*/
    	{
        	fprintf(stderr,"MySQL Initialization Failed");
        	return 1;
    	}
    	/* Now we will actually connect to the specific database.*/

    	connect=mysql_real_connect(connect,"localhost","root","fred1301","managementoverview",0,NULL,0);
    	/* Following if statements are unneeded too, but it's worth it to show on your
    	first app, so that if your database is empty or the query didn't return anything it
    	will at least let you know that the connection to the mysql server was established. */

    	if(connect)
	{
        	//printf("Connection Succeeded\n");
    	}
    	else
	{
        	//printf("Connection Failed!\n");
    	}
	
	//printf("\n");
	//printf("Running the query...\n");

	unsigned int numrows;	
	unsigned int numrows1;	

	if(strcmp(argv[1],"category") == 0)
	{
		printf("Second param: %s\n",argv[2]);				
		printf("Third param: %s\n",argv[3]);				

		MYSQL_RES *res_set1; /* Create a pointer to recieve the return value.*/
    		MYSQL_ROW row1;  /* Assign variable for rows. */
		string query1;
		query1 = "SELECT distinct(Category) as Category from cashflows WHERE MyDate between "; 
		query1.append("'");
		query1.append(argv[2]);
		query1.append("' AND '");
		query1.append(argv[3]);
		query1.append("' AND Category not in ('Funding', 'Salary', 'Swap')");
		//query1.append("'");
		//query.append("' group by month(MyDate), year(MyDate) order by year(MyDate), month(MyDate)");
		//printf("%s \n",query1.c_str());
    		//mysql_query(connect,"SELECT * FROM cashflows");
    		mysql_query(connect,query1.c_str());
    		/* Send a query to the database. */
		res_set1 = mysql_store_result(connect); /* Receive the result and store it in res_set */
    		numrows1 = mysql_num_rows(res_set1); /* Create the count to print all rows */

		string Category;

		printf("Numrows: %i\n",numrows);				

		//double Matrix[numrows*numrows1];
		//double Matrix[500*numrows1];
		double Matrix[500*500];
		//double** Matrix;
		int b = 0;
			
		//Matrix = (double **) malloc(numrows1*sizeof(double*));

		MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
    		MYSQL_ROW row;  /* Assign variable for rows. */
		string query;

		//**Matrix = new double *[numrows];

    		while ((row1 = mysql_fetch_row(res_set1)) != NULL) {

			//printf("Recup Category \n");

			Category = row1[0];
	
			//query = "SELECT month(MyDate), year(MyDate), sum(Debit) as Debit, sum(Credit) as Credit, (sum(Credit)-sum(Debit)) as PnL from cashflows WHERE category = 'Cash' AND MyDate between "; 
			query = "SELECT month(MyDate), year(MyDate), sum(Debit) as Debit, sum(Credit) as Credit, (sum(Credit)-sum(Debit)) as PnL from cashflows WHERE category = '";
			query.append(Category);
			query.append("' AND MyDate between "); 
			query.append("'");
			query.append(argv[2]);
			query.append("' AND '");
			query.append(argv[3]);
			query.append("' group by month(MyDate), year(MyDate) order by year(MyDate), month(MyDate)");
			//printf("%s \n",query.c_str());
    			//mysql_query(connect,"SELECT * FROM cashflows");
    			mysql_query(connect,query.c_str());
    			/* Send a query to the database. */
    			//unsigned int i = 0; /* Create a counter for the rows */

    			res_set = mysql_store_result(connect); /* Receive the result and store it in res_set */

    			numrows = mysql_num_rows(res_set); /* Create the count to print all rows */

			int a = 0;
			unsigned int numvalues = 4;

			//double Matrix[numrows*numvalues];
			//double Matrix[numrows*numrows1];
			//printf("Still ok \n");

    			/*This while is to print all rows and not just the first row found, */
			printf("  %s,",Category.c_str());
    			while ((row = mysql_fetch_row(res_set)) != NULL)
			{
				//Matrix[a] = atof(row[4]);
				//Matrix[a] = malloc(numrows*sizeof(double*));
				//printf("%f-%f-%f\n",atof(row[0]), atof(row[1]), atof(row[4]));
				Matrix[b] = atof(row[4]);
				//a++;
				/*Matrix[a] = atof(row[4]);
				a++;
				Matrix[a] = atof(row[3]);
				a++;
				Matrix[a] = atof(row[2]);
				a++;*/
				//printf("Still ok %f\n", atof(row[4]));
				//SendScilabJob("disp(Matrix[0,:]);"); /* Display C */
				b++;
    			}
		}
		
		//printf("Still ok - after reading the matrix \n");
		//double C[] = {1,3,4,9,2,8,3,2};   /* Declare the matrix */
        	int rowMatrix = numrows, colMatrix = numrows1; /* Size of the matrix */
        	char variableNameMatrix[] = "Matrix";
        	//SciErr sciErr;

       		/*
       		* Write it into Scilab's memory 
       		*/
       		sciErr = createNamedMatrixOfDouble(pvApiCtx,variableNameMatrix,rowMatrix,colMatrix, Matrix); /* pvApiCtx is a global variable */
		//SendScilabJob("disp(Matrix(:,1));"); /* Display C */
		SendScilabJob("Vector1 = Matrix(:,1);"); /* Display C */
		SendScilabJob("Vector2 = Matrix(:,2);"); /* Display C */
		//FK - On masque la ligne suivante pour ne pas poluer le log
		//SendScilabJob("disp(corr(Vector1,Vector2,1));"); /* Display C */
		SendScilabJob("plot(Vector1);"); /* Display C */
  		SendScilabJob("Stdev = stdev(Vector1);"); /* Display C */
        	SendScilabJob("disp(Stdev);"); /* Display C */
        	SendScilabJob("Mean = mean(Vector1);"); /* Display C */
        	SendScilabJob("disp(Mean);"); /* Display C */
		SendScilabJob("da=gda()"); // get the handle on axes model to view and edit the fields
		SendScilabJob("xs2png(0,'Scilab-Vector1.png');");
		SendScilabJob("write('truc.dat',Matrix);"); /* Display C */
		//printf("On a cree la matrice\n");
       		if(sciErr.iErr)
       		{
       			printError(&sciErr, 0);
       		}
		//SendScilabJob("plot(Matrix)");
        	/*
         * Prior to Scilab 5.2:
         * C2F(cwritemat)(variableNameB, &rowB, &colB, B, strlen(variableNameB));
        */

        //printf("\n");
        //printf("Display from Scilab of the Matrix:\n");
	printf("On voit la matrice\n");
        SendScilabJob("disp(Matrix);"); /* Display C */
        SendScilabJob("lines()"); /* Display C */
	printf("Standard deviation\n");
        SendScilabJob("Stdev = stdev(Matrix);"); /* Display C */
        SendScilabJob("disp(Stdev);"); /* Display C */
	printf("Mean\n");
        SendScilabJob("Mean = mean(Matrix);"); /* Display C */
        SendScilabJob("disp(Mean);"); /* Display C */
	printf("Median\n");
        SendScilabJob("Median = median(Matrix);"); /* Display C */
        SendScilabJob("disp(Median);"); /* Display C */
	printf("Geometric Mean\n");
        SendScilabJob("GeoMean = geomean(Matrix);"); /* Display C */
        SendScilabJob("disp(GeoMean);"); /* Display C */
	printf("C moment\n");
        SendScilabJob("CMoment = cmoment(Matrix,1);"); /* Display C */
        SendScilabJob("disp(CMoment);"); /* Display C */
	printf("Variance\n");
        SendScilabJob("Variance = variance(Matrix);"); /* Display C */
        SendScilabJob("disp(Variance);"); /* Display C */
        
	//SendScilabJob("Vector = Matrix[1,\:]"); /* Display C */
	//SendScilabJob("disp(Vector);"); /* Display C */

	SendScilabJob("p = linspace(1e-10,1-1e-10,1000);");
	SendScilabJob("q = 1-p;");
	SendScilabJob("x = cdfnor('X',Mean*ones(p),Stdev*ones(p),p,q);");
	//SendScilabJob("x = cdfnor('X',Mean,Stdev,p,q);");
	SendScilabJob("plot(p,x);");
  	SendScilabJob("da=gda()"); // get the handle on axes model to view and edit the fields
	SendScilabJob("xs2png(0,'Scilab-gaussian.png');");
        //SendScilabJob("Sum = sum(Vector);"); /* Display C */
        //SendScilabJob("disp(Sum);"); /* Display C */
  	SendScilabJob("da=gda()"); // get the handle on axes model to view and edit the fields
	// title by default
	//SendScilabJob("da.title.text="My DefaultTitle"");
	
	SendScilabJob("plot(Matrix)");
	//SendScilabJob("hist3d(Matrix)");
	//SendScilabJob("plot3d(Matrix)");

	//FK - This works but is not as detailed as the previous one
	//SendScilabJob("plot2d(Matrix)");

	//TODO	
	//FK - Ca, ca sera le top, et puis ca résoudrait le problème du burndwon aussi
	//SendScilabJob("plot3d(Matrix)");

	SendScilabJob("xtitle('All categories in one graph')");
	SendScilabJob("legend(['Histogramme des dépenses']");
	SendScilabJob("f=get('current_figure')");
	SendScilabJob("f.figure_size=[700,400]");
	SendScilabJob("xs2png(0,'Scilab-categories.png');");
	/****** TERMINATION **********/
    	}
	
	printf("On referme la connection...\n");
    	mysql_close(connect);   /* Close and shutdown */


	if ( TerminateScilab(NULL) == FALSE ) 
	{
  		fprintf(stderr,"Error while calling TerminateScilab\n");
		return -2;
	}

    	return 0;
}
