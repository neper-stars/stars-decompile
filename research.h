#ifndef RESEARCH_H_
#define RESEARCH_H_


#include "types.h"

/* globals */
extern int32_t rglTechCost[27];  /* MEMORY_RESEARCH:0x1d4e */
extern uint16_t rggrbitBrParts[17];  /* MEMORY_RESEARCH:0x1eb6 */

/* functions */
int32_t CostOfDevelopingItem(char *rgTech);  /* MEMORY_RESEARCH:0x66e0 */
int32_t GetTechLevelCost(int16_t iTech, int16_t iLevel, int16_t iplr);  /* MEMORY_RESEARCH:0x1dba */
int16_t ResearchDlg(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_RESEARCH:0x0000 */
int16_t FTrackResearchDlg(uint16_t hwnd, int16_t x, int16_t y, int16_t fkb);  /* MEMORY_RESEARCH:0x1a8c */
int32_t ProjectedResearchSpending(int32_t pct);  /* MEMORY_RESEARCH:0x65ae */
void DrawResearchDlg(uint16_t hwnd, uint16_t hdc, RECT *prc, int16_t grbitDraw);  /* MEMORY_RESEARCH:0x090a */
void DisplayComponentInfo(uint16_t hdc, int16_t dx, int16_t dy, PART *ppart);  /* MEMORY_RESEARCH:0x2ac6 */
int16_t FShouldPartBeHidden(PART *ppart);  /* MEMORY_RESEARCH:0x68c4 */
int16_t BrowserDlg(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_RESEARCH:0x1ed8 */
int32_t BrowserWndProc(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_RESEARCH:0x2876 */

#endif /* RESEARCH_H_ */
