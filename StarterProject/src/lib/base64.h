#ifndef BASE64_H_
#define BASE64_H_

#include "type.h"

extern uint16 base64_encode(uint8* _src, uint32 len,uint8* _dest);
extern uint16 base64_decode(uint8* _src,uint16 len,uint8* _dest);

#endif /* BASE64_H_ */
