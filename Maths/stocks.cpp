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
    //int *newMatrixOfBoolean = NULL;
    //int i = 0;

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
    //sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &matrixOfDouble);
    if (sciErr.iErr)
    {
        //printError(&sciErr, 0);
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
        //Scierror(999, "%s: Wrong size for input arguments: Same size expected.\n", fname, 1);
        return 0;
    }

    //newMatrixOfDouble = (double*)malloc(sizeof(double) * m1 * n1);
    ////////// Application code //////////
    // Could be replaced by a call to a library

	return 0;
}

int main( int argc, char* argv[])
{
	//const double PI = 3.1415926535;
	//char myGrade = 'A';
	//bool isHappy = true;
	//setenv("SCI","/home/frederickerdraon/.Scilab/scilab-5.5.0",1);
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

	printf("Connection to the database...\n");

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
        	printf("Connection Succeeded\n");
    	}
    	else
	{
        	printf("Connection Failed!\n");
    	}
	
	printf("\n");
	printf("Running the query...\n");

	int numrows;	
	if(strcmp(argv[1],"stocks") == 0)
	{
			printf("Second param: %s\n",argv[2]);				
			printf("Third param: %s\n",argv[3]);				

			MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
    			MYSQL_ROW row;  /* Assign variable for rows. */
			string query;
			query = "SELECT * FROM stocks WHERE Date between "; 
			query.append("'");
			query.append(argv[2]);
			query.append("' AND '");
			query.append(argv[3]);
			query.append("'");
			printf("%s \n",query.c_str());
    			//mysql_query(connect,"SELECT * FROM cashflows");
    			mysql_query(connect,query.c_str());
    			/* Send a query to the database. */
    			unsigned int i = 0; /* Create a counter for the rows */
			
			printf("On est connecté...\n");

    			res_set = mysql_store_result(connect); /* Receive the result and store it in res_set */
			
			printf("On a récupéré le résultat...\n");

    			numrows = mysql_num_rows(res_set); /* Create the count to print all rows */
			
			printf("On a compté le nombre de lignes...\n");

			if(numrows == 0){
				printf("On a aucune ligne!\n");
			} else {
				printf("Nombre de lignes: %d\n",numrows);
			}
			int a = 0;
			
			/*double Vector[numrows];
			double Vector2[numrows];
			double Vector3[numrows];
			double Vector4[numrows];*/
			
			double *Vector = new double[numrows];
			double *Vector2 = new double[numrows];
			double *Vector3 = new double[numrows];
			double *Vector4 = new double[numrows];
	
			printf("On a alloué les vecteurs...\n");

    			/*This while is to print all rows and not just the first row found, */
    			while ((row = mysql_fetch_row(res_set)) != NULL)
			{
				Vector[a] = atof(row[4]);
				Vector2[a] = atof(row[3]);
				Vector3[a] = atof(row[2]);
				Vector4[a] = atof(row[1]);
				a++;
    			}
			printf("On a parcouru le résultat...\n");
			//double C[] = {1,3,4,9,2,8,3,2};   /* Declare the matrix */
        		int rowVector = numrows, colVector = 1; /* Size of the matrix */
        		char variableNameVector[] = "Vector";
        		char variableNameVector2[] = "Vector2";
        		char variableNameVector3[] = "Vector3";
        		char variableNameVector4[] = "Vector4";
        		//SciErr sciErr;

        		/*
         		* Write it into Scilab's memory 
         		*/
        		sciErr = createNamedMatrixOfDouble(pvApiCtx,variableNameVector,rowVector,colVector, Vector); /* pvApiCtx is a global variable */
        		sciErr = createNamedMatrixOfDouble(pvApiCtx,variableNameVector2,rowVector,colVector, Vector2); /* pvApiCtx is a global variable */
        		sciErr = createNamedMatrixOfDouble(pvApiCtx,variableNameVector3,rowVector,colVector, Vector3); /* pvApiCtx is a global variable */
        		sciErr = createNamedMatrixOfDouble(pvApiCtx,variableNameVector4,rowVector,colVector, Vector4); /* pvApiCtx is a global variable */

			//double *matrixOfDouble = (double*)malloc((numrows, 1)*sizeof(double));
			int alpha = 1;
			//sciErr = readNamedMatrixOfDouble(pvApiCtx, variableNameVector, &numrows, &alpha, matrixOfDouble);
			printf("On a cree la matrice\n");
        		if(sciErr.iErr)
        		{
            			printError(&sciErr, 0);
        		}
        	/*
         * Prior to Scilab 5.2:
         * C2F(cwritemat)(variableNameB, &rowB, &colB, B, strlen(variableNameB));
        */

        //printf("\n");
        //printf("Display from Scilab of Vector:\n");
        //SendScilabJob("disp(Vector);"); /* Display C */
	printf("Standard deviation\n");
        SendScilabJob("Stdev = stdev(Vector);"); /* Display C */
        SendScilabJob("disp(Stdev);"); /* Display C */
	printf("Mean\n");
        SendScilabJob("Mean = mean(Vector);"); /* Display C */
        SendScilabJob("disp(Mean);"); /* Display C */
	printf("Median\n");
        SendScilabJob("Median = median(Vector);"); /* Display C */
        SendScilabJob("disp(Median);"); /* Display C */
	printf("Geometric Mean\n");
        SendScilabJob("GeoMean = geomean(Vector);"); /* Display C */
        SendScilabJob("disp(GeoMean);"); /* Display C */
	printf("C moment\n");
        SendScilabJob("CMoment = cmoment(Vector,1);"); /* Display C */
        SendScilabJob("disp(CMoment);"); /* Display C */
	printf("Variance\n");
        SendScilabJob("Variance = variance(Vector);"); /* Display C */
        SendScilabJob("disp(Variance);"); /* Display C */
        //SendScilabJob("Sum = sum(Vector);"); /* Display C */
        //SendScilabJob("disp(Sum);"); /* Display C */
  	SendScilabJob("da=gda()"); // get the handle on axes model to view and edit the fields
	// title by default
	//SendScilabJob("da.title.text="My DefaultTitle"");
	int alpha2 = 1;
	double *Stdev = (double*)malloc((1, 1)*sizeof(double));
        sciErr = readNamedMatrixOfDouble(pvApiCtx, "Stdev", &alpha2, &alpha2, Stdev);
        printf("The result for the standard deviation is: %lf\n", Stdev[0]);

	//string StdevString = std::to_string(Stdev[0]);
	char buffer [50];	
	sprintf (buffer, "%f", Stdev[0]);
	string StdevString = string(buffer);

	string instrument="INSERT INTO stockAnalysis (Stdev) VALUES ("+ StdevString + ")";
                                        //printf("Added: %s\n",instrument.c_str());
        mysql_query(connect,instrument.c_str());

	SendScilabJob("subplot(211)");
	SendScilabJob("plot(Vector)");
	SendScilabJob("plot(Vector2)");
	SendScilabJob("plot(Vector3)");
	SendScilabJob("plot(Vector4)");
    	SendScilabJob("xlabel('Time')");
    	SendScilabJob("ylabel('Value')");
	SendScilabJob("xtitle('Spot HSBC')");
	SendScilabJob("legend(['Histogramme du stock']");

	SendScilabJob("subplot(212)");
	SendScilabJob("histplot(200,Vector4)");
    	SendScilabJob("xlabel('Epochs')");
    	SendScilabJob("ylabel('Average Error')");

	SendScilabJob("f=get('current_figure')");
	SendScilabJob("f.figure_size=[700,400]");
	SendScilabJob("xs2png(0,'Scilab-stocks.png');");
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
