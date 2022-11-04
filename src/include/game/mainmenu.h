#ifndef IN_GAME_MAINMENU_H
#define IN_GAME_MAINMENU_H
#include <ultra64.h>
#include "data.h"
#include "types.h"

extern u16 g_ControlStyleOptions[];

extern struct menudialogdef g_AcceptMissionMenuDialog;
extern struct menudialogdef g_PreAndPostMissionBriefingMenuDialog;
extern struct menudialogdef g_RetryMissionMenuDialog;
extern struct menudialogdef g_NextMissionMenuDialog;

s32 menuhandlerControlStyleImpl(s32 operation, struct menuitem *item, union handlerdata *data, s32 mpindex);
s32 func0f104720(s32 value);
char *func0f105664(struct menuitem *item);
char *func0f1056a0(struct menuitem *item);
char *invMenuTextPrimaryFunction(struct menuitem *item);
char *invMenuTextSecondaryFunction(struct menuitem *item);
void func0f105948(s32 weaponnum);
char *invMenuTextWeaponName(struct menuitem *item);
char *invMenuTextWeaponManufacturer(struct menuitem *item);
char *invMenuTextWeaponDescription(struct menuitem *item);
bool soloChoosePauseDialog(void);
s32 menudialogBriefing(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data);
s32 menudialog00103608(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data);
s32 menudialogCoopAntiOptions(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data);
s32 menudialog0010559c(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data);
s32 inventoryMenuDialog(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data);
s32 menudialogAbortMission(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data);
s32 soloMenuDialogPauseStatus(s32 operation, struct menudialogdef *dialogdef, union handlerdata *data);
s32 menuhandler001024dc(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandler001024fc(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAcceptMission(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAcceptPdModeSettings(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerBuddyOptionsContinue(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerCoopDifficulty(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAntiDifficulty(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerMissionList(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerControlStyle(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerFrInventoryList(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerInventoryList(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAbortMission(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerCinema(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAimControl(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAlternativeTitle(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAlwaysShowTarget(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAmmoOnScreen(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAntiPlayer(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAntiRadar(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerAutoAim(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerChangeAgent(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerCoopBuddy(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerCoopFriendlyFire(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerCoopRadar(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerCutsceneSubtitles(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerHeadRoll(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerHiRes(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerInGameSubtitles(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerLangFilter(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerLookAhead(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerMusicVolume(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerPaintball(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerPdMode(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerPdModeSetting(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerReversePitch(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerScreenRatio(s32 operation, struct menuitem *item, union handlerdata *data);
#if PAL
s32 menuhandlerLanguage(s32 operation, struct menuitem *item, union handlerdata *data);
#endif
s32 menuhandlerScreenSize(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerScreenSplit(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerSfxVolume(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerShowGunFunction(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerShowMissionTime(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerShowZoomRange(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerSightOnScreen(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerSoloDifficulty(s32 operation, struct menuitem *item, union handlerdata *data);
s32 menuhandlerSoundMode(s32 operation, struct menuitem *item, union handlerdata *data);

#endif
