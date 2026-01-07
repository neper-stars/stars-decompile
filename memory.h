#ifndef MEMORY_H_
#define MEMORY_H_


#include "types.h"

/* functions */
void ResetHb(int16_t ht);  /* MEMORY_MEMORY:0x0348 */
void FreePl(PL *lppl);  /* MEMORY_MEMORY:0x0918 */
HB * LphbReAlloc(HB *lphb);  /* RETFAR */  /* MEMORY_MEMORY:0x0108 */
PL * LpplReAlloc(PL *lppl, uint16_t cAlloc);  /* RETFAR */  /* MEMORY_MEMORY:0x0836 */
HB * LphbFromLpHt(void *lp, int16_t ht);  /* RETFAR */  /* MEMORY_MEMORY:0x058c */
void FreeLp(void *lp, int16_t ht);  /* MEMORY_MEMORY:0x07a8 */
void * LpAlloc(uint16_t cb, int16_t ht);  /* RETFAR */  /* MEMORY_MEMORY:0x03b2 */
void * LpReAlloc(void *lp, uint16_t cb, int16_t ht);  /* RETFAR */  /* MEMORY_MEMORY:0x0660 */
HB * LphbAlloc(uint16_t cb, int16_t ht);  /* RETFAR */  /* MEMORY_MEMORY:0x0000 */
PL * LpplAlloc(uint16_t cbItem, uint16_t cAlloc, int16_t ht);  /* RETFAR */  /* MEMORY_MEMORY:0x088c */
void FreeHb(HB *lphb);  /* MEMORY_MEMORY:0x02d8 */

#endif /* MEMORY_H_ */
