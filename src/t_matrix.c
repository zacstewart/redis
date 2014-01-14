#include "redis.h"

void addReplyMatrixShape(redisClient *c, matrix *matrix) {
    addReplyMultiBulkLen(c,matrix->dims);
    for (int i = 0; i < matrix->dims; i++) {
        addReplyBulkLongLong(c,matrix->shape[i]);
    }
}

void xgetCommand(redisClient *c) {
    robj *xobj = lookupKeyWrite(c->db,c->argv[1]);
    long long dims = 0;
    dims = (long long)c->argc - 2;
    long long index[dims];

    if (!xobj) {
        addReply(c,shared.nokeyerr);
        return;
    }

    if (xobj && xobj->type != REDIS_MATRIX) {
        addReply(c,shared.wrongtypeerr);
        return;
    }

    matrix *m = xobj->ptr;

    if (dims > m->dims) {
        addReply(c,shared.outofrangeerr);
        return;
    }

    for (int j = 0; j < dims; j++) {
        getLongLongFromObjectOrReply(c,c->argv[j+2],&index[j],NULL);
    }

    matrix *sub = matrixSlice(m, dims, index);

    addReplyMatrixShape(c,sub);
    matrixFree(sub);
}

void xsetCommand(redisClient *c) {
    long long dims = 0;
    dims = c->argc - 3;
    long long index[dims], shape[dims];
    double value;

    getDoubleFromObjectOrReply(c,c->argv[c->argc-1],&value,NULL);

    for (int j = 0; j < dims; j++) {
        getLongLongFromObjectOrReply(c,c->argv[j+2],&index[j],NULL);
        shape[j] = index[j]+1;
    }

    robj *xobj = lookupKeyWrite(c->db,c->argv[1]);

    if (xobj && xobj->type != REDIS_MATRIX) {
        addReply(c,shared.wrongtypeerr);
        return;
    }

    if (!xobj) {
        xobj = createMatrixObject(dims,shape);
        dbAdd(c->db,c->argv[1],xobj);
        incrRefCount(xobj);
    }

    matrix *m = xobj->ptr;

    if (dims > m->dims) {
        addReply(c,shared.outofrangeerr);
        return;
    }

    if (matrixSetValueAtIndex(m,index,value)) {
        signalModifiedKey(c->db,c->argv[1]);
        notifyKeyspaceEvent(REDIS_NOTIFY_MATRIX,"xset",c->argv[1],c->db->id);
        server.dirty += 1;
    }

    addReplyMatrixShape(c,m);
}
