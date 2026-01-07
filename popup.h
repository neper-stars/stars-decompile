#ifndef POPUP_H_
#define POPUP_H_


#include "types.h"

/* globals */
extern uint16_t mpimdgrbitBU[8];  /* MEMORY_POPUP:0x0138 */

/* functions */
int32_t PopupWndProc(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_POPUP:0x0000 */
int16_t PopupMenu(uint16_t hwnd, int16_t x, int16_t y, int16_t cString, int32_t *rgids, char * *rgsz, int16_t iChecked, int16_t fRightBtn);  /* MEMORY_POPUP:0x136c */
void DrawPopup(uint16_t hwnd, uint16_t hdc);  /* MEMORY_POPUP:0x01c0 */
POINT PtDisplayResourceInfo(uint16_t hdc, int16_t dx, int16_t fPrint);  /* MEMORY_POPUP:0x3378 */
POINT PtDisplayPlanetStateInfo(uint16_t hdc, int16_t fPrint);  /* MEMORY_POPUP:0x1938 */
void Popup(uint16_t hwnd, int16_t x, int16_t y);  /* MEMORY_POPUP:0x0c7c */
int16_t FIsPopupHullType(int16_t ishdef);  /* MEMORY_POPUP:0x0148 */
POINT PtDisplayString(uint16_t hdc, int16_t dx, int16_t fPrint);  /* MEMORY_POPUP:0x355c */
POINT PtDisplayPlanetPopInfo(uint16_t hdc, int16_t fPrint);  /* MEMORY_POPUP:0x228e */
POINT PtDisplayZipOrdInfo(uint16_t hdc, int16_t xCtr, int16_t fPrint);  /* MEMORY_POPUP:0x2d66 */
POINT PtDisplayFactoryMineInfo(uint16_t hdc, int16_t dx, int16_t fPrint);  /* MEMORY_POPUP:0x3110 */

#endif /* POPUP_H_ */
