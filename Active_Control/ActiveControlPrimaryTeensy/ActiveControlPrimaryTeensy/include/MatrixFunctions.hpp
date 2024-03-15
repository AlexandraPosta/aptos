#ifndef MATRIX_FUNCTIONS
#define MATRIX_FUNCTIONS

namespace MatrixFunctions
{
    int ravelIndex(int jmax, int kmax, int i, int j, int k)
    {
        return i * jmax * kmax + j * kmax + k;
    }

    int ravelIndex(int jmax, int i, int j)
    {
        return i * jmax + j;
    }
};

#endif
