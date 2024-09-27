#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "balloc.h"
#include "common.h"

int main(){
    char *filenames[5] = {"1.txt","2.txt","3.txt","4.txt","5.txt"};    
    FILE *fp;int i,j,d;
    srand((int) time(NULL));

    /*Writing to the files*/
    for (i = 0;i<5;i++){
        if (NULL == (fp = fopen(filenames[i], "w"))) ERR_MESG("Error opening file");
        d = (rand()%500) + 1;
        fprintf(fp,"%d ",d);
        for (j = 0;j<d;j++){
            fprintf(fp,"%d ",rand()%100);
        }
        fclose(fp);
    }

    /*Storing the files values in a buffer matrix*/
    int **A = (int **)bmalloc(5*sizeof(int *));
    int sizes[5];int total = 0;

    for (i = 0;i<5;i++){
        if (NULL == (fp = fopen(filenames[i], "r"))) ERR_MESG("Error opening file");
        fscanf(fp,"%d",&sizes[i]);
        total += sizes[i];
        A[i] = (int *)bmalloc(sizes[i]*sizeof(int));
        //freelist_Status();
        for (j = 0;j<sizes[i];j++){
            fscanf(fp,"%d",A[i]+j);
        }
        fclose(fp);
    }

    /*Writing to merge file*/
    if (NULL == (fp = fopen("merge.txt", "w"))) ERR_MESG("Error opening file");

    fprintf(fp,"%d ",total);
    for (i=0;i<5;i++){
        for (j=0;j<sizes[i];j++){
            fprintf(fp,"%d ",A[i][j]);
        }
        bfree(A[i]);
    }
    bfree(A);
    fclose(fp);
    //freelist_Status();

    /*Merge check*/
    int temp,temp1;FILE *fp1;
    if (NULL == (fp1 = fopen("merge.txt", "r"))) ERR_MESG("Error opening file");
    for (i = 0;i<5;i++){
        if (NULL == (fp = fopen(filenames[i], "r"))) ERR_MESG("Error opening file");
        fscanf(fp,"%d",&sizes[i]);
        if (i==0) fscanf(fp1,"%d",&total);
        for (j = 0;j<sizes[i];j++){
            fscanf(fp,"%d",&temp);
            fscanf(fp1,"%d",&temp1);
            if (temp == temp1) continue;
            else ERR_MESG("Wrongly merged the files");
        }
        fclose(fp);
    }
    fclose(fp1); 
    printf("Successfully merged the files\n");





    return 0;
}