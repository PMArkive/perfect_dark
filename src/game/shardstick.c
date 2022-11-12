#include <ultra64.h>
#include "constants.h"
#include "bss.h"
#include "data.h"
#include "types.h"

void shardsTick(void)
{
	f32 lvupdate;
	s32 i;
	s32 j;

	lvupdate = (g_Vars.lvupdate60 < TICKS(15)) ? g_Vars.lvupdate60 : TICKS(15);

	g_ShardsActive = false;
	g_WoodShardsActive = false;
	g_GlassShardsActive = false;

	for (i = 0; i < g_MaxShards; i++) {
		if (g_Shards[i].age60 > 0) {
			g_Shards[i].age60 += (s32)lvupdate;

			g_Shards[i].rot.x += g_Shards[i].rotspeed.x * lvupdate;
			g_Shards[i].rot.y += g_Shards[i].rotspeed.y * lvupdate;
			g_Shards[i].rot.z += g_Shards[i].rotspeed.z * lvupdate;
			g_Shards[i].pos.x += g_Shards[i].vel.x * lvupdate;
			g_Shards[i].pos.z += g_Shards[i].vel.z * lvupdate;

			for (j = 0; j < (s32)lvupdate; j++) {
				g_Shards[i].pos.y += g_Shards[i].vel.y;
				g_Shards[i].vel.y -= PALUPF(0.1f);
			}

			if (g_Shards[i].age60 >= TICKS(150) || g_Shards[i].pos.y < -30000 || g_Shards[i].pos.y > 30000) {
				g_Shards[i].age60 = 0;
			} else {
				g_ShardsActive = true;

				if (g_Shards[i].type == SHARDTYPE_GLASS) {
					g_GlassShardsActive = true;
				} else if (g_Shards[i].type == SHARDTYPE_WOOD) {
					g_WoodShardsActive = true;
				}
			}
		}
	}
}
