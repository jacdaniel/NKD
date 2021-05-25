
#include <stdio.h>
#include <utils.h>
#include <seismicPolarity.h>

#define hp
#define MAXCHIFFRE 1

void c_bset_NK(char* a, size_t k)
{
    a[k / 8] |= 0x80 >> (k % 8);
}

size_t c_btesti_NK(char* a, size_t k)
{
#ifdef hp
    if (a[k / 8] & (0x80 >> (k % 8))) return(MAXCHIFFRE);
    else                                  return(0);
#else
    return (a[k / 8] & (0x80 >> (k % 8)));
#endif
}

int getTraceType(char* pol, int ir, int dimx)
{
    int type = 0;
    if (ir >= dimx - 1) return type;
    if (c_btesti_NK(pol, ir) != c_btesti_NK(pol, ir + 1)) {
        if (c_btesti_NK(pol, ir) != 0)
            type = 1;
        else
            type = -1;
    }
    return type;
}

int seismisPolarityTraceFill(short* trace, int dimx, char* pol, int polarityDimx)
{
    for (int x = 1; x < dimx; x++)
    {
        if (trace[x] - trace[x - 1] >= 0)
        {
            c_bset_NK(pol, x);
        }
    }
    return SUCCESS;
}




int SnapMinMax(char* pol, int ir, int dimx, int type)
{
    int irs = ir;
    if (getTraceType(pol, ir, dimx) == type) return ir;
    if (type == 1) {
        if (c_btesti_NK(pol, irs) != 0 && irs < dimx - 1) {
            while (c_btesti_NK(pol, irs + 1) != 0 && irs < dimx - 2) {
                irs++; // fprintf(stderr, "a irs: %d\n", irs);
            }
        }
        else {
            while (c_btesti_NK(pol, irs) == 0 && irs > 1) {
                irs--; // fprintf(stderr, "b irs: %d\n", irs);
            }
        }
    }
    else {
        if (c_btesti_NK(pol, ir) != 0) {
            while (c_btesti_NK(pol, irs) != 0 && irs > 1) {
                irs--; // fprintf(stderr, "c irs: %d\n", irs);
            }
        }
        else {
            while (c_btesti_NK(pol, irs + 1) == 0 && irs < dimx - 2) {
                irs++; // fprintf(stderr, "d irs: %d\n", irs);
            }
        }
    }
    return irs;
}
