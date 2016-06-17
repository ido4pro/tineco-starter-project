/*
 * _base64.cpp
 *
 *  Created on: 24 janv. 2016
 *      Author: tcousin
 */

#include "base64.h"

/*
Value 	Char 	Value 	Char 	Value 	Char 	Value 	Char
0 		A 		16 		Q 		32 		g 		48 		w
1 		B 		17 		R 		33 		h 		49 		x
2 		C 		18 		S 		34 		i 		50 		y
3 		D 		19 		T 		35 		j 		51 		z
4 		E 		20 		U 		36 		k 		52 		0
5 		F 		21 		V 		37 		l 		53 		1
6 		G 		22 		W 		38 		m 		54 		2
7 		H 		23 		X 		39 		n 		55 		3
8 		I 		24 		Y 		40 		o 		56 		4
9 		J 		25 		Z 		41 		p 		57 		5
10 		K 		26 		a 		42 		q 		58 		6
11 		L 		27 		b 		43 		r 		59 		7
12 		M 		28 		c 		44 		s 		60 		8
13 		N 		29 		d 		45 		t 		61 		9
14 		O 		30 		e 		46 		u 		62 		+
15 		P 		31 		f 		47 		v 		63 		/

*/

static uint8 _base64[] =
{
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
		'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
		'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
		'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/',
};

int8 _returnNum(char _c)
{
	int i;
	for(i = 0;i < 64;i++)
	{
		if(_c == _base64[i])
			return i;
	}

	return 0;
}

//  | xxxx xxxx | xxxx xxxx | xxxx xxxx |
//	| xxxx xx|xx xxxx | xxxx xx|xx xxxx |

uint16 base64_encode(uint8* _src, uint32 len,uint8* _dest)
{
	int _idxSrc = 0;
	int _idxDest = 0;

	int idx;
	uint8 _temp[2048];

	for(idx = 0;idx < 2048;idx++)
		_temp[idx] = 0;

	idx = 0;
	for(_idxSrc = 0;_idxSrc < len;_idxSrc)
	{
		// 0
		_temp[idx++]  	 = ((_src[_idxSrc] >> 2) & 0x3F);
		// 1
		_temp[idx]  	 = ((_src[_idxSrc] << 4) & 0x30);
		_idxSrc++;
		if(_idxSrc >= len) { idx++;break;}
		_temp[idx++] 	|= ((_src[_idxSrc] >> 4) & 0x0F);
		// 2
		_temp[idx]  	 = ((_src[_idxSrc] << 2) & 0x3C);
		_idxSrc++;
		if(_idxSrc >= len) { idx++;break;}
		_temp[idx++]  	|= ((_src[_idxSrc] >> 6) & 0x03);
		// 3
		_temp[idx++]  	 = (_src[_idxSrc] & 0x3F);
		_idxSrc++;
	}

/*	m_printf("_temp: %d ",idx);
	for(_idxDest = 0;_idxDest < idx;_idxDest++)
		m_printf("%02X ",_temp[_idxDest]);
	m_printf("\n\n");
*/
	for(_idxDest = 0;_idxDest < idx;_idxDest++)
		_dest[_idxDest] = _base64[(_temp[_idxDest] & 0x3F)];

	while((_idxDest % 4) != 0) _dest[_idxDest++] = '=';

	return _idxDest;
}

uint16 base64_decode(uint8* _src,uint16 len,uint8* _dest)
{
	int i;
	int idx;
	uint8 _temp[2048];

	//debugf("base64_decode %d\n\r",len);

	if((len % 4) != 0)
	{
		//debugf("(len % 4) != 0\n\r");
		return 0;
	}

//	m_printf("-> ");
	for(i = 0;i < len;i++)
	{
		_temp[i] = _returnNum(_src[i]);
//		m_printf("%02X ",_temp[i]);
	}
	//debugf("\n\r");

	i = 0;
	idx = 0;
	do
	{
		*_dest  	 = _temp[i++];
		*_dest  	<<= 2;
		*_dest++ 	|= (_temp[i] >> 4) & 0x03;

		*_dest  	 = _temp[i++];
		*_dest  	<<= 4;
		*_dest++ 	|= (_temp[i] >> 2) & 0x0F;

		*_dest  	 = _temp[i++];
		*_dest  	<<= 6;
		*_dest++ 	|= _temp[i++] & 0x3F;

		idx += 3;
	}
	while(i < len);

	return idx;
}
