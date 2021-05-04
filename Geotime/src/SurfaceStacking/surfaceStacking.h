#ifndef __SURFACESTACKING__
#define __SURFACESTACKING__

int surfaceStackingInit(void** p);
int surfaceStackingRelease(void** _p);

int surfaceStackingSetSize(void* _p, int* size);
int surfaceStackingSetStack(void* p, void* stack);
int surfaceStackingSetStackFormat(void* _p, int format);
int surfaceStackingSetSurface(void* _p, void* surface);
int surfaceStackingSetSurfaceFormat(void* _p, int format);

int surfaceStackingRun(void* _p);


#endif