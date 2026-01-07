#ifndef REPORT_H_
#define REPORT_H_


#include "types.h"

/* globals */
extern uint16_t mpicolgrbitBU[12];  /* MEMORY_REPORT:0x0000 */

/* functions */
int16_t ScoreXDlg(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_REPORT:0x0f66 */
int32_t ReportDlg(uint16_t hwnd, uint16_t msg, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_REPORT:0x0018 */
int16_t PrintMapDlg(uint16_t hwnd, uint16_t msg, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_REPORT:0xa1c2 */
void SetHScrollBar(void);  /* MEMORY_REPORT:0x09e8 */
void SortReportCache(int16_t irpt, int16_t icol);  /* MEMORY_REPORT:0x589c */
void InitScoreDlg(uint16_t hwnd, int16_t fVictory);  /* MEMORY_REPORT:0x13b6 */
void ReportColumnPopup(POINT pt, int16_t icol, int16_t fRightBtn);  /* MEMORY_REPORT:0x74d4 */
int16_t FDestIsWP0(FLEET *lpfl);  /* MEMORY_REPORT:0x50b4 */
int16_t ICompReport(void *arg1, void *arg2);  /* MEMORY_REPORT:0x5bb8 */
void DrawReport(uint16_t hwnd, uint16_t hdc, RECT *prc);  /* MEMORY_REPORT:0x0bae */
void InvalidateReport(int16_t irpt, int16_t fReload);  /* MEMORY_REPORT:0x7af6 */
void DumpUniverse(void);  /* MEMORY_REPORT:0x851e */
void DumpFleets(void);  /* MEMORY_REPORT:0x9530 */
int16_t DxReportColHdr(int16_t irpt, int16_t iCol, char *psz, uint16_t hdc);  /* MEMORY_REPORT:0x305e */
void DumpPlanets(void);  /* MEMORY_REPORT:0x86e4 */
int32_t LFetchScoreXVal(SCOREX *lpsx, int16_t iVal);  /* MEMORY_REPORT:0x2f94 */
char * PszGetETA(uint16_t hdc, FLEET *lpfl, int16_t *pcYears);  /* MEMORY_REPORT:0x51a8 */
void ExecuteReportClick(POINT pt, int16_t irpt, int16_t icol, int16_t irow);  /* MEMORY_REPORT:0x7cd6 */
void DrawVCReport(uint16_t hdc);  /* MEMORY_REPORT:0x168e */
void DrawReportItem(uint16_t hdc, RECT *prc, int16_t irpt, int16_t irow, int16_t icol);  /* MEMORY_REPORT:0x3398 */
char * PszGetTaskName(FLEET *lpfl, int16_t *picr);  /* MEMORY_REPORT:0x53b8 */
char * PszGetDestName(FLEET *lpfl, uint16_t hdc);  /* MEMORY_REPORT:0x4f60 */
void DrawMineralItem(uint16_t hdc, int16_t x, int16_t y, int16_t iMineral, int32_t l);  /* MEMORY_REPORT:0x4ebe */
void DrawHistoryReport(uint16_t hdc);  /* MEMORY_REPORT:0x2494 */
void DrawScoreReport(uint16_t hdc);  /* MEMORY_REPORT:0x1e0c */

#endif /* REPORT_H_ */
