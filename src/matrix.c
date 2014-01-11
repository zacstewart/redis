#include "matrix.h"
#include "zmalloc.h"
#include <stdio.h>
#include <string.h>

matrix *matrixCreate(long long dims, long long shape[]) {
    struct matrix *matrix;
    if ((matrix = zmalloc(sizeof(*matrix))) == NULL)
        return NULL;

    memcpy(&matrix->dims,&dims,sizeof(dims));

    if ((matrix->shape = zmalloc(dims*sizeof(*matrix->shape))) == NULL)
        return NULL;
    memcpy(matrix->shape,shape,dims*sizeof(*matrix->shape));

    long long size = 1;
    for (int i = 0; i < dims; i++)
        size *= shape[i];

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

matrix *sliceMatrix(matrix *m, long long dims, long long *index) {
    long long i, begin = 0, end = 0, stride = 1, newdims = 0, newsize = 1;
    long long beg_idx[dims], end_idx[dims], newshape[dims];


    for (i = 0; i < dims; i++) {
      if (index[i] > 0)
          stride += i * (m->shape[i] - 1);
      else {
          newshape[newdims++] = m->shape[i];
      }

      beg_idx[i] = index[i] < 0 ? 0 : index[i];
      end_idx[i] = index[i] < 0 ? m->shape[i] - 1 : index[i];
    }

    matrix *sub = matrixCreate(newdims, newshape);

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
    return matrix->values[getReshapedIndex(matrix, index)];
}

int matrixSetValueAtIndex(matrix *matrix, long long index[], double value) {
    matrix->values[getReshapedIndex(matrix, index)] = value;

    return 1;
}
