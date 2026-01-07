#ifndef VCR_H_
#define VCR_H_


#include "types.h"

/* functions */
void EnableVCRButtons(void);  /* MEMORY_VCR:0x48f6 */
int16_t PopupVCRMenu(uint16_t hwnd, int16_t x, int16_t y, uint8_t brc);  /* MEMORY_VCR:0x4518 */
void DrawVCR(uint16_t hdc, int16_t iStart, int16_t iEnd);  /* MEMORY_VCR:0x1c62 */
void GetVCRStats(int16_t itok, int32_t *pdpArmor, DV *pdv, int32_t *pdpShields, int16_t *pcsh);  /* MEMORY_VCR:0x193e */
BTLDATA * BtlDataGet(int16_t i);  /* RETFAR */  /* MEMORY_VCR:0x0362 */
void BattleVCR(int16_t iBattle);  /* MEMORY_VCR:0x0000 */
int32_t LdpFromItokDv(int16_t itok, DV *lpdv);  /* MEMORY_VCR:0x07a8 */
int32_t CBattleKills(BTLDATA *lpbd, int16_t fOurDead);  /* MEMORY_VCR:0x062e */
int32_t CBattleUnits(BTLDATA *lpbd, uint16_t grbitBU);  /* MEMORY_VCR:0x0450 */
int16_t CBattles(void);  /* MEMORY_VCR:0x028c */
int16_t SetVCRBoard(int16_t iStep);  /* MEMORY_VCR:0x08d8 */
int16_t VCRDlg(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_VCR:0x0e90 */
void AnimateAttack(uint16_t hdc);  /* MEMORY_VCR:0x3ac2 */
void Delay(int16_t ctick);  /* MEMORY_VCR:0x3a3e */

#endif /* VCR_H_ */
