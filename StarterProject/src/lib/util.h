#ifndef _UTIL_H
#define _UTIL_H

#include "type.h"

int32	_convertArrayToInt			(int8 *_s);
char* 	_convertHexToArray			(int V_I_n, char *V_P_C_s, int V_I_j);

bool	_compareBeginArray			(int8 *_s1, int8 *_s2, uint32 _cpt);

uint32	_sizeArray					(int8 *_s);
int8*	_concatArray				(int8 *_s1, int8 *_s2, int8 *_target);

void	_strcpy						(int8* _target, int8* _source);
void	_memcpy						(uint8* _target, uint8* _source,uint32 _len);

#endif
