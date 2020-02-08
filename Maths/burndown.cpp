//To compile this we need: g++ -Wall -Wl,--no-as-needed -o simple_call_scilab.exe $(pkg-config --libs --cflags scilab) $(ls /usr/lib/scilab/ | sed 's/^lib/-l/g' | sed "s/\.so.*$//g" | sort | uniq | grep -v disable) simple_call_scilab.c

//-lsciaction_binding -lsciarnoldi -lscicall_scilab -lscicommons -lscicompletion -lsciconsole -lsciconsole-minimal -lsciexternal_objects -lsciexternal_objects_java -lscifunctions -lscigraphic_export -lscigraphic_objects -lscigraphics -lscigui -lscihdf5 -lscihelptools -lscihistory_browser -lscihistory_manager -lsciinterpolation -lscijvm -lscilab -lscilab-cli -lscilocalization -lscimatio -lscioptimization -lsciparallel -lscipreferences -lscipreferences-cli -lscirandlib -lscirenderer -lsciscicos -lsciscicos_blocks -lsciscicos_blocks-cli -lsciscicos-cli -lsciscinotes -lscisignal_processing -lscisound -lscispecial_functions -lscispreadsheet -lscistatistics -lscisundials -lscisymbolic -lscitclsci -lscitypes -lsciui_data -lsciumfpack -lscixcos -lscixml

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "matrix.h"
#include "rational.h"
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

	unsigned int numrows;	
	if(strcmp(argv[1],"burndown") == 0)
	{
			printf("Second param: %s\n",argv[2]);				
			printf("Third param: %s\n",argv[3]);				

			MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
    			MYSQL_ROW row;  /* Assign variable for rows. */
			string query;
			//query = "select * from kapital order by Date desc limit 50"; 
			query = "select * from contacts order by Rating desc limit 50"; 
			//query.append("'");
			//query.append(argv[2]);
			//query.append("' AND '");
			//query.append(argv[3]);
			//query.append("'");
			printf("%s \n",query.c_str());
    			//mysql_query(connect,"SELECT * FROM cashflows");
    			mysql_query(connect,query.c_str());
    			/* Send a query to the database. */
    			unsigned int i = 0; /* Create a counter for the rows */

    			res_set = mysql_store_result(connect); /* Receive the result and store it in res_set */

    			numrows = mysql_num_rows(res_set); /* Create the count to print all rows */

			int a = 0;
			double Vector[numrows];
			double Matrix[numrows][4];	
			//Matrix <double> matrix = new Matrix <double> ( 49 , 4, 1 ); // no more, no less, than a matrix
			//matrix[ slice( 2, col, row ) ] = pi; // set third column to pi
			//matrix[ slice( 3*row, row, 1 ) ] = e; 

    			/*This while is to print all rows and not just the first row found, */
			//int count = 0;
    			while ((row = mysql_fetch_row(res_set)) != NULL)
			{
				//Matrix[0][a] = 10*atof(row[1]);
				//Matrix[1][a] = 10*atof(row[2]);
				//Matrix[2][a] = 10*atof(row[3]);
				//Matrix[4][a] = 10*atof(row[4]);
				//Matrix[a][0] = 10*atof(row[2]);
				//Matrix[a][1] = 10*atof(row[2]);
				//Matrix[a][2] = 10*atof(row[2]);
				//Matrix[a][3] = 10*atof(row[2]);
				Matrix[a][0] = atof(row[2])/10;
				Matrix[a][1] = atof(row[2])/10;
				Matrix[a][2] = atof(row[2])/10;
				Matrix[a][3] = atof(row[2])/10;
				a++;
    			}
			//double C[] = {1,3,4,9,2,8,3,2};   /* Declare the matrix */
        		int rowVector = numrows, colVector = 4; /* Size of the matrix */
        		char variableNameMatrix[] = "Matrix";
        		//SciErr sciErr;

        		/*
         		* Write it into Scilab's memory 
         		*/
        		sciErr = createNamedMatrixOfDouble(pvApiCtx,variableNameMatrix,rowVector,colVector, *Matrix); /* pvApiCtx is a global variable */
			printf("On a cree la matrice\n");
        		if(sciErr.iErr)
        		{
            			printError(&sciErr, 0);
        		}
        	/*
         * Prior to Scilab 5.2:
         * C2F(cwritemat)(variableNameB, &rowB, &colB, B, strlen(variableNameB));
        */

        const char* Title = "Mon titre";
	//printf("\n");
        //printf("Display from Scilab of Vector:\n");
        SendScilabJob("Z(0:100,:) = Matrix;"); /* Display C */
        SendScilabJob("disp(Matrix)"); /* Display C */
	//printf("Standard deviation\n");
        SendScilabJob("hist3d(Matrix)"); /* Display C */
        //SendScilabJob("hist3d(10*rand(10,10));"); /* Display C */
	//FK - Ici on masque l'affichage de la matrice
        //SendScilabJob("disp(Matrix)"); /* Display C */
  	//SendScilabJob("da=gda()"); // get the handle on axes model to view and edit the fields
	// title by default
	//FK - Tentative avec une matrice pour le burndown

	//SendScilabJob("da.title.text=Title");
	//SendScilabJob("plot(Matrix)");
	//SendScilabJob("legend(['Histogramme du cours']");
	SendScilabJob("xtitle('Burndown non normalisé')");
	SendScilabJob("f=get('current_figure')");
	
	//SendScilabJob("f.figure_size=[700,500]");
	SendScilabJob("f.color_map=jetcolormap(64)");
	//SendScilabJob("f.color_map=hotcolormap(128)");
	SendScilabJob("xs2png(0,'Scilab-burndown.png');");
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
