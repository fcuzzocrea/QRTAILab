/***************************************************************************
 *   Copyright (C) 2008 by Holger Nahrstaedt                               *
 *                                                                         *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License                  *
 *   as published by  the Free Software Foundation; either version 2       *
 *   of the License, or  (at your option) any later version.               *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*
 file:		parameters.h
 describtion:
   file for the classes QRL_ParametersManager
*/

#ifndef _ALOG_DATA_H
#define _ALOG_DATA_H 1

#include <QtGui> 

#include "qrtailab.h"

class QRL_ALogData
{
public:
  QRL_ALogData(int,int,float,char*);
  int getNRow(){return nrow;}
  int getNCol(){return ncol;}
  // enum Param_Class {rt_SCALAR,rt_VECTOR,rt_MATRIX_ROW_MAJOR,rt_MATRIX_COL_MAJOR,rt_MATRIX_COL_MAJOR_ND};
  const char* getName(){return name.c_str();}
  float getDt(){return dt;} 

private:
        const int nrow;
        const int ncol;
        const float dt;
        std::string name;
};



#endif
