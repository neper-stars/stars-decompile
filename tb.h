#ifndef TB_H_
#define TB_H_


#include "types.h"

/* globals */
extern char vrgTBBtn[29];  /* MEMORY_TB:0x0000 */
extern int16_t vrgpctZoom[9];  /* MEMORY_TB:0x0da4 */

/* functions */
int32_t FakeComboProc(uint16_t hwnd, uint16_t msg, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_TB:0x1d72 */
void ShowTooltip(int16_t ids, RECT *prc);  /* MEMORY_TB:0x17ea */
int32_t TbWndProc(uint16_t hwnd, uint16_t msg, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_TB:0x001e */
void DrawToolbar(uint16_t hdc, RECT *prc);  /* MEMORY_TB:0x06f0 */
int16_t DxOfBtn(int16_t itb);  /* MEMORY_TB:0x0bb2 */
void DrawBitmapButton(uint16_t hdc, POINT pt, int16_t ibtn, int16_t fDown);  /* MEMORY_TB:0x078c */
void ExecuteButton(int16_t itb, int16_t fDown);  /* MEMORY_TB:0x0db6 */
int32_t TooltipWndProc(uint16_t hwnd, uint16_t msg, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_TB:0x19e4 */
int16_t FIsButtonDown(int16_t itb);  /* MEMORY_TB:0x0c3a */
int16_t ItbFromPpt(POINT *ppt);  /* MEMORY_TB:0x0b12 */
int32_t FakeCEProc(uint16_t hwnd, uint16_t msg, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_TB:0x1df0 */
void TerminateToolbarFocus(int16_t fCancel);  /* MEMORY_TB:0x1680 */

#endif /* TB_H_ */
