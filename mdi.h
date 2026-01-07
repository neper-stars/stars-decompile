#ifndef MDI_H_
#define MDI_H_


#include "types.h"

/* globals */
extern uint8_t vrgbShuffleSerial[21];  /* MEMORY_MDI:0x2870 */
extern char rgTOWidth[2][2];  /* MEMORY_MDI:0x7702 */

/* functions */
void VerifyTurns(void);  /* MEMORY_MDI:0x6686 */
int16_t FSerialAndEnvFromSz(int32_t *plSerial, uint8_t *pbEnv, char *pszIn);  /* MEMORY_MDI:0x2aec */
void FormatSerialAndEnv(int32_t lSerial, uint8_t *pbEnv, char *pszOut);  /* MEMORY_MDI:0x2886 */
void RestoreSelection(void);  /* MEMORY_MDI:0x2614 */
void RefitFrameChildren(void);  /* MEMORY_MDI:0x8c88 */
int16_t FWasRaceFile(char *szFile, int16_t fChkPass);  /* MEMORY_MDI:0x5da8 */
int16_t HostModeDialog(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_MDI:0x6c16 */
void EnsureAis(void);  /* MEMORY_MDI:0x56bc */
int16_t FFindSomethingAndSelectIt(void);  /* MEMORY_MDI:0x2e1c */
int16_t CFindTurnsOutstanding(void);  /* MEMORY_MDI:0x6a26 */
int32_t TitleWndProc(uint16_t hwnd, uint16_t msg, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_MDI:0x9126 */
void CommandHandler(uint16_t hwnd, uint16_t wParam);  /* MEMORY_MDI:0x2f7a */
int32_t FrameWndProc(uint16_t a1, uint16_t a2, uint16_t a3, int32_t a4);  /* PASCAL */  /* MEMORY_MDI:0x06d6 */
void GetWindowRc(uint16_t hwnd, RECT *prc);  /* MEMORY_MDI:0x79ac */
void DrawHostDialog2(uint16_t hwnd, uint16_t hdcIn);  /* MEMORY_MDI:0x6240 */
void DrawHostOptions(uint16_t hwnd, uint16_t hdc, int16_t iDraw);  /* MEMORY_MDI:0x7706 */
void WriteIniSettings(void);  /* MEMORY_MDI:0x7a76 */
void HostTimerProc(uint16_t hwnd, uint16_t msg, uint16_t idTimer, uint32_t dwTime);  /* PASCAL */  /* MEMORY_MDI:0x7716 */
uint16_t GetASubMenu(uint16_t hwnd, int16_t iMenu);  /* MEMORY_MDI:0x589a */
int16_t FOpenGame(uint16_t hwnd, int16_t fRaceOnly);  /* MEMORY_MDI:0x58f4 */
void InitializeMenu(uint16_t hmenu);  /* MEMORY_MDI:0x5376 */
uint16_t HcrsFromFrameWindowPt(POINT pt, int16_t *pgrSel);  /* MEMORY_MDI:0x24b0 */
POINT InvertPaneBorder(uint16_t hdc, int16_t grSel, POINT dpt, POINT *pdptPrev);  /* MEMORY_MDI:0x1e3c */
int16_t CTurnsOutSafe(void);  /* MEMORY_MDI:0x6996 */
void BringUpHostDlg(void);  /* MEMORY_MDI:0x5ffc */
int16_t HostOptionsDialog(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_MDI:0x75ce */
int16_t InitMDIApp(void);  /* MEMORY_MDI:0x0000 */
void CreateChildWindows(void);  /* MEMORY_MDI:0x038c */
void SetWindowIniString(char *sz, uint16_t hwnd);  /* MEMORY_MDI:0x79f6 */

#endif /* MDI_H_ */
