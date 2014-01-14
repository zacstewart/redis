#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct matrix {
    long long dims;
    long long *shape;
    double *values;
} matrix;

matrix *matrixCreate(long long dims, long long shape[]);
void matrixFree(matrix *matrix);
void matrixPrint(matrix *m);
int matrixSetValueAtIndex(matrix *matrix, long long index[], double value);
matrix *sliceMatrix(matrix *matrix, long long dims, long long index[]);
double matrixGetValueAtIndex(matrix *matrix, long long index[]);

#endif /* __MATRIX_H__ */
