/*******************************************************
 #
 # Panwen Wang, April 29th, 2009
 # pwwang AT pwwang.com
 # get eigenvalue and eigenvector of
 # a real symmetric matrix like
 #  1  -1  0
 # -1   1  2
 #  0   2  1
 #
 # Algorthm:
 # 1. get a nonzero and non-diagonal element a[i,j] of matrix A, usually
 #    the maximun absolute value of non-diagonal elements of A (max(A));
 # 2. give sin(phi) and cos(phi) by
 #    ( a[j,j]-a[i,i] )*sin(2*phi) + 2*a[i,j]*cos(2*phi) = 0 ;
 #    => tan(2*phi) = 2*a[i,j]/(a[i,i]-a[j,j])
 #    => phi = arctan(2*a[i,j]/(a[i,i]-a[j,j]))/2
 # 3. get elements of a new matrix A1(a1[i,j]) by
 #    a1[i,i] = a[i,i]*cos^2(phi) + a[j,j]*sin^2(phi) + 2*a[i,j]*cos(phi)*sin(phi)
 #    a1[j,j] = a[i,i]*sin^2(phi) + a[j,j]*cos^2(phi) - 2*a[i,j]*cos(phi)*sin(phi)
 #    a1[i,l] = a1[l,i] = a[i,l]*cos(phi) + a[j,l]*sin(phi)       ( l!=i,j )
 #    a1[j,l] = a1[l,j] = -a[i,l]*sin(phi) + a[j,l]*cos(phi)      ( l!=i,j )
 #    a1[l,m] = a1[m,l] = a[m,l]                                  ( m,l != i,j )
 #    a1[i,j] = a1[j,i] = { (a[j,j]-a[i,i])*sin(2*phi) }/2 + a[i,j]*(cos^2(phi) - sin^2(phi))
 # 4. let A1 be the substitution of A, repeat step 1,2,3 and get A2, and A3,A4,...,An can be
 #    obtained by the same way. Calculation ceases if max(An) is less than the given threshold.
 #
 # Usage:
 #        JACOBI_ARRAY ja(3), value;
 #        JACOBI_MATRIX jm, vector;
 #        ja[0] = 1; ja[1] = -1; ja[2] = 0;
 #        jm.push_back(ja);
 #        ja[0] = -1; ja[1] = 1; ja[2] = 2;
 #        jm.push_back(ja);
 #        ja[0] = 0; ja[1] = 2; ja[0] = 1;
 #        jm.push_back(ja);
 #        Jacobi j;
 #        j.setMatrix(jm);
 #         value = j.getEigenvalues();
 #        vector = j.getEigenvectors();
 #
 ********************************************************/
#ifndef _JACOBI_H_
#define _JACOBI_H_

#include <vector>
#include <cmath>
#include "Vector.h"
typedef vector<double> JACOBI_ARRAY;
typedef vector<vector<double> > JACOBI_MATRIX;
typedef vector<double>::size_type JACOBI_vint;
using namespace std;

class Jacobi
{
    private:
        JACOBI_MATRIX matrix;
        JACOBI_ARRAY eigenvalues;
        JACOBI_MATRIX eigenvectors;
        double e;   //precision
        int dimen;
        bool debug;

        double max(const JACOBI_MATRIX &, int &, int &);
        JACOBI_MATRIX newMatrix(const JACOBI_MATRIX &, const int &, const int &,
                const double &);
        void jacobi();

    public:
        Jacobi();
        void setMatrix(const JACOBI_MATRIX &);
        void setE(const double &);
        void setDebug(const bool &d = true);
        JACOBI_ARRAY getEigenvalues();
        JACOBI_MATRIX getEigenvectors();
        void getEigenVec(int i, Vector<float>& alpha);
        float getEigenValue(int i);
        void printEigen();
        void printMatrix();
        void printMatrix(const JACOBI_MATRIX &);
        void demo();
};


#endif
