#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct scalar {
  double value;
  int reference_count;
} scalar;

typedef struct matrix {
    long long dims;
    long long *shape;
    long long size;
    scalar **values;
} matrix;

matrix *matrixZero(long long dims, long long shape[]);
matrix *matrixCreate(long long dims, long long shape[]);
void matrixFree(matrix *matrix);
void matrixPrint(matrix *m);
int matrixSetValueAtIndex(matrix *matrix, long long index[], double value);
matrix *matrixSlice(matrix *matrix, long long dims, long long index[]);
double matrixGetValueAtIndex(matrix *matrix, long long index[]);
void scalarRelease(scalar *scalar);
void scalarRetain(scalar *scalar);

#endif /* __MATRIX_H__ */
