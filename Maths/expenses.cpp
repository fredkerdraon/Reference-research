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

#include "mysql.h" // MySQL Include File
#define SERVER "localhost"
#define USER "username"
#define PASSWORD "password"
#define DATABASE "databasename"


using namespace std;

int main( int argc, char* argv[])
{
	setenv("START","/usr/share/scilab/etc/scilab.start",1);
	setenv("SCI","/usr/share/scilab",1);

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
	unsigned int numrows2;	
	if(strcmp(argv[1],"expenses") == 0)
	{
			printf("Second param: %s\n",argv[2]);				
			printf("Third param: %s\n",argv[3]);				

                        MYSQL_RES *res_set2; /* Create a pointer to recieve the return value.*/
                        MYSQL_ROW row2;  /* Assign variable for rows. */
                        string query2;
                        //query2 = "select distinct(Category) from cashflows where Category in ('Debt','Telephone','Cash','Salary','Other','Toxics');";
                        query2 = "select distinct(Category) from cashflows limit 5";
    			mysql_query(connect,query2.c_str());
    			res_set2 = mysql_store_result(connect); /* Receive the result and store it in res_set */
			numrows2 = mysql_num_rows(res_set2); /* Create the count to print all rows */

			int b = 0;
			string Category; 
			double Matrix[numrows2][24];	

			//printf("%s\n",Categoryc_str());	

                        while ((row2 = mysql_fetch_row(res_set2)) != NULL)
                        {
                                Category = row2[0];
				//printf("%s\n",Categoryc_str());	
				MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
    				MYSQL_ROW row;  /* Assign variable for rows. */
				string query;
				
				query = "select DATE_FORMAT(MyDate,'%y') Year, DATE_FORMAT(MyDate,'%b') Month,Category, sum(Debit), sum(Credit) from cashflows where Category = "; 
				query.append("'");
				query.append(Category);
				query.append("'");
				query.append(" group by MONTH(MyDate), YEAR(MyDate), Category order by Year, MONTH(MyDate), sum(Debit) desc;");
				//printf("%s \n",query.c_str());
				printf("%s \n",Category.c_str());
    				mysql_query(connect,query.c_str());
    				res_set = mysql_store_result(connect); /* Receive the result and store it in res_set */
    				numrows = mysql_num_rows(res_set); /* Create the count to print all rows */

				int a = 0;

				//double Vector[numrows];
    				while ((row = mysql_fetch_row(res_set)) != NULL)
				{
					Matrix[b][a] = atof(row[3]);
					a++;
    				}
				printf("Number of items for that category: %d\n",a);
				b++;
			}
			printf("Number of categories: %d\n",b);
			//double C[] = {1,3,4,9,2,8,3,2};   /* Declare the matrix */
        		int rowVector = numrows2, colVector = numrows; /* Size of the matrix */
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
        const char* Title = "Mon titre";
        printf("Display from Scilab of Vector:\n");
        SendScilabJob("hist3d(Matrix)"); /* Display C */
        SendScilabJob("disp(Matrix)"); /* Display C */

	SendScilabJob("xtitle('Burndown non normalisé')");
	SendScilabJob("f=get('current_figure')");
	SendScilabJob("f.figure_size=[700,500]");
	SendScilabJob("f.color_map=jetcolormap(64)");
	SendScilabJob("xs2png(0,'Scilab-expenses.png');");
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
