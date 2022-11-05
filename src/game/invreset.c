#include <ultra64.h>
#include "constants.h"
#include "constants.h"
#include "game/inv.h"
#include "bss.h"
#include "lib/memp.h"
#include "data.h"
#include "types.h"

void invReset(void)
{
	s32 i;

	g_Vars.currentplayer->equipallguns = false;

	for (i = 0; i != 10; i++) {
		g_Vars.currentplayer->gunheldarr[i].totaltime240_60 = -1;
	}
}

void invInit(void)
{
	g_Vars.currentplayer->equipmaxitems = 8; // Unarmed, 6 MP weapons + data uplink
	g_Vars.currentplayer->equipment = mempAlloc(ALIGN16(g_Vars.currentplayer->equipmaxitems * sizeof(struct invitem)), MEMPOOL_STAGE);
	invClear();
}
