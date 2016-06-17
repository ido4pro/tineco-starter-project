#include "util.h"

int32 irom _convertArrayToInt(int8 *_s)
{
	int _return = 0;
	int _m = 0;

	if (*_s == '-')
	{
		_m = 1;
		_s++;
	}
	int _i;
	for (_i=0; *_s!=0; _i++, _s++)
	{
		if(*_s >= '0' && *_s <= '9')
		{
			_return *= 10;
			_return += *_s - '0';
		}
	}
	if (_m)
		_return = -_return;

	return _return;
}

char* _convertHexToArray(int V_I_n, char *V_P_C_s, int V_I_j)
{
  int V_I_y;
  for (V_I_y=V_I_j-1; V_I_y>=0; V_I_y--)
  {
    char V_C_c = V_I_n & 0xF;
    V_I_n >>= 4;
    if (V_C_c < 10)
      V_P_C_s[V_I_y] = V_C_c + '0';
    else
      V_P_C_s[V_I_y] = V_C_c - 0xA + 'A';
  }
  V_P_C_s[V_I_j] = 0;
  return V_P_C_s;
}


bool irom _compareBeginArray(int8 *_s1, int8 *_s2, uint32 _cpt)
{
  uint32 _i;

  for (_i=0;_i < _cpt;_i++)
  {
    if (_s1[_i] != _s2[_i])
      return false;

  }
  return true;
}

uint32 irom _sizeArray(int8 *_s)
{
	uint32 _cpt = 0;

	while (_s[_cpt] != 0 && _cpt < 2000)
	{
		_cpt++;
	}

	return _cpt;
}

int8* irom _concatArray(int8 *_s1, int8 *_s2, int8 *_target)
{
	uint32 _size = _sizeArray(_s1) + _sizeArray(_s2)+1;

	int8	*_p1 = _s1,
			*_p2 = _s2,
			*_t  = _target;

	int32 _cpt = 0;
	while (*_p1 != 0 && _cpt < 1000)
	{
		*_t++ = *_p1++;
		_cpt++;
	}

	_cpt = 0;

	do
	{
		*_t++ = *_p2++;
		_cpt++;
	}
	while (*_p2 != 0 && _cpt < 1000);

	_target[_size-1]=0;

	return _target;
}

void irom _strcpy(int8* _target, int8* _source)
{
	uint32 _cpt = 0;

	while (*_source != 0 && _cpt < 2000)
	{
		_cpt++;
		*_target++ = *_source++;
	}
	*_target = 0;
}

void irom _memcpy(uint8* _target, uint8* _source,uint32 _len)
{
	uint32 _cpt = 0;
	for(_cpt = 0;_cpt < _len;_cpt++)
		*_target++ = *_source++;
}
