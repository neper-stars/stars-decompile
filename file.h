#ifndef FILE_H_
#define FILE_H_


#include "types.h"

/* functions */
void FileError(int16_t ids);  /* MEMORY_IO:0x4a10 */
void StreamOpen(char *szFile, int16_t mdOpen);  /* MEMORY_IO:0x52ae */
void UnpackBattlePlan(uint8_t *lpb, BTLPLAN *lpbtlplan, int16_t iplan);  /* MEMORY_IO:0x40ce */
int16_t FBadFileError(int16_t ids);  /* MEMORY_IO:0x524e */
void ReadRtPlr(PLAYER *pplr, uint8_t *pbIn);  /* MEMORY_IO:0x05e2 */
void UpdateBattleRecords(void);  /* MEMORY_IO:0x41ac */
int16_t FReadFleet(FLEET *lpfl);  /* MEMORY_IO:0x3a4c */
int16_t FLoadGame(char *pszFileName, char *pszExt);  /* MEMORY_IO:0x0810 */
int16_t FReadShDef(RTSHDEF *lprt, SHDEF *lpshdef, int16_t iplrLoad);  /* MEMORY_IO:0x0006 */
void ReadRt(void);  /* MEMORY_IO:0x5168 */
int16_t FOpenFile(uint16_t dt, int16_t iPlayer, int16_t md);  /* MEMORY_IO:0x4ac2 */
int16_t AskSaveDialog(void);  /* PASCAL */  /* MEMORY_IO:0x432a */
void StreamClose(void);  /* MEMORY_IO:0x53cc */
int16_t FNewTurnAvail(int16_t idPlayer);  /* MEMORY_IO:0x4f22 */
void GetFileStatus(int16_t dt, int16_t iPlayer);  /* MEMORY_IO:0x4a60 */
int16_t FReadPlanet(int16_t iPlayer, PLANET *lppl, int16_t fHistory, int16_t fPreInited);  /* MEMORY_IO:0x3206 */
void PromptSaveGame(void);  /* MEMORY_IO:0x43ee */
int16_t FCheckFile(uint16_t dt, int16_t iPlayer, uint16_t md);  /* MEMORY_IO:0x4fb2 */
int16_t FValidSerialLong(uint32_t lSerial);  /* MEMORY_IO:0x48c4 */
void DestroyCurGame(void);  /* MEMORY_IO:0x44b0 */
void RgFromStream(void *rg, uint16_t cb);  /* MEMORY_IO:0x53f4 */
int16_t FBogusLong(uint32_t lSerial);  /* MEMORY_IO:0x484c */

#endif /* FILE_H_ */
