#include "balloc.h"
void print_matrix(int **matrix,int rows,int cols){
    for (int i = 0;i<rows;i++){
        for (int j = 0;j<cols;j++){
            printf("%d  ",matrix[i][j]);
        }
        putchar('\n');
    }
    return;
}
int main(){
    //OS End Sem matrix allocation example

    printf("Fragment 1 allocation\n");

    int *temp, **A; int ii;
    int rows=5; int cols = 5;
    if (NULL == (temp = (int *) bmalloc(rows*cols*sizeof(int))) || NULL == (A = (int **) bmalloc(rows*sizeof(int *)))) exit(1);
    for (ii=0;ii<rows;temp += cols,ii++){
        A[ii] = temp;
        for (int j = 0;j<cols;j++){
            A[ii][j] = j;
            printf("%d  ",A[ii][j]);
        }
        putchar('\n');
    }
    printf("Freeing the matrix\n\n");
    bfree(A); 
    bfree(A[0]);
    freelist_Status(); 

    printf("Fragment 2 allocation\n\n");

    if (NULL == (A = (int **)bmalloc(rows*sizeof(int *)))) exit(1);
    for (ii=0;ii<rows;ii++){
        if (NULL == (A[ii] = (int *) bmalloc(cols*sizeof(int)))) exit(1);
        for (int j = 0;j<cols;j++){
            A[ii][j] = j;
        } 
    }
    print_matrix(A,rows,cols);
    //freelist_Status();
    
    int newrows = rows+4;int newcols = cols+9;
    printf("Realloacting Fragment 2 matrix with rows = %d , cols = %d\n\n",newrows,newcols);
    if (NULL == (A = (int **) brealloc(A,newrows*sizeof(int *)))) exit(1);
    for (ii=0;ii<rows;ii++){
        if (NULL == (A[ii] = (int *)brealloc(A[ii],newcols*sizeof(int)))) exit(1);
    }
    for (ii=rows;ii<newrows;ii++){
        if (NULL == (A[ii] = (int *)bmalloc(newcols*sizeof(int)))) exit(1);
    }
    
    print_matrix(A,newrows,newcols);

    printf("freeing the fragment 2 matrix\n\n");

    for (ii = 0;ii<newrows;ii++){
        bfree(A[ii]);
    }
    bfree(A);
    freelist_Status();
    
    return 0;
}