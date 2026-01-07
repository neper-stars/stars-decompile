#ifndef SHIP_H_
#define SHIP_H_


#include "types.h"

/* functions */
void UpdateOrdersDDs(int16_t iLevel);  /* MEMORY_SHIP:0x93ee */
void SetFleetDropDownSel(int16_t id);  /* MEMORY_SHIP:0x400e */
int32_t GetFuelFree(FLEET *lpfl);  /* MEMORY_SHIP:0x6004 */
void ShipCommandProc(uint16_t hwnd, uint16_t wParam, int32_t lParam);  /* MEMORY_SHIP:0x2640 */
void DrawShipOrders(uint16_t hdc, TILE *ptile, OBJ obj);  /* MEMORY_SHIP:0x0000 */
int32_t GetCargoFree(FLEET *lpfl);  /* MEMORY_SHIP:0x5f98 */
int32_t XferSupply(int16_t iSupply, int32_t cQuan);  /* MEMORY_SHIP:0x64cc */
void DrawFleetGauge(uint16_t hdc, RECT *prc, FLEET *lpfl, int16_t grbit);  /* MEMORY_SHIP:0x44b6 */
int16_t CshQueued(int16_t ishdef, int16_t *pfProgress, int16_t fSpaceDocks);  /* MEMORY_SHIP:0xc82c */
int32_t LGetFleetStat(FLEET *lpfl, int16_t grStat);  /* MEMORY_SHIP:0x40f2 */
void FillBattleDD(int16_t iSel);  /* MEMORY_SHIP:0x9a36 */
int16_t FCanSplitAll(int32_t cBoat);  /* MEMORY_SHIP:0x24ae */
int32_t EstFuelUse(FLEET *lpfl, int16_t iOrd, int16_t iWarp, int32_t dTravel, int16_t fRangeOnly);  /* MEMORY_SHIP:0x9fe4 */
void DeleteCurWayPoint(int16_t fBackup);  /* MEMORY_SHIP:0x9b08 */
void DrawFleetCargoXferSide(uint16_t hdc, RECT *prc, FLEET *pfl, int16_t iSupply);  /* MEMORY_SHIP:0x72de */
int32_t FakeEditProc(uint16_t hwnd, uint16_t msg, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_SHIP:0xa700 */
int16_t TransferStuff(int16_t id1, int16_t grobj1, int16_t id2, int16_t grobj2, int16_t mdXfer);  /* MEMORY_SHIP:0x4faa */
void DrawThingXferSide(uint16_t hdc, RECT *prc, THING *pth, int16_t iSupply);  /* MEMORY_SHIP:0x7088 */
void DrawShipWayPtOrders(uint16_t hdc, TILE *ptile, OBJ obj);  /* MEMORY_SHIP:0x0912 */
void Merge2Fleets(FLEET *lpflDst, FLEET *lpflDel, int16_t fNoDelete);  /* MEMORY_SHIP:0xc9f6 */
int16_t TransferDlg(uint16_t hwnd, uint16_t message, uint16_t wParam, int32_t lParam);  /* PASCAL */  /* MEMORY_SHIP:0x5686 */
void DrawXferDlg(uint16_t hwnd, uint16_t hdc, RECT *prc, int16_t iSupply);  /* MEMORY_SHIP:0x6908 */
void DrawFleetShipsXferSide(uint16_t hdc, RECT *prc, FLEET *pfl, int16_t iSupply);  /* MEMORY_SHIP:0x7726 */
void DrawShipPlanet(uint16_t hdc, TILE *ptile, OBJ obj);  /* MEMORY_SHIP:0x17b6 */
void DrawFleetComp(uint16_t hdc, TILE *ptile, OBJ obj);  /* MEMORY_SHIP:0x1e72 */
void FleetTransferCargoBalance(FLEET *pflNew1, FLEET *pflNew2);  /* MEMORY_SHIP:0xae74 */
void SetOrdersLbSel(int16_t iSel);  /* MEMORY_SHIP:0x9354 */
void SelectAdjFleet(int16_t dInc, int16_t idFleet);  /* MEMORY_SHIP:0x3d32 */
int16_t IFindIdealWarp(FLEET *lpfl, int16_t fIgnoreScoops);  /* MEMORY_SHIP:0xa76e */
void DrawPlanetXferSide(uint16_t hdc, RECT *prc, PLANET *ppl, int16_t iSupply);  /* MEMORY_SHIP:0x79ca */
void DeleteWpFar(FLEET *lpfl, int16_t iDel, int16_t fRecycle);  /* MEMORY_SHIP:0x9e28 */
int32_t ChgCargo(int16_t grobj, int16_t id, int16_t iSupply, int32_t dChg, void *pobj);  /* MEMORY_SHIP:0x6034 */
int16_t FTrackXfer(uint16_t hwnd, int16_t x, int16_t y, int16_t fkb);  /* MEMORY_SHIP:0x5a16 */
int16_t FCanSplit(int32_t cBoat);  /* MEMORY_SHIP:0x245c */
int16_t FCanMerge(FLEET *pfl);  /* MEMORY_SHIP:0x2522 */
void FillFleetCompLB(void);  /* MEMORY_SHIP:0x9166 */
uint16_t ClickInShipOrders(POINT pt, int16_t sks, int16_t fCursor, int16_t fRightBtn);  /* MEMORY_SHIP:0x7cda */
void DestroyAllIshdef(int16_t ishdef, int16_t iplr);  /* MEMORY_SHIP:0xc336 */
int16_t WtMaxShdefStat(SHDEF *lpshdef, int16_t grStat);  /* MEMORY_SHIP:0x41b2 */
int16_t FEnumCalcJettison(void *lprt, int16_t rt, int16_t cb, PLANET *lppl, int16_t iFleet);  /* MEMORY_SHIP:0x4df4 */
void UpdateXferBtns(void);  /* MEMORY_SHIP:0x66a8 */
int16_t FSetupXferBtns(RECT *prc);  /* MEMORY_SHIP:0x6bea */
void DrawFleetBitmap(FLEET *lpfl, uint16_t hdc, int16_t x, int16_t y, int16_t fFrame, int16_t ibmp, int16_t cDiff, int16_t fShrink, int16_t ibmpRace, int16_t csh);  /* MEMORY_SHIP:0x490e */
void DestroyAllIshdefSB(int16_t ishdefSB, int16_t iplr);  /* MEMORY_SHIP:0xc280 */
void GetTruePartCost(int16_t iPlayer, PART *ppart, uint16_t *rgCost);  /* MEMORY_SHIP:0xcd00 */
void RemoveIshdefFromAllQueues(int16_t ishdef, int16_t fSpaceDocks);  /* MEMORY_SHIP:0xc5e6 */
void DrawShipCargo(uint16_t hdc, TILE *ptile, OBJ obj);  /* MEMORY_SHIP:0x1a54 */
void FillOrdersLB(void);  /* MEMORY_SHIP:0x928c */
int32_t LFuelUseToWaypoint(FLEET *lpfl, int16_t iwp, int16_t fMaxCargo);  /* MEMORY_SHIP:0xa9f4 */
void FleetOrdersChangeTarget(FLEET *lpflOld);  /* MEMORY_SHIP:0xcafe */
void GetXferLeftRightRcs(RECT *prcWhole, RECT *prcLeft, RECT *prcRight);  /* MEMORY_SHIP:0x6b46 */

#endif /* SHIP_H_ */
