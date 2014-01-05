///////////////////////////////////////////////////////////////////////////////
//
// SVD.cpp
//
// Abstract:  Singular-Value Decomposition.  Numerous informative comments 
//            througout code.
//
// Revision History Abstract:
// xxJul1989 Ronen Barzel    Initial code
// xxFeb1990 Ronen Barzel    Bug fixes
// xxMay1993 Ronen Barzel    ANSI-C backsubst
// 21Jan1997 ChuckR          Move into namespace
// 21Jul1997 ChuckR          Fix array bound error in underdetermined case
// 13Apr1999 ChuckR          Templatize and decryptify the code (somewhat)
///////////////////////////////////////////////////////////////////////////////

//#include "stdafx.h"

#include "Svd.h"


