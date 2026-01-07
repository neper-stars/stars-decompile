#ifndef MINE_H_
#define MINE_H_


#include "types.h"

/* functions */
void GetMineFieldCounts(uint16_t id, int16_t *pithm, int16_t *pcthm);  /* MEMORY_MINE:0x05ac */
void MineClick(int16_t x, int16_t y, int16_t msg, int16_t sks);  /* MEMORY_MINE:0x3b4e */
int16_t FOtherStuffAtScanSel(void);  /* MEMORY_MINE:0x4d6e */
void DrawMineSurvey(uint16_t hdc, RECT *prc);  /* MEMORY_MINE:0x065a */
void InvalidateMineralBars(void);  /* MEMORY_MINE:0x040a */
int32_t MineWndProc(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_MINE:0x0000 */
void DrawSelectionArrow(uint16_t hdc, RECT *prc, int16_t fEnabled);  /* MEMORY_MINE:0x4a68 */
void PopupMineralScanChoices(uint16_t hwnd, int16_t x, int16_t y);  /* MEMORY_MINE:0x4ecc */
void SetMineralTitleBar(uint16_t hwnd);  /* MEMORY_MINE:0x47dc */
void EstMineralsMined(PLANET *lppl, int32_t *plQuan, int32_t cMines, int16_t fApply);  /* MEMORY_MINE:0x5362 */
int16_t HtMineWindow(uint16_t hwnd, int16_t x, int16_t y);  /* MEMORY_MINE:0x37ac */
void DrawDiamond(uint16_t hdc, RECT *prc, uint16_t hbr);  /* MEMORY_MINE:0x4b60 */

#endif /* MINE_H_ */
