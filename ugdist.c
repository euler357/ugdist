/****************************************************
* Uniform to Gaussian Distribution                  *
* Uses Box-Muller transform                         *
* Ref: http://bit.ly/2GIs0mn                        *
*                                                   *
* Chris K Cockrum                                   *
* https://cockrum.net                               *
* 02/03/2018                                        *
*                                                   *
* Infile: binary uniform distribution file          *
* (i.e. from TrueRNG binary mode capture)           *
* OutFile: ASCII text file of floating point values *
* Output will have Gaussian distribution with mean  *
* and standard deviation as defined below or input  *
* conversion if the INPUT_ONLY is set               *
****************************************************/

#include<stdio.h>
#include<math.h>

#define two_pi    2*3.14159265358979323846264338327
#define mean      0.0
#define stddev    10.0

#define INPUT_ONLY 0

int main(int argc, char** argv)
{
   FILE *infile;           /* Input File Pointer */
   FILE *outfile;          /* Output File Pointer */
   FILE *outfile2;         /* Output File Pointer */
   int x_in,y_in;          /* Variables to read input binary values */
   double x,y;             /* Variables for input values as floats */
   double g0,g1;           /* Variables to hold Gaussian distributed values */
   size_t xbytesread;      /* Number of bytes read */
   size_t ybytesread;      /* Number of bytes read */

   /* If we don't have 2 command line inputs */
   if (argc !=3 )
   {
      printf("Usage:\n ugdist infile outfile\n");
      printf("Infile: binary uniform distribution file (i.e. from TrueRNG binary mode capture)\n");
      printf("OutFile: ASCII text file of floating point values\n");
      return 0;
   }

   /* Open the Input File */
   infile=fopen(argv[1],"rb");
   if(infile<=0)
   {
      printf("Can't open %s\n",argv[1]);
      return 0;
   }

   /* Open the Output File */
   outfile=fopen(argv[2],"w+");
   if(outfile<=0)
   {
      printf("Can't open %s\n",argv[2]);
      fclose(infile);
      return 0;
   }

   /* Print output file comment */
   fprintf(outfile,"%% Gaussian Distribution Random Numbers\n");

   while(1)
   {
      /* Read 4 bytes to each int */
      xbytesread=fread(&x_in,1,4,infile);
      ybytesread=fread(&y_in,1,4,infile);

      /* If we are out of data then exit this loop */
      if((xbytesread!=4)||(ybytesread!=4))
         break;

      /* Scale to +/- 1 */
      x=(double)x_in / (double)(1<<31);
      y=(double)y_in / (double)(1<<31);

      #if INPUT_ONLY 

      /* Only print converted and scaled input values */
      fprintf(outfile,"%f\n",x);
      fprintf(outfile,"%f\n",y);

      #else
         
      /* If x != 0 -- prevent discontinuity in log at 0 */
      if(x!=0)
      {

         /* change sign for -log since the log function doesn't support complex numbers */
         if(x>0)
         {
            g0=sqrt(-2.0*log(x))*cos(two_pi*y);
            g1=sqrt(-2.0*log(x))*sin(two_pi*y);
         }
         else
         {
            g0=-sqrt(-2.0*log(-x))*cos(two_pi*y);
            g1=-sqrt(-2.0*log(-x))*sin(two_pi*y);
         }

         /* Apply mean and standard deviation then print to output file */
         fprintf(outfile,"%f\n",g0*stddev+mean);
         fprintf(outfile,"%f\n",g1*stddev+mean);
      }
      
      #endif

   };

   /* Close Files */
   fclose(infile);
   fclose(outfile);

   return 0;
}
