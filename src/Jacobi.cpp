#include "Jacobi.h"

Jacobi::Jacobi() :
        e(1e-5), debug(false)
{
}

void Jacobi::setE(const double &ee)
{
    e = ee;
}

void Jacobi::setDebug(const bool &d)
{
    debug = d;
}

void Jacobi::setMatrix(const JACOBI_MATRIX & jm)
{
    dimen = jm.size();
    matrix = jm;
    eigenvalues.resize(dimen);
    eigenvectors.resize(dimen);
    //matrix.resize(dimen);
    //for(JACOBI_vint i=0; i<dimen; i++){ matrix[i] = jm[i];}
    jacobi();
}

JACOBI_ARRAY Jacobi::getEigenvalues()
{
    return eigenvalues;
}

JACOBI_MATRIX Jacobi::getEigenvectors()
{
    return eigenvectors;
}

double Jacobi::max(const JACOBI_MATRIX &jm, int &maxi, int &maxj)
{
    double m = jm[0][1];
    maxi = 0, maxj = 1;
    for (JACOBI_vint i = 0; i < dimen; i++)
        for (JACOBI_vint j = 0; j < dimen; j++)
            if (i != j)
                if (fabs(jm[i][j]) > m)
                {
                    m = fabs(jm[i][j]);
                    maxi = i;
                    maxj = j;
                }
    return m;
}

JACOBI_MATRIX Jacobi::newMatrix(const JACOBI_MATRIX &jm, const int &i,
        const int &j, const double &phi)
{
    JACOBI_MATRIX jm1(dimen);
    double sp = sin(phi), cp = cos(phi);
    for (int ii = 0; ii < dimen; ii++)
    {
        jm1[ii].resize(dimen);
        for (int jj = 0; jj < dimen; jj++)
        {
            if (ii == i)
            {     // row i
                if (jj == i)
                    jm1[ii][jj] = jm[i][i] * cp * cp + jm[j][j] * sp * sp
                            + 2 * jm[i][j] * cp * sp;
                else if (jj == j)
                    jm1[ii][jj] = (jm[j][j] - jm[i][i]) * sp * cp
                            + jm[i][j] * (cp * cp - sp * sp);
                else
                    jm1[ii][jj] = jm[i][jj] * cp + jm[j][jj] * sp;
            }
            else if (ii == j)
            {     // row j
                if (jj == i)
                    jm1[ii][jj] = (jm[j][j] - jm[i][i]) * sp * cp
                            + jm[i][j] * (cp * cp - sp * sp);
                else if (jj == j)
                    jm1[ii][jj] = jm[i][i] * sp * sp + jm[j][j] * cp * cp
                            - 2 * jm[i][j] * cp * sp;
                else
                    jm1[ii][jj] = jm[j][jj] * cp - jm[i][jj] * sp;
            }
            else
            {            // row l ( l!=i,j )
                if (jj == i)
                    jm1[ii][jj] = jm[i][ii] * cp + jm[j][ii] * sp;
                else if (jj == j)
                    jm1[ii][jj] = jm[j][ii] * cp - jm[i][ii] * sp;
                else
                    jm1[ii][jj] = jm[ii][jj];
            }
        }
    }
    return jm1;
}

void Jacobi::jacobi()
{
    JACOBI_MATRIX jm = matrix;  // initial matrix
    double phi, m;
    int i, j;
    JACOBI_MATRIX tempVectors(dimen), vectors(dimen), cmat(dimen);
    vectors[0].resize(dimen);
    vectors[1].resize(dimen);
    for (JACOBI_vint x = 0; x < dimen; x++)
    {
        tempVectors[x].resize(dimen);
        for (JACOBI_vint y = 0; y < dimen; y++)
        {
            if (x == y)
            {
                tempVectors[x][y] = 1.0;
            }
            else
            {
                tempVectors[x][y] = 0.0;
            }
        }
    }

    for (;;)
    { // step 4
        m = max(jm, i, j);  // step 1
        if (debug)
            printf("%-3d, %-3d, %.9f\n", i, j, m);
        if (m < e)
            break;
        // if the maximum value of the matrix LE(Less than or Equal to) the limit, break
        phi = atan2(2 * jm[i][j], jm[i][i] - jm[j][j]) / 2;  // step 2
        jm = newMatrix(jm, i, j, phi); // step 3
        for (JACOBI_vint x = 0; x < dimen; x++)
        {
            cmat[x].resize(dimen);
            vectors[x].resize(dimen);
            for (JACOBI_vint y = 0; y < dimen; y++)
            {
                if (x == y)
                {
                    cmat[x][y] = 1.0;
                }
                else
                {
                    cmat[x][y] = 0.0;
                }
                vectors[x][y] = 0.0;
            }
        }
        cmat[i][i] = cos(phi);
        cmat[j][j] = cos(phi);
        cmat[i][j] = -sin(phi);
        cmat[j][i] = sin(phi);
        for (JACOBI_vint x = 0; x < dimen; x++)
        { // for eigenvectors
            for (JACOBI_vint y = 0; y < dimen; y++)
            {
                for (JACOBI_vint z = 0; z < dimen; z++)
                {
                    vectors[x][y] = vectors[x][y]
                            + tempVectors[x][z] * cmat[z][y];
                }
                //print vectors[x][y],"-",tempVectors[x][y]," ";
            }
        }
        tempVectors = vectors;
        //@tempVectors = @vectors;
    }
    // the digonal elements are the eigenvalues
    for (JACOBI_vint x = 0; x < dimen; x++)
        eigenvalues[x] = jm[x][x];

    for (JACOBI_vint x = 0; x < dimen; x++)
    {
        eigenvectors[x].resize(dimen);
        for (JACOBI_vint y = 0; y < dimen; y++)
        {
            eigenvectors[x][y] = vectors[y][x];
        }
    }
    if (debug)
        printEigen();
}

void Jacobi::printEigen()
{
    for (JACOBI_vint i = 0; i < dimen; i++)
    {
        cout << "Eigenvalue [" << i << "]: " << eigenvalues[i] << "\n";
        cout << "Eigenvector[" << i << "]: [ ";
        for (JACOBI_vint j = 0; j < dimen; j++)
        {
            printf("%9.6f, ", eigenvectors[i][j]);
        }
        cout << " ] \n";
    }
}

void Jacobi::printMatrix()
{
    printMatrix(matrix);
}

void Jacobi::printMatrix(const JACOBI_MATRIX &jm)
{
    for (JACOBI_vint i = 0; i < dimen; i++)
    {
        for (JACOBI_vint j = 0; j < dimen; j++)
        {
            printf("%9.6f, ", jm[i][j]);
        }
        cout << "\n";
    }
}

void Jacobi::getEigenVec(int i, Vector<float>& alpha)
{
    alpha.allocate(dimen);
    for (JACOBI_vint j = 0; j < dimen; j++)
    {
        alpha[j] = eigenvectors[i][j];
    }
}

float Jacobi::getEigenValue(int i)
{
    return eigenvalues[i];
}

void Jacobi::demo()
{
    dimen = 3;
    eigenvalues.resize(dimen);
    eigenvectors.resize(dimen);
    matrix.resize(dimen);
    debug = false;
    e = 1e-5;
    JACOBI_ARRAY ja(3);
    ja[0] = 2;
    ja[1] = -1;
    ja[2] = 0;
    matrix[0] = ja;
    ja[0] = -1;
    ja[1] = 2;
    ja[2] = -1;
    matrix[1] = ja;
    ja[0] = 0;
    ja[1] = -1;
    ja[2] = 2;
    matrix[2] = ja;
    printMatrix();
    jacobi();
    printEigen();
    /*
     2.000000, -1.000000,  0.000000,
     -1.000000,  2.000000, -1.000000,
     0.000000, -1.000000,  2.000000,
     Eigenvalue [0]: 3.41421
     Eigenvector[0]: [  0.500000, -0.707107,  0.500000,  ]
     Eigenvalue [1]: 2
     Eigenvector[1]: [  0.707107,  0.000000, -0.707107,  ]
     Eigenvalue [2]: 0.585786
     Eigenvector[2]: [  0.500000,  0.707107,  0.500000,  ]
     */
}
