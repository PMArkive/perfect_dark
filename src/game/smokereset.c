#include <ultra64.h>
#include "constants.h"
#include "bss.h"
#include "lib/memp.h"
#include "data.h"
#include "types.h"

void smokeReset(void)
{
	s32 i;
	s32 j;

	g_MaxSmokes = 20;

	if (g_Vars.stagenum >= STAGE_TITLE) {
		g_MaxSmokes = 0;
	}

	if (g_MaxSmokes == 0) {
		g_Smokes = NULL;
	} else {
		g_Smokes = mempAlloc(g_MaxSmokes * sizeof(struct smoke), MEMPOOL_STAGE);

		for (i = 0; i < g_MaxSmokes; i++) {
			g_Smokes[i].prop = NULL;

			for (j = 0; j < ARRAYCOUNT(g_Smokes[i].parts); j++) {
				g_Smokes[i].parts[j].size = 0;
			}
		}
	}
}
