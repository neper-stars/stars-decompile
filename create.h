#ifndef CREATE_H_
#define CREATE_H_


#include "types.h"

/* globals */
extern uint8_t vrgWormholeMin[5];  /* MEMORY_CREATE:0x0000 */
extern uint8_t vrgWormholeVar[5];  /* MEMORY_CREATE:0x0006 */
extern BTLPLAN rgbtlplanT[5];  /* MEMORY_CREATE:0x000c */
extern char rgNG3Width[9][2];  /* MEMORY_CREATE:0x9d4c */
extern PLAYER vrgplrComp[6][4];  /* MEMORY_CREATE:0xa370 */
extern int16_t vrgvcMax[10];  /* MEMORY_CREATE:0xb5a8 */

/* functions */
int16_t CreateStartupShip(int16_t iplr, int16_t idPlanet, int16_t ishdef, int16_t fAddShdef);  /* MEMORY_CREATE:0x4690 */
int16_t GetVCCheck(GAME *pgame, int16_t vc);  /* MEMORY_CREATE:0xb60c */
void InitBattlePlan(BTLPLAN *lpbtlplan, int16_t iplan, int16_t iplr);  /* MEMORY_CREATE:0x00c0 */
void InitNewGamePlr(int16_t iStepMaxSoFar, int16_t lvlAi);  /* MEMORY_CREATE:0x6e44 */
void SetNGWTitle(uint16_t hwnd, int16_t iStep);  /* MEMORY_CREATE:0xa320 */
int16_t GetVCVal(GAME *pgame, int16_t vc, int16_t fRaw);  /* MEMORY_CREATE:0xb710 */
void SetVCCheck(GAME *pgame, int16_t vc, int16_t fChecked);  /* MEMORY_CREATE:0xb5bc */
void CreateTutorWorld(void);  /* MEMORY_CREATE:0x5e5e */
int16_t FTrackNewGameDlg3(uint16_t hwnd, POINT pt, int16_t kbd);  /* MEMORY_CREATE:0xa210 */
void NewGameWizard(uint16_t hwnd, int16_t fReadOnly);  /* MEMORY_CREATE:0x6022 */
int16_t NewGameDlg3(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_CREATE:0x99f4 */
int16_t NewGameDlg2(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_CREATE:0x87d2 */
int16_t NewGameDlg(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_CREATE:0x7e92 */
int16_t SimpleNewGameDlg(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_CREATE:0x76aa */
int16_t SetVCVal(GAME *pgame, int16_t vc, int16_t val);  /* MEMORY_CREATE:0xb644 */
int16_t GenerateWorld(int16_t fBatchMode);  /* MEMORY_CREATE:0x0136 */
PLAYER * LpplrComp(int16_t idAi, int16_t lvlAi);  /* RETFAR */  /* MEMORY_CREATE:0xb570 */
int16_t FGetNewGameName(char *szFileSuggest);  /* MEMORY_CREATE:0x7508 */
void InitNewGame3(void);  /* MEMORY_CREATE:0x74f8 */
void DrawNewGame3(uint16_t hwnd, uint16_t hdc, int16_t iDraw);  /* MEMORY_CREATE:0x9d5e */
void DrawNewGame2(uint16_t hwnd, uint16_t hdc, int16_t iDraw);  /* MEMORY_CREATE:0x9556 */
int16_t GenNewGameFromFile(char *pszFile);  /* MEMORY_CREATE:0x4812 */

#endif /* CREATE_H_ */
