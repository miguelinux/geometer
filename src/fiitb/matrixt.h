/*
|¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯|
|   Projekt  :                                                             |
|   Version  : 6.0                                                         |
|   Sprache  : Microsoft Visual C++ Version 6.0                            |
|--------------------------------------------------------------------------|
|   Include  : MatrixT.H                                                   |
|   Inhalt   : Matrix Klasse                                               |
|   Stand    : 24.11.2000                                                  |
|--------------------------------------------------------------------------|
|   Autoren  : Christian W. Frey, Armin Alt                                |
|   Copyright: (c) 1992-2000 Fraunhofer Institut für                       |
|              Informations- und Datenverarbeitung (IITB)
|
|   Featuring Propaganda - P-machinery                                     |
|__________________________________________________________________________|
*/

#pragma once

#ifndef MatrixT_t
#define MatrixT_t MatrixT_t

#ifndef CHECKSIZE
    #define CHECKSIZE CHECKSIZE
#endif








/////////////////////////////////////////////////////////////////////
/// Includes
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <limits>
#include <cmath>
#include <cstring>
using namespace std;
//#define MATL_MEX
#ifdef MATL_MEX
    #include "mex.h"
#endif


#define max(a,b)        (((a) > (b)) ? (a) : (b))
#define min(a,b)        (((a) < (b)) ? (a) : (b))

//#define NaN 1e30;
/* IEEE 754 NaNs.
 * double d=0;
 * if(std::numeric_limits<double>::has_quiet_NaN)
 *   d=std::numeric_limits<double>::quiet_NaN();
 * else if (std::numeric_limits<double>::has_signaling_NaN)
 *   d=std::numeric_limits<double>::signaling_NaN();
 * else cerr<<"NaN for double isn¿t supported";
 */
#ifndef NaN
  #define NaN std::numeric_limits<double>::quiet_NaN();
#endif

/////////////////////////////////////////////////////////////////////
/// Fehlerausgabe
/////////////////////////////////////////////////////////////////////

inline void _matrix_error (const char* pErrMsg)                                                 //Routine zur Dokumentation
{                                                                                               //von Fehler während des
    #ifdef MATL_MEX
        mexErrMsgTxt(pErrMsg);
    #else
        cout << pErrMsg << endl;
        exit(1);
    #endif
}


inline bool vergl(char* buf_a, char* buf_b)                                                     //Methode zum Vergleich
{                                                                                               //von Matlab Variablennamen
    register int i;
    for (i=0; i<100; i++)
        if (buf_a[i] != buf_b[i]) return false;
    return true;
}


/////////////////////////////////////////////////////////////////////
/// Klassendeklaration: TemplateKlasse CMatrixT
/////////////////////////////////////////////////////////////////////

template<class T>
class CMatrixT
{
    private:
        int m;                                                                                  //Anzahl Zeilen
        int n;                                                                                  //Anzahl Spalten
        bool ini;                                                                               //Initialisiert Ja/Nein

    public:
        //*** Konstruktor/Destruktor
        inline CMatrixT(const unsigned int &rows, const unsigned int &cols, const T &initVal);
		inline CMatrixT(const unsigned int &rows, const unsigned int &cols);
        inline CMatrixT();

        //*** Copyconstructor
        inline CMatrixT(const CMatrixT& matrixA);
        //***Destruktor
        inline ~CMatrixT();
        //*** MEX Gateway
        inline void lomxArray(double *s, int r, int c);
        inline void remxArray(double *d);
#ifdef MATL_MEX
        inline mxArray* setmxArray();
        inline void getmxArray(mxArray *inp);
#endif
        //*** Verschiedene Funktionen
        inline int matrixRows() const   { return m; }                                           //Gibt Zeilenzahl zurueck
        inline int matrixCols() const   { return n; }                                           //Gibt Spaltenzahl zurueck
        inline void matrixDisplay() const;                                                      //Gibt die Matrix auf den Bildschirm aus
		inline void matrixShowSize() const;                                                      //Gibt die Matrix auf den Bildschirm aus
        inline void matrixShowSize(const char *message) const;                                                      //Gibt die Matrix auf den Bildschirm aus
        inline void init_byVal(const T &initVal);                                               //Initialisieren der Matrix mit Wert x
        inline void init_byRandom(const T &randMax);                                            //Initialisieren der Matrix mit Zufallswert
        inline void matrixSetsize(const int &r, const int &c);                                   //Größenänderung der Matrix und init mit 0
        inline void matrixSetsize(const int &r, const int &c, T v);                              //Größenänderung der Matrix und init mit v
		inline void matrixSetsize(const CMatrixT<T> &matrixA);
		inline void matrixResize(const int& r, const int& c);
		inline void matrixResize(const int& r, const int& c, T v);
        inline void swapn(const int &p1, const int &p2);                                        //Vertauscht Vektoren der Matrix (1-n)
        inline void swapm(const int &p1, const int &p2);                                        //Vertauscht Zeilen der Matrix (1-n)
        inline void sortm(const int &r, const int &left, const int &right);                     //sortiert Spalten anhand einer Zeile
        inline void sortn(const int &r, const int &upper, const int &lower);                    //sortiert Zeilen anhand einer Spalte
        inline CMatrixT<T> copyp(const int &row1, const int &row2, const int &col1, const int &col2);//kopiert Teil einer Matrix
        inline void copyc(const CMatrixT<T> &s, const int &cs, const int &cd);                  //kopiert Spalten
        inline void copyr(const CMatrixT<T> &s, const int &rs, const int &rd);                  //kopiert Zeilen
        inline bool compc(const CMatrixT<T> &s, const int &cs, const int &cd);                  //vergleicht Spalten mit 2. Matrix
        inline bool compc(const int &c1, const int &c2);                                        //vergleicht Spalten innerhalb der Matrix
        inline bool compr(const CMatrixT<T> &s, const int &rs, const int &rd);                  //vergleicht Zeilen mit 2. Matrix
        inline bool compr(const int &r1, const int &r2);
        inline void flipud();
        inline void fliplr();
        inline CMatrixT<int> findm(const int &m,const T v);
        inline CMatrixT<int> findn(const int &n,const T v);
		inline bool is_element_m(const int &m,const T v);
        inline bool is_element_n(const int &n,const T v);
        inline void unit();                                                                     //Macht akt. Matrix zur Einheitsmatrix
        inline void unit(const int &row);
        inline CMatrixT<int> maxind_m();
        inline CMatrixT<int> maxind_n();
        inline void maxind_m(const CMatrixT<double> &inp);
        inline void maxind_n(const CMatrixT<double> &inp);
		inline void maxind_m(const CMatrixT<int> &inp);
        inline void maxind_n(const CMatrixT<int> &inp);
        inline void nul(const int &row, const int &col);                                        //Größe der Matrix aendern und mit 0 initialisieren
        inline void nul();                                                                      //vgl. init_byVal(0)
        void diag(const CMatrixT<T> &matrixA);
        inline int pivot(const int &row);
        inline double norm();                                                                   //Norm der Matrix
        inline bool isScalar();                                                                 //Matrix Skalar ?
        inline bool isDiagonal();                                                               //Matrix Diagonal ?
        inline bool isUnit();                                                                   //Einheitsmatrix ?
        inline bool isNull();                                                                   //Nullmatrix ?
        inline bool isSymmetric();                                                              //Matrix Symmetrisch ?
        inline bool isSkewSymmetric();                                                          //Negierte Symmetrische Matrix ?
        inline bool isUpperTriangular();                                                        //Obere Dreiecksmatrix ?
        inline bool isLowerTriangular();                                                        //Untere Dreiecksmatrix ?
        inline T det();
        inline void findred(const CMatrixT<T> &s);
        inline CMatrixT<T> column(const int &cnr);
        inline CMatrixT<T> row(const int &rnr);
        //*** Mathematische Funktionen
        T matrixSum() const;                                                                    //Summe der einzelnen Werte der Matrix
        T matrixGetMean() const;   
        T matrixGetMax() const;                                                                 //Ermittlung des Maximalwertes
        T matrixGetMin() const;                                                                 //Ermittlung des Minimalwertes
		T matrixGetMax(CMatrixT<int> &max_ind) const;
		T matrixGetMin(CMatrixT<int> &min_ind) const;
        void scale_n(const T scale);
        void scale_m(const T scale);
        void s_pow(const T exp);
        void s_inv();
        void s_mult(const CMatrixT<T> &matrixA,const CMatrixT<T> &matrixB);
        void matrixTransp();                                                                    //Transponiert Matrix
        void matrixTransp(const int diag);                                                      //Transponiert Matrix
        void matrixNeg();                                                                       //Negation der Matrix auch (* -1 möglich)
        void matrixSkalarProdukt(const T &Faktor);                                              //Produkt Matrix x Skalar
        void matrixAdd(const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB);                 //Summe zweier Matrizen
        void matrixSub(const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB);                 //Differenz zweier Matrizen
        void matrixMult(const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB);                //Produkt zweier Matrizen
        inline CMatrixT<T> matrixInv();

		//*** Rechenoperatoren
        inline CMatrixT<T>& operator= (const CMatrixT<T> &matrixA);                                     //Zuweisungsoperator
        inline CMatrixT<T>& operator= (const T A);
                
        //*** kombinierte Rechenoperatoren
        inline CMatrixT<T>& operator+= (const CMatrixT<T> &matrixA);
        inline CMatrixT<T>& operator+= (const T &value);            
        inline CMatrixT<T>& operator-= (const CMatrixT<T> &matrixA);
        inline CMatrixT<T>& operator-= (const T &value);            
        inline CMatrixT<T>& operator*= (const CMatrixT<T> &matrixA);
        inline CMatrixT<T>& operator*= (const T &value);            
        inline CMatrixT<T>& operator/= (const CMatrixT<T> &matrixA);
        inline CMatrixT<T>& operator/= (const T &value);

        //*** Ausgabe-/Eingabeoperator
        T operator() (const int &r, const int &c)    { return val[r][c]; }
		
		//*** EinAusgabeFunktionen für Matlab-Dateien
        void SaveFile(const char* Fname, const char* Vname);
        void LoadFile(const char* FileName, const bool &AutoResize);
        void LoadFile(const char* FileName, const char* Vname, const bool &AutoResize);

        //*** Public-Attribute
        T **val;                                                                                //MATRIX

        //*** Test-Routinen
        void checkSortM(const int &row);
        void checkSortN(const int &row);
        //friend CMatrixT<T> operator[](const CMatrixT<T> &matrixA);

};


/////////////////////////////////////////////////////////////////////
/// Konstruktor
/////////////////////////////////////////////////////////////////////

template<class T>
inline CMatrixT<T>::CMatrixT(const unsigned int &rows, const unsigned int &cols, const T &initVal)
{
    
    if (rows > 0 && cols > 0)
    {
		m = rows;
	    n = cols;
		register int i;
		val = new (std::nothrow) T* [m];
		val[0] = new (std::nothrow)T[m*n];
		for (i=1; i<m; i++)                                                            //Speicher allokieren
			val[i] = val[0]+i*n;
		for (i=0; i<n; i++)
			val[0][i] = initVal;
		int row_len=n*sizeof(T);
		for (i=1; i<m; i++)
			memcpy(val[i],val[0],row_len);
	}
	    
    return;
}

template<class T>
inline CMatrixT<T>::CMatrixT(const unsigned int &rows, const unsigned int &cols)
{
    
    if (rows > 0 && cols > 0)
    {
		m = rows;
	    n = cols;
	
		val = new (std::nothrow) T* [m];
		val[0] = new (std::nothrow) T[m*n];
		for (register int i=1; i<m; i++)                                                            //Speicher allokieren
			val[i] = val[0]+i*n;
    }
	    
    return;
}

template<class T>
inline CMatrixT<T>::CMatrixT()
{
    m = 0;
    n = 0;
    
    ////////////////////////////////////////
    //val = new T* [m];
    //val[0] = new T[m*n];
    //for (register int i=1; i<m; i++)                                                            //Speicher allokieren
    //    val[i] = val[0]+i*n;
    //for (i=0; i<m; i++)                                                                         //Matrix initialisieren
    //    for (register int j=0; j<n; j++)
    //        val[i][j] = 0;
    //ini = true;*/
    ////////////////////////////////////////
    
    return;
}

template<class T>
inline CMatrixT<T>::CMatrixT(const CMatrixT& matrixA)
{
    m = 0;
    n = 0;
    if (matrixA.m > 0 && matrixA.n > 0)
    {
        register int i;
		if ((matrixA.m != m) || (matrixA.n != n))
        {
            if (m > 0 && n > 0)
            {
                delete val[0];                                                                          //Speicher freigeben
                delete val;
            }
            m = matrixA.m;
            n = matrixA.n;
			val = new (std::nothrow) T* [m];
            val[0] = new (std::nothrow) T[m*n];
            for (i=1; i<m; i++)                                                                     //Speicher neu allokieren
                val[i] = val[0]+i*n;
        }
		memcpy(val[0],(matrixA.val[0]),m*n*sizeof(T));
    }
    else
    {
        if (m > 0 && n > 0)
        {
            delete val[0];                                                                          //Speicher freigeben
            delete val;
        }
        m=0;
        n=0;
    }

    return;
}



/////////////////////////////////////////////////////////////////////
/// Destruktor
/////////////////////////////////////////////////////////////////////

template<class T>
inline CMatrixT<T>::~CMatrixT()
{
    if (m > 0 && n > 0)                                                                       //Matrix loeschen
    {
        delete val[0];
        delete val;
		m=0;
		n=0;
    }
    return;
}


/////////////////////////////////////////////////////////////////////
/// Sonstige Methoden
/////////////////////////////////////////////////////////////////////

template<class T>                                                                               //Matrix loeschen und neu anlegen
inline void CMatrixT<T>::matrixSetsize(const int& r, const int& c)
{
    if (r > 0 && c > 0)
    {
        if ((r != m) || (c != n))
        {
            int i;
            if (m > 0 && n > 0)
            {
                delete val[0];                                                                          //Speicher freigeben
                delete val;
            }
            m = r;
            n = c;
            val = new (std::nothrow) T* [m];
            val[0] = new (std::nothrow) T[m*n];
            for (i=1; i<m; i++)                                                                     //Speicher neu allokieren
                val[i] = val[0]+i*n;
        }
    }
    else
    {
        if (m > 0 && n > 0)
        {
            delete val[0];                                                                          //Speicher freigeben
            delete val;
        }
        m=0;
        n=0;
    }
    return;
}

template<class T>                                                                               //Matrix loeschen und neu anlegen
inline void CMatrixT<T>::matrixSetsize(const int& r, const int& c, T v)
{
    if (r > 0 && c > 0)
    {
        register int i,j;
		if ((r != m) || (c != n))
        {
            if (m > 0 && n > 0)
            {
                delete val[0];                                                                          //Speicher freigeben
                delete val;
            }
            m = r;
            n = c;
            val = new (std::nothrow) T* [m];
            val[0] = new (std::nothrow) T[m*n];
            for (i=1; i<m; i++)                                                                     //Speicher neu allokieren
                val[i] = val[0]+i*n;
        }
        for (j=0; j<n; j++)
			val[0][j] = v;
		int row_len=n*sizeof(T);
		for (i=1; i<m; i++)
			memcpy(val[i],val[0],row_len);
			
    }
    else
    {
        if (m > 0 && n > 0)
        {
            delete val[0];                                                                          //Speicher freigeben
            delete val;
        }
        m=0;
        n=0;
    }

    return;
}

template<class T>                                                                               //Matrix loeschen und neu anlegen
inline void CMatrixT<T>::matrixSetsize(const CMatrixT<T> &matrixA)
{
	if (matrixA.m > 0 && matrixA.n > 0)
    {
        register int i;
		if ((matrixA.m != m) || (matrixA.n != n))
        {
            if (m > 0 && n > 0)
            {
                delete val[0];                                                                          //Speicher freigeben
                delete val;
            }
            m = matrixA.m;
            n = matrixA.n;
			val = new (std::nothrow) T* [m];
            val[0] = new (std::nothrow) T[m*n];
            for (i=1; i<m; i++)                                                                     //Speicher neu allokieren
                val[i] = val[0]+i*n;
        }
		memcpy(val[0],(matrixA.val[0]),m*n*sizeof(T));

    }
    else
    {
        if (m > 0 && n > 0)
        {
            delete val[0];                                                                          //Speicher freigeben
            delete val;
        }
        m=0;
        n=0;
    }

    return;
}

template<class T>                                                                               //Matrix loeschen und neu anlegen
void CMatrixT<T>::matrixResize(const int& r, const int& c)
{
	if (r > 0 && c > 0)
    {
        if (r !=m || c !=n)
		{
			int p,q;
			register int i,j;
			CMatrixT<T> tempMatrix(m,n);
			if (m > 0 && n > 0)
            {
            	memcpy(tempMatrix.val[0],(val[0]),m*n*sizeof(T));
			}
            p=min(r,m);
			q=min(c,n);
			
			matrixSetsize(r,c,T(0.0));
			
			for (i=0; i< p; i++)
				for (j=0; j< q; j++)
					val[i][j] = tempMatrix.val[i][j];
			
		}
    }
    else
    {
        if (m > 0 && n > 0)
        {
            delete val[0];                                                                          //Speicher freigeben
            delete val;
        }
        m=0;
        n=0;
    }
	
	return;
}

template<class T>                                                                               //Matrix loeschen und neu anlegen
inline void CMatrixT<T>::matrixResize(const int& r, const int& c, T v)
{
	if (r > 0 && c > 0)
    {
        if (r !=m || c !=n)
		{
			int p,q;
			register int i,j;
			CMatrixT<T> tempMatrix(m,n);
			if (m > 0 && n > 0)
            {
            	memcpy(tempMatrix.val[0],(val[0]),m*n*sizeof(T));
			}
            p=min(r,m);
			q=min(c,n);
			matrixSetsize(r,c,v);
			for (i=0; i< p; i++)
				for (j=0; j< q; j++)
					val[i][j] = tempMatrix.val[i][j];
			
		}
    }
    else
    {
        if (m > 0 && n > 0)
        {
            delete val[0];                                                                          //Speicher freigeben
            delete val;
        }
        m=0;
        n=0;
    }

    return;
}



template<class T>                                                                               //Ausgabe der Matrix mit Tabulatoren
inline void CMatrixT<T>::matrixDisplay() const
{
    register int i,j;
    #ifdef MATL_MEX
        for (i=0; i<m; i++)
        {
            for (j=0; j<n; j++)
                mexPrintf("%g ",(double)val[i][j]);
            mexPrintf("\n");
        }
        mexPrintf("\n");
    #else
        for (i=0; i<m; i++)
        {
            for (j=0; j<n; j++)
                cout << val[i][j] << " ";
            cout << endl;
        }
        cout << endl;
    #endif
    return;
}

template<class T>                                                                               //Ausgabe der Matrix mit Tabulatoren
inline void CMatrixT<T>::matrixShowSize() const
{
    #ifdef MATL_MEX
		mexPrintf("matrixShowSize: (%i,%i)\n",m,n);
    #else
        cout << "matrixShowSize: (" << m << "," << n << ")" << endl;
    #endif
    return;
}

template<class T>                                                                               //Ausgabe der Matrix mit Tabulatoren
inline void CMatrixT<T>::matrixShowSize(const char *message) const
{
    #ifdef MATL_MEX
        mexPrintf("size(");
        mexPrintf(message);

		mexPrintf(") = [%i,%i]\n",m,n);
    #else
        cout <<"size(" << message << ") = [" << m << "," << n << "]" << endl;
    #endif
    return;
}

template<class T>                                                                               //Initialisieren der kompl. Matrix
inline void CMatrixT<T>::init_byVal(const T &initVal)                                           //mit einem Wert
{
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] = initVal;
    return;
}

template<class T>                                                                               //Initialisieren der kompl. Matrix mit
inline void CMatrixT<T>::init_byRandom(const T &randMax)                                        //Pseudo-Zufalls Werten im jeweiligen Typ
{
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
        {
            val[i][j] = T(rand())/T(RAND_MAX)*randMax;
        }
    return;
}

template<class T>
inline void CMatrixT<T>::unit(const int& row)                                                   //Matrix zur Einheitsmatrix umformen
{
    if ((m != row) || (n != row))
        matrixSetsize(row,row);

    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<m; j++)
            val[i][j] = i == j ? (T)(1) : (T)(0);
    return;
}

template<class T>
inline void CMatrixT<T>::unit()
{
    if (m != n)
        _matrix_error("CMatrixT::UNIT(): Unit of a non-square Matrix!");

    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] = i == j ? (T)(1) : (T)(0);
    return;
}

template<class T>                                                                               //Teil einer Matrix kopieren
CMatrixT<T> CMatrixT<T>::copyp(const int& row1, const int& row2, const int& col1, const int& col2)
{
    if ((row1 >= m) || (row2 >= m) || (col1 >= n) || (col2 >= n))
        _matrix_error("CMatrixT::Part: Index out of range !");

    int rowtemp = row2 - row1 + 1;
    int coltemp = col2 - col1 + 1;
    CMatrixT<T> tempMat(rowtemp, coltemp,0);
    register int i,j;
    for (i=0; i<rowtemp; i++)
        for (j=0; j<coltemp; j++)
            tempMat.val[i][j] = val[row1+i][col1+j];
    return tempMat;
}

template<class T>
inline void CMatrixT<T>::nul(const int &row, const int &col)
{
    if ((m != row) || (n != col))
        matrixSetsize(row,col);
    init_byVal(0);
    return;
}

template<class T>
inline void CMatrixT<T>::nul()
{
    init_byVal(0);
    return;
}

template<class T>
void CMatrixT<T>::diag(const CMatrixT<T> &matrixA)
{
    int i,j;
    if ((matrixA.n == m)&&(matrixA.m == 1))
    {
        for (i=0; i<m; i++)
            for (j=0; j<n; j++)
                val[i][j] = i == j ? matrixA.val[0][i] : (T)(0);
    }
    else if ((matrixA.n == 1)&&(matrixA.m == m))
    {
        for (i=0; i<m; i++)
            for (j=0; j<n; j++)
                val[i][j] = i == j ? matrixA.val[i][0] : (T)(0);
    }
    else
    {
        _matrix_error("CMatrixT::DIAG(): vector required size (n,1)");
    }
    
    return;
}

template<class T>
inline int CMatrixT<T>::pivot(const int &row)
{
    int k = row;
    double temp;                        //muessen Werte double sein ???
    double amax = -1.0;
    register int i;
    for (i=row; i<m; i++)
        if (((temp = fabs(double(val[i][row]))) > amax) && (temp != T(0)))
        {
            amax = temp;
            k = i;
        }
    if (val[k][row] == T(0))
        return -1;
    if (k != row)
    {
        swapm(k,row);
        return k;
    }
    return 0;
}

template<class T>
inline double CMatrixT<T>::norm()                                                               //Norm der Matrix
{
    double retval = double(0);
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
                retval += val[i][j] * val[i][j];
        retval = sqrt(retval);
    return retval;
}

template<class T>
inline bool CMatrixT<T>::isDiagonal()                                                           //Diagonale Matrix ?
{                                                                                               //  2   0   0   0
    if (m != n)                                                                                 //  0   5   0   0
        return false;                                                                           //  0   0   -2  0
    register int i,j;                                                                           //  0   0   0   6
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            if ((i != j) && (val[i][j] != double(0)))
                return false;
    return true;
}

template<class T>
inline bool CMatrixT<T>::isScalar()                                                             //Skalare Matrix ?
{                                                                                               //  3   0   0   0
    if(!isDiagonal())                                                                           //  0   3   0   0
        return false;                                                                           //  0   0   3   0
    T v = val[0][0];                                                                            //  0   0   0   3
    register int i;
    for (i=1; i<m; i++)
        if (val[i][i] != v)
            return false;
    return true;
}

template<class T>
inline bool CMatrixT<T>::isUnit()                                                               //Einheitsmatrix ?
{                                                                                               //  1   0   0   0
    if (isScalar() && (val[0][0] == T(1)))                                                      //  0   1   0   0
        return true;                                                                            //  0   0   1   0
    return false;                                                                               //  0   0   0   1
}

template<class T>
inline bool CMatrixT<T>::isNull()                                                               //Nullmatrix ?
{                                                                                               //  0   0   0   0
    register int i,j;                                                                           //  0   0   0   0
    for (i=0; i<m; i++)                                                                         //  0   0   0   0
        for (j=0; j<n; j++)                                                                     //  0   0   0   0
            if (val[i][j] != T(0))
                return false;
    return true;
}

template<class T>
inline bool CMatrixT<T>::isSymmetric()                                                          //Symmetrische Matrix ?
{                                                                                               //  1   2   3   4
    if (m != n)                                                                                 //  2   2   0   0
        return false;                                                                           //  3   0   3   0
    register int i,j;                                                                           //  4   0   0   4
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            if (val[i][j] != val[j][i])
                return false;
    return true;
}

template<class T>
inline bool CMatrixT<T>::isSkewSymmetric()                                                      //Negierte Symmetrische Matrix ?
{                                                                                               //  1   -2  -3  -4
    if (m != n)                                                                                 //  2   2   -3  -2
        return false;                                                                           //  3   3   3   -3
    register int i,j;                                                                           //  4   2   3   4
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            if ((i != j) && (val[i][j] != -val[j][i]))
                return false;
    return true;
}

template<class T>
inline bool CMatrixT<T>::isUpperTriangular()                                                    //Obere Dreiecksmatrix ?
{                                                                                               //  3   2   3   4
    if (m != n)                                                                                 //  0   2   1   5
        return false;                                                                           //  0   0   3   7
    register int i,j;                                                                           //  0   0   0   6
    for (i=1; i<m; i++)
        for (j=0; j<i; j++)
            if (val[i][j] != T(0))
                return false;
    return true;
}

template<class T>
inline bool CMatrixT<T>::isLowerTriangular()                                                    //Untere Dreiecksmatrix ?
{                                                                                               //  3   0   0   0
    if (m != n)                                                                                 //  2   4   0   0
        return false;                                                                           //  5   3   2   0
    register int i,j;                                                                           //  7   6   3   4
    for (j=1; j<n; j++)
        for (i=0; i<j; i++)
            if (val[i][j] != T(0))
                return false;
    return true;
}

template<class T>
inline T CMatrixT<T>::det()                                                                     //Determinante der Matrix
{
    register int i,j,k;
    double piv;
    T detVal = T(1);

    if (m != n)
        _matrix_error("CMatrixT::DET(): Determinant a non-square matrix!");

    CMatrixT<T> tempMat(m,n,0); //= *this;
    tempMat(m,n,0) = *this;
    //tempMat = *this;

    for (k=0; k<m; k++)
    {
        int indx = tempMat.pivot(k);
        if (indx == -1)
            return 0;
        if (indx != 0)
        //vorher: detVal = - detVal;
        detVal *= -1;

        //vorher: detVal = detVal * tempMat.val[k][k];
        detVal *= tempMat.val[k][k];

        for (i=k+1; i<m; i++)
        {
            piv = tempMat.val[i][k] / tempMat.val[k][k];
            for (j=k+1; j<m; j++)
                tempMat.val[i][j] -= piv * tempMat.val[k][j];
        }
    }
    return detVal;
}

template<class T>
inline CMatrixT<T> CMatrixT<T>::matrixInv()                                       //Inverse der Matrix
{
    if (m != n)
        _matrix_error("CMatrixT::Operator!: Inversion of a non-square matrix!");

    register int i,j,k,l;
    T a1,a2;
    CMatrixT<T> tempMatA(m, n,0);
    tempMatA.unit();
    for (k=0; k<m; k++)
    {
        int indx = pivot(k);
        if (indx == -1)
            _matrix_error("CMatrixT::Operator!: Inversion of a singular matrix!");
        if (indx != 0)
        {
            tempMatA.swapm(k,indx);
        }
        a1 = val[k][k];
        for (j=0; j<m; j++)
        {
            val[k][j] /= a1;
            tempMatA.val[k][j] /= a1;
        }
        for (i=0; i<m; i++)
            if (i != k)
            {
                a2 = val[i][k];
                for (l=0; l<m; l++)
                {
                    val[i][l] -= a2 * val[k][l];
                    tempMatA.val[i][l] -= a2 * tempMatA.val[k][l];
                }
            }
    }
	*this=tempMatA;
    return tempMatA;
}

template<class T>
inline void CMatrixT<T>::copyc(const CMatrixT<T> &s, const int &cs, const int &cd)              //kopiert eine Spalte
{
    if ((cs >= s.n) || (cs < 0) || (cd < 0) || (cd >= n))
        _matrix_error("CMatrixT::Copyc:Invalid parameters");

    if (s.m == m)
    {
        register int i;
        for (i=0; i<m; i++)
            val[i][cd]=s.val[i][cs];
        return;
    }
    else
        _matrix_error("CMatrixT::Copyc:Size of columns is different");
}

template<class T>
inline void CMatrixT<T>::copyr(const CMatrixT<T> &s, const int &rs, const int &rd)              //kopiert eine Zeile
{
    if ((rs >= s.m) || (rs < 0) || (rd < 0) || (rd >= m))
        _matrix_error("CMatrixT::Copyr:Invalid parameters");

    if (s.n == n)
    {
        register int i;
        for (i=0; i<n; i++)
            val[rd][i]=s.val[rs][i];
        return;
    }
    else
        _matrix_error("CMatrixT::Copyr:Size of Rows is different");
}

template<class T>
bool CMatrixT<T>::compc(const CMatrixT<T> &s, const int &cs, const int &cd)                     //vergleicht Spalten
{
    if ((cs >= s.n) || (cs < 0) || (cd < 0) || (cd >= n))
        _matrix_error("CMatrixT::Compc:Invalid parameters");

    if (s.m == m)
    {
        register int i;
        for (i=0; i<m; i++)
        {
            if (val[i][cd]!=s.val[i][cs])
                return false;
        }
        return true;
    }
    else
        _matrix_error("CMatrixT::Compc:Size of columns is different");
    return false;
}

template<class T>
bool CMatrixT<T>::compc(const int &c1, const int &c2)                                           //vergleicht Spalten innerhalb der Matrix
{
    if ((c1 >= n) || (c1 < 0) || (c2 < 0)  || (c2 >= n))
        _matrix_error("CMatrixT::Compc:Invalid parameters");

    register int i;
    for (i=0; i<m; i++)
    {
        if (val[i][c1]!=val[i][c2])
            return false;
    }
    return true;
}

template<class T>
bool CMatrixT<T>::compr(const CMatrixT<T> &s, const int &rs, const int &rd)                     //vergleicht Zeilen
{
    if ((rs >= s.m) || (rs < 0) || (rd < 0) || (rd >= m))
        _matrix_error("CMatrixT::Compr:Invalid parameters");

    if (s.n == n)
    {
        register int i;
        for (i=0; i<n; i++)
        {
            if (val[rd][i]!=s.val[rs][i])
                return false;
        }
        return true;
    }
    else
        _matrix_error("CMatrixT::Compr:Size of rows is different");
    return false;
}

template<class T>
bool CMatrixT<T>::compr(const int &c1, const int &c2)                                           //vergleicht Zeilen innerhalb der Matrix
{
    if ((c1 >= m) || (c1 < 0) || (c2 < 0) || (c2 >= m) )
        _matrix_error("CMatrixT::Compr:Invalid parameters");

    register int i;
    for (i=0; i<n; i++)
    {
        if (val[c1][i]!=val[c2][i])
            return false;
    }
    return true;
}

template<class T>
CMatrixT<int> CMatrixT<T>::findn(const int &n_n,const T v)
{
    int i,k;
    k=0;
    CMatrixT<int> ind;
    if (n_n<n)
    {
        for (i=0;i<m;i++)
            if (val[i][n_n]==v) k++;
        
        if (k>0)
        {
            ind.matrixSetsize(k,1);
            k=0;
            for (i=0;i<m;i++)
                if(val[i][n_n]==v)
                {
                    ind.val[k][0]=i;
                    k++;
                }
        }
    }
    return ind;
}

template<class T>
bool CMatrixT<T>::is_element_n(const int &n_n,const T v)
{
    int i;
    if (n_n<n)
    {
        for (i=0;i<m;i++)
            if (val[i][n_n]==v) return true;
    }
    return false;
}

template<class T>
CMatrixT<int> CMatrixT<T>::findm(const int &n_m,const T v)
{
    int j,k;
    k=0;
    CMatrixT<int> ind;
    if (n_m<m)
    {
        for (j=0;j<n;j++)
            if (val[n_m][j]==v) k++;
        
        if (k>0)
        {
            ind.matrixSetsize(k,1);
            k=0;
            for (j=0;j<n;j++)
                if(val[n_m][j]==v)
                {
                    ind.val[k][0]=j;
                    k++;
                }
        }
    }
    return ind;
}

template<class T>
bool CMatrixT<T>::is_element_m(const int &n_m,const T v)
{
    int j;
    if (n_m<m)
    {
        for (j=0;j<n;j++)
            if (val[n_m][j]==v) return true;
    }
    return false;
}

template<class T>
void CMatrixT<T>::flipud()
{
    int j,top,bot;
    T temp;
    
    if(m>1)
        for(j=0;j<n;j++)
        {
            top=0;
            bot=m-1;
            while(top<bot)
            {
                temp=val[top][j];
                val[top][j]=val[bot][j];
                val[bot][j]=temp;
                top++;bot--;
            }
        }
}

template<class T>
void CMatrixT<T>::fliplr()
{
    int i,lef,rig;
    T temp;
    
    if(n>1)
        for(i=0;i<m;i++)
        {
            lef=0;
            rig=n-1;
            while(lef<rig)
            {
                temp=val[i][lef];
                val[i][lef]=val[i][rig];
                val[i][rig]=temp;
                lef++;rig--;
            }
        }
}


template<class T>
void CMatrixT<T>::maxind_m(const CMatrixT<double> &inp)
{
    register int i,j;
    int index, maxcount;
    
	matrixSetsize(1,inp.matrixCols(),0);

    for (i=0; i<inp.matrixCols(); i++)
    {
        maxcount = 0;
        index = 0;
        for (j=1; j<inp.matrixRows(); j++)
        {
            if (inp.val[j][i] > inp.val[index][i])
            {
                maxcount = 1;
                index = j;
                val[0][i] = (T)index;
            }
            else
            {
                if (inp.val[j][i]==inp.val[index][i])
                    maxcount = 2;
            }
        }
        
        //if (maxcount == 2)
        //  _matrix_error("CMatrixT::Maxind_m:More than one Max-Value found");
    }
}

template<class T>
void CMatrixT<T>::maxind_m(const CMatrixT<int> &inp)
{
    register int i,j;
    int index, maxcount;
    
	matrixSetsize(1,inp.matrixCols(),0);

    for (i=0; i<inp.matrixCols(); i++)
    {
        maxcount = 0;
        index = 0;
        for (j=1; j<inp.matrixRows(); j++)
        {
            if (inp.val[j][i] > inp.val[index][i])
            {
                maxcount = 1;
                index = j;
                val[0][i] = (T)index;
            }
            else
            {
                if (inp.val[j][i]==inp.val[index][i])
                    maxcount = 2;
            }
        }
        
        //if (maxcount == 2)
        //  _matrix_error("CMatrixT::Maxind_m:More than one Max-Value found");
    }
}

template<class T>
void CMatrixT<T>::maxind_n(const CMatrixT<double> &inp)
{
    register int i,j;
    int index, maxcount;
    matrixSetsize(inp.matrixRows(),1,0);
	for (i=0; i<inp.matrixRows(); i++)
    {
        maxcount = 0;
        index = 0;
        for (j=1; j<inp.matrixCols(); j++)
        {
            if (inp.val[i][j] > inp.val[i][index])
            {
                maxcount = 1;
                index = j;
                val[i][0]=(T)index;
            }
            else
            {
                if (inp.val[i][j] == inp.val[i][index])
                    maxcount = 2;
            }
        }
        //if (maxcount == 2)
        //  _matrix_error("CMatrixT::Maxind_n:More than one Max-Value found");
    }
}

template<class T>
void CMatrixT<T>::maxind_n(const CMatrixT<int> &inp)
{
    register int i,j;
    int index, maxcount;
    matrixSetsize(inp.matrixRows(),1,0);
	for (i=0; i<inp.matrixRows(); i++)
    {
        maxcount = 0;
        index = 0;
        for (j=1; j<inp.matrixCols(); j++)
        {
            if (inp.val[i][j] > inp.val[i][index])
            {
                maxcount = 1;
                index = j;
                val[i][0]=(T)index;
            }
            else
            {
                if (inp.val[i][j] == inp.val[i][index])
                    maxcount = 2;
            }
        }
        //if (maxcount == 2)
        //  _matrix_error("CMatrixT::Maxind_n:More than one Max-Value found");
    }
}
        


template<class T>
CMatrixT<int> CMatrixT<T>::maxind_m()
{
    register int i,j;
    int index, maxcount;
    CMatrixT<int> maxindM(1,n,0);
    for (i=0; i<n; i++)
    {
        maxcount = 0;
        index = 0;
        for (j=1; j<m; j++)
        {
            if (val[j][i] > val[index][i])
            {
                maxcount = 1;
                index = j;
                maxindM.val[0][i] = index;
            }
            else
            {
                if (val[j][i]==val[index][i])
                    maxcount = 2;
            }
        }
        
        //if (maxcount == 2)
        //  _matrix_error("CMatrixT::Maxind_m:More than one Max-Value found");
    }
    return maxindM;
}

template<class T>
inline CMatrixT<int> CMatrixT<T>::maxind_n()
{
    register int i,j;
    int index, maxcount;
    CMatrixT<int> maxindN(m,1,0);
    for (i=0; i<m; i++)
    {
        maxcount = 0;
        index = 0;
        for (j=1; j<n; j++)
        {
            if (val[i][j] > val[i][index])
            {
                maxcount = 1;
                index = j;
                maxindN.val[i][0]=index;
            }
            else
            {
                if (val[i][j] == val[i][index])
                    maxcount = 2;
            }
        }
        //if (maxcount == 2)
        //  _matrix_error("CMatrixT::Maxind_n:More than one Max-Value found");
    }
    return maxindN;
}



/////////////////////////////////////////////////////////////////////
/// Mathematische Operationen
/////////////////////////////////////////////////////////////////////

template<class T>
T CMatrixT<T>::matrixSum() const                                                                //Berechnung der Summer aller Einzelwerte
{
    T summe = T(0);
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            summe += val[i][j];
    return summe;
}

template<class T>
T CMatrixT<T>::matrixGetMean() const                                                                //Berechnung der Summer aller Einzelwerte
{
    T summe = T(0);
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            summe += val[i][j];
    if(m>0&&n>0)
        summe/=double(m*n);
    return summe;
}

template<class T>
inline T CMatrixT<T>::matrixGetMax() const                                                      //Berechnung des maximalen Einzelwertes
{
    T maximum = val[0][0];
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            if (val[i][j] > maximum)
                maximum = val[i][j];
    return maximum;
}

template<class T>
inline T CMatrixT<T>::matrixGetMin() const                                                      //Berechnung des minimalen Einzelwertes
{
    T minimum = val[0][0];
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            if (val[i][j] < minimum)
                minimum = val[i][j];
    return minimum;
}

template<class T>
inline T CMatrixT<T>::matrixGetMax(CMatrixT<int> &max_ind) const                                                      //Berechnung des maximalen Einzelwertes
{
    max_ind.matrixSetsize(1,2,0);
	T maximum = val[0][0];
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            if (val[i][j] > maximum)
			{
                maximum = val[i][j];
				max_ind.val[0][0]=i;
				max_ind.val[0][1]=j;
			}

    return maximum;
}

template<class T>
inline T CMatrixT<T>::matrixGetMin(CMatrixT<int> &min_ind) const                                                      //Berechnung des minimalen Einzelwertes
{
    min_ind.matrixSetsize(1,2,0);
	T minimum = val[0][0];
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            if (val[i][j] < minimum)
			{
                minimum = val[i][j];
				min_ind.val[0][0]=i;
				min_ind.val[0][1]=j;
			}
    return minimum;
}

template<class T>                                                                               //Vertauschen von Spalten (Vektoren)
inline void CMatrixT<T>::swapn(const int &p1, const int &p2)
{
    if (p1 != p2)
    {
        if ((p1 >= 0) && (p2 >= 0) && (p1 < n) && (p2 < n))
        {
            register int i;
            T temp;
            for(i=0; i<m; i++)
            {
                temp = val[i][p1];
                val[i][p1] = val[i][p2];
                val[i][p2] = temp;
            }
        }
        else
            _matrix_error("CMatrixT::SWAPN: Index out of range !");
    }
    return;
}

template<class T>                                                                               //Vertauschen von Zeilen
inline void CMatrixT<T>::swapm(const int &p1, const int &p2)
{
    if (p1 != p2)
    {
        if ((p1 >= 0) && (p2 >= 0) && (p1 < m) && (p2 < m))
        {
            register int i;
            T temp;
            for(i=0; i<n; i++)
            {
                temp = val[p1][i];
                val[p1][i] = val[p2][i];
                val[p2][i] = temp;
            }
        }
        else
            _matrix_error("CMatrixT::SWAPM: Index out of range !");
    }
    return;
}


template<class T>
inline void CMatrixT<T>::sortm(const int &r, const int &left, const int &right)
{

    if (left < right)       //(left <= right) auskommentiert um gleiche Sortierung zu gewähleisten
    {
        T temp = val[r][right];
        register int i = left-1, j = right;

        while (i < j)       //(i <= j) auskommentiert um gleiche Sortierung zu gewähleisten
        {
            while((val[r][++i]<temp) && (i<=right)) ;
            while((val[r][--j]>temp) && (j>left)) ;   //(j>=left) auskommentiert um gleiche Sortierung zu gewähleisten
            if (i<j) swapn(i,j);
        }
        swapn(right,i);
        sortm(r, left, i-1);
        sortm(r, i+1, right);
    }
    return;
}

template<class T>                                                                               //In der alten Version nicht implementiert
inline void CMatrixT<T>::sortn(const int &c, const int &upper, const int &lower)
{
    if (upper < lower)
    {
        T temp = val[lower][c];
        register int i = upper-1, j = lower;
        while (i < j)
        {
            while((val[++i][c]<temp) && (i<=lower)) ;
            while((val[--j][c]>temp)&& (j>upper)) ;
            if (i<j) swapm(i,j);
        }
        swapm(lower,i);
        sortn(c, upper, i-1);
        sortn(c, i+1, lower);
    }
    return;
}

template<class T>
void CMatrixT<T>::scale_m(const T scale)
{
    int i,j;
    T sum_m;
    for(i=0; i<m; i++)
    {
        sum_m = (T)(0.0);
        for(j=0; j<n; j++)
            sum_m+=val[i][j];
        for(j=0; j<n; j++)
            val[i][j]=val[i][j]/sum_m*scale;

    }
}

template<class T>
void CMatrixT<T>::scale_n(const T scale)
{
    int i,j;
    T sum_n;
    for(j=0; j<n; j++)
    {
        sum_n = (T)(0.0);
        for(i=0; i<m; i++)
            sum_n+=val[i][j];
        for(i=0; i<m; i++)
            val[i][j]=val[i][j]/sum_n*scale;

    }
}

template<class T>
void CMatrixT<T>::s_pow(const T exp)
{
    int i,j;
    for(i=0; i<m; i++)
        for(j=0; j<n; j++)
            val[i][j]=(T)(pow(double(val[i][j]),double(exp)));
}

template<class T>
void CMatrixT<T>::s_inv()
{
    int i,j;
    for(i=0; i<m; i++)
        for(j=0; j<n; j++)
            val[i][j]=(T)(1.0/double(val[i][j]));
}

template<class T>
void CMatrixT<T>::s_mult(const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)
{
    if ((matrixA.m != matrixB.m) || (matrixA.n != matrixB.n))
        _matrix_error("CMatrixT::s_mult: Different Matrices !");
    matrixSetsize(matrixA.m,matrixA.n);
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j]=matrixA.val[i][j] * matrixB.val[i][j];
    return;


}

template<class T>
void CMatrixT<T>::matrixTransp()
{                                                                                               //Berechnung der Transponierten
    CMatrixT<T> tempMat(n,m,0);                                                                 //einer Matrix
    register int i,j;
    for (i=0; i<tempMat.m; i++)
            for (j=0; j<tempMat.n; j++)
                    tempMat.val[i][j] = val[j][i];
    *this = tempMat;
    return;
}

template<class T>
void CMatrixT<T>::matrixTransp(const int diag)
{                                                                                               //Berechnung der Transponierten
    CMatrixT<T> tempMat(n,m,0);                                                                 //einer Matrix
    register int i,j;
    if (diag)
    {
        for (i=0; i<tempMat.m; i++)
                for (j=0; j<tempMat.n; j++)
                        tempMat.val[i][j] = val[j][i];
        *this = tempMat;
    }
    else
    {
        for (i=0; i<tempMat.m; i++)
                for (j=0; j<tempMat.n; j++)
                        tempMat.val[i][j] = val[n-j-1][m-i-1];
        *this = tempMat;
    }
    return;
}

template<class T>
void CMatrixT<T>::matrixAdd(const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)             //Addition von 2 Matrizen
{                                                                                               //Ergebnis: Aufrufende Matrix
    if ((matrixA.m != matrixB.m) || (matrixA.n != matrixB.n))
        _matrix_error("CMatrixT::MATRIXADD: Different Matrices !");

    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j]=matrixA.val[i][j] + matrixB.val[i][j];
    return;
}

template<class T>
void CMatrixT<T>::matrixSub(const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)             //Subtraktion von 2 Matrizen
{                                                                                               //Ergebnis: Aufrufende Matrix
    if ((matrixA.m != matrixB.m) || (matrixA.n != matrixB.n))
        _matrix_error("CMatrixT::MATRIXSUB: Different Matrices !");

    register int i,j;
    for (i=0; i<m; i++)
            for (j=0; j<n; j++)
                val[i][j]=matrixA.val[i][j] - matrixB.val[i][j];
    return;
}

template<class T>
void CMatrixT<T>::matrixMult(const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)            //Multiplikation von 2 Matrizen
{                                                                                               //Ergebnis: Aufrufende Matrix
    if (matrixA.n != matrixB.m)
        _matrix_error("CMatrixT::MATRIXMULT: Matrices could not be multiplied !");

    T tempVal;
    register int i,j,k;
    for (i=0; i<matrixB.n; i++)
        for (j=0; j<matrixA.m; j++)
        {
            tempVal = (T)(0);
            for (k=0; k<matrixA.n; k++)
                tempVal += matrixA.val[j][k] * matrixB.val[k][i];
            val[j][i] = tempVal;
        }
    return;
}

template<class T>
void CMatrixT<T>::matrixNeg()                                                                   //Negation einer Matrix
{                                                                                               //Jedes Element wird mit -1 multipliziert
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] *= (T)(-1);
    return;
}

template<class T>
void CMatrixT<T>::matrixSkalarProdukt(const T& Faktor)                                          //Multiplikation: Matrix x Skalar
{                                                                                               //Ergebnis: Aufrufende Matrix
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] *= Faktor;
    return;
}

template<class T>
inline void CMatrixT<T>::findred(const CMatrixT<T> &s)
{
    register int i,j,k=0;
    CMatrixT<T> s_help = s;
    
    if (m!=1 || n!=s.n)
        matrixSetsize(1,s.n);
    else
        init_byVal(0);

    for (i=0;i<s.n;i++)
    {
        for (j=0;j<s.n;j++)
        {
            if (i<j)
                if (s_help.compc(i,j))
                {
                    if (val[0][i]==0 && val[0][j]==0)
                    {
                        k++;
                        val[0][j]=k;
                        val[0][i]=k;
                    }
                    if (val[0][j]==0)
                        val[0][j]=val[0][i];
                }
        }
    }
    return;
}

template<class T>
inline CMatrixT<T> CMatrixT<T>::column(const int &cnr)
{
    CMatrixT<T> tempMat(m,1,0);
    register int i;
    for (i=0; i<m; i++)
        tempMat.val[i][0] = val[i][cnr];
    return tempMat;
}

template<class T>
inline CMatrixT<T> CMatrixT<T>::row(const int &rnr)
{
    CMatrixT<T> tempMat(1,n,0);
    register int i;
    for (i=0; i<n; i++)
        tempMat.val[0][i] = val[rnr][i];
    return tempMat;
}

/////////////////////////////////////////////////////////////////////
/// Mathematische Operatoren +, -, *, =
/////////////////////////////////////////////////////////////////////

template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator+= (const CMatrixT<T> &matrixA)
{
	if ((matrixA.m != m) || (matrixA.n != n))
        _matrix_error("CMatrixT::Operator + : Different Matrices !");

    
	register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] = val[i][j] + matrixA.val[i][j];
    return *this;

}

template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator+= (const T &value)
{
	register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] = val[i][j] + value;
    return *this;
}

template<class T>
inline CMatrixT<T> operator+ (const CMatrixT<T> &matrixA, const T &value)                       //Binärer Additionsoperator
{
    CMatrixT<T> tempMat(matrixA);
	tempMat+=value;	
    return tempMat;
}

template<class T>
inline CMatrixT<T> operator+ (const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)           //Binärer Additionsoperator
{
    CMatrixT<T> tempMat(matrixA);
	tempMat+=matrixB;	
    return tempMat;
}

template<class T>
inline CMatrixT<T> operator+ (const T &value, const CMatrixT<T> &matrixA)                       //Binärer Additionsoperator
{
    CMatrixT<T> tempMat(matrixA);
	tempMat+=value;	
    return tempMat;
}


template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator-= (const CMatrixT<T> &matrixA)
{
	if ((matrixA.m != m) || (matrixA.n != n))
        _matrix_error("CMatrixT::Operator + : Different Matrices !");

    
	register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] = val[i][j] - matrixA.val[i][j];
    return *this;

}

template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator-= (const T &value)
{
	register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] = val[i][j] - value;
    return *this;
}

template<class T>
inline CMatrixT<T> operator- (const CMatrixT<T> &matrixA, const T &value)                       //Binärer Additionsoperator
{
    CMatrixT<T> tempMat(matrixA);
	tempMat-=value;	
    return tempMat;
}

template<class T>
inline CMatrixT<T> operator- (const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)           //Binärer Additionsoperator
{
    CMatrixT<T> tempMat(matrixA);
	tempMat-=matrixB;	
    return tempMat;
}

template<class T>
inline CMatrixT<T> operator- (const T &value, const CMatrixT<T> &matrixA)                       //Binärer Additionsoperator
{
    CMatrixT<T> tempMat(matrixA);
	tempMat*=(T)(1);
	tempMat+=value;	
    return tempMat;
}



template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator*= (const CMatrixT<T> &matrixA)
{
	if (n != matrixA.m)
        _matrix_error("CMatrixT::Operator * : Matrices could not be multiplied !");
	
    CMatrixT<T> tempMat(m,matrixA.n);
    T tempVal;
    register int i,j,k;
    for (i=0; i<matrixA.n; i++)
        for (j=0; j<m; j++)
        {
            tempVal = (T)(0);
            for (k=0; k<n; k++)
                tempVal += val[j][k] * matrixA.val[k][i];
            tempMat.val[j][i] = tempVal;
        }
    *this = tempMat;
    return *this;
}

template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator*= (const T &value)
{
	register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] = val[i][j] * value;
    return *this;
}


template<class T>
inline CMatrixT<T> operator* (const CMatrixT<T> &matrixA, const T &value)                       //Binärer Multiplikationsoperator
{                                                                                               //Produkt: Matrix x Skalar
    CMatrixT<T> tempMat(matrixA);
	tempMat*=value;	
    return tempMat;
}

template<class T>
inline CMatrixT<T> operator* (const T &value, const CMatrixT<T> &matrixA)                       //Binärer Multiplikationsoperator
{                                                                                               //Produkt: Skalar x Matrix
    CMatrixT<T> tempMat(matrixA);
	tempMat*=value;	
    return tempMat;
}

template<class T>
inline CMatrixT<T> operator* (const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)           //Binärer Multiplikationsoperator
{                                                                                              //Produkt: Matrix x Matrix
	CMatrixT<T> tempMat(matrixA);
	tempMat*=matrixB;	
    return tempMat;   
}

template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator/= (const CMatrixT<T> &matrixA)
{
	*this *=(!matrixA);
    return *this;
}

template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator/= (const T &value)
{
	register int i,j;
	if(value!=((T)0))
	{
		for (i=0; i<m; i++)
			for (j=0; j<n; j++)
				val[i][j] = val[i][j]/value;
	}
	else
	{
		for (i=0; i<m; i++)
			for (j=0; j<n; j++)
				val[i][j] = NaN;
	}

	return *this;
}

template<class T>
inline CMatrixT<T> operator/ (const CMatrixT<T> &matrixA, const T &value)                       //Binärer Divisionsoperator
{                                                                                               //Produkt: Matrix / Skalar
    if(value!=((T)0))
		return (matrixA * (T(1)/value));
	else 
	{
		CMatrixT<T> tempMat(matrixA);
		tempMat/=(T)0;
		return tempMat;
		//return (matrixA * NaN);
	}
}

template<class T>
inline CMatrixT<T> operator/ (const T &value, const CMatrixT<T> &matrixA)
{
    return (!matrixA) * value;
}

template<class T>
inline CMatrixT<T> operator/ (const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)
{
    return (matrixA * (!matrixB));
}

template<class T>
inline CMatrixT<T> operator! (const CMatrixT<T> &matrixA) 
{
    CMatrixT<T> tempMat(matrixA);
	return tempMat.matrixInv();	
}


template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator= (const CMatrixT<T> &matrixA)         //Zuweisungsoperator
{
    matrixSetsize(matrixA);
    return *this;
}

template<class T>
inline CMatrixT<T>& CMatrixT<T>::operator= (const T A)                          //Zuweisungsoperator
{
    register int i,j;
    for (i=0; i<m; i++)
        for (j=0; j<n; j++)
            val[i][j] = A;
    return *this;
}

template <class T>
inline ostream& operator<< (std::ostream& ostrm, const CMatrixT<T> &matrixA)                         //Links-Shift Operator
{                                                                                               //Ausgabe der Matrix mit cout
    register int i,j;
    ostrm << matrixA.matrixRows() << " ";
    ostrm << matrixA.matrixCols() << endl;
    for (i=0; i<matrixA.matrixRows(); i++)
    {
        for (j=0; j<matrixA.matrixCols()-1; j++)
            ostrm << matrixA.val[i][j] << " ";
		ostrm << matrixA.val[i][matrixA.matrixCols()-1] << endl;
    }
    return ostrm;
}

template <class T>
inline istream& operator>> (std::istream& istrm, CMatrixT<T> &matrixA)                               //Rechts-Shift Operator
{
    register int i,j,_m,_n;
    double temp_inp;
    istrm >> _m;
    istrm >> _n;
    matrixA.matrixSetsize(_m,_n);
    for (i=0; i<_m; i++)
        for (j=0; j<_n; j++)
        {
            istrm >> temp_inp;
            matrixA.val[i][j]=(T)temp_inp;
        }
    return istrm;
}

template <class T>
inline bool operator== (const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)                 //Binärer Vergleichsoperator
{                                                                                               //Logischer Gleichheit der Matrizen
    if ((matrixA.m != matrixB.m) || (matrixA.n != matrixB.n))
        return false;
    register int i,j;
    for (i=0; i<matrixA.m; i++)
        for (j=0; j<matrixA.n; j++)
            if (matrixA.val[i][j] != matrixB.val[i][j])
                return false;
    return true;
}

template <class T>
inline bool operator!= (const CMatrixT<T> &matrixA, const CMatrixT<T> &matrixB)                 //Binärer Vergleichsoperator
{                                                                                               //Logischer Ungleichheit der Matrizen
    return (matrixA == matrixB) ? false : true;
}

template <class T>
inline CMatrixT<T> operator~ (const CMatrixT<T> &matrixA)                                       //Unärer Transponierungsoperator
{                                                                                               //Matrix wird transponiert
    CMatrixT<T> tempMat(matrixA.n,matrixA.m,0);
    register int i,j;
    for (i=0; i<matrixA.m; i++)
        for (j=0; j<matrixA.n; j++)
            tempMat.val[j][i] = matrixA.val[i][j];
    return tempMat;
}

template <class T>
inline CMatrixT<T> operator^ (const CMatrixT<T> &matrixA, const int &pow)                       //Binärer Exponentialoperator
{
    if (matrixA.m != matrixA.n)
        _matrix_error("CMatrixT::Operator ^ : Matrix must be square !");

    //CMatrixT<T> tempMat() = matrixA;
    CMatrixT<T> tempMat(matrixA);
    register int i;
    for (i=2; i<= pow; i++)
        tempMat *= tempMat;
    return tempMat;
}


/////////////////////////////////////////////////////////////////////
/// Ein-/Ausgabe Methoden für Matlab-Dateien
/////////////////////////////////////////////////////////////////////

template<class T>
void CMatrixT<T>::SaveFile(const char* FileName, const char* VarName)
{
    struct CMatfilehead {long type; long m; long n; long im; long vnlen; };
    FILE *m_file;
    bool m_open = false;
    register int i, j;
    double wert;
    CMatfilehead header;
    header.type = 0;
    header.m = m;
    header.n = n;
    header.im = 0;
    header.vnlen = strlen(VarName) + 1;
    m_file = NULL;
    m_open = ((m_file = fopen(FileName, "wb")) != NULL );
    if (m_open == true)
    {
        fwrite(&header, sizeof(CMatfilehead), (size_t) 1, m_file);
        fwrite(VarName,sizeof(char), (size_t) header.vnlen, m_file);
        for (i=0; i<header.n; i++)
            for (j=0; j<header.m; j++)
            {
                wert = double(val[j][i]);
                fwrite(&wert, sizeof(double),(size_t) 1, m_file);
            }
    }
    else
        _matrix_error("CMatrixT::matrixSaveFile: Error occured, file not saved !");
    if (m_open == true)
        {
            fclose(m_file);
            m_open = false;
        }
    return;
}

template<class T>
void CMatrixT<T>::LoadFile(const char* FileName, const bool &AutoResize)
{
    struct CMatfilehead {long type; long m; long n; long im; long vnlen; };
    FILE *m_file;
    int loadm, loadn;
    m_file = NULL;
    bool m_open = ((m_file = fopen(FileName, "rb")) != NULL );
    long buf_size[5];
    char buf_name[100];
    if (m_open == true)
    {
        fread(buf_size, sizeof(long),5,m_file);
        fread(buf_name, sizeof(char),(int)buf_size[4],m_file);
        loadm = (int)buf_size[1];
        loadn = (int)buf_size[2];
        register int i,j;
        double wert;
        if (m != loadm || n != loadn)
        {
            if (AutoResize)
                matrixSetsize(loadm,loadn);
            else
            {
                if (m_open == true)
                {
                    fclose(m_file);
                    m_open = false;
                }
                _matrix_error("CMatrixT::LoadFile: Wrong dimensions of matrix to load");
            }
        }

        for (i=0; i<n; i++)
            for (j=0; j<m; j++)
                {
                    fread(&wert,sizeof(double),1,m_file);
                    val[j][i] = (T)wert;
                }
    }
    else
        _matrix_error("CMatrixT::matrixLoadFile: Error occured file not loaded !");
    return;
}

template<class T>
void CMatrixT<T>::LoadFile(const char* FileName, const char* Vname, const bool &AutoResize)
{

    struct CMatfilehead {long type; long m; long n; long im; long vnlen; };
    FILE *m_file;

    char var_name[100]="";
    register int i,j;
    for (i=0; i< int(strlen(Vname)-1);i++)
        var_name[i]=Vname[i];
    int loadm, loadn;
    m_file = NULL;
    bool m_open = ((m_file = fopen(FileName, "rb")) != NULL );
    long buf_size[5];
    char buf_name[100]="";
    double wert;
    if (m_open)
    {
        while(!feof(m_file))
        {
            fread(buf_size, sizeof(long),5,m_file);
            fread(buf_name, sizeof(char),(int)buf_size[4],m_file);
            loadm = (int)buf_size[1];
            loadn = (int)buf_size[2];
            if (vergl(buf_name, var_name))
            {
                if (m != loadm || n != loadn)
                {
                    if (AutoResize)
                        matrixSetsize(loadm,loadn);
                    else
                    {
                        if (m_open == true)
                        {
                            fclose(m_file);
                            m_open = false;
                        }
                        _matrix_error("CMatrixT::LoadFile: Wrong dimensions of matrix to load");
                    }
                }

                for (i=0; i<n; i++)
                    for (j=0; j<m; j++)
                        {
                            fread(&wert,sizeof(double),1,m_file);
                            val[j][i] = (T)wert;
                        }

                fclose(m_file);
                m_open = false;
                return;
            }
            else
            {
                for (i=0; i<loadn; i++)
                    for (j=0; j<loadm; j++)
                        fread(&wert,sizeof(double),1,m_file);
            }
        }
        fclose(m_file);
        m_open = false;
        _matrix_error("CMatrixT::LoadFile: Variable not found");
    }
    else
        _matrix_error("CMatrixT::LoadFile: Error occurred, file not loaded");
    return;
}

#ifdef MATL_MEX

template<class T>                                                                               //Matrix loeschen und neu anlegen
inline void CMatrixT<T>::lomxArray(double *s, int r, int c)
{
    register int i,j;
    if (r > 0 && c > 0)
    {
        if ((r != m) || (c != n))
        {
            if (m > 0 && n > 0)
            {
                delete val[0];                                                                          //Speicher freigeben
                delete val;
            }
            m = r;
            n = c;
            val = new (std::nothrow) T* [m];
            val[0] = new (std::nothrow) T[m*n];
            for (i=1; i<m; i++)                                                                     //Speicher neu allokieren
                val[i] = val[0]+i*n;
        }
        for (i=0; i<m; i++)                                                                     //Mit 0 initialisieren
            for (j=0; j<n; j++)
                val[i][j] = (T) s[i+j*m];
    }
    return;
}

template<class T>                                                                               //Matrix loeschen und neu anlegen
inline void CMatrixT<T>::remxArray(double *d)
{
    register int i,j;
    for (i=0; i<m; i++)                                                                     //Mit 0 initialisieren
        for (j=0; j<n; j++)
            d[i+j*m] = (double) val[i][j];
    return;
}

template<class T>                                                                               //Matrix loeschen und neu anlegen
inline mxArray* CMatrixT<T>::setmxArray()
{
    register int i,j;
    mxArray *out;
    double *d;
    out = mxCreateDoubleMatrix(m,n,mxREAL);
    d = mxGetPr(out);
    for (i=0; i<m; i++)                                                                     //Mit 0 initialisieren
        for (j=0; j<n; j++)
            d[i+j*m] = (double) val[i][j];
    return out;


}

template<class T>                                                                               //Matrix loeschen und neu anlegen
inline void CMatrixT<T>::getmxArray(mxArray *inp)
{
    register int i,j;
    double *d;
    if ((mxGetM(inp) != m) || (mxGetN(inp) != n))
    {
        delete val[0];                                                                          //Speicher freigeben
        delete val;
        m = mxGetM(inp);
        n = mxGetN(inp);
        val = new (std::nothrow) T* [m];
        val[0] = new (std::nothrow) T[m*n];
        for (i=1; i<m; i++)                                                                     //Speicher neu allokieren
            val[i] = val[0]+i*n;
    }
    d = mxGetPr(inp);
    for (i=0; i<m; i++)                                                                     //Mit 0 initialisieren
        for (j=0; j<n; j++)
            val[i][j] = (T) d[i+j*m];

    return;
}

#endif

/////////////////////////////////////////////////////////////////////
/// TEST ROUTINEN
/////////////////////////////////////////////////////////////////////

template<class T>
inline void CMatrixT<T>::checkSortM(const int &row)
{
    register int i;
    T temp;
    temp = val[row][0];
    cout << "Checking RowNr.: \t" << row << " ... ";
    for(i=1; i<n; i++)
    {
        if (temp <= val[row][i])
            temp = val[row][i];
        else
            _matrix_error("CMatrixT::SortM(): Not correctly sorted !");
    }
}

template<class T>
inline void CMatrixT<T>::checkSortN(const int &col)
{
    register int i;
    T temp;
    temp = val[0][col];
    cout << "Checking ColNr.: \t" << col << " ... ";
    for(i=1; i<m; i++)
    {
        if (temp <= val[i][col])
            temp = val[i][col];
        else
            _matrix_error("CMatrixT::SortN(): Not correctly sorted !");
    }
}



#endif // MatrixT_t

