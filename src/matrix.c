#include "matrix.h"
#include "zmalloc.h"
#include <stdio.h>
#include <string.h>

void matrixPrint(matrix *m) {
  long long i, size = 1;
  printf("dims:\t%lld\n", m->dims);

  printf("shape:\t[ ");
  for (i = 0; i<m->dims; i++) {
    size *= m->shape[i];
    printf("%lld ", m->shape[i]);
  }
  printf("]\n");

  printf("values:\t[ ");
  for (i = 0; i<size; i++) printf("%f ", m->values[i]);
  printf("]\n");
}

matrix *matrixCreate(long long dims, long long shape[]) {
    struct matrix *matrix;
    long long size = 1;
    int i;

    if ((matrix = zmalloc(sizeof(*matrix))) == NULL)
        return NULL;

    memcpy(&matrix->dims,&dims,sizeof(dims));

    if ((matrix->shape = zmalloc(dims*sizeof(*matrix->shape))) == NULL)
        return NULL;
    memcpy(matrix->shape,shape,dims*sizeof(*matrix->shape));

    for (i = 0; i < dims; i++) size *= shape[i];

    if ((matrix->values = zcalloc(size*sizeof(*matrix->values))) == NULL)
        return NULL;

    return matrix;
}

void matrixFree(matrix *matrix) {
    zfree(matrix->shape);
    zfree(matrix->values);
    zfree(matrix);
}

long long getReshapedIndex(matrix *m, long long index[]) {
    long long idx = 0;

    for (int i = 0; i < m->dims; i++) {
        idx += index[i];

        if (i < m->dims-1)
            idx *= m->shape[i+1];
    }
    return idx;
}

matrix *matrixSlice(matrix *m, long long dims, long long *index) {
    long long i, begin = 0, end = 0, stride = 1, newdims = 0, newsize = 1;
    long long beg_idx[dims], end_idx[dims], newshape[dims];
    matrix *sub;

    for (i = 0; i < dims; i++) {
      if (index[i] == -1) {
          newshape[newdims++] = m->shape[i];
          beg_idx[i] = 0;
          end_idx[i] = m->shape[i] - 1;
      } else {
          stride += i * (m->shape[i] - 1);
          beg_idx[i] = index[i];
          end_idx[i] = index[i];
      }
    }

    sub = matrixCreate(newdims, newshape);

    begin = getReshapedIndex(m, beg_idx);
    end = getReshapedIndex(m, end_idx);

    for (i = 0; i < newdims; i++)
        newsize *= newshape[i];
    long long newvalues[newsize];
    for (i = begin; i <= end; i++)
        newvalues[i] = m->values[i*stride];

    memcpy(sub->values, newvalues, newsize*sizeof(*newvalues));

    return sub;
}

double matrixGetValueAtIndex(matrix *matrix, long long index[]) {
    double value = matrix->values[getReshapedIndex(matrix, index)];
    return value;
}

int matrixSetValueAtIndex(matrix *matrix, long long index[], double value) {
    long long idx = getReshapedIndex(matrix, index);
    memcpy(&matrix->values[idx],&value,sizeof(&value));
    return 1; // TODO: error handling
}
