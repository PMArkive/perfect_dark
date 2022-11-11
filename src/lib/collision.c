#include <ultra64.h>
#include "constants.h"
#include "game/prop.h"
#include "game/game_1531a0.h"
#include "game/bg.h"
#include "bss.h"
#include "lib/dma.h"
#include "lib/memp.h"
#include "lib/mtx.h"
#include "lib/lib_17ce0.h"
#include "lib/anim.h"
#include "lib/collision.h"
#include "lib/lib_2f490.h"
#include "lib/libc/ll.h"
#include "data.h"
#include "types.h"

#define SURFACE_FLOOR   0
#define SURFACE_CEILING 1

struct debugtri {
	s16 vertices[3][3];
	u8 unk12;
};

union filedataptr g_TileFileData;
s32 g_TileNumRooms;
u32 *g_TileRooms;
s32 var8009a8ac;
f32 var8009a8b0;
s32 var8009a8b4;
struct coord g_CdEdgeVtx1;
s32 var8009a8c4;
struct coord g_CdEdgeVtx2;
struct prop *g_CdObstacleProp;
s32 var8009a8d8;
s32 var8009a8dc;
struct coord g_CdObstaclePos;
s32 var8009a8ec;
f32 var8009a8f0;
bool g_CdHasSavedPos;
struct coord g_CdPos1;
s32 var8009a904;
struct coord g_CdPos2;
s32 var8009a914;
struct geoblock g_CdSavedBlock;
struct geo *g_CdObstacleGeo;
s32 var8009a968;
s32 var8009a96c;

s32 var8005f030 = 0;
bool g_CdHasSavedBlock = false;
s32 var8005f038 = 0;

f32 cd00024e40(void)
{
	return var8009a8f0;
}

void cdGetEdge(struct coord *vtx1, struct coord *vtx2, u32 line, char *file)
{
	*vtx1 = g_CdEdgeVtx1;

	*vtx2 = g_CdEdgeVtx2;
}

f32 cd00024e98(void)
{
	return var8009a8b0;
}

s32 cd00024ea4(void)
{
	return var8009a8ac;
}

struct prop *cdGetObstacleProp(void)
{
	return g_CdObstacleProp;
}

void cdGetPos(struct coord *pos, u32 line, char *file)
{
	*pos = g_CdObstaclePos;
}

void cdGetObstacleNormal(struct coord *normal)
{
	cdGetGeoNormal(g_CdObstacleGeo, normal);
}

u32 cdGetGeoFlags(void)
{
	u32 flags = 0;

	switch (g_CdObstacleGeo->type) {
	case GEOTYPE_TILE_I:
		flags = g_CdObstacleGeo->flags;
		break;
	case GEOTYPE_TILE_F:
		flags = g_CdObstacleGeo->flags;
		break;
	case GEOTYPE_BLOCK:
		flags = GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT;
		break;
	case GEOTYPE_CYL:
		flags = g_CdObstacleGeo->flags;
		break;
	}

	return flags;
}

void cdClearResults(void)
{
	var8009a8b4 = 0;
	var8009a8ac = 0;
	g_CdObstacleProp = NULL;
	var8009a8d8 = 0;
	var8009a8ec = 0;
	g_CdHasSavedPos = false;
	g_CdHasSavedBlock = false;
	var8005f038 = 0;
}

void cdSetObstacleVtxProp(struct coord *vtx1, struct coord *vtx2, struct prop *prop)
{
	g_CdEdgeVtx1 = *vtx1;

	g_CdEdgeVtx2 = *vtx2;

	var8009a8b4 = 1;
	var8009a8ac = 0;
	g_CdObstacleProp = prop;
	var8009a8d8 = 0;
	var8009a8ec = 0;
	g_CdHasSavedPos = false;
	g_CdHasSavedBlock = false;
	var8005f038 = 0;
}

void cdSetObstacleVtxPropFlt(struct coord *vtx1, struct coord *vtx2, struct prop *prop, f32 arg3)
{
	var8009a8b0 = arg3;

	g_CdEdgeVtx1 = *vtx1;

	g_CdEdgeVtx2 = *vtx2;

	var8009a8b4 = 1;
	var8009a8ac = 1;
	g_CdObstacleProp = prop;
	var8009a8d8 = 0;
	var8009a8ec = 0;
	g_CdHasSavedPos = false;
	g_CdHasSavedBlock = false;
	var8005f038 = 0;
}

void cd000250cc(struct coord *arg0, struct coord *arg1, f32 width)
{
	struct widthxz sp34;
	struct xz sp2c;
	struct xz sp24;
	struct xz sp1c;

	sp34.width = width;
	sp34.x = arg0->x;
	sp34.z = arg0->z;

	sp1c.x = arg1->x;
	sp1c.z = arg1->z;

	sp2c.x = g_CdEdgeVtx1.x;
	sp2c.z = g_CdEdgeVtx1.z;

	sp24.x = g_CdEdgeVtx2.x;
	sp24.z = g_CdEdgeVtx2.z;

	var8009a8b0 = func0f1579cc(&sp34, &sp2c, &sp24, &sp1c);
	var8009a8ac = 1;
}

void cdSetObstacleProp(struct prop *prop)
{
	var8009a8b4 = 0;
	var8009a8ac = 0;
	g_CdObstacleProp = prop;
	var8009a8d8 = 0;
	var8009a8ec = 0;
	g_CdHasSavedPos = false;
	g_CdHasSavedBlock = false;
	var8005f038 = 0;
}

void cdSetObstacleVtxColProp(struct coord *vtxpos1, struct coord *vtxpos2, struct coord *collisionpos, struct prop *prop)
{
	g_CdEdgeVtx1 = *vtxpos1;

	g_CdEdgeVtx2 = *vtxpos2;

	g_CdObstaclePos = *collisionpos;

	var8009a8b4 = 1;
	var8009a8ac = 0;
	g_CdObstacleProp = prop;
	var8009a8d8 = 1;
	var8009a8ec = 0;
	g_CdHasSavedPos = false;
	g_CdHasSavedBlock = false;
	var8005f038 = 0;
}

void cdSetObstacleVtxColPropFltGeo(struct coord *vtxpos1, struct coord *vtxpos2, struct coord *collisionpos, struct prop *prop, f32 arg4, struct geo *geo)
{
	g_CdEdgeVtx1 = *vtxpos1;

	g_CdEdgeVtx2 = *vtxpos2;

	g_CdObstaclePos = *collisionpos;

	var8009a8b4 = 1;
	var8009a8ac = 0;
	g_CdObstacleProp = prop;
	var8009a8d8 = 1;
	var8009a8f0 = arg4;
	var8009a8ec = 1;
	g_CdHasSavedPos = false;
	g_CdHasSavedBlock = false;
	g_CdObstacleGeo = geo;
	var8005f038 = 1;
}

void cdSetSavedPos(struct coord *pos1, struct coord *pos2)
{
	g_CdPos1 = *pos1;

	g_CdPos2 = *pos2;

	g_CdHasSavedPos = true;
}

bool cdGetSavedPos(struct coord *pos1, struct coord *pos2)
{
	if (g_CdHasSavedPos) {
		*pos1 = g_CdPos1;

		*pos2 = g_CdPos2;
	}

	return g_CdHasSavedPos;
}

void cdSetSavedBlock(struct geoblock *block)
{
	g_CdSavedBlock = *block;
	g_CdHasSavedBlock = true;
}

s32 cd00025410(f32 arg0, f32 arg1, f32 arg2, f32 arg3)
{
	f32 f0 = arg0 * arg3;
	f32 f2 = arg1 * arg2;

	if (f2 < f0) {
		return 1;
	}

	if (f2 > f0) {
		return -1;
	}

	if (arg0 * arg2 < 0.0f || arg1 * arg3 < 0.0f) {
		return -1;
	}

	if (arg0 * arg0 + arg1 * arg1 < arg2 * arg2 + arg3 * arg3) {
		return 1;
	}

	return 0;
}

s32 cd000254d8(struct coord *arg0, struct coord *arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 *arg6)
{
	f32 sp54;
	f32 sp50;
	f32 sp4c;
	f32 sp48;
	s32 sp44;
	s32 sp40;
	s32 sp3c;
	s32 sp38;
	s32 sp34;
	s32 sp30;
	bool result = false;

	sp54 = arg0->x - arg2;
	sp50 = arg0->z - arg3;

	sp3c = cd00025410(arg4 - arg2, arg5 - arg3, sp54, sp50);
	sp44 = cd00025410(arg4 - arg2, arg5 - arg3, arg1->x - arg2, arg1->z - arg3);
	sp38 = sp3c * sp44;

	if (sp38 <= 0) {
		sp4c = arg1->x - arg0->x;
		sp48 = arg1->z - arg0->z;

		sp34 = cd00025410(sp4c, sp48, -sp54, -sp50);
		sp40 = cd00025410(sp4c, sp48, arg4 - arg0->x, arg5 - arg0->z);
		sp30 = sp34 * sp40;

		if (sp30 <= 0) {
			result = true;
		}
	}

	if (*arg6 && (result || sp3c <= 0)) {
		*arg6 = 0;
	}

	return result;
}

f32 cd00025654(f32 x1, f32 z1, f32 x2, f32 z2, f32 x3, f32 z3)
{
	u32 stack[8];
	f32 result;

	result = sqrtf((x2 - x1) * (x2 - x1) + (z2 - z1) * (z2 - z1));

	if (result == 0.0f) {
		return sqrtf((x3 - x2) * (x3 - x2) + (z3 - z2) * (z3 - z2));
	}

	return ((x3 - x1) * (z2 - z1) + -(x2 - x1) * (z3 - z1)) / result;
}

f32 cd00025724(f32 x1, f32 z1, f32 x2, f32 z2)
{
	x2 -= x1;
	z2 -= z1;

	return sqrtf(x2 * x2 + z2 * z2);
}

bool cd00025774(f32 x1, f32 z1, f32 x2, f32 z2, f32 x3, f32 z3)
{
	f32 f0;
	f32 f2;
	f32 f16;
	f32 f18;

	x3 -= x1;
	z3 -= z1;

	f0 = x2 - x1;
	f2 = z2 - z1;

	f16 = x3 * f0 + z3 * f2;
	f18 = f0 * f0 + f2 * f2;

	return (f18 < f16 && f16 < 0) || (f16 > 0 && f16 < f18);
}

void cd00025848(f32 tilex, f32 tilez, f32 tilewidth, f32 posx, f32 posz, f32 *x1, f32 *z1, f32 *x2, f32 *z2)
{
	posx -= tilex;
	posz -= tilez;

	if (posx != 0 || posz != 0) {
		f32 dist = sqrtf(posx * posx + posz * posz);

		if (dist > 0) {
			dist = tilewidth / dist;
			posx *= dist;
			posz *= dist;
		}
	}

	*x1 = tilex + posx + posz;
	*z1 = tilez + posz - posx;
	*x2 = tilex + posx - posz;
	*z2 = tilez + posz + posx;
}

void cdGetGeoNormal(struct geo *geo, struct coord *normal)
{
	if (geo->type == GEOTYPE_TILE_I) {
		struct geotilei *tile = (struct geotilei *) geo;
		s32 sp38[3];
		s32 sp2c[3];
		s32 sp20[3];

		sp38[0] = tile->vertices[1][0] - tile->vertices[0][0];
		sp38[1] = tile->vertices[1][1] - tile->vertices[0][1];
		sp38[2] = tile->vertices[1][2] - tile->vertices[0][2];

		sp2c[0] = tile->vertices[2][0] - tile->vertices[0][0];
		sp2c[1] = tile->vertices[2][1] - tile->vertices[0][1];
		sp2c[2] = tile->vertices[2][2] - tile->vertices[0][2];

		sp20[0] = sp38[1] * sp2c[2] - sp38[2] * sp2c[1];
		sp20[1] = sp38[2] * sp2c[0] - sp38[0] * sp2c[2];
		sp20[2] = sp38[0] * sp2c[1] - sp38[1] * sp2c[0];

		normal->x = sp20[0];
		normal->y = sp20[1];
		normal->z = sp20[2];
	} else if (geo->type == GEOTYPE_TILE_F) {
		struct geotilef *tile = (struct geotilef *) geo;
		f32 sp10[3];
		f32 sp04[3];

		sp10[0] = tile->vertices[1].x - tile->vertices[0].x;
		sp10[1] = tile->vertices[1].y - tile->vertices[0].y;
		sp10[2] = tile->vertices[1].z - tile->vertices[0].z;

		sp04[0] = tile->vertices[2].x - tile->vertices[0].x;
		sp04[1] = tile->vertices[2].y - tile->vertices[0].y;
		sp04[2] = tile->vertices[2].z - tile->vertices[0].z;

		normal->x = sp10[1] * sp04[2] - sp10[2] * sp04[1];
		normal->y = sp10[2] * sp04[0] - sp10[0] * sp04[2];
		normal->z = sp10[0] * sp04[1] - sp10[1] * sp04[0];
	} else if (geo->type == GEOTYPE_BLOCK) {
		normal->x = 0;
		normal->y = 1;
		normal->z = 0;
	} else if (geo->type == GEOTYPE_CYL) {
		normal->x = 0;
		normal->y = 1;
		normal->z = 0;
	}
}

void cdGetFloorCol(struct geo *geo, u16 *floorcol)
{
	if (geo == NULL) {
		*floorcol = 0xfff;
		return;
	}

	if (geo->type == GEOTYPE_TILE_I) {
		struct geotilei *tile = (struct geotilei *) geo;
		*floorcol = tile->floorcol;
		return;
	}

	if (geo->type == GEOTYPE_TILE_F) {
		struct geotilef *tile = (struct geotilef *) geo;
		*floorcol = tile->floorcol;
		return;
	}

	if (geo->type == GEOTYPE_BLOCK) {
		*floorcol = 0xfff;
		return;
	}

	if (geo->type == GEOTYPE_CYL) {
		*floorcol = 0xfff;
	}
}

void cdGetFloorType(struct geo *geo, u8 *floortype)
{
	bool water = false;

	if (geo && (geo->flags & GEOFLAG_UNDERWATER)) {
		water = true;
	}

	if (geo == NULL) {
		*floortype = 0xff;
		return;
	}

	if (water) {
		*floortype = FLOORTYPE_WATER;
		return;
	}

	if (geo->type == GEOTYPE_TILE_I) {
		struct geotilei *tile0 = (struct geotilei *) geo;
		*floortype = tile0->floortype;
		return;
	}

	if (geo->type == GEOTYPE_TILE_F) {
		struct geotilef *tile1 = (struct geotilef *) geo;
		*floortype = tile1->floortype;
		return;
	}

	if (geo->type == GEOTYPE_BLOCK) {
		*floortype = 0xff;
		return;
	}

	if (geo->type == GEOTYPE_CYL) {
		*floortype = 0xff;
	}
}

f32 cdFindGroundInIntTileAtVertex(struct geotilei *tile, f32 x, f32 z, s32 vertexindex)
{
	struct coord sp7c;
	struct coord sp70;
	s64 sp68;
	s64 sp60;
	s64 sp58;
	s64 tmp;
	f32 ground;
	s32 next;

	if (vertexindex == 0) {
		vertexindex = 1;
	}

	next = (vertexindex + 1) % tile->header.numvertices;

	if (next == 0) {
		next = 1;
	}

	sp7c.x = tile->vertices[vertexindex][0] - tile->vertices[0][0];
	sp7c.y = tile->vertices[vertexindex][1] - tile->vertices[0][1];
	sp7c.z = tile->vertices[vertexindex][2] - tile->vertices[0][2];

	sp70.x = tile->vertices[next][0] - tile->vertices[0][0];
	sp70.y = tile->vertices[next][1] - tile->vertices[0][1];
	sp70.z = tile->vertices[next][2] - tile->vertices[0][2];

	sp58 = sp7c.f[1] * sp70.f[2] - sp7c.f[2] * sp70.f[1];
	sp60 = sp7c.f[2] * sp70.f[0] - sp7c.f[0] * sp70.f[2];
	sp68 = sp7c.f[0] * sp70.f[1] - sp7c.f[1] * sp70.f[0];

	tmp = sp58 * tile->vertices[0][0]
		+ sp60 * tile->vertices[0][1]
		+ sp68 * tile->vertices[0][2];

	if (sp60 == 0) {
		return *(s16 *)(tile->ymax + (u32)tile);
	}

	ground = (tmp - (f64)x * sp58 - (f64)z * sp68) / sp60;

	if (ground > *(s16 *)(tile->ymax + (u32)tile)) {
		ground = *(s16 *)(tile->ymax + (u32)tile);
	} else if (ground < *(s16 *)(tile->ymin + (u32)tile)) {
		ground = *(s16 *)(tile->ymin + (u32)tile);
	}

	return ground;
}

#if VERSION < VERSION_NTSC_1_0
f32 cdFindGroundInTileType0AtVertex1(struct geotilei *tile, f32 x, f32 z)
{
	return cdFindGroundInIntTileAtVertex(tile, x, z, 1);
}
#endif

f32 cdFindGroundInIntTile(struct geotilei *tile, f32 x, f32 z)
{
	s32 i = 1;
	s32 ival = -1;
	struct geotilei *tile2 = tile;

	if (tile->header.numvertices >= 4) {
		while (i < tile->header.numvertices) {
			f32 tmpz = tile2->vertices[i][2];
			f32 tmpx = tile2->vertices[i][0];

			f32 fval = ((tile->vertices[0][2] - tmpz) * (x - tmpx))
				- ((tile2->vertices[0][0] - tmpx) * (0, z - tmpz));

			if (fval != 0) {
				if (ival < 0) {
					ival = (fval > 0);
				} else if (ival != 0 && fval < 0) {
					i--;
					break;
				} else if (ival == 0 && fval > 0) {
					i--;
					break;
				}
			}

			i++;
		}
	}

	return cdFindGroundInIntTileAtVertex(tile, x, z, i);
}

f32 cdFindGroundInFltTile(struct geotilef *tile, f32 x, f32 z)
{
	struct coord sp24;
	struct coord sp18;
	struct coord sp0c;
	f32 tmp;
	f32 ground;

	sp24.x = tile->vertices[1].x - tile->vertices[0].x;
	sp24.y = tile->vertices[1].y - tile->vertices[0].y;
	sp24.z = tile->vertices[1].z - tile->vertices[0].z;

	sp18.x = tile->vertices[2].x - tile->vertices[0].x;
	sp18.y = tile->vertices[2].y - tile->vertices[0].y;
	sp18.z = tile->vertices[2].z - tile->vertices[0].z;

	sp0c.x = sp24.f[1] * sp18.f[2] - sp24.f[2] * sp18.f[1];
	sp0c.y = sp24.f[2] * sp18.f[0] - sp24.f[0] * sp18.f[2];
	sp0c.z = sp24.f[0] * sp18.f[1] - sp24.f[1] * sp18.f[0];

	tmp = sp0c.f[0] * tile->vertices[0].f[0]
		+ sp0c.f[1] * tile->vertices[0].f[1]
		+ sp0c.f[2] * tile->vertices[0].f[2];

	if (sp0c.f[1] == 0) {
		return tile->vertices[tile->ymax].y;
	}

	ground = (tmp - (f64)x * (f64)sp0c.f[0] - (f64)z * (f64)sp0c.f[2]) / (f64)sp0c.f[1];

	if (ground > tile->vertices[tile->ymax].y) {
		ground = tile->vertices[tile->ymax].y;
	} else if (ground < tile->vertices[tile->ymin].y) {
		ground = tile->vertices[tile->ymin].y;
	}

	return ground;
}

bool cdIs2dPointInIntTile(struct geotilei *tile, f32 x, f32 z)
{
	s32 result = -1;
	s32 numvertices = tile->header.numvertices;
	s32 i;

	for (i = 0; i < numvertices; i++) {
		s32 next = (i + 1) % numvertices;

		f32 value = ((f32)tile->vertices[next][2] - (f32)tile->vertices[i][2]) * (x - tile->vertices[i][0])
			- ((f32)tile->vertices[next][0] - (f32)tile->vertices[i][0]) * (z - tile->vertices[i][2]);

		if (value != 0) {
			if (i == 0 || result < 0) {
				result = (value > 0);
			} else {
				if (result != 0 && value < 0) {
					return false;
				}

				if (result == 0 && value > 0) {
					return false;
				}
			}
		}
	}

	if (result < 0) {
		return false;
	}

	return true;
}

bool cdIs2dPointInFltTile(struct geotilef *tile, f32 x, f32 z)
{
	s32 result = -1;
	s32 numvertices = tile->header.numvertices;
	s32 i;

	for (i = 0; i < numvertices; i++) {
		s32 next = (i + 1) % numvertices;

		f32 value = (tile->vertices[next].z - tile->vertices[i].z) * (x - tile->vertices[i].x)
			- (tile->vertices[next].x - tile->vertices[i].x) * (z - tile->vertices[i].z);

		if (value != 0) {
			if (i == 0 || result < 0) {
				result = (value > 0);
			} else {
				if (result != 0 && value < 0) {
					return false;
				}

				if (result == 0 && value > 0) {
					return false;
				}
			}
		}
	}

	if (result < 0) {
		return false;
	}

	return true;
}

bool cdIs2dPointInBlock(struct geoblock *tile, f32 x, f32 z)
{
	s32 result = -1;
	s32 numvertices = tile->header.numvertices;
	s32 i;

	for (i = 0; i < numvertices; i++) {
		s32 next = (i + 1) % numvertices;

		f32 value = (tile->vertices[next][1] - tile->vertices[i][1]) * (x - tile->vertices[i][0])
			- (tile->vertices[next][0] - tile->vertices[i][0]) * (z - tile->vertices[i][1]);

		if (value != 0) {
			if (i == 0 || result < 0) {
				result = (value > 0);
			} else {
				if (result != 0 && value < 0) {
					return false;
				}

				if (result == 0 && value > 0) {
					return false;
				}
			}
		}
	}

	if (result < 0) {
		return false;
	}

	return true;
}

bool cdIs2dPointInCyl(struct geocyl *cyl, f32 x, f32 z)
{
	f32 xdiff = x - cyl->x;
	f32 zdiff = z - cyl->z;

	return xdiff * xdiff + zdiff * zdiff <= cyl->radius * cyl->radius;
}

bool cd000266a4(f32 x, f32 z, struct geo *geo)
{
	if (geo == NULL) {
		return false;
	}

	if (geo->type == GEOTYPE_BLOCK) {
		return cdIs2dPointInBlock((struct geoblock *) geo, x, z);
	}

	if (geo->type == GEOTYPE_CYL) {
		return cdIs2dPointInCyl((struct geocyl *) geo, x, z);
	}

	return false;
}

/**
 * For a lift or escalator step, find the props which are riding on it.
 */
void cdGetPropsOnPlatform(struct prop *platform, s16 *propnums, s32 maxlen)
{
	u8 *start;
	u8 *end;
	s16 roompropnums[257];
	struct prop *prop;
	s16 *roompropnumptr;
	struct geo *geo;
	s32 len = 0;

	if (propUpdateGeometry(platform, &start, &end)) {
		roomGetProps(platform->rooms, roompropnums, 256);
		roompropnumptr = roompropnums;

		while (*roompropnumptr >= 0) {
			prop = &g_Vars.props[*roompropnumptr];

			if (prop != platform) {
				geo = (struct geo *) start;

				while (geo < (struct geo *) end) {
					if (geo->type == GEOTYPE_TILE_I) {
						struct geotilei *tile = (struct geotilei *) geo;
						geo = (struct geo *)((u32)geo + sizeof(struct geotilei) + sizeof(tile->vertices[0]) * (tile->header.numvertices - ARRAYCOUNT(tile->vertices)));
					} else if (geo->type == GEOTYPE_TILE_F) {
						struct geotilef *tile = (struct geotilef *) geo;
						struct coord *pos = &prop->pos;

						if ((geo->flags & (GEOFLAG_FLOOR1 | GEOFLAG_FLOOR2))
								&& pos->x >= tile->vertices[tile->xmin].x
								&& pos->x <= tile->vertices[tile->xmax].x
								&& pos->z >= tile->vertices[tile->zmin].z
								&& pos->z <= tile->vertices[tile->zmax].z
								&& pos->y >= tile->vertices[tile->ymin].y
								&& cdIs2dPointInFltTile(tile, pos->x, pos->z)
								&& pos->y >= cdFindGroundInFltTile(tile, pos->x, pos->z)) {
							break;
						}

						geo = (struct geo *)((u32)geo + sizeof(struct geotilef) + sizeof(struct coord) * (tile->header.numvertices - ARRAYCOUNT(tile->vertices)));
					} else if (geo->type == GEOTYPE_BLOCK) {
						geo = (struct geo *)((u32)geo + sizeof(struct geoblock));
					} else if (geo->type == GEOTYPE_CYL) {
						geo = (struct geo *)((u32)geo + sizeof(struct geocyl));
					}
				}

				if (geo < (struct geo *) end) {
					if (len < maxlen - 2) {
						propnums[len] = *roompropnumptr;
						len++;
					} else {
						break;
					}
				}
			}

			roompropnumptr++;
		}
	}

	propnums[len] = -1;
}

#if VERSION < VERSION_NTSC_1_0
void cdSetPropYBounds(struct prop *prop, f32 ymax, f32 ymin)
{
	u8 *start;
	u8 *end;

	if (propUpdateGeometry(prop, &start, &end)) {
		struct geo *geo = (struct geo *) start;

		while (geo < (struct geo *) end) {
			if (geo->type == GEOTYPE_TILE_I) {
				struct geotilei *tile = (struct geotilei *) geo;
				geo = (struct geo *)((u32)geo + sizeof(struct geotilei) + sizeof(tile->vertices[0]) * (tile->header.numvertices - ARRAYCOUNT(tile->vertices)));
			} else if (geo->type == GEOTYPE_TILE_F) {
				struct geotilef *tile = (struct geotilef *) geo;
				geo = (struct geo *)((u32)geo + sizeof(struct geotilef) + sizeof(struct coord) * (tile->header.numvertices - ARRAYCOUNT(tile->vertices)));
			} else if (geo->type == GEOTYPE_BLOCK) {
				struct geoblock *block = (struct geoblock *) geo;
				block->ymax = ymax;
				block->ymin = ymin;
				geo = (struct geo *)((u32)geo + sizeof(struct geoblock));
			} else if (geo->type == GEOTYPE_CYL) {
				struct geocyl *cyl = (struct geocyl *) geo;
				cyl->ymax = ymax;
				cyl->ymin = ymin;
				geo = (struct geo *)((u32)geo + sizeof(struct geocyl));
			}
		}
	}
}
#endif

bool cd00026a04(struct coord *pos, u8 *start, u8 *end, u16 geoflags, s32 room, struct geo **tileptr, s32 *roomptr, f32 *groundptr, bool ceiling)
{
	bool result = false;
	struct geo *geo = (struct geo *) start;

	if (room);

	while (geo < (struct geo *) end) {
		if (geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) geo;

			if ((geo->flags & geoflags)
					&& pos->x >= *(s16 *)(tile->xmin + (u32)tile)
					&& pos->x <= *(s16 *)(tile->xmax + (u32)tile)
					&& pos->z >= *(s16 *)(tile->zmin + (u32)tile)
					&& pos->z <= *(s16 *)(tile->zmax + (u32)tile)) {
				if ((!ceiling && pos->y >= *(s16 *)(tile->ymin + (u32)tile))
						|| (ceiling && pos->y <= *(s16 *)(tile->ymax + (u32)tile))) {
					if (cdIs2dPointInIntTile(tile, pos->x, pos->z)) {
						f32 ground = cdFindGroundInIntTile(tile, pos->x, pos->z);

						if ((!ceiling && ground <= pos->y && ground > *groundptr)
								|| (ceiling && ground >= pos->y && ground < *groundptr)) {
							*groundptr = ground;
							*tileptr = geo;
							*roomptr = room;
							result = true;
						}
					}
				}
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geotilei) + sizeof(tile->vertices[0]) * (tile->header.numvertices - ARRAYCOUNT(tile->vertices)));
		} else if (geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) geo;

			if ((geo->flags & geoflags)
					&& pos->x >= tile->vertices[tile->xmin].x
					&& pos->x <= tile->vertices[tile->xmax].x
					&& pos->z >= tile->vertices[tile->zmin].z
					&& pos->z <= tile->vertices[tile->zmax].z) {
				if ((!ceiling && pos->y >= tile->vertices[tile->ymin].y)
						|| (ceiling && pos->y <= tile->vertices[tile->ymax].y)) {
					if (cdIs2dPointInFltTile(tile, pos->x, pos->z)) {
						f32 ground = cdFindGroundInFltTile(tile, pos->x, pos->z);

						if ((!ceiling && pos->y >= ground && ground > *groundptr)
								|| (ceiling && pos->y <= ground && ground < *groundptr)) {
							*groundptr = ground;
							*tileptr = geo;
							*roomptr = room;
							result = true;
						}
					}
				}
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geotilef) + sizeof(struct coord) * (tile->header.numvertices - ARRAYCOUNT(tile->vertices)));
		} else if (geo->type == GEOTYPE_BLOCK) {
			geo = (struct geo *)((u32)geo + sizeof(struct geoblock));
		} else if (geo->type == GEOTYPE_CYL) {
			geo = (struct geo *)((u32)geo + sizeof(struct geocyl));
		}
	}

	return result;
}

void cdFindClosestVertical(struct coord *pos, s16 *rooms, u16 geoflags, struct geo **geoptr, s16 *roomptr, f32 *groundptr, struct prop **propptr, bool ceiling)
{
	s16 *roomptr2;
	s32 roomnum;
	u8 *start;
	u8 *end;
	f32 closesty;
	struct geo *geo = NULL;
	s32 room = 0;
	struct prop *bestprop = NULL;
	s16 *propnumptr;
	s16 propnums[256];

	if (ceiling) {
		closesty = 4294967296;
	} else {
		closesty = -4294967296;
	}

	roomptr2 = rooms;
	roomnum = rooms[0];

	while (roomnum != -1) {
		if (roomnum < g_TileNumRooms) {
			start = g_TileFileData.u8 + g_TileRooms[roomnum];
			end = g_TileFileData.u8 + g_TileRooms[roomnum + 1];

			cd00026a04(pos, start, end, geoflags, roomnum, &geo, &room, &closesty, ceiling);
		}

		roomptr2++;
		roomnum = *roomptr2;
	}

	roomGetProps(rooms, propnums, 256);
	propnumptr = propnums;

	while (*propnumptr >= 0) {
		struct prop *prop = &g_Vars.props[*propnumptr];

		if (propUpdateGeometry(prop, &start, &end)
				&& cd00026a04(pos, start, end, geoflags, prop->rooms[0], &geo, &room, &closesty, ceiling)) {
			bestprop = prop;
		}

		propnumptr++;
	}

	*geoptr = geo;
	*roomptr = room;
	*groundptr = closesty;

	if (propptr != NULL) {
		*propptr = bestprop;
	}
}

bool cd0002709cIntTile(struct geotilei *tile, f32 x, f32 z, f32 radius, struct prop *prop, struct collision *collision)
{
	bool result = false;

	if (cdIs2dPointInIntTile(tile, x, z)) {
		collision->geo = &tile->header;
		collision->vertexindex = 0;
		collision->prop = prop;
		result = true;
	} else {
		s32 numvertices = tile->header.numvertices;
		s32 i;

		for (i = 0; i < numvertices; i++) {
			s32 next = (i + 1) % numvertices;
			f32 value = cd00025654(tile->vertices[i][0], tile->vertices[i][2], tile->vertices[next][0], tile->vertices[next][2], x, z);

			if (value < 0) {
				value = -value;
			}

			if (value <= radius
					&& (cd00025724(tile->vertices[i][0], tile->vertices[i][2], x, z) <= radius
						|| cd00025724(tile->vertices[next][0], tile->vertices[next][2], x, z) <= radius
						|| cd00025774(tile->vertices[i][0], tile->vertices[i][2], tile->vertices[next][0], tile->vertices[next][2], x, z))) {
				collision->geo = &tile->header;
				collision->vertexindex = i;
				collision->prop = prop;
				result = true;
				break;
			}
		}
	}

	return result;
}

bool cd000272f8FltTile(struct geotilef *tile, f32 x, f32 z, f32 radius, struct prop *prop, struct collision *collision)
{
	bool result = false;

	if (cdIs2dPointInFltTile(tile, x, z)) {
		collision->geo = &tile->header;
		collision->vertexindex = 0;
		collision->prop = prop;
		result = true;
	} else {
		s32 numvertices = tile->header.numvertices;
		s32 i;

		for (i = 0; i < numvertices; i++) {
			s32 next = (i + 1) % numvertices;
			f32 value = cd00025654(tile->vertices[i].x, tile->vertices[i].z, tile->vertices[next].x, tile->vertices[next].z, x, z);

			if (value < 0) {
				value = -value;
			}

			if (value <= radius
					&& (cd00025724(tile->vertices[i].x, tile->vertices[i].z, x, z) <= radius
						|| cd00025724(tile->vertices[next].x, tile->vertices[next].z, x, z) <= radius
						|| cd00025774(tile->vertices[i].x, tile->vertices[i].z, tile->vertices[next].x, tile->vertices[next].z, x, z))) {
				collision->geo = &tile->header;
				collision->vertexindex = i;
				collision->prop = prop;
				result = true;
				break;
			}
		}
	}

	return result;
}

s32 cd000274e0Block(struct geoblock *tile, f32 x, f32 z, f32 radius, struct prop *prop, struct collision *collision)
{
	bool result = false;

	if (cdIs2dPointInBlock(tile, x, z)) {
		if (collision) {
			collision->geo = &tile->header;
			collision->vertexindex = 0;
			collision->prop = prop;
		}

		result = true;
	} else {
		s32 numvertices = tile->header.numvertices;
		s32 i;

		for (i = 0; i < numvertices; i++) {
			s32 next = (i + 1) % numvertices;
			f32 value = cd00025654(tile->vertices[i][0], tile->vertices[i][1],
					tile->vertices[next][0], tile->vertices[next][1],
					x, z);

			if (value < 0) {
				value = -value;
			}

			if (value <= radius
					&& (cd00025724(tile->vertices[i][0], tile->vertices[i][1], x, z) <= radius
						|| cd00025724(tile->vertices[next][0], tile->vertices[next][1], x, z) <= radius
						|| cd00025774(tile->vertices[i][0], tile->vertices[i][1], tile->vertices[next][0], tile->vertices[next][1], x, z))) {
				if (collision) {
					collision->geo = &tile->header;
					collision->vertexindex = i;
					collision->prop = prop;
				}

				result = true;
				break;
			}
		}
	}

	return result;
}

bool cd000276c8Cyl(struct geocyl *cyl, f32 x, f32 z, f32 radius, struct prop *prop, struct collision *collision)
{
	bool result = false;

	f32 sumx = x - cyl->x;
	f32 sumz = z - cyl->z;
	f32 sumwidth = cyl->radius + radius;

	if (sumx * sumx + sumz * sumz <= sumwidth * sumwidth) {
		result = true;

		if (collision) {
			collision->geo = &cyl->header;
			collision->vertexindex = 0;
			collision->prop = prop;
		}
	}

	return result;
}

s32 cdTestRampWall(struct geotilei *tile, struct coord *pos, f32 width, f32 y1, f32 y2);

void cdCollectGeoForCylFromList(struct coord *pos, f32 radius, u8 *start, u8 *end, u16 geoflags,
		bool checkvertical, f32 arg6, f32 arg7, struct prop *prop,
		struct collision *collisions, s32 maxcollisions, s32 *numcollisions, s32 roomnum)
{
	struct geo *geo = (struct geo *) start;
	s32 result;

	while (geo < (struct geo *) end) {
		if (geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) geo;

			if ((geo->flags & geoflags)
					&& pos->x >= *(s16 *)(tile->xmin + (u32)tile) - radius
					&& pos->x <= *(s16 *)(tile->xmax + (u32)tile) + radius
					&& pos->z >= *(s16 *)(tile->zmin + (u32)tile) - radius
					&& pos->z <= *(s16 *)(tile->zmax + (u32)tile) + radius
					&& (!checkvertical || (pos->y + arg6 >= *(s16 *)(tile->ymin + (u32)tile)
							&& pos->y + arg7 <= *(s16 *)(tile->ymax + (u32)tile)))) {
				if (geo->flags & GEOFLAG_RAMPWALL) {
					result = cdTestRampWall(tile, pos, radius, pos->y + arg7, pos->y + arg6);
				} else {
					result = 1;
				}

				if (result != 0) {
					if (cd0002709cIntTile(tile, pos->x, pos->z, radius, prop, &collisions[*numcollisions])) {
						collisions[*numcollisions].room = roomnum;
						*numcollisions = *numcollisions + 1;

						if (*numcollisions >= maxcollisions) {
							break;
						}
					}
				}
			}

			geo = (struct geo *)((u32)geo + tile->header.numvertices * 6 + 0xe);
		} else if (geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) geo;
			s32 tmp = 0x40;

			if ((geo->flags & geoflags)
					&& pos->x >= *(f32 *)((u32)tile + tile->xmin * 0xc + 0x10) - radius
					&& pos->x <= *(f32 *)((u32)tile + tile->xmax * 0xc + 0x10) + radius
					&& pos->z >= *(f32 *)((u32)tile + tile->zmin * 0xc + 0x18) - radius
					&& pos->z <= *(f32 *)((u32)tile + tile->zmax * 0xc + 0x18) + radius
					&& (!checkvertical || (pos->y + arg6 >= *(f32*)((u32)tile + tile->ymin * 0xc + 0x14)
							&& pos->y + arg7 <= *(f32 *)((u32)tile + tile->ymax * 0xc + 0x14)))) {
				result = cd000272f8FltTile(tile, pos->x, pos->z, radius, prop, &collisions[*numcollisions]);

				if (result != 0) {
					collisions[*numcollisions].room = roomnum;
					*numcollisions = *numcollisions + 1;

					if (*numcollisions >= maxcollisions) {
						break;
					}
				}
			}

			geo = (struct geo *)((u32)geo + (tile->header.numvertices - tmp) * 0xc + 0x310);
		} else if (geo->type == GEOTYPE_BLOCK) {
			struct geoblock *block = (struct geoblock *) geo;

			if ((geoflags & (GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT))
					&& (!checkvertical || (pos->y + arg6 >= block->ymin && pos->y + arg7 <= block->ymax))) {
				result = cd000274e0Block(block, pos->x, pos->z, radius, prop, &collisions[*numcollisions]);

				if (result) {
					collisions[*numcollisions].room = roomnum;
					*numcollisions = *numcollisions + 1;

					if (*numcollisions >= maxcollisions) {
						break;
					}
				}
			}

			geo = (struct geo *)((u32)geo + 0x4c);
		} else if (geo->type == GEOTYPE_CYL) {
			struct geocyl *cyl = (struct geocyl *) geo;

			if ((geoflags & geo->flags)
					&& (!checkvertical || (pos->y + arg6 >= cyl->ymin && pos->y + arg7 <= cyl->ymax))) {
				result = cd000276c8Cyl(cyl, pos->x, pos->z, radius, prop, &collisions[*numcollisions]);

				if (result) {
					collisions[*numcollisions].room = roomnum;
					*numcollisions = *numcollisions + 1;

					if (*numcollisions >= maxcollisions) {
						break;
					}
				}
			}

			geo = (struct geo *)((u32)geo + 0x18);
		}
	}
}

void cdCollectGeoForCyl(struct coord *pos, f32 radius, s16 *rooms, u32 types, u16 geoflags, bool checkvertical, f32 ymax, f32 ymin, struct collision *collisions, s32 maxcollisions)
{
	s16 *roomptr;
	s32 roomnum;
	u8 *start;
	u8 *end;
	s32 numcollisions = 0;
	s16 *propnumptr;
	s16 propnums[256];

	// Check BG
	if (types & CDTYPE_BG) {
		roomptr = rooms;
		roomnum = rooms[0];

		while (roomnum != -1) {
			if (roomnum < g_TileNumRooms) {
				start = g_TileFileData.u8 + g_TileRooms[roomnum];
				end = g_TileFileData.u8 + g_TileRooms[roomnum + 1];

				cdCollectGeoForCylFromList(pos, radius, start, end, geoflags, checkvertical, ymax, ymin, NULL, collisions, maxcollisions, &numcollisions, roomnum);

				if (numcollisions >= maxcollisions) {
					goto end;
				}
			}

			roomptr++;
			roomnum = *roomptr;
		}
	}

	// Check props
	roomGetProps(rooms, propnums, 256);
	propnumptr = propnums;

	while (*propnumptr >= 0) {
		struct prop *prop = &g_Vars.props[*propnumptr];

		if (propIsOfCdType(prop, types) && propUpdateGeometry(prop, &start, &end)) {
			cdCollectGeoForCylFromList(pos, radius, start, end, geoflags, checkvertical, ymax, ymin, prop, collisions, maxcollisions, &numcollisions, prop->rooms[0]);

			if (numcollisions >= maxcollisions) {
				break;
			}
		}

		propnumptr++;
	}

end:
	collisions[numcollisions].geo = NULL;
}

void cd00027f78(struct geotilei *tile, f32 arg1, f32 arg2, f32 arg3, struct prop *prop, struct collision *collisions, s32 maxcollisions, s32 *numcollisions)
{
	s32 i;
	s32 numvertices = tile->header.numvertices;

	for (i = 0; i < numvertices; i++) {
		s32 next = (i + 1) % numvertices;

		if (tile->vertices[i][0] != tile->vertices[next][0] || tile->vertices[i][2] != tile->vertices[next][2]) {
			f32 f0 = cd00025654(tile->vertices[i][0], tile->vertices[i][2], tile->vertices[next][0], tile->vertices[next][2], arg1, arg2);

			if (f0 < 0.0f) {
				f0 = -f0;
			}

			if (f0 <= arg3
					&& (cd00025724(tile->vertices[i][0], tile->vertices[i][2], arg1, arg2) <= arg3
						|| cd00025724(tile->vertices[next][0], tile->vertices[next][2], arg1, arg2) <= arg3
						|| cd00025774(tile->vertices[i][0], tile->vertices[i][2], tile->vertices[next][0], tile->vertices[next][2], arg1, arg2))) {
				if (*numcollisions < maxcollisions) {
					collisions[*numcollisions].geo = &tile->header;
					collisions[*numcollisions].vertexindex = i;
					collisions[*numcollisions].prop = prop;
					*numcollisions += 1;
				} else {
					break;
				}
			}
		}
	}
}

/**
 * Triangular wall tiles are generally implemented with just a bounding box check,
 * but this means if the player gets on top of one they can walk across it.
 * This is a problem for ramps that the player can jump off at any height
 * such as the Air Base staircase near the baggage terminal.
 *
 * To handle this, the ramp wall tiles are flagged with GEOFLAG_RAMPWALL.
 * When collision checks are being done, tiles with this flag are passed to
 * this function which does a more extensive check.
 */
s32 cdTestRampWall(struct geotilei *tile, struct coord *pos, f32 width, f32 y1, f32 y2)
{
	s32 count;
	s32 i;
	s32 y1count;
	s32 y2count;
	s32 numverts;

	if (!g_Vars.enableslopes && (tile->header.flags & GEOFLAG_SLOPE)) {
		return 0;
	}

	numverts = tile->header.numvertices;
	y2count = 0;
	y1count = 0;
	count = 0;

	for (i = 0; i < numverts; i++) {
		s32 next = i + 1;
		s32 last = numverts - 1;
		f32 posval;
		f32 thisvals[2];
		f32 nextvals[2];
		f32 somefloat;
		f32 somefloat2;
		s32 xdiff;
		s32 zdiff;

		if (i == last) {
			next = 0;
		}

		xdiff = tile->vertices[next][0] - tile->vertices[i][0];
		zdiff = tile->vertices[next][2] - tile->vertices[i][2];

		if (xdiff < 0) {
			xdiff = -xdiff;
		}

		if (zdiff < 0) {
			zdiff = -zdiff;
		}

		if (xdiff || zdiff) {
			thisvals[1] = tile->vertices[i][1];
			nextvals[1] = tile->vertices[next][1];

			if (zdiff < xdiff) {
				thisvals[0] = tile->vertices[i][0];
				nextvals[0] = tile->vertices[next][0];
				posval = pos->x;
			} else {
				thisvals[0] = tile->vertices[i][2];
				nextvals[0] = tile->vertices[next][2];
				posval = pos->z;
			}

			somefloat = (posval - thisvals[0]) / (nextvals[0] - thisvals[0]);

			if (somefloat <= 1.0f && somefloat >= 0.0f) {
				somefloat2 = thisvals[1] + (nextvals[1] - thisvals[1]) * somefloat;

				if (somefloat2 >= y2 - 1.0f) {
					y2count++;

					if (y1count != 0) {
						count++;
					}
				} else if (somefloat2 <= y1 + 1.0f) {
					y1count++;

					if (y2count != 0) {
						count++;
					}
				} else {
					count++;
				}
			}
		}
	}

	return count;
}

void cd0002840c(struct geotilef *tile, f32 arg1, f32 arg2, f32 arg3, struct prop *prop, struct collision *collisions, s32 maxcollisions, s32 *numcollisions)
{
	s32 i;
	s32 numvertices = tile->header.numvertices;

	for (i = 0; i < numvertices; i++) {
		s32 next = (i + 1) % numvertices;

		if (tile->vertices[i].x != tile->vertices[next].x || tile->vertices[i].z != tile->vertices[next].z) {
			f32 f0 = cd00025654(tile->vertices[i].x, tile->vertices[i].z, tile->vertices[next].x, tile->vertices[next].z, arg1, arg2);

			if (f0 < 0.0f) {
				f0 = -f0;
			}

			if (f0 <= arg3
					&& (cd00025724(tile->vertices[i].x, tile->vertices[i].z, arg1, arg2) <= arg3
						|| cd00025724(tile->vertices[next].x, tile->vertices[next].z, arg1, arg2) <= arg3
						|| cd00025774(tile->vertices[i].x, tile->vertices[i].z, tile->vertices[next].x, tile->vertices[next].z, arg1, arg2))) {
				if (*numcollisions < maxcollisions) {
					collisions[*numcollisions].geo = &tile->header;
					collisions[*numcollisions].vertexindex = i;
					collisions[*numcollisions].prop = prop;
					*numcollisions += 1;
				} else {
					break;
				}
			}
		}
	}
}

void cd00028638(struct geoblock *block, f32 arg1, f32 arg2, f32 arg3, struct prop *prop, struct collision *collisions, s32 maxcollisions, s32 *numcollisions)
{
	s32 i;
	s32 numvertices = block->header.numvertices;

	for (i = 0; i < numvertices; i++) {
		s32 next = (i + 1) % numvertices;

		if (block->vertices[i][0] != block->vertices[next][0] || block->vertices[i][1] != block->vertices[next][1]) {
			f32 f0 = cd00025654(block->vertices[i][0], block->vertices[i][1], block->vertices[next][0], block->vertices[next][1], arg1, arg2);

			if (f0 < 0.0f) {
				f0 = -f0;
			}

			if (f0 <= arg3
					&& (cd00025724(block->vertices[i][0], block->vertices[i][1], arg1, arg2) <= arg3
						|| cd00025724(block->vertices[next][0], block->vertices[next][1], arg1, arg2) <= arg3
						|| cd00025774(block->vertices[i][0], block->vertices[i][1], block->vertices[next][0], block->vertices[next][1], arg1, arg2))) {
				if (*numcollisions < maxcollisions) {
					collisions[*numcollisions].geo = &block->header;
					collisions[*numcollisions].vertexindex = i;
					collisions[*numcollisions].prop = prop;
					*numcollisions += 1;
				} else {
					break;
				}
			}
		}
	}
}

void cd0002885c(struct geocyl *cyl, f32 x, f32 z, f32 arg3, struct prop *prop, struct collision *collisions, s32 maxcollisions, s32 *numcollisions)
{
	f32 xdiff = x - cyl->x;
	f32 zdiff = z - cyl->z;
	f32 f16 = arg3 + cyl->radius;

	if (xdiff * xdiff + zdiff * zdiff <= f16 * f16) {
		if (*numcollisions < maxcollisions) {
			collisions[*numcollisions].geo = &cyl->header;
			collisions[*numcollisions].vertexindex = 0;
			collisions[*numcollisions].prop = prop;
			*numcollisions += 1;
		}
	}
}

void cdCollectGeoForCylMoveFromList(u8 *start, u8 *end, struct coord *pos, f32 radius, u16 geoflags,
		bool checkvertical, f32 arg6, f32 arg7, struct prop *prop,
		struct collision *collisions, s32 maxcollisions, s32 *numcollisions)
{
	struct geo *geo = (struct geo *) start;

	while (geo < (struct geo *) end) {
		if (geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) geo;

			if (geo->flags & geoflags) {
				if (pos->x >= *(s16 *)(tile->xmin + (u32)tile) - radius
						&& pos->x <= *(s16 *)(tile->xmax + (u32)tile) + radius
						&& pos->z >= *(s16 *)(tile->zmin + (u32)tile) - radius
						&& pos->z <= *(s16 *)(tile->zmax + (u32)tile) + radius
						&& (!checkvertical || (pos->y + arg6 >= *(s16 *)(tile->ymin + (u32)tile)
								&& pos->y + arg7 <= *(s16 *)(tile->ymax + (u32)tile)))) {
					bool pass;

					if (geo->flags & GEOFLAG_RAMPWALL) {
						pass = cdTestRampWall(tile, pos, radius, pos->y + arg7, pos->y + arg6);
					} else {
						pass = true;
					}

					if (pass) {
						cd00027f78(tile, pos->x, pos->z, radius, prop, collisions, maxcollisions, numcollisions);
					}
				}
			}

			geo = (struct geo *)((u32)geo + tile->header.numvertices * 6 + 0xe);
		} else if (geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) geo;

			if ((geo->flags & geoflags)
					&& pos->x >= tile->vertices[tile->xmin].x - radius
					&& pos->x <= tile->vertices[tile->xmax].x + radius
					&& pos->z >= tile->vertices[tile->zmin].z - radius
					&& pos->z <= tile->vertices[tile->zmax].z + radius
					&& (!checkvertical || (pos->y + arg6 >= tile->vertices[tile->ymin].y
							&& pos->y + arg7 <= tile->vertices[tile->ymax].y))) {
				cd0002840c(tile, pos->x, pos->z, radius, prop, collisions, maxcollisions, numcollisions);
			}

			geo = (struct geo *)((u32)geo + (u32)(tile->header.numvertices - 0x40) * 0xc + 0x310);
		} else if (geo->type == GEOTYPE_BLOCK) {
			struct geoblock *block = (struct geoblock *) geo;

			if ((geoflags & (GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT))
					&& (!checkvertical || (pos->y + arg6 >= block->ymin && pos->y + arg7 <= block->ymax))) {
				cd00028638(block, pos->x, pos->z, radius, prop, collisions, maxcollisions, numcollisions);
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geoblock));
		} else if (geo->type == GEOTYPE_CYL) {
			struct geocyl *cyl = (struct geocyl *) geo;

			if ((geoflags & geo->flags)
					&& (!checkvertical || (pos->y + arg6 >= cyl->ymin && pos->y + arg7 <= cyl->ymax))) {
				cd0002885c(cyl, pos->x, pos->z, radius, prop, collisions, maxcollisions, numcollisions);
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geocyl));
		}
	}
}

void cdCollectGeoForCylMove(struct coord *pos, f32 width, s16 *rooms, u32 types, u16 geoflags, bool checkvertical, f32 ymax, f32 ymin, struct collision *collisions, s32 maxcollisions)
{
	s16 *roomptr;
	s32 roomnum;
	u8 *start;
	u8 *end;
	s32 numcollisions = 0;
	s16 *propnumptr;
	s16 propnums[256];

	// Check BG
	if (types & CDTYPE_BG) {
		roomptr = rooms;
		roomnum = rooms[0];

		while (roomnum != -1) {
			if (roomnum < g_TileNumRooms) {
				start = g_TileFileData.u8 + g_TileRooms[roomnum];
				end = g_TileFileData.u8 + g_TileRooms[roomnum + 1];

				cdCollectGeoForCylMoveFromList(start, end, pos, width, geoflags, checkvertical, ymax, ymin, NULL, collisions, maxcollisions, &numcollisions);
			}

			roomptr++;
			roomnum = *roomptr;
		}
	}

	// Check props
	roomGetProps(rooms, propnums, 256);
	propnumptr = propnums;

	while (*propnumptr >= 0) {
		struct prop *prop = &g_Vars.props[*propnumptr];

		if (propIsOfCdType(prop, types) && propUpdateGeometry(prop, &start, &end)) {
			cdCollectGeoForCylMoveFromList(start, end, pos, width, geoflags, checkvertical, ymax, ymin, prop, collisions, maxcollisions, &numcollisions);
		}

		propnumptr++;
	}

	collisions[numcollisions].geo = NULL;
}

void cd0002901c(struct coord *pos, struct coord *dist, f32 width, struct collision *collisions)
{
	s32 i;
	struct widthxz spf8;
	struct xz spf0;
	struct xz spe8;
	struct xz spe0;
	f32 bestvalue = 0.0f;
	s32 bestindex = -1;
	u32 stack;
	f32 value;
	s32 curr;
	s32 next;
	struct coord vtx1;
	struct coord vtx2;
	struct geo *geo;

	for (i = 0; (geo = collisions[i].geo) != NULL; i++) {
		if (1);
		if (geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) geo;

			spf8.width = width;
			spf8.x = pos->x;
			spf8.z = pos->z;

			spe0.x = dist->x;
			spe0.z = dist->z;

			curr = collisions[i].vertexindex;
			next = (curr + 1) % tile->header.numvertices;

			spf0.x = tile->vertices[curr][0];
			spf0.z = tile->vertices[curr][2];

			spe8.x = tile->vertices[next][0];
			spe8.z = tile->vertices[next][2];

			value = func0f1579cc(&spf8, &spf0, &spe8, &spe0);

			if (bestindex < 0 || value < bestvalue) {
				bestvalue = value;
				bestindex = i;
			}
		} else if (geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) geo;

			spf8.width = width;
			spf8.x = pos->x;
			spf8.z = pos->z;

			spe0.x = dist->x;
			spe0.z = dist->z;

			curr = collisions[i].vertexindex;
			next = (curr + 1) % tile->header.numvertices;

			spf0.x = tile->vertices[curr].x;
			spf0.z = tile->vertices[curr].z;

			spe8.x = tile->vertices[next].x;
			spe8.z = tile->vertices[next].z;

			value = func0f1579cc(&spf8, &spf0, &spe8, &spe0);

			if (bestindex < 0 || value < bestvalue) {
				bestvalue = value;
				bestindex = i;
			}
		} else if (geo->type == GEOTYPE_BLOCK) {
			struct geoblock *block = (struct geoblock *) geo;

			spf8.width = width;
			spf8.x = pos->x;
			spf8.z = pos->z;

			spe0.x = dist->x;
			spe0.z = dist->z;

			curr = collisions[i].vertexindex;
			next = (curr + 1) % block->header.numvertices;

			spf0.x = block->vertices[curr][0];
			spf0.z = block->vertices[curr][1];

			spe8.x = block->vertices[next][0];
			spe8.z = block->vertices[next][1];

			value = func0f1579cc(&spf8, &spf0, &spe8, &spe0);

			if (bestindex < 0 || value < bestvalue) {
				bestvalue = value;
				bestindex = i;
			}
		} else if (geo->type == GEOTYPE_CYL) {
			struct geocyl *cyl = (struct geocyl *) geo;

			spf8.width = cyl->radius + width;
			spf8.x = pos->x;
			spf8.z = pos->z;

			spe0.x = dist->x;
			spe0.z = dist->z;

			spf0.x = cyl->x;
			spf0.z = cyl->z;

			spe8.x = cyl->x;
			spe8.z = cyl->z;

			value = func0f1579cc(&spf8, &spf0, &spe8, &spe0);

			if (bestindex < 0 || value < bestvalue) {
				bestvalue = value;
				bestindex = i;
			}
		}
	}

	if (collisions[bestindex].geo->type == GEOTYPE_TILE_I) {
		struct geotilei *tile = (struct geotilei *) collisions[bestindex].geo;
		s32 curr = collisions[bestindex].vertexindex;
		s32 next = (curr + 1) % tile->header.numvertices;

		vtx1.x = tile->vertices[curr][0];
		vtx1.y = tile->vertices[curr][1];
		vtx1.z = tile->vertices[curr][2];

		vtx2.x = tile->vertices[next][0];
		vtx2.y = tile->vertices[next][1];
		vtx2.z = tile->vertices[next][2];
	} else if (collisions[bestindex].geo->type == GEOTYPE_TILE_F) {
		struct geotilef *tile = (struct geotilef *) collisions[bestindex].geo;
		s32 curr = collisions[bestindex].vertexindex;
		s32 next = (curr + 1) % tile->header.numvertices;

		vtx1 = tile->vertices[curr];

		vtx2 = tile->vertices[next];
	} else if (collisions[bestindex].geo->type == GEOTYPE_BLOCK) {
		struct geoblock *block = (struct geoblock *) collisions[bestindex].geo;
		s32 curr = collisions[bestindex].vertexindex;
		s32 next = (curr + 1) % block->header.numvertices;

		vtx1.x = block->vertices[curr][0];
		vtx1.y = pos->y;
		vtx1.z = block->vertices[curr][1];

		vtx2.x = block->vertices[next][0];
		vtx2.y = pos->y;
		vtx2.z = block->vertices[next][1];
	} else if (collisions[bestindex].geo->type == GEOTYPE_CYL) {
		struct geocyl *cyl = (struct geocyl *) collisions[bestindex].geo;

		cd00025848(cyl->x, cyl->z, cyl->radius, pos->x, pos->z, &vtx1.x, &vtx1.z, &vtx2.x, &vtx2.z);

		vtx1.y = pos->y;
		vtx2.y = pos->y;
	}

	cdSetObstacleVtxPropFlt(&vtx1, &vtx2, collisions[bestindex].prop, bestvalue);
}

f32 cdFindGroundFromList(struct collision *collisions, struct coord *pos, struct collision **collisionptr, f32 width);

#if VERSION >= VERSION_NTSC_1_0
// NTSC beta mismatches due to swapped s5/s6 registers
// collision should be s6; next should be s5
f32 cdFindGroundFromList(struct collision *collisions, struct coord *pos, struct collision **collisionptr, f32 width)
{
	struct collision *collision;
	s32 i;
	f32 curground = -4294967296;
	bool hasground = false;
	bool anyintile = false;
	bool hasflag0100 = false;
#if VERSION >= VERSION_NTSC_1_0
	bool hasdie = false;
	bool hasgroundfromearlier;
	bool isdie;
#else
	u32 unused1;
#endif
	f32 nextvalue;
	f32 spe4;
	f32 f30;
	f32 x;
	f32 z;
	f32 spd4;
	f32 f14;
	f32 spb4;
	f32 thisvalue;
	u32 unused2;
	s32 next;
	s32 numvertices;
	f32 spb8;
#if VERSION >= VERSION_NTSC_1_0
	u32 unused3[8];
#else
	u32 unused3[7];
#endif
	f32 sp94;
	u32 unused4[6];
	f32 sp78;
	f32 sp74;
	f32 nextx;
	f32 nextz;
	f32 ground;
	u32 unused5[5];
	f32 thisx;
	f32 thisz;

	*collisionptr = NULL;

	for (collision = collisions; collision->geo != NULL; collision++) {
		if (collision->geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) collision->geo;

			if (tile->header.flags & GEOFLAG_DIE) {
				collision->intile = false;
			} else {
				if (tile->header.flags & GEOFLAG_SLOPE) {
					hasflag0100 = true;
				}

				collision->intile = cdIs2dPointInIntTile(tile, pos->x, pos->z);

				if (collision->intile) {
					anyintile = true;
				}
			}
		} else if (collision->geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) collision->geo;

			collision->intile = cdIs2dPointInFltTile(tile, pos->x, pos->z);

			if (collision->intile) {
				anyintile = true;
			}
		}
	}

	if (anyintile) {
		for (collision = collisions; collision->geo != NULL; collision++) {
			if (collision->intile) {
				if (collision->geo->type == GEOTYPE_TILE_I) {
					struct geotilei *tile = (struct geotilei *) collision->geo;

					if ((tile->header.flags & GEOFLAG_STEP) == 0) {
						ground = cdFindGroundInIntTile((void *)collision->geo, pos->x, pos->z);

						if (ground >= curground && ground < pos->y) {
							curground = ground;
							*collisionptr = collision;
							hasground = true;
						}
					}
				} else if (collision->geo->type == GEOTYPE_TILE_F) {
					struct geotilef *tile = (struct geotilef *) collision->geo;

					ground = cdFindGroundInFltTile((void *)collision->geo, pos->x, pos->z);

					if (ground >= curground && ground < pos->y) {
						curground = ground;
						*collisionptr = collision;
						hasground = true;
					}
				}
			}
		}

		for (collision = collisions; collision->geo != NULL; collision++) {
			if (collision->intile) {
				if (collision->geo->type == GEOTYPE_TILE_I) {
					struct geotilei *tile = (struct geotilei *) collision->geo;

					if (tile->header.flags & GEOFLAG_STEP) {
						ground = cdFindGroundInIntTile((void *)collision->geo, pos->x, pos->z);

						if (ground >= curground && (ground < pos->y || !hasground)) {
							curground = ground;
							*collisionptr = collision;
							hasground = true;
						}
					}
				}
			}
		}
	}

#if VERSION >= VERSION_NTSC_1_0
	hasgroundfromearlier = hasground;
#endif

	if (!hasground || hasflag0100) {
		spe4 = 4294967296.0f;

		for (collision = collisions; collision->geo != NULL; collision++) {
#if VERSION >= VERSION_NTSC_1_0
			if (collision->intile == false
					&& (!hasgroundfromearlier || (collision->geo->type == GEOTYPE_TILE_I && (collision->geo->flags & GEOFLAG_SLOPE))))
#else
			if (collision->intile == false
					&& (!hasground || (collision->geo->type == GEOTYPE_TILE_I && (collision->geo->flags & GEOFLAG_SLOPE))))
#endif
			{
				if (collision->geo->type == GEOTYPE_TILE_I) {
					struct geotilei *tile = (struct geotilei *) collision->geo;
					numvertices = tile->header.numvertices;
#if VERSION >= VERSION_NTSC_1_0
					isdie = (tile->header.flags & GEOFLAG_DIE) != 0;
#endif

#if VERSION >= VERSION_NTSC_1_0
					if (!isdie || !hasground)
#endif
					{
						for (i = 0; i < numvertices; i++) {
							thisx = tile->vertices[i][0];
							thisz = tile->vertices[i][2];

							next = (i + 1) % numvertices;

							nextx = tile->vertices[next][0];
							nextz = tile->vertices[next][2];

							spd4 = cd00025654(thisx, thisz, nextx, nextz, pos->x, pos->z);
							f30 = spd4;

							if (f30 < 0.0f) {
								f30 = -f30;
							}

#if VERSION >= VERSION_NTSC_1_0
							if (f30 < spe4 || hasdie)
#else
							if (f30 < spe4)
#endif
							{
								if (cd00025774(thisx, thisz, nextx, nextz, pos->x, pos->z)) {
									spb8 = nextx - thisx;
									spb4 = nextz - thisz;
									f14 = spd4 / sqrtf(spb8 * spb8 + spb4 * spb4);
									x = pos->x + f14 * -spb4;
									z = pos->z + f14 * spb8;

									ground = cdFindGroundInIntTileAtVertex(tile, x, z, i);

									if (ground < pos->y || (collision->geo->flags & GEOFLAG_STEP)) {
										curground = ground;
										*collisionptr = collision;
										spe4 = f30;
#if VERSION >= VERSION_NTSC_1_0
										hasground = true;
										hasdie = isdie;
#endif
									}
								} else {
									thisvalue = cd00025724(thisx, thisz, pos->x, pos->z);
									nextvalue = cd00025724(nextx, nextz, pos->x, pos->z);

									if (thisvalue < nextvalue) {
#if VERSION >= VERSION_NTSC_1_0
										if (thisvalue < spe4 || hasdie)
#else
										if (thisvalue < spe4)
#endif
										{
											x = tile->vertices[i][0];
											z = tile->vertices[i][2];
											ground = cdFindGroundInIntTileAtVertex(tile, x, z, i);

											if (ground < pos->y || (collision->geo->flags & GEOFLAG_STEP)) {
												curground = ground;
												*collisionptr = collision;
												spe4 = thisvalue;
#if VERSION >= VERSION_NTSC_1_0
												hasground = true;
												hasdie = isdie;
#endif
											}
										}
									} else {
#if VERSION >= VERSION_NTSC_1_0
										if (nextvalue < spe4 || hasdie)
#else
										if (nextvalue < spe4)
#endif
										{
											x = tile->vertices[next][0];
											z = tile->vertices[next][2];
											ground = cdFindGroundInIntTileAtVertex(tile, x, z, i);

											if (ground < pos->y || (collision->geo->flags & GEOFLAG_STEP)) {
												curground = ground;
												*collisionptr = collision;
												spe4 = nextvalue;
#if VERSION >= VERSION_NTSC_1_0
												hasground = true;
												hasdie = isdie;
#endif
											}
										}
									}
								}
							}
						}
					}
				} else if (collision->geo->type == GEOTYPE_TILE_F) {
					struct geotilef *tile = (struct geotilef *) collision->geo;
					s32 numvertices = tile->header.numvertices;
					s32 i;

					for (i = 0; i < numvertices; i++) {
						thisx = tile->vertices[i].x;
						thisz = tile->vertices[i].z;

						next = (i + 1) % numvertices;

						nextx = tile->vertices[next].x;
						nextz = tile->vertices[next].z;

						sp94 = cd00025654(thisx, thisz, nextx, nextz, pos->x, pos->z);
						f30 = sp94;

						if (f30 < 0.0f) {
							f30 = -f30;
						}

						if (f30 < spe4) {
							if (cd00025774(thisx, thisz, nextx, nextz, pos->x, pos->z)) {
								sp78 = nextx - thisx;
								sp74 = nextz - thisz;
								f14 = sp94 / sqrtf(sp78 * sp78 + sp74 * sp74);
								x = pos->x + f14 * -sp74;
								z = pos->z + f14 * sp78;

								ground = cdFindGroundInFltTile(tile, x, z);

								if (ground < pos->y) {
									curground = ground;
									*collisionptr = collision;
									spe4 = f30;
#if VERSION >= VERSION_NTSC_1_0
									hasground = true;
									hasdie = false;
#endif
								}
							} else {
								thisvalue = cd00025724(thisx, thisz, pos->x, pos->z);
								nextvalue = cd00025724(nextx, nextz, pos->x, pos->z);

								if (thisvalue < nextvalue) {
									if (thisvalue < spe4) {
										x = tile->vertices[i].x;
										z = tile->vertices[i].z;
										ground = cdFindGroundInFltTile(tile, x, z);

										if (ground < pos->y) {
											curground = ground;
											*collisionptr = collision;
											spe4 = thisvalue;
#if VERSION >= VERSION_NTSC_1_0
											hasground = true;
											hasdie = false;
#endif
										}
									}
								} else {
									if (nextvalue < spe4) {
										x = tile->vertices[next].x;
										z = tile->vertices[next].z;
										ground = cdFindGroundInFltTile(tile, x, z);

										if (ground < pos->y) {
											curground = ground;
											*collisionptr = collision;
											spe4 = nextvalue;
#if VERSION >= VERSION_NTSC_1_0
											hasground = true;
											hasdie = false;
#endif
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return curground;
}
#else
GLOBAL_ASM(
glabel cdFindGroundFromList
/*    2ab9c:	27bdfef8 */ 	addiu	$sp,$sp,-264
/*    2aba0:	3c01cf80 */ 	lui	$at,0xcf80
/*    2aba4:	44812000 */ 	mtc1	$at,$f4
/*    2aba8:	afbf0074 */ 	sw	$ra,0x74($sp)
/*    2abac:	afbe0070 */ 	sw	$s8,0x70($sp)
/*    2abb0:	afb7006c */ 	sw	$s7,0x6c($sp)
/*    2abb4:	afb60068 */ 	sw	$s6,0x68($sp)
/*    2abb8:	afb50064 */ 	sw	$s5,0x64($sp)
/*    2abbc:	afb40060 */ 	sw	$s4,0x60($sp)
/*    2abc0:	afb3005c */ 	sw	$s3,0x5c($sp)
/*    2abc4:	afb20058 */ 	sw	$s2,0x58($sp)
/*    2abc8:	afb10054 */ 	sw	$s1,0x54($sp)
/*    2abcc:	afb00050 */ 	sw	$s0,0x50($sp)
/*    2abd0:	f7be0048 */ 	sdc1	$f30,0x48($sp)
/*    2abd4:	f7bc0040 */ 	sdc1	$f28,0x40($sp)
/*    2abd8:	f7ba0038 */ 	sdc1	$f26,0x38($sp)
/*    2abdc:	f7b80030 */ 	sdc1	$f24,0x30($sp)
/*    2abe0:	f7b60028 */ 	sdc1	$f22,0x28($sp)
/*    2abe4:	f7b40020 */ 	sdc1	$f20,0x20($sp)
/*    2abe8:	afa70114 */ 	sw	$a3,0x114($sp)
/*    2abec:	afa000f8 */ 	sw	$zero,0xf8($sp)
/*    2abf0:	e7a400fc */ 	swc1	$f4,0xfc($sp)
/*    2abf4:	acc00000 */ 	sw	$zero,0x0($a2)
/*    2abf8:	8c8e0000 */ 	lw	$t6,0x0($a0)
/*    2abfc:	00a09025 */ 	or	$s2,$a1,$zero
/*    2ac00:	00809825 */ 	or	$s3,$a0,$zero
/*    2ac04:	00c0f025 */ 	or	$s8,$a2,$zero
/*    2ac08:	00008025 */ 	or	$s0,$zero,$zero
/*    2ac0c:	00008825 */ 	or	$s1,$zero,$zero
/*    2ac10:	11c00025 */ 	beqz	$t6,.NB0002aca8
/*    2ac14:	0080b025 */ 	or	$s6,$a0,$zero
/*    2ac18:	8ec70000 */ 	lw	$a3,0x0($s6)
/*    2ac1c:	90e20000 */ 	lbu	$v0,0x0($a3)
.NB0002ac20:
/*    2ac20:	24010001 */ 	addiu	$at,$zero,0x1
/*    2ac24:	14400013 */ 	bnez	$v0,.NB0002ac74
/*    2ac28:	00000000 */ 	sll	$zero,$zero,0x0
/*    2ac2c:	94e20002 */ 	lhu	$v0,0x2($a3)
/*    2ac30:	00e02025 */ 	or	$a0,$a3,$zero
/*    2ac34:	304f4000 */ 	andi	$t7,$v0,0x4000
/*    2ac38:	51e00004 */ 	beqzl	$t7,.NB0002ac4c
/*    2ac3c:	30580100 */ 	andi	$t8,$v0,0x100
/*    2ac40:	10000015 */ 	beqz	$zero,.NB0002ac98
/*    2ac44:	aec00004 */ 	sw	$zero,0x4($s6)
/*    2ac48:	30580100 */ 	andi	$t8,$v0,0x100
.NB0002ac4c:
/*    2ac4c:	53000003 */ 	beqzl	$t8,.NB0002ac5c
/*    2ac50:	8e450000 */ 	lw	$a1,0x0($s2)
/*    2ac54:	24110001 */ 	addiu	$s1,$zero,0x1
/*    2ac58:	8e450000 */ 	lw	$a1,0x0($s2)
.NB0002ac5c:
/*    2ac5c:	0c009da6 */ 	jal	cdIs2dPointInIntTile
/*    2ac60:	8e460008 */ 	lw	$a2,0x8($s2)
/*    2ac64:	1040000c */ 	beqz	$v0,.NB0002ac98
/*    2ac68:	aec20004 */ 	sw	$v0,0x4($s6)
/*    2ac6c:	1000000a */ 	beqz	$zero,.NB0002ac98
/*    2ac70:	24100001 */ 	addiu	$s0,$zero,0x1
.NB0002ac74:
/*    2ac74:	54410009 */ 	bnel	$v0,$at,.NB0002ac9c
/*    2ac78:	8ec70014 */ 	lw	$a3,0x14($s6)
/*    2ac7c:	00e02025 */ 	or	$a0,$a3,$zero
/*    2ac80:	8e450000 */ 	lw	$a1,0x0($s2)
/*    2ac84:	0c009dfd */ 	jal	cdIs2dPointInFltTile
/*    2ac88:	8e460008 */ 	lw	$a2,0x8($s2)
/*    2ac8c:	10400002 */ 	beqz	$v0,.NB0002ac98
/*    2ac90:	aec20004 */ 	sw	$v0,0x4($s6)
/*    2ac94:	24100001 */ 	addiu	$s0,$zero,0x1
.NB0002ac98:
/*    2ac98:	8ec70014 */ 	lw	$a3,0x14($s6)
.NB0002ac9c:
/*    2ac9c:	26d60014 */ 	addiu	$s6,$s6,0x14
/*    2aca0:	54e0ffdf */ 	bnezl	$a3,.NB0002ac20
/*    2aca4:	90e20000 */ 	lbu	$v0,0x0($a3)
.NB0002aca8:
/*    2aca8:	52000065 */ 	beqzl	$s0,.NB0002ae40
/*    2acac:	8faa00f8 */ 	lw	$t2,0xf8($sp)
/*    2acb0:	8e620000 */ 	lw	$v0,0x0($s3)
/*    2acb4:	0260b025 */ 	or	$s6,$s3,$zero
/*    2acb8:	1040003a */ 	beqz	$v0,.NB0002ada4
/*    2acbc:	00000000 */ 	sll	$zero,$zero,0x0
/*    2acc0:	8ed90004 */ 	lw	$t9,0x4($s6)
.NB0002acc4:
/*    2acc4:	53200032 */ 	beqzl	$t9,.NB0002ad90
/*    2acc8:	8ecc0014 */ 	lw	$t4,0x14($s6)
/*    2accc:	8ec70000 */ 	lw	$a3,0x0($s6)
/*    2acd0:	24010001 */ 	addiu	$at,$zero,0x1
/*    2acd4:	90e20000 */ 	lbu	$v0,0x0($a3)
/*    2acd8:	14400018 */ 	bnez	$v0,.NB0002ad3c
/*    2acdc:	00000000 */ 	sll	$zero,$zero,0x0
/*    2ace0:	94e80002 */ 	lhu	$t0,0x2($a3)
/*    2ace4:	31092000 */ 	andi	$t1,$t0,0x2000
/*    2ace8:	55200029 */ 	bnezl	$t1,.NB0002ad90
/*    2acec:	8ecc0014 */ 	lw	$t4,0x14($s6)
/*    2acf0:	00e02025 */ 	or	$a0,$a3,$zero
/*    2acf4:	8e450000 */ 	lw	$a1,0x0($s2)
/*    2acf8:	0c009ce8 */ 	jal	cdFindGroundInIntTile
/*    2acfc:	8e460008 */ 	lw	$a2,0x8($s2)
/*    2ad00:	c7a600fc */ 	lwc1	$f6,0xfc($sp)
/*    2ad04:	4600303e */ 	c.le.s	$f6,$f0
/*    2ad08:	00000000 */ 	sll	$zero,$zero,0x0
/*    2ad0c:	45020020 */ 	bc1fl	.NB0002ad90
/*    2ad10:	8ecc0014 */ 	lw	$t4,0x14($s6)
/*    2ad14:	c6480004 */ 	lwc1	$f8,0x4($s2)
/*    2ad18:	240a0001 */ 	addiu	$t2,$zero,0x1
/*    2ad1c:	4608003c */ 	c.lt.s	$f0,$f8
/*    2ad20:	00000000 */ 	sll	$zero,$zero,0x0
/*    2ad24:	4502001a */ 	bc1fl	.NB0002ad90
/*    2ad28:	8ecc0014 */ 	lw	$t4,0x14($s6)
/*    2ad2c:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2ad30:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2ad34:	10000015 */ 	beqz	$zero,.NB0002ad8c
/*    2ad38:	afaa00f8 */ 	sw	$t2,0xf8($sp)
.NB0002ad3c:
/*    2ad3c:	54410014 */ 	bnel	$v0,$at,.NB0002ad90
/*    2ad40:	8ecc0014 */ 	lw	$t4,0x14($s6)
/*    2ad44:	00e02025 */ 	or	$a0,$a3,$zero
/*    2ad48:	8e450000 */ 	lw	$a1,0x0($s2)
/*    2ad4c:	0c009d37 */ 	jal	cdFindGroundInFltTile
/*    2ad50:	8e460008 */ 	lw	$a2,0x8($s2)
/*    2ad54:	c7aa00fc */ 	lwc1	$f10,0xfc($sp)
/*    2ad58:	4600503e */ 	c.le.s	$f10,$f0
/*    2ad5c:	00000000 */ 	sll	$zero,$zero,0x0
/*    2ad60:	4502000b */ 	bc1fl	.NB0002ad90
/*    2ad64:	8ecc0014 */ 	lw	$t4,0x14($s6)
/*    2ad68:	c6500004 */ 	lwc1	$f16,0x4($s2)
/*    2ad6c:	240b0001 */ 	addiu	$t3,$zero,0x1
/*    2ad70:	4610003c */ 	c.lt.s	$f0,$f16
/*    2ad74:	00000000 */ 	sll	$zero,$zero,0x0
/*    2ad78:	45020005 */ 	bc1fl	.NB0002ad90
/*    2ad7c:	8ecc0014 */ 	lw	$t4,0x14($s6)
/*    2ad80:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2ad84:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2ad88:	afab00f8 */ 	sw	$t3,0xf8($sp)
.NB0002ad8c:
/*    2ad8c:	8ecc0014 */ 	lw	$t4,0x14($s6)
.NB0002ad90:
/*    2ad90:	26d60014 */ 	addiu	$s6,$s6,0x14
/*    2ad94:	5580ffcb */ 	bnezl	$t4,.NB0002acc4
/*    2ad98:	8ed90004 */ 	lw	$t9,0x4($s6)
/*    2ad9c:	0260b025 */ 	or	$s6,$s3,$zero
/*    2ada0:	8e620000 */ 	lw	$v0,0x0($s3)
.NB0002ada4:
/*    2ada4:	50400026 */ 	beqzl	$v0,.NB0002ae40
/*    2ada8:	8faa00f8 */ 	lw	$t2,0xf8($sp)
/*    2adac:	8ecd0004 */ 	lw	$t5,0x4($s6)
.NB0002adb0:
/*    2adb0:	51a0001f */ 	beqzl	$t5,.NB0002ae30
/*    2adb4:	8ec90014 */ 	lw	$t1,0x14($s6)
/*    2adb8:	8ec70000 */ 	lw	$a3,0x0($s6)
/*    2adbc:	90ee0000 */ 	lbu	$t6,0x0($a3)
/*    2adc0:	55c0001b */ 	bnezl	$t6,.NB0002ae30
/*    2adc4:	8ec90014 */ 	lw	$t1,0x14($s6)
/*    2adc8:	94ef0002 */ 	lhu	$t7,0x2($a3)
/*    2adcc:	31f82000 */ 	andi	$t8,$t7,0x2000
/*    2add0:	53000017 */ 	beqzl	$t8,.NB0002ae30
/*    2add4:	8ec90014 */ 	lw	$t1,0x14($s6)
/*    2add8:	00e02025 */ 	or	$a0,$a3,$zero
/*    2addc:	8e450000 */ 	lw	$a1,0x0($s2)
/*    2ade0:	0c009ce8 */ 	jal	cdFindGroundInIntTile
/*    2ade4:	8e460008 */ 	lw	$a2,0x8($s2)
/*    2ade8:	c7b200fc */ 	lwc1	$f18,0xfc($sp)
/*    2adec:	4600903e */ 	c.le.s	$f18,$f0
/*    2adf0:	00000000 */ 	sll	$zero,$zero,0x0
/*    2adf4:	4502000e */ 	bc1fl	.NB0002ae30
/*    2adf8:	8ec90014 */ 	lw	$t1,0x14($s6)
/*    2adfc:	c6440004 */ 	lwc1	$f4,0x4($s2)
/*    2ae00:	4604003c */ 	c.lt.s	$f0,$f4
/*    2ae04:	00000000 */ 	sll	$zero,$zero,0x0
/*    2ae08:	45030005 */ 	bc1tl	.NB0002ae20
/*    2ae0c:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2ae10:	8fb900f8 */ 	lw	$t9,0xf8($sp)
/*    2ae14:	57200006 */ 	bnezl	$t9,.NB0002ae30
/*    2ae18:	8ec90014 */ 	lw	$t1,0x14($s6)
/*    2ae1c:	e7a000fc */ 	swc1	$f0,0xfc($sp)
.NB0002ae20:
/*    2ae20:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2ae24:	24080001 */ 	addiu	$t0,$zero,0x1
/*    2ae28:	afa800f8 */ 	sw	$t0,0xf8($sp)
/*    2ae2c:	8ec90014 */ 	lw	$t1,0x14($s6)
.NB0002ae30:
/*    2ae30:	26d60014 */ 	addiu	$s6,$s6,0x14
/*    2ae34:	5520ffde */ 	bnezl	$t1,.NB0002adb0
/*    2ae38:	8ecd0004 */ 	lw	$t5,0x4($s6)
/*    2ae3c:	8faa00f8 */ 	lw	$t2,0xf8($sp)
.NB0002ae40:
/*    2ae40:	3c014f80 */ 	lui	$at,0x4f80
/*    2ae44:	51400004 */ 	beqzl	$t2,.NB0002ae58
/*    2ae48:	44813000 */ 	mtc1	$at,$f6
/*    2ae4c:	5220016b */ 	beqzl	$s1,.NB0002b3fc
/*    2ae50:	8fbf0074 */ 	lw	$ra,0x74($sp)
/*    2ae54:	44813000 */ 	mtc1	$at,$f6
.NB0002ae58:
/*    2ae58:	0260b025 */ 	or	$s6,$s3,$zero
/*    2ae5c:	e7a600e4 */ 	swc1	$f6,0xe4($sp)
/*    2ae60:	8e6b0000 */ 	lw	$t3,0x0($s3)
/*    2ae64:	51600165 */ 	beqzl	$t3,.NB0002b3fc
/*    2ae68:	8fbf0074 */ 	lw	$ra,0x74($sp)
/*    2ae6c:	8ecc0004 */ 	lw	$t4,0x4($s6)
.NB0002ae70:
/*    2ae70:	8fad00f8 */ 	lw	$t5,0xf8($sp)
/*    2ae74:	5580015d */ 	bnezl	$t4,.NB0002b3ec
/*    2ae78:	8ec80014 */ 	lw	$t0,0x14($s6)
/*    2ae7c:	11a00008 */ 	beqz	$t5,.NB0002aea0
/*    2ae80:	8ec70000 */ 	lw	$a3,0x0($s6)
/*    2ae84:	90ee0000 */ 	lbu	$t6,0x0($a3)
/*    2ae88:	55c00158 */ 	bnezl	$t6,.NB0002b3ec
/*    2ae8c:	8ec80014 */ 	lw	$t0,0x14($s6)
/*    2ae90:	94ef0002 */ 	lhu	$t7,0x2($a3)
/*    2ae94:	31f80100 */ 	andi	$t8,$t7,0x100
/*    2ae98:	53000154 */ 	beqzl	$t8,.NB0002b3ec
/*    2ae9c:	8ec80014 */ 	lw	$t0,0x14($s6)
.NB0002aea0:
/*    2aea0:	90e20000 */ 	lbu	$v0,0x0($a3)
/*    2aea4:	544000ba */ 	bnezl	$v0,.NB0002b190
/*    2aea8:	24010001 */ 	addiu	$at,$zero,0x1
/*    2aeac:	90f70001 */ 	lbu	$s7,0x1($a3)
/*    2aeb0:	00e0a025 */ 	or	$s4,$a3,$zero
/*    2aeb4:	00008825 */ 	or	$s1,$zero,$zero
/*    2aeb8:	1ae0014b */ 	blez	$s7,.NB0002b3e8
/*    2aebc:	00e09825 */ 	or	$s3,$a3,$zero
.NB0002aec0:
/*    2aec0:	26350001 */ 	addiu	$s5,$s1,0x1
/*    2aec4:	02b7001a */ 	div	$zero,$s5,$s7
/*    2aec8:	00004810 */ 	mfhi	$t1
/*    2aecc:	00095080 */ 	sll	$t2,$t1,0x2
/*    2aed0:	01495023 */ 	subu	$t2,$t2,$t1
/*    2aed4:	000a5040 */ 	sll	$t2,$t2,0x1
/*    2aed8:	028a8021 */ 	addu	$s0,$s4,$t2
/*    2aedc:	860b000e */ 	lh	$t3,0xe($s0)
/*    2aee0:	860c0012 */ 	lh	$t4,0x12($s0)
/*    2aee4:	8679000e */ 	lh	$t9,0xe($s3)
/*    2aee8:	86680012 */ 	lh	$t0,0x12($s3)
/*    2aeec:	448b8000 */ 	mtc1	$t3,$f16
/*    2aef0:	448c9000 */ 	mtc1	$t4,$f18
/*    2aef4:	44994000 */ 	mtc1	$t9,$f8
/*    2aef8:	44885000 */ 	mtc1	$t0,$f10
/*    2aefc:	468086a0 */ 	cvt.s.w	$f26,$f16
/*    2af00:	c6440000 */ 	lwc1	$f4,0x0($s2)
/*    2af04:	e7a40010 */ 	swc1	$f4,0x10($sp)
/*    2af08:	46809720 */ 	cvt.s.w	$f28,$f18
/*    2af0c:	c6460008 */ 	lwc1	$f6,0x8($s2)
/*    2af10:	4406d000 */ 	mfc1	$a2,$f26
/*    2af14:	e7a60014 */ 	swc1	$f6,0x14($sp)
/*    2af18:	468045a0 */ 	cvt.s.w	$f22,$f8
/*    2af1c:	4407e000 */ 	mfc1	$a3,$f28
/*    2af20:	46805620 */ 	cvt.s.w	$f24,$f10
/*    2af24:	16e00002 */ 	bnez	$s7,.NB0002af30
/*    2af28:	00000000 */ 	sll	$zero,$zero,0x0
/*    2af2c:	0007000d */ 	break	0x7
.NB0002af30:
/*    2af30:	2401ffff */ 	addiu	$at,$zero,-1
/*    2af34:	16e10004 */ 	bne	$s7,$at,.NB0002af48
/*    2af38:	3c018000 */ 	lui	$at,0x8000
/*    2af3c:	16a10002 */ 	bne	$s5,$at,.NB0002af48
/*    2af40:	00000000 */ 	sll	$zero,$zero,0x0
/*    2af44:	0006000d */ 	break	0x6
.NB0002af48:
/*    2af48:	4600c386 */ 	mov.s	$f14,$f24
/*    2af4c:	0c009a8d */ 	jal	cd00025654
/*    2af50:	4600b306 */ 	mov.s	$f12,$f22
/*    2af54:	44804000 */ 	mtc1	$zero,$f8
/*    2af58:	e7a000d4 */ 	swc1	$f0,0xd4($sp)
/*    2af5c:	46000786 */ 	mov.s	$f30,$f0
/*    2af60:	4608003c */ 	c.lt.s	$f0,$f8
/*    2af64:	c7aa00e4 */ 	lwc1	$f10,0xe4($sp)
/*    2af68:	45020003 */ 	bc1fl	.NB0002af78
/*    2af6c:	460af03c */ 	c.lt.s	$f30,$f10
/*    2af70:	46000787 */ 	neg.s	$f30,$f0
/*    2af74:	460af03c */ 	c.lt.s	$f30,$f10
.NB0002af78:
/*    2af78:	00000000 */ 	sll	$zero,$zero,0x0
/*    2af7c:	4502007f */ 	bc1fl	.NB0002b17c
/*    2af80:	02a08825 */ 	or	$s1,$s5,$zero
/*    2af84:	c6500000 */ 	lwc1	$f16,0x0($s2)
/*    2af88:	4406d000 */ 	mfc1	$a2,$f26
/*    2af8c:	4407e000 */ 	mfc1	$a3,$f28
/*    2af90:	e7b00010 */ 	swc1	$f16,0x10($sp)
/*    2af94:	c6520008 */ 	lwc1	$f18,0x8($s2)
/*    2af98:	4600b306 */ 	mov.s	$f12,$f22
/*    2af9c:	4600c386 */ 	mov.s	$f14,$f24
/*    2afa0:	0c009ad5 */ 	jal	cd00025774
/*    2afa4:	e7b20014 */ 	swc1	$f18,0x14($sp)
/*    2afa8:	50400028 */ 	beqzl	$v0,.NB0002b04c
/*    2afac:	4600b306 */ 	mov.s	$f12,$f22
/*    2afb0:	4616d001 */ 	sub.s	$f0,$f26,$f22
/*    2afb4:	4618e501 */ 	sub.s	$f20,$f28,$f24
/*    2afb8:	46000102 */ 	mul.s	$f4,$f0,$f0
/*    2afbc:	e7a000b8 */ 	swc1	$f0,0xb8($sp)
/*    2afc0:	4614a182 */ 	mul.s	$f6,$f20,$f20
/*    2afc4:	0c012e84 */ 	jal	sqrtf
/*    2afc8:	46062300 */ 	add.s	$f12,$f4,$f6
/*    2afcc:	c7a800d4 */ 	lwc1	$f8,0xd4($sp)
/*    2afd0:	4600a407 */ 	neg.s	$f16,$f20
/*    2afd4:	c7a600b8 */ 	lwc1	$f6,0xb8($sp)
/*    2afd8:	46004383 */ 	div.s	$f14,$f8,$f0
/*    2afdc:	c64a0000 */ 	lwc1	$f10,0x0($s2)
/*    2afe0:	c6440008 */ 	lwc1	$f4,0x8($s2)
/*    2afe4:	02802025 */ 	or	$a0,$s4,$zero
/*    2afe8:	02203825 */ 	or	$a3,$s1,$zero
/*    2afec:	46107482 */ 	mul.s	$f18,$f14,$f16
/*    2aff0:	00000000 */ 	sll	$zero,$zero,0x0
/*    2aff4:	46067202 */ 	mul.s	$f8,$f14,$f6
/*    2aff8:	46125080 */ 	add.s	$f2,$f10,$f18
/*    2affc:	46082300 */ 	add.s	$f12,$f4,$f8
/*    2b000:	44051000 */ 	mfc1	$a1,$f2
/*    2b004:	44066000 */ 	mfc1	$a2,$f12
/*    2b008:	0c009c15 */ 	jal	cdFindGroundInIntTileAtVertex
/*    2b00c:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b010:	c6500004 */ 	lwc1	$f16,0x4($s2)
/*    2b014:	4610003c */ 	c.lt.s	$f0,$f16
/*    2b018:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b01c:	45030007 */ 	bc1tl	.NB0002b03c
/*    2b020:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2b024:	8ecd0000 */ 	lw	$t5,0x0($s6)
/*    2b028:	95ae0002 */ 	lhu	$t6,0x2($t5)
/*    2b02c:	31cf2000 */ 	andi	$t7,$t6,0x2000
/*    2b030:	51e00052 */ 	beqzl	$t7,.NB0002b17c
/*    2b034:	02a08825 */ 	or	$s1,$s5,$zero
/*    2b038:	e7a000fc */ 	swc1	$f0,0xfc($sp)
.NB0002b03c:
/*    2b03c:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2b040:	1000004d */ 	beqz	$zero,.NB0002b178
/*    2b044:	e7be00e4 */ 	swc1	$f30,0xe4($sp)
/*    2b048:	4600b306 */ 	mov.s	$f12,$f22
.NB0002b04c:
/*    2b04c:	4600c386 */ 	mov.s	$f14,$f24
/*    2b050:	8e460000 */ 	lw	$a2,0x0($s2)
/*    2b054:	0c009ac1 */ 	jal	cd00025724
/*    2b058:	8e470008 */ 	lw	$a3,0x8($s2)
/*    2b05c:	46000506 */ 	mov.s	$f20,$f0
/*    2b060:	4600d306 */ 	mov.s	$f12,$f26
/*    2b064:	4600e386 */ 	mov.s	$f14,$f28
/*    2b068:	8e460000 */ 	lw	$a2,0x0($s2)
/*    2b06c:	0c009ac1 */ 	jal	cd00025724
/*    2b070:	8e470008 */ 	lw	$a3,0x8($s2)
/*    2b074:	4600a03c */ 	c.lt.s	$f20,$f0
/*    2b078:	46000586 */ 	mov.s	$f22,$f0
/*    2b07c:	45020021 */ 	bc1fl	.NB0002b104
/*    2b080:	c7a800e4 */ 	lwc1	$f8,0xe4($sp)
/*    2b084:	c7aa00e4 */ 	lwc1	$f10,0xe4($sp)
/*    2b088:	460aa03c */ 	c.lt.s	$f20,$f10
/*    2b08c:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b090:	4502003a */ 	bc1fl	.NB0002b17c
/*    2b094:	02a08825 */ 	or	$s1,$s5,$zero
/*    2b098:	8678000e */ 	lh	$t8,0xe($s3)
/*    2b09c:	86790012 */ 	lh	$t9,0x12($s3)
/*    2b0a0:	02802025 */ 	or	$a0,$s4,$zero
/*    2b0a4:	44989000 */ 	mtc1	$t8,$f18
/*    2b0a8:	44993000 */ 	mtc1	$t9,$f6
/*    2b0ac:	02203825 */ 	or	$a3,$s1,$zero
/*    2b0b0:	468090a0 */ 	cvt.s.w	$f2,$f18
/*    2b0b4:	46803320 */ 	cvt.s.w	$f12,$f6
/*    2b0b8:	44051000 */ 	mfc1	$a1,$f2
/*    2b0bc:	44066000 */ 	mfc1	$a2,$f12
/*    2b0c0:	0c009c15 */ 	jal	cdFindGroundInIntTileAtVertex
/*    2b0c4:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b0c8:	c6440004 */ 	lwc1	$f4,0x4($s2)
/*    2b0cc:	4604003c */ 	c.lt.s	$f0,$f4
/*    2b0d0:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b0d4:	45030007 */ 	bc1tl	.NB0002b0f4
/*    2b0d8:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2b0dc:	8ec80000 */ 	lw	$t0,0x0($s6)
/*    2b0e0:	95090002 */ 	lhu	$t1,0x2($t0)
/*    2b0e4:	312a2000 */ 	andi	$t2,$t1,0x2000
/*    2b0e8:	51400024 */ 	beqzl	$t2,.NB0002b17c
/*    2b0ec:	02a08825 */ 	or	$s1,$s5,$zero
/*    2b0f0:	e7a000fc */ 	swc1	$f0,0xfc($sp)
.NB0002b0f4:
/*    2b0f4:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2b0f8:	1000001f */ 	beqz	$zero,.NB0002b178
/*    2b0fc:	e7b400e4 */ 	swc1	$f20,0xe4($sp)
/*    2b100:	c7a800e4 */ 	lwc1	$f8,0xe4($sp)
.NB0002b104:
/*    2b104:	4608003c */ 	c.lt.s	$f0,$f8
/*    2b108:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b10c:	4502001b */ 	bc1fl	.NB0002b17c
/*    2b110:	02a08825 */ 	or	$s1,$s5,$zero
/*    2b114:	860b000e */ 	lh	$t3,0xe($s0)
/*    2b118:	860c0012 */ 	lh	$t4,0x12($s0)
/*    2b11c:	02802025 */ 	or	$a0,$s4,$zero
/*    2b120:	448b8000 */ 	mtc1	$t3,$f16
/*    2b124:	448c5000 */ 	mtc1	$t4,$f10
/*    2b128:	02203825 */ 	or	$a3,$s1,$zero
/*    2b12c:	468080a0 */ 	cvt.s.w	$f2,$f16
/*    2b130:	46805320 */ 	cvt.s.w	$f12,$f10
/*    2b134:	44051000 */ 	mfc1	$a1,$f2
/*    2b138:	44066000 */ 	mfc1	$a2,$f12
/*    2b13c:	0c009c15 */ 	jal	cdFindGroundInIntTileAtVertex
/*    2b140:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b144:	c6520004 */ 	lwc1	$f18,0x4($s2)
/*    2b148:	4612003c */ 	c.lt.s	$f0,$f18
/*    2b14c:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b150:	45030007 */ 	bc1tl	.NB0002b170
/*    2b154:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2b158:	8ecd0000 */ 	lw	$t5,0x0($s6)
/*    2b15c:	95ae0002 */ 	lhu	$t6,0x2($t5)
/*    2b160:	31cf2000 */ 	andi	$t7,$t6,0x2000
/*    2b164:	51e00005 */ 	beqzl	$t7,.NB0002b17c
/*    2b168:	02a08825 */ 	or	$s1,$s5,$zero
/*    2b16c:	e7a000fc */ 	swc1	$f0,0xfc($sp)
.NB0002b170:
/*    2b170:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2b174:	e7b600e4 */ 	swc1	$f22,0xe4($sp)
.NB0002b178:
/*    2b178:	02a08825 */ 	or	$s1,$s5,$zero
.NB0002b17c:
/*    2b17c:	16b7ff50 */ 	bne	$s5,$s7,.NB0002aec0
/*    2b180:	26730006 */ 	addiu	$s3,$s3,0x6
/*    2b184:	10000099 */ 	beqz	$zero,.NB0002b3ec
/*    2b188:	8ec80014 */ 	lw	$t0,0x14($s6)
/*    2b18c:	24010001 */ 	addiu	$at,$zero,0x1
.NB0002b190:
/*    2b190:	54410096 */ 	bnel	$v0,$at,.NB0002b3ec
/*    2b194:	8ec80014 */ 	lw	$t0,0x14($s6)
/*    2b198:	90f50001 */ 	lbu	$s5,0x1($a3)
/*    2b19c:	00e09825 */ 	or	$s3,$a3,$zero
/*    2b1a0:	00001025 */ 	or	$v0,$zero,$zero
/*    2b1a4:	1aa00090 */ 	blez	$s5,.NB0002b3e8
/*    2b1a8:	00e08825 */ 	or	$s1,$a3,$zero
.NB0002b1ac:
/*    2b1ac:	24540001 */ 	addiu	$s4,$v0,0x1
/*    2b1b0:	0295001a */ 	div	$zero,$s4,$s5
/*    2b1b4:	0000c010 */ 	mfhi	$t8
/*    2b1b8:	0018c880 */ 	sll	$t9,$t8,0x2
/*    2b1bc:	0338c823 */ 	subu	$t9,$t9,$t8
/*    2b1c0:	0019c880 */ 	sll	$t9,$t9,0x2
/*    2b1c4:	02798021 */ 	addu	$s0,$s3,$t9
/*    2b1c8:	c6460000 */ 	lwc1	$f6,0x0($s2)
/*    2b1cc:	c61a0010 */ 	lwc1	$f26,0x10($s0)
/*    2b1d0:	c61c0018 */ 	lwc1	$f28,0x18($s0)
/*    2b1d4:	c6360010 */ 	lwc1	$f22,0x10($s1)
/*    2b1d8:	c6380018 */ 	lwc1	$f24,0x18($s1)
/*    2b1dc:	e7a60010 */ 	swc1	$f6,0x10($sp)
/*    2b1e0:	c6440008 */ 	lwc1	$f4,0x8($s2)
/*    2b1e4:	16a00002 */ 	bnez	$s5,.NB0002b1f0
/*    2b1e8:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b1ec:	0007000d */ 	break	0x7
.NB0002b1f0:
/*    2b1f0:	2401ffff */ 	addiu	$at,$zero,-1
/*    2b1f4:	16a10004 */ 	bne	$s5,$at,.NB0002b208
/*    2b1f8:	3c018000 */ 	lui	$at,0x8000
/*    2b1fc:	16810002 */ 	bne	$s4,$at,.NB0002b208
/*    2b200:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b204:	0006000d */ 	break	0x6
.NB0002b208:
/*    2b208:	4406d000 */ 	mfc1	$a2,$f26
/*    2b20c:	4407e000 */ 	mfc1	$a3,$f28
/*    2b210:	4600b306 */ 	mov.s	$f12,$f22
/*    2b214:	4600c386 */ 	mov.s	$f14,$f24
/*    2b218:	0c009a8d */ 	jal	cd00025654
/*    2b21c:	e7a40014 */ 	swc1	$f4,0x14($sp)
/*    2b220:	44804000 */ 	mtc1	$zero,$f8
/*    2b224:	e7a00098 */ 	swc1	$f0,0x98($sp)
/*    2b228:	46000786 */ 	mov.s	$f30,$f0
/*    2b22c:	4608003c */ 	c.lt.s	$f0,$f8
/*    2b230:	c7b000e4 */ 	lwc1	$f16,0xe4($sp)
/*    2b234:	45020003 */ 	bc1fl	.NB0002b244
/*    2b238:	4610f03c */ 	c.lt.s	$f30,$f16
/*    2b23c:	46000787 */ 	neg.s	$f30,$f0
/*    2b240:	4610f03c */ 	c.lt.s	$f30,$f16
.NB0002b244:
/*    2b244:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b248:	45020065 */ 	bc1fl	.NB0002b3e0
/*    2b24c:	02801025 */ 	or	$v0,$s4,$zero
/*    2b250:	c64a0000 */ 	lwc1	$f10,0x0($s2)
/*    2b254:	4406d000 */ 	mfc1	$a2,$f26
/*    2b258:	4407e000 */ 	mfc1	$a3,$f28
/*    2b25c:	e7aa0010 */ 	swc1	$f10,0x10($sp)
/*    2b260:	c6520008 */ 	lwc1	$f18,0x8($s2)
/*    2b264:	4600b306 */ 	mov.s	$f12,$f22
/*    2b268:	4600c386 */ 	mov.s	$f14,$f24
/*    2b26c:	0c009ad5 */ 	jal	cd00025774
/*    2b270:	e7b20014 */ 	swc1	$f18,0x14($sp)
/*    2b274:	50400022 */ 	beqzl	$v0,.NB0002b300
/*    2b278:	4600b306 */ 	mov.s	$f12,$f22
/*    2b27c:	4616d001 */ 	sub.s	$f0,$f26,$f22
/*    2b280:	4618e501 */ 	sub.s	$f20,$f28,$f24
/*    2b284:	46000182 */ 	mul.s	$f6,$f0,$f0
/*    2b288:	e7a0007c */ 	swc1	$f0,0x7c($sp)
/*    2b28c:	4614a102 */ 	mul.s	$f4,$f20,$f20
/*    2b290:	0c012e84 */ 	jal	sqrtf
/*    2b294:	46043300 */ 	add.s	$f12,$f6,$f4
/*    2b298:	c7a80098 */ 	lwc1	$f8,0x98($sp)
/*    2b29c:	4600a287 */ 	neg.s	$f10,$f20
/*    2b2a0:	c7a4007c */ 	lwc1	$f4,0x7c($sp)
/*    2b2a4:	46004383 */ 	div.s	$f14,$f8,$f0
/*    2b2a8:	c6500000 */ 	lwc1	$f16,0x0($s2)
/*    2b2ac:	c6460008 */ 	lwc1	$f6,0x8($s2)
/*    2b2b0:	02602025 */ 	or	$a0,$s3,$zero
/*    2b2b4:	460a7482 */ 	mul.s	$f18,$f14,$f10
/*    2b2b8:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b2bc:	46047202 */ 	mul.s	$f8,$f14,$f4
/*    2b2c0:	46128080 */ 	add.s	$f2,$f16,$f18
/*    2b2c4:	46083300 */ 	add.s	$f12,$f6,$f8
/*    2b2c8:	44051000 */ 	mfc1	$a1,$f2
/*    2b2cc:	44066000 */ 	mfc1	$a2,$f12
/*    2b2d0:	0c009d37 */ 	jal	cdFindGroundInFltTile
/*    2b2d4:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b2d8:	c64a0004 */ 	lwc1	$f10,0x4($s2)
/*    2b2dc:	460a003c */ 	c.lt.s	$f0,$f10
/*    2b2e0:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b2e4:	4502003e */ 	bc1fl	.NB0002b3e0
/*    2b2e8:	02801025 */ 	or	$v0,$s4,$zero
/*    2b2ec:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2b2f0:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2b2f4:	10000039 */ 	beqz	$zero,.NB0002b3dc
/*    2b2f8:	e7be00e4 */ 	swc1	$f30,0xe4($sp)
/*    2b2fc:	4600b306 */ 	mov.s	$f12,$f22
.NB0002b300:
/*    2b300:	4600c386 */ 	mov.s	$f14,$f24
/*    2b304:	8e460000 */ 	lw	$a2,0x0($s2)
/*    2b308:	0c009ac1 */ 	jal	cd00025724
/*    2b30c:	8e470008 */ 	lw	$a3,0x8($s2)
/*    2b310:	46000506 */ 	mov.s	$f20,$f0
/*    2b314:	4600d306 */ 	mov.s	$f12,$f26
/*    2b318:	4600e386 */ 	mov.s	$f14,$f28
/*    2b31c:	8e460000 */ 	lw	$a2,0x0($s2)
/*    2b320:	0c009ac1 */ 	jal	cd00025724
/*    2b324:	8e470008 */ 	lw	$a3,0x8($s2)
/*    2b328:	4600a03c */ 	c.lt.s	$f20,$f0
/*    2b32c:	46000586 */ 	mov.s	$f22,$f0
/*    2b330:	45020017 */ 	bc1fl	.NB0002b390
/*    2b334:	c7a400e4 */ 	lwc1	$f4,0xe4($sp)
/*    2b338:	c7b000e4 */ 	lwc1	$f16,0xe4($sp)
/*    2b33c:	4610a03c */ 	c.lt.s	$f20,$f16
/*    2b340:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b344:	45020026 */ 	bc1fl	.NB0002b3e0
/*    2b348:	02801025 */ 	or	$v0,$s4,$zero
/*    2b34c:	c6220010 */ 	lwc1	$f2,0x10($s1)
/*    2b350:	c62c0018 */ 	lwc1	$f12,0x18($s1)
/*    2b354:	02602025 */ 	or	$a0,$s3,$zero
/*    2b358:	44051000 */ 	mfc1	$a1,$f2
/*    2b35c:	44066000 */ 	mfc1	$a2,$f12
/*    2b360:	0c009d37 */ 	jal	cdFindGroundInFltTile
/*    2b364:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b368:	c6520004 */ 	lwc1	$f18,0x4($s2)
/*    2b36c:	4612003c */ 	c.lt.s	$f0,$f18
/*    2b370:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b374:	4502001a */ 	bc1fl	.NB0002b3e0
/*    2b378:	02801025 */ 	or	$v0,$s4,$zero
/*    2b37c:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2b380:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2b384:	10000015 */ 	beqz	$zero,.NB0002b3dc
/*    2b388:	e7b400e4 */ 	swc1	$f20,0xe4($sp)
/*    2b38c:	c7a400e4 */ 	lwc1	$f4,0xe4($sp)
.NB0002b390:
/*    2b390:	4604003c */ 	c.lt.s	$f0,$f4
/*    2b394:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b398:	45020011 */ 	bc1fl	.NB0002b3e0
/*    2b39c:	02801025 */ 	or	$v0,$s4,$zero
/*    2b3a0:	c6020010 */ 	lwc1	$f2,0x10($s0)
/*    2b3a4:	c60c0018 */ 	lwc1	$f12,0x18($s0)
/*    2b3a8:	02602025 */ 	or	$a0,$s3,$zero
/*    2b3ac:	44051000 */ 	mfc1	$a1,$f2
/*    2b3b0:	44066000 */ 	mfc1	$a2,$f12
/*    2b3b4:	0c009d37 */ 	jal	cdFindGroundInFltTile
/*    2b3b8:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b3bc:	c6460004 */ 	lwc1	$f6,0x4($s2)
/*    2b3c0:	4606003c */ 	c.lt.s	$f0,$f6
/*    2b3c4:	00000000 */ 	sll	$zero,$zero,0x0
/*    2b3c8:	45020005 */ 	bc1fl	.NB0002b3e0
/*    2b3cc:	02801025 */ 	or	$v0,$s4,$zero
/*    2b3d0:	e7a000fc */ 	swc1	$f0,0xfc($sp)
/*    2b3d4:	afd60000 */ 	sw	$s6,0x0($s8)
/*    2b3d8:	e7b600e4 */ 	swc1	$f22,0xe4($sp)
.NB0002b3dc:
/*    2b3dc:	02801025 */ 	or	$v0,$s4,$zero
.NB0002b3e0:
/*    2b3e0:	1695ff72 */ 	bne	$s4,$s5,.NB0002b1ac
/*    2b3e4:	2631000c */ 	addiu	$s1,$s1,0xc
.NB0002b3e8:
/*    2b3e8:	8ec80014 */ 	lw	$t0,0x14($s6)
.NB0002b3ec:
/*    2b3ec:	26d60014 */ 	addiu	$s6,$s6,0x14
/*    2b3f0:	5500fe9f */ 	bnezl	$t0,.NB0002ae70
/*    2b3f4:	8ecc0004 */ 	lw	$t4,0x4($s6)
/*    2b3f8:	8fbf0074 */ 	lw	$ra,0x74($sp)
.NB0002b3fc:
/*    2b3fc:	c7a000fc */ 	lwc1	$f0,0xfc($sp)
/*    2b400:	d7b40020 */ 	ldc1	$f20,0x20($sp)
/*    2b404:	d7b60028 */ 	ldc1	$f22,0x28($sp)
/*    2b408:	d7b80030 */ 	ldc1	$f24,0x30($sp)
/*    2b40c:	d7ba0038 */ 	ldc1	$f26,0x38($sp)
/*    2b410:	d7bc0040 */ 	ldc1	$f28,0x40($sp)
/*    2b414:	d7be0048 */ 	ldc1	$f30,0x48($sp)
/*    2b418:	8fb00050 */ 	lw	$s0,0x50($sp)
/*    2b41c:	8fb10054 */ 	lw	$s1,0x54($sp)
/*    2b420:	8fb20058 */ 	lw	$s2,0x58($sp)
/*    2b424:	8fb3005c */ 	lw	$s3,0x5c($sp)
/*    2b428:	8fb40060 */ 	lw	$s4,0x60($sp)
/*    2b42c:	8fb50064 */ 	lw	$s5,0x64($sp)
/*    2b430:	8fb60068 */ 	lw	$s6,0x68($sp)
/*    2b434:	8fb7006c */ 	lw	$s7,0x6c($sp)
/*    2b438:	8fbe0070 */ 	lw	$s8,0x70($sp)
/*    2b43c:	03e00008 */ 	jr	$ra
/*    2b440:	27bd0108 */ 	addiu	$sp,$sp,0x108
);
#endif

/**
 * Test if the given cylinder is intersecting a tile with the given geoflags.
 * If so, populate the laddernormal argument and return true.
 *
 * There is nothing specific to ladders in this function, but it's only used
 * for finding ladders.
 */
bool cdFindLadder(struct coord *pos, f32 width, f32 ymax, f32 ymin, s16 *rooms, u16 geoflags, struct coord *laddernormal)
{
	struct collision collisions[2];

	cdCollectGeoForCyl(pos, width, rooms, CDTYPE_BG, geoflags, CHECKVERTICAL_YES, ymax, ymin, collisions, 1);

	if (collisions[0].geo) {
		struct geotilei *tile = (struct geotilei *) collisions[0].geo;
		struct coord dist;

		cdGetGeoNormal(collisions[0].geo, laddernormal);

		dist.x = pos->x - tile->vertices[0][0];
		dist.y = pos->y - tile->vertices[0][1];
		dist.z = pos->z - tile->vertices[0][2];

		if (dist.f[0] * laddernormal->f[0] + dist.f[1] * laddernormal->f[1] + dist.f[2] * laddernormal->f[2] < 0) {
			laddernormal->x = -laddernormal->x;
			laddernormal->y = -laddernormal->y;
			laddernormal->z = -laddernormal->z;
		}

		return true;
	}

	return false;
}

bool cd0002a13c(struct coord *pos, f32 radius, f32 ymax, f32 ymin, s16 *rooms, u16 geoflags)
{
	struct collision collisions[2];

	cdCollectGeoForCyl(pos, radius, rooms, CDTYPE_BG, geoflags, CHECKVERTICAL_YES, ymax, ymin, collisions, 1);

	if (collisions[0].geo) {
		return true;
	}

	return false;
}

f32 cdFindGroundInfoAtCyl(struct coord *pos, f32 radius, s16 *rooms, u16 *floorcol,
		u8 *floortype, u16 *floorflags, s16 *floorroom, s32 *inlift, struct prop **lift)
{
	struct collision collisions[21];
	struct collision *sp72 = NULL;
	f32 ground;
	struct geo *geo = NULL;

	cdCollectGeoForCyl(pos, radius, rooms, CDTYPE_ALL, GEOFLAG_FLOOR1 | GEOFLAG_FLOOR2, CHECKVERTICAL_NO, 0, 0, collisions, 20);
	ground = cdFindGroundFromList(collisions, pos, &sp72, radius);

	if (sp72) {
		geo = sp72->geo;
	}

	if (floorcol) {
		cdGetFloorCol(geo, floorcol);
	}

	if (floortype) {
		cdGetFloorType(geo, floortype);
	}

	if (floorflags && geo) {
		*floorflags = geo->flags;
	}

	if (floorroom) {
		if (sp72) {
			*floorroom = sp72->room;
		} else {
			*floorroom = -1;
		}
	}

	if (inlift) {
		if (geo && geo->type == GEOTYPE_TILE_F && (geo->flags & GEOFLAG_LIFTFLOOR)) {
			*inlift = true;
			*lift = sp72->prop;

			if (*lift && (*lift)->obj->modelnum == MODEL_ESCA_STEP && floortype) {
				*floortype = FLOORTYPE_METAL;
			}
		} else {
			*inlift = false;
			*lift = NULL;
		}
	}

	return ground;
}

/**
 * This function must come immediately after cdFindGroundInfoAtCyl.
 *
 * A piracy check looks for this function, then backtracks two instructions to
 * nop the jr ra at the end of cdFindGroundInfoAtCyl, causing it to flow into this
 * function and return 0.
 */
f32 cdReturnZero(void)
{
	return 0;
}

f32 cdFindGroundAtCyl(struct coord *pos, f32 radius, s16 *rooms, u16 *floorcol, u8 *floortype)
{
	return cdFindGroundInfoAtCyl(pos, radius, rooms, floorcol, floortype, NULL, NULL, NULL, NULL);
}

f32 cdFindFloorYColourTypeAtPos(struct coord *pos, s16 *rooms, u16 *floorcol, u8 *floortype)
{
	struct geo *geo;
	s16 sp30[2];
	f32 sp2c;
	f32 result = -4294967296;

	cdFindClosestVertical(pos, rooms, GEOFLAG_FLOOR1 | GEOFLAG_FLOOR2, &geo, &sp30[1], &sp2c, NULL, SURFACE_FLOOR);

	if (geo) {
		result = sp2c;
	}

	if (floorcol) {
		cdGetFloorCol(geo, floorcol);
	}

	if (floortype) {
		cdGetFloorType(geo, floortype);
	}

	return result;
}

s32 cdFindFloorRoomAtPos(struct coord *pos, s16 *nearrooms)
{
	struct geo *geo;
	s16 room;
	f32 sp2c;

	cdFindClosestVertical(pos, nearrooms, GEOFLAG_FLOOR1 | GEOFLAG_FLOOR2, &geo, &room, &sp2c, 0, SURFACE_FLOOR);

	return room;
}

#if VERSION >= VERSION_NTSC_1_0
s16 cdFindFloorRoomYColourFlagsAtPos(struct coord *pos, s16 *rooms, f32 *arg2, u16 *floorcolptr, u16 *flagsptr)
#else
s16 cdFindFloorRoomYColourFlagsAtPos(struct coord *pos, s16 *rooms, f32 *arg2, u16 *floorcolptr)
#endif
{
	struct geo *geo;
	s16 room;
	f32 sp2c;

	cdFindClosestVertical(pos, rooms, GEOFLAG_FLOOR1 | GEOFLAG_FLOOR2, &geo, &room, &sp2c, NULL, SURFACE_FLOOR);

	if (geo != NULL) {
		*arg2 = sp2c;
	}

	if (floorcolptr != NULL) {
		cdGetFloorCol(geo, floorcolptr);
	}

#if VERSION >= VERSION_NTSC_1_0
	if (flagsptr != NULL && geo != NULL) {
		*flagsptr = geo->flags;
	}
#endif

	return room;
}

#if VERSION >= VERSION_NTSC_1_0
s16 cdFindCeilingRoomYColourFlagsAtPos(struct coord *pos, s16 *rooms, f32 *arg2, u16 *floorcolptr, u16 *flagsptr)
#else
s16 cdFindCeilingRoomYColourFlagsAtPos(struct coord *pos, s16 *rooms, f32 *arg2, u16 *floorcolptr)
#endif
{
	struct geo *geo;
	s16 sp32;
	f32 sp2c;

	cdFindClosestVertical(pos, rooms, GEOFLAG_FLOOR1 | GEOFLAG_FLOOR2, &geo, &sp32, &sp2c, NULL, SURFACE_CEILING);

	if (geo != NULL) {
		*arg2 = sp2c;
	}

	if (floorcolptr != NULL) {
		cdGetFloorCol(geo, floorcolptr);
	}

#if VERSION >= VERSION_NTSC_1_0
	if (flagsptr != NULL && geo != NULL) {
		*flagsptr = geo->flags;
	}
#endif

	return sp32;
}

s16 cdFindFloorRoomYColourNormalPropAtPos(struct coord *pos, s16 *rooms, f32 *arg2, u16 *floorcol, struct coord *normal, struct prop **propptr)
{
	struct geo *geo;
	s16 room;
	f32 sp2c;

	cdFindClosestVertical(pos, rooms, GEOFLAG_FLOOR1 | GEOFLAG_FLOOR2, &geo, &room, &sp2c, propptr, SURFACE_FLOOR);

	if (geo) {
		*arg2 = sp2c;
		cdGetGeoNormal(geo, normal);
	}

	if (floorcol) {
		cdGetFloorCol(geo, floorcol);
	}

	return room;
}

s16 cdFindCeilingRoomYColourFlagsNormalAtPos(struct coord *pos, s16 *rooms, f32 *arg2, u16 *floorcol, u16 *flagsptr, struct coord *normal)
{
	struct geo *geo;
	s16 sp32;
	f32 sp2c;

	cdFindClosestVertical(pos, rooms, GEOFLAG_FLOOR1 | GEOFLAG_FLOOR2, &geo, &sp32, &sp2c, NULL, SURFACE_CEILING);

	if (geo) {
		*arg2 = sp2c;
		cdGetGeoNormal(geo, normal);
	}

	if (floorcol) {
		cdGetFloorCol(geo, floorcol);
	}

	if (flagsptr != NULL && geo != NULL) {
		*flagsptr = geo->flags;
	}

	return sp32;
}

/**
 * Tests if a cylinder volume fits in the given position.
 */
s32 cdTestVolume(struct coord *pos, f32 width, s16 *rooms, s32 types, bool checkvertical, f32 ymax, f32 ymin)
{
	struct collision collisions[2];
	bool result = true;

	cdCollectGeoForCyl(pos, width, rooms, types, GEOFLAG_WALL, checkvertical, ymax, ymin, collisions, 1);

	if (collisions[0].geo) {
		result = false;
		cdSetObstacleProp(collisions[0].prop);
	}

	return result;
}

s32 cdExamCylMove01(struct coord *pos, struct coord *pos2, f32 radius, s16 *rooms, s32 types, bool checkvertical, f32 ymax, f32 ymin)
{
	struct collision collisions[2]; // 80
	s32 cdresult;
	struct coord sp70;
	struct coord sp64;

	cdresult = CDRESULT_NOCOLLISION;

	cdCollectGeoForCyl(pos2, radius, rooms, types, GEOFLAG_WALL, checkvertical, ymax, ymin, collisions, 1);

	// 768
	if (collisions[0].geo != NULL) {
		cdresult = CDRESULT_COLLISION;

		// 778
		if (collisions[0].geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) collisions[0].geo;
			s32 this = collisions[0].vertexindex;
			s32 next = (this + 1) % collisions[0].geo->numvertices;

			sp70.x = tile->vertices[this][0];
			sp70.y = tile->vertices[this][1];
			sp70.z = tile->vertices[this][2];

			sp64.x = tile->vertices[next][0];
			sp64.y = tile->vertices[next][1];
			sp64.z = tile->vertices[next][2];
		} else /*854*/ if (collisions[0].geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) collisions[0].geo;
			s32 this = collisions[0].vertexindex;
			s32 next = (this + 1) % collisions[0].geo->numvertices;

			sp70 = tile->vertices[this];

			sp64 = tile->vertices[next];
		} else if (collisions[0].geo->type == GEOTYPE_BLOCK) {
			struct geoblock *block = (struct geoblock *) collisions[0].geo;
			s32 this = collisions[0].vertexindex;
			s32 next = (this + 1) % collisions[0].geo->numvertices;

			sp70.x = block->vertices[this][0];
			sp70.y = pos->y;
			sp70.z = block->vertices[this][1];

			sp64.x = block->vertices[next][0];
			sp64.y = pos->y;
			sp64.z = block->vertices[next][1];
		} else if (collisions[0].geo->type == GEOTYPE_CYL) {
			struct geocyl *cyl = (struct geocyl *) collisions[0].geo;

			if (1);

			cd00025848(cyl->x, cyl->z, cyl->radius, pos->x, pos->z, &sp70.x, &sp70.z, &sp64.x, &sp64.z);

			sp70.y = pos->y;
			sp64.y = pos->y;
		}

		cdSetObstacleVtxProp(&sp70, &sp64, collisions[0].prop);
	}

	return cdresult;
}

s32 cdExamCylMove02(struct coord *origpos, struct coord *dstpos, f32 width, s16 *dstrooms, s32 types, bool checkvertical, f32 ymax, f32 ymin)
{
	struct collision collisions[21];
	struct coord dist;
	s32 result = CDRESULT_NOCOLLISION;

	cdCollectGeoForCylMove(dstpos, width, dstrooms, types, GEOFLAG_WALL, checkvertical, ymax, ymin, collisions, 20);

	if (collisions[0].geo) {
		result = CDRESULT_COLLISION;

		dist.x = dstpos->x - origpos->x;
		dist.y = dstpos->y - origpos->y;
		dist.z = dstpos->z - origpos->z;

		cd0002901c(origpos, &dist, width, collisions);
	}

	return result;
}

bool cd0002aac0IntTile(struct coord *arg0, struct coord *arg1, struct coord *arg2, struct geotilei *tile, struct coord *arg4, struct coord *arg5)
{
	s32 i;
	u8 numvertices = tile->header.numvertices;

	for (i = 2; i < numvertices; i++) {
		if (func0002f490((struct vec3s16 *)&tile->vertices[0][0],
					(struct vec3s16 *)&tile->vertices[i - 1][0],
					(struct vec3s16 *)&tile->vertices[i][0],
					0, arg0, arg1, arg2, arg4, arg5)) {
			return true;
		}
	}

	return false;
}

bool cd0002ab98FltTile(struct coord *arg0, struct coord *arg1, struct coord *arg2, struct geotilef *tile, struct coord *arg4, struct coord *arg5)
{
	s32 i;
	u8 numvertices = tile->header.numvertices;

	for (i = 2; i < numvertices; i++) {
		if (func0002f560(&tile->vertices[0], &tile->vertices[i - 1], &tile->vertices[i],
					0, arg0, arg1, arg2, arg4, arg5)) {
			return true;
		}
	}

	return false;
}

bool cd0002ac70IntTile(struct coord *arg0, struct coord *arg1, struct coord *arg2, struct geotilei *tile,
		struct coord *arg4, struct coord *arg5, struct coord *arg6, bool arg7, f32 arg8, f32 arg9)
{
	bool result = false;
	s32 i;
	f32 f0;
	s32 numvertices = tile->header.numvertices;
	s32 next;
	s32 spb8 = 1;
	f32 f22 = 1.0f;
	s32 spb0;
	f32 spac;
	f32 spa8;
	f32 ymax = *(s16 *)(tile->ymax + (u32)tile);
	f32 ymin = *(s16 *)(tile->ymin + (u32)tile);
	f32 spa0[2];
	f32 sp98[2];
	f32 sp90[2];
	f32 sp88[2];

	if (!arg7
			|| (arg0->y + arg8 >= ymin && arg1->y + arg9 <= ymax)
			|| (arg0->y + arg9 <= ymax && arg1->y + arg8 >= ymin)) {
		for (i = 0; i < numvertices; i++) {
			next = (i + 1) % numvertices;

			if (cd000254d8(arg0, arg1, tile->vertices[i][0], tile->vertices[i][2], tile->vertices[next][0], tile->vertices[next][2], &spb8)) {
				spa0[0] = arg0->x;
				spa0[1] = arg0->z;
				sp98[0] = arg1->x;
				sp98[1] = arg1->z;
				sp90[0] = tile->vertices[i][0];
				sp90[1] = tile->vertices[i][2];
				sp88[0] = tile->vertices[next][0];
				sp88[1] = tile->vertices[next][2];

				f0 = func0f1577f0(spa0, sp98, sp90, sp88);

				if (f0 < f22) {
					if (arg7) {
						spa8 = (arg1->y - arg0->y) * f0 + arg0->y;
						spac = spa8 + arg8;
						spa8 = spa8 + arg9;
					}

					if (!arg7 || (!(spa8 >= ymax) && !(spac <= ymin))) {
						result = true;
						f22 = f0;
						spb0 = i;
					}
				}
			}
		}

		if (result) {
			arg4->x = arg0->x + arg2->f[0] * f22;
			arg4->y = arg0->y + arg2->f[1] * f22;
			arg4->z = arg0->z + arg2->f[2] * f22;

			if (arg5 != NULL && arg6 != NULL) {
				arg5->x = tile->vertices[spb0][0];
				arg5->y = arg4->y;
				arg5->z = tile->vertices[spb0][2];

				arg6->x = tile->vertices[(spb0 + 1) % numvertices][0];
				arg6->y = arg4->y;
				arg6->z = tile->vertices[(spb0 + 1) % numvertices][2];
			}
		} else if (!result && spb8) {
			result = true;

			*arg4 = *arg0;

			if (arg5 != NULL && arg6 != NULL) {
				*arg5 = *arg0;

				*arg6 = *arg0;
			}
		}
	}

	return result;
}

bool cd0002b128FltTile(struct coord *arg0, struct coord *arg1, struct coord *arg2, struct geotilef *tile,
		struct coord *arg4, struct coord *arg5, struct coord *arg6, bool arg7, f32 arg8, f32 arg9)
{
	bool result = false;
	s32 i;
	f32 f0;
	s32 numvertices = tile->header.numvertices;
	s32 next;
	s32 spb8 = 1;
	f32 f22 = 1.0f;
	s32 spb0;
	f32 spac;
	f32 spa8;
	f32 ymax = tile->vertices[tile->ymax].y;
	f32 ymin = tile->vertices[tile->ymin].y;
	f32 spa0[2];
	f32 sp98[2];
	f32 sp90[2];
	f32 sp88[2];

	if (!arg7
			|| (arg0->y + arg8 >= ymin && arg1->y + arg9 <= ymax)
			|| (arg0->y + arg9 <= ymax && arg1->y + arg8 >= ymin)) {
		for (i = 0; i < numvertices; i++) {
			next = (i + 1) % numvertices;

			if (cd000254d8(arg0, arg1, tile->vertices[i].x, tile->vertices[i].z, tile->vertices[next].x, tile->vertices[next].z, &spb8)) {
				spa0[0] = arg0->x;
				spa0[1] = arg0->z;
				sp98[0] = arg1->x;
				sp98[1] = arg1->z;
				sp90[0] = tile->vertices[i].x;
				sp90[1] = tile->vertices[i].z;
				sp88[0] = tile->vertices[next].x;
				sp88[1] = tile->vertices[next].z;

				f0 = func0f1577f0(spa0, sp98, sp90, sp88);

				if (f0 < f22) {
					if (arg7) {
						spa8 = (arg1->y - arg0->y) * f0 + arg0->y;
						spac = spa8 + arg8;
						spa8 = spa8 + arg9;
					}

					if (!arg7 || (!(spa8 >= ymax) && !(spac <= ymin))) {
						result = true;
						f22 = f0;
						spb0 = i;
					}
				}
			}
		}

		if (result) {
			arg4->x = arg0->x + arg2->f[0] * f22;
			arg4->y = arg0->y + arg2->f[1] * f22;
			arg4->z = arg0->z + arg2->f[2] * f22;

			if (arg5 != NULL && arg6 != NULL) {
				arg5->x = tile->vertices[spb0].x;
				arg5->y = arg4->y;
				arg5->z = tile->vertices[spb0].z;

				arg6->x = tile->vertices[(spb0 + 1) % numvertices].x;
				arg6->y = arg4->y;
				arg6->z = tile->vertices[(spb0 + 1) % numvertices].z;
			}
		} else if (!result && spb8) {
			result = true;

			*arg4 = *arg0;

			if (arg5 != NULL && arg6 != NULL) {
				*arg5 = *arg0;

				*arg6 = *arg0;
			}
		}
	}

	return result;
}

bool cd0002b560Block(struct coord *arg0, struct coord *arg1, struct coord *arg2, struct geoblock *block,
		struct coord *arg4, struct coord *arg5, struct coord *arg6, bool arg7, f32 arg8, f32 arg9)
{
	bool result = false;
	s32 i;
	f32 f0;
	s32 numvertices = block->header.numvertices;
	s32 next;
	s32 spb8 = 1;
	f32 f22 = 1.0f;
	s32 spb0;
	f32 spac;
	f32 spa8;
	f32 spa0[2];
	f32 sp98[2];
	f32 sp90[2];
	f32 sp88[2];

	if (!arg7
			|| (arg0->y + arg8 >= block->ymin && arg1->y + arg9 <= block->ymax)
			|| (arg0->y + arg9 <= block->ymax && arg1->y + arg8 >= block->ymin)) {
		for (i = 0; i < numvertices; i++) {
			next = (i + 1) % numvertices;

			if (cd000254d8(arg0, arg1, block->vertices[i][0], block->vertices[i][1], block->vertices[next][0], block->vertices[next][1], &spb8)) {
				spa0[0] = arg0->x;
				spa0[1] = arg0->z;
				sp98[0] = arg1->x;
				sp98[1] = arg1->z;
				sp90[0] = block->vertices[i][0];
				sp90[1] = block->vertices[i][1];
				sp88[0] = block->vertices[next][0];
				sp88[1] = block->vertices[next][1];

				f0 = func0f1577f0(spa0, sp98, sp90, sp88);

				if (f0 < f22) {
					if (arg7) {
						spa8 = (arg1->y - arg0->y) * f0 + arg0->y;
						spac = spa8 + arg8;
						spa8 = spa8 + arg9;
					}

					if (!arg7 || (!(spa8 >= block->ymax) && !(spac <= block->ymin))) {
						result = true;
						f22 = f0;
						spb0 = i;
					}
				}
			}
		}

		if (result) {
			arg4->x = arg0->x + arg2->f[0] * f22;
			arg4->y = arg0->y + arg2->f[1] * f22;
			arg4->z = arg0->z + arg2->f[2] * f22;

			if (arg5 != NULL && arg6 != NULL) {
				arg5->x = block->vertices[spb0][0];
				arg5->y = arg4->y;
				arg5->z = block->vertices[spb0][1];

				arg6->x = block->vertices[(spb0 + 1) % numvertices][0];
				arg6->y = arg4->y;
				arg6->z = block->vertices[(spb0 + 1) % numvertices][1];
			}
		} else if (!result && spb8) {
			result = true;

			*arg4 = *arg0;

			if (arg5 != NULL && arg6 != NULL) {
				*arg5 = *arg0;

				*arg6 = *arg0;
			}
		}
	}

	return result;
}

bool cd0002b954Cyl(struct coord *arg0, struct coord *arg1, struct coord *arg2, struct geocyl *cyl,
		struct coord *arg4, struct coord *arg5, struct coord *arg6, bool arg7, f32 arg8, f32 arg9)
{
	bool result = false;
	f32 mult;
	f32 sp74;
	f32 x = cyl->x;
	f32 z = cyl->z;
	f32 radius = cyl->radius;

	if (!arg7
			|| (arg0->y + arg8 >= cyl->ymin && arg1->y + arg9 <= cyl->ymax)
			|| (arg0->y + arg9 <= cyl->ymax && arg1->y + arg8 >= cyl->ymin)) {
		sp74 = cd00025654(arg0->x, arg0->z, arg1->x, arg1->z, x, z);

		if (sp74 < 0.0f) {
			sp74 = -sp74;
		}

		if (sp74 < radius
				&& (cd00025724(arg0->x, arg0->z, x, z) < radius
					|| cd00025724(arg1->x, arg1->z, x, z) < radius
					|| cd00025774(arg0->x, arg0->z, arg1->x, arg1->z, x, z))) {
			f32 xdiff = arg1->x - arg0->x;
			f32 zdiff = arg1->z - arg0->z;
			f32 sqdist;
			f32 dist;
			u32 stack;
			f32 sp50;
			f32 sp4c;
			f32 sp48;

			sp50 = sqrtf(xdiff * xdiff + zdiff * zdiff);

			if (sp50 > 0.0f) {
				xdiff = x - arg0->x;
				zdiff = z - arg0->z;

				sqdist = xdiff * xdiff + zdiff * zdiff;

				if (sp74 * sp74 <= sqdist) {
					dist = sqrtf(sqdist - sp74 * sp74) - sqrtf(radius * radius - sp74 * sp74);
				} else {
					dist = 0.0f;
				}

				mult = dist / sp50;
			} else {
				mult = 0.0f;
			}

			if (mult < 1.0f) {
				if (arg7) {
					sp48 = (arg1->y - arg0->y) * mult + arg0->y;
					sp4c = sp48 + arg8;
					sp48 = sp48 + arg9;
				}

				if (!arg7 || (!(sp48 >= cyl->ymax) && !(sp4c <= cyl->ymin))) {
					result = true;

					arg4->x = arg0->x + arg2->f[0] * mult;
					arg4->y = arg0->y + arg2->f[1] * mult;
					arg4->z = arg0->z + arg2->f[2] * mult;

					if (arg5 != NULL && arg6 != NULL) {
						cd00025848(x, z, radius, arg0->x, arg0->z, &arg5->x, &arg5->z, &arg6->x, &arg6->z);

						arg5->y = arg4->y;
						arg6->y = arg4->y;
					}
				}
			}
		}
	}

	return result;
}

bool cdTestAToBGeolist(u8 *start, u8 *end, struct coord *arg2, struct coord *arg3, struct coord *arg4, u16 geoflags, bool checkvertical, s32 arg7, f32 arg8, f32 arg9)
{
	struct geo *geo = (struct geo *) start;

	while (geo < (struct geo *) end) {
		if (geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) geo;
			struct coord min;
			struct coord max;
			struct coord spc4;
			struct coord spb8;

			if (tile->header.flags & geoflags) {
				min.x = *(s16 *)(tile->xmin + (u32)tile);

				if ((!(arg2->x < min.x)) || !(arg3->x < min.x)) {
					max.x = *(s16 *)(tile->xmax + (u32)tile);

					if ((!(arg2->x > max.x)) || !(arg3->x > max.x)) {
						min.z = *(s16 *)(tile->zmin + (u32)tile);

						if ((!(arg2->z < min.z)) || !(arg3->z < min.z)) {
							max.z = *(s16 *)(tile->zmax + (u32)tile);

							if ((!(arg2->z > max.z)) || !(arg3->z > max.z)) {
								if (1);
								if (checkvertical) {
									min.y = *(s16 *)(tile->ymin + (u32)tile);
									max.y = *(s16 *)(tile->ymax + (u32)tile);

									if ((!(arg2->y < min.y) || !(arg3->y < min.y))
											&& (!(arg2->y > max.y) || !(arg3->y > max.y))
											&& bgTestLineIntersectsBbox(arg2, arg4, &min, &max)
											&& cd0002aac0IntTile(arg2, arg3, arg4, tile, &spc4, &spb8)) {
										return false;
									}
								} else if (cd0002ac70IntTile(arg2, arg3, arg4, tile, &spc4, 0, 0, arg7, arg8, arg9)) {
									return false;
								}
							}
						}
					}
				}
			}

			geo = (struct geo *)((u32)geo + tile->header.numvertices * 6 + 0xe);
		} else if (geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) geo;
			struct coord min;
			struct coord max;
			struct coord sp90;
			struct coord sp84;

			if (tile->header.flags & geoflags) {
				min.x = tile->vertices[tile->xmin].x;
				max.x = tile->vertices[tile->xmax].x;
				min.z = tile->vertices[tile->zmin].z;
				max.z = tile->vertices[tile->zmax].z;

				if (((!(arg2->x < min.x)) || !(arg3->x < min.x))
						&& (!(arg2->x > max.x) || !(arg3->x > max.x))
						&& ((!(arg2->z < min.z)) || !(arg3->z < min.z))
						&& (!(arg2->z > max.z) || !(arg3->z > max.z))) {
					if (checkvertical) {
						min.y = tile->vertices[tile->ymin].y;
						max.y = tile->vertices[tile->ymax].y;

						if ((!(arg2->y < min.y) || !(arg3->y < min.y))
								&& (!(arg2->y > max.y) || !(arg3->y > max.y))
								&& bgTestLineIntersectsBbox(arg2, arg4, &min, &max)
								&& cd0002ab98FltTile(arg2, arg3, arg4, tile, &sp90, &sp84)) {
							return false;
						}
					} else if (cd0002b128FltTile(arg2, arg3, arg4, tile, &sp90, 0, 0, arg7, arg8, arg9)) {
						return false;
					}
				}
			}

			geo = (struct geo *)((u32)geo + (u32)(tile->header.numvertices - 0x40) * 0xc + 0x310);
		} else if (geo->type == GEOTYPE_BLOCK) {
			struct coord sp78;
			struct geoblock *block = (struct geoblock *) geo;

			if ((geoflags & (GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT))
					&& cd0002b560Block(arg2, arg3, arg4, block, &sp78, 0, 0, arg7, arg8, arg9)) {
				return false;
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geoblock));
		} else if (geo->type == GEOTYPE_CYL) {
			struct coord sp68;
			struct geocyl *cyl = (struct geocyl *) geo;

			if ((geoflags & cyl->header.flags)
					&& cd0002b954Cyl(arg2, arg3, arg4, cyl, &sp68, 0, 0, arg7, arg8, arg9)) {
				return false;
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geocyl));
		}
	}

	return true;
}

void cd0002c328IntTile(struct geotilei *tile, struct coord *arg1, struct coord *arg2, struct coord *arg3, struct coord *arg4)
{
	struct coord sp3c;
	u32 stack[2];
	u8 numvertices;
	f32 max = 0.0f;
	f32 min = 0.0f;
	f32 dist;
	s32 i;

	if (arg2->x != 0.0f || arg2->z != 0.0f) {
		sp3c.x = arg2->z;
		sp3c.y = 0.0f;
		sp3c.z = -arg2->x;

		dist = sqrtf(sp3c.f[0] * sp3c.f[0] + sp3c.f[2] * sp3c.f[2]);

		if (dist > 0.0f) {
			sp3c.x *= 1.0f / dist;
			sp3c.z *= 1.0f / dist;
		} else {
			sp3c.x = 0.0f;
			sp3c.y = 0.0f;
			sp3c.z = 1.0f;
		}
	} else {
		sp3c.x = 0.0f;
		sp3c.y = 0.0f;
		sp3c.z = 1.0f;
	}

	numvertices = tile->header.numvertices;

	for (i = 0; i < numvertices; i++) {
		f32 xdiff = tile->vertices[i][0] - arg1->x;
		f32 zdiff = tile->vertices[i][2] - arg1->z;
		f32 f0 = xdiff * sp3c.f[0] + zdiff * sp3c.f[2];

		if (f0 > max) {
			max = f0;
		} else if (f0 < min) {
			min = f0;
		}
	}

	arg3->x = arg1->x + sp3c.f[0] * max;
	arg3->y = arg1->y;
	arg3->z = arg1->z + sp3c.f[2] * max;

	arg4->x = arg1->x + sp3c.f[0] * min;
	arg4->y = arg1->y;
	arg4->z = arg1->z + sp3c.f[2] * min;
}

void cd0002c528FltTile(struct geotilef *tile, struct coord *arg1, struct coord *arg2, struct coord *arg3, struct coord *arg4)
{
	struct coord sp3c;
	u32 stack[2];
	u8 numvertices;
	f32 max = 0.0f;
	f32 min = 0.0f;
	f32 dist;
	s32 i;

	if (arg2->x != 0.0f || arg2->z != 0.0f) {
		sp3c.x = arg2->z;
		sp3c.y = 0.0f;
		sp3c.z = -arg2->x;

		dist = sqrtf(sp3c.f[0] * sp3c.f[0] + sp3c.f[2] * sp3c.f[2]);

		if (dist > 0.0f) {
			sp3c.x *= 1.0f / dist;
			sp3c.z *= 1.0f / dist;
		} else {
			sp3c.x = 0.0f;
			sp3c.y = 0.0f;
			sp3c.z = 1.0f;
		}
	} else {
		sp3c.x = 0.0f;
		sp3c.y = 0.0f;
		sp3c.z = 1.0f;
	}

	numvertices = tile->header.numvertices;

	for (i = 0; i < numvertices; i++) {
		f32 xdiff = tile->vertices[i].x - arg1->x;
		f32 zdiff = tile->vertices[i].z - arg1->z;
		f32 f0 = xdiff * sp3c.f[0] + zdiff * sp3c.f[2];

		if (f0 > max) {
			max = f0;
		} else if (f0 < min) {
			min = f0;
		}
	}

	arg3->x = arg1->x + sp3c.f[0] * max;
	arg3->y = arg1->y;
	arg3->z = arg1->z + sp3c.f[2] * max;

	arg4->x = arg1->x + sp3c.f[0] * min;
	arg4->y = arg1->y;
	arg4->z = arg1->z + sp3c.f[2] * min;
}

bool cdExamAToBGeolist(u8 *start, u8 *end, struct coord *arg2, struct coord *arg3, struct coord *arg4,
		u16 geoflags, bool checkvertical, s32 arg7, f32 ymax, f32 ymin, f32 *arg10, struct coord *arg11,
		struct coord *arg12, struct coord *arg13, struct geo **geoptr, s32 roomnum)
{
	struct geo *geo;
	f32 x;
	f32 y;
	f32 z;
	f32 sum;
	bool ok;
	bool result = false;

	geo = (struct geo *) start;

	while (geo < (struct geo *) end) {
		if (geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) geo;
			struct coord min;
			struct coord max;
			struct coord sp12c;
			struct coord sp120;
			struct coord sp114;
			struct coord sp108;

			if (geo->flags & GEOFLAG_RAMPWALL) {
				ok = cdTestRampWall(tile, arg2, 0, arg2->y + ymin, arg2->y + ymax);
			} else {
				ok = true;
			}

			if (ok && (geo->flags & geoflags)) {
				min.x = *(s16 *)(tile->xmin + (u32)tile);

				if (!(arg2->x < min.x) || !(arg3->x < min.x)) {
					max.x = *(s16 *)(tile->xmax + (u32)tile);

					if (!(arg2->x > max.x) || !(arg3->x > max.x)) {
						min.z = *(s16 *)(tile->zmin + (u32)tile);

						if (!(arg2->z < min.z) || !(arg3->z < min.z)) {
							max.z = *(s16 *)(tile->zmax + (u32)tile);

							if (!(arg2->z > max.z) || !(arg3->z > max.z)) {
								if (checkvertical) {
									min.y = *(s16 *)(tile->ymin + (u32)tile);
									max.y = *(s16 *)(tile->ymax + (u32)tile);

									if ((!(arg2->y < min.y) || !(arg3->y < min.y))
											&& (!(arg2->y > max.y) || !(arg3->y > max.y))
											&& bgTestLineIntersectsBbox(arg2, arg4, &min, &max)
											&& cd0002aac0IntTile(arg2, arg3, arg4, tile, &sp12c, &sp120)) {
										x = sp12c.x - arg2->x;
										y = sp12c.y - arg2->y;
										z = sp12c.z - arg2->z;

										sum = x * x + y * y + z * z;

										if (sum < *arg10) {
											result = true;
											*arg10 = sum;

											*arg11 = sp12c;

											cd0002c328IntTile(tile, &sp12c, &sp120, arg12, arg13);

											*geoptr = geo;
										}
									}
								} else if (cd0002ac70IntTile(arg2, arg3, arg4, tile, &sp12c, &sp114, &sp108, arg7, ymax, ymin)) {
									x = sp12c.x - arg2->x;
									y = sp12c.y - arg2->y;
									z = sp12c.z - arg2->z;

									sum = x * x + y * y + z * z;

									if (sum < *arg10) {
										result = true;
										*arg10 = sum;

										*arg11 = sp12c;

										*arg12 = sp114;

										*arg13 = sp108;

										*geoptr = geo;
									}
								}
							}
						}
					}
				}
			}

			geo = (struct geo *)((u32)geo + (u32)(tile->header.numvertices * 6) + 0xe);
		} else if (geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) geo;
			struct coord min;
			struct coord max;
			struct coord spe0;
			struct coord spd4;
			struct coord spc8;
			struct coord spbc;

			if (geo->flags & geoflags) {
				min.x = tile->vertices[tile->xmin].x;
				max.x = tile->vertices[tile->xmax].x;
				min.z = tile->vertices[tile->zmin].z;
				max.z = tile->vertices[tile->zmax].z;

				if ((!(arg2->x < min.x) || !(arg3->x < min.x))
						&& (!(arg2->x > max.x) || !(arg3->x > max.x))
						&& (!(arg2->z < min.z) || !(arg3->z < min.z))
						&& (!(arg2->z > max.z) || !(arg3->z > max.z))) {
					if (checkvertical) {
						min.y = tile->vertices[tile->ymin].y;
						max.y = tile->vertices[tile->ymax].y;

						if ((!(arg2->y < min.y) || !(arg3->y < min.y))
								&& (!(arg2->y > max.y) || !(arg3->y > max.y))
								&& bgTestLineIntersectsBbox(arg2, arg4, &min, &max)
								&& cd0002ab98FltTile(arg2, arg3, arg4, tile, &spe0, &spd4)) {
							x = spe0.x - arg2->x;
							y = spe0.y - arg2->y;
							z = spe0.z - arg2->z;

							sum = x * x + y * y + z * z;

							if (sum < *arg10) {
								result = true;
								*arg10 = sum;

								*arg11 = spe0;

								cd0002c528FltTile(tile, &spe0, &spd4, arg12, arg13);

								*geoptr = geo;
							}
						}
					} else if (cd0002b128FltTile(arg2, arg3, arg4, tile, &spe0, &spc8, &spbc, arg7, ymax, ymin)) {
						x = spe0.x - arg2->x;
						y = spe0.y - arg2->y;
						z = spe0.z - arg2->z;

						sum = x * x + y * y + z * z;

						if (sum < *arg10) {
							result = true;
							*arg10 = sum;

							*arg11 = spe0;

							*arg12 = spc8;

							*arg13 = spbc;

							*geoptr = geo;
						}
					}
				}
			}

			geo = (struct geo *)((u32)geo + (u32)(tile->header.numvertices - 0x40) * 0xc + 0x310);
		} else if (geo->type == GEOTYPE_BLOCK) {
			struct coord spb0;
			struct coord spa4;
			struct coord sp98;

			if ((geoflags & (GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT))
					&& (cd0002b560Block(arg2, arg3, arg4, (struct geoblock *)geo, &spb0, &spa4, &sp98, arg7, ymax, ymin))) {
				x = spb0.x - arg2->x;
				y = spb0.y - arg2->y;
				z = spb0.z - arg2->z;

				sum = x * x + y * y + z * z;

				if (sum < *arg10) {
					result = true;
					*arg10 = sum;

					*arg11 = spb0;

					*arg12 = spa4;

					*arg13 = sp98;

					*geoptr = geo;
				}
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geoblock));
		} else if (geo->type == GEOTYPE_CYL) {
			struct geocyl *cyl = (struct geocyl *) geo;
			struct coord sp88;
			struct coord sp7c;
			struct coord sp70;

			if ((geoflags & geo->flags)
					&& cd0002b954Cyl(arg2, arg3, arg4, cyl, &sp88, &sp7c, &sp70, arg7, ymax, ymin)) {
				x = sp88.x - arg2->x;
				y = sp88.y - arg2->y;
				z = sp88.z - arg2->z;

				sum = x * x + y * y + z * z;

				if (sum < *arg10) {
					result = true;
					*arg10 = sum;

					*arg11 = sp88;

					*arg12 = sp7c;

					*arg13 = sp70;

					*geoptr = geo;
				}
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geocyl));
		}
	}

	return !result;
}

bool cdTestAToB(struct coord *pos, struct coord *coord2, s16 *rooms, u32 types, u16 geoflags, bool checkvertical, s32 arg6, f32 ymax, f32 ymin)
{
	s32 roomnum;
	s16 *roomptr;
	u8 *start;
	u8 *end;
	struct coord sp27c;
	s16 *propnumptr;
	s16 propnums[256];

	sp27c.x = coord2->x - pos->x;
	sp27c.y = coord2->y - pos->y;
	sp27c.z = coord2->z - pos->z;

	if (types & CDTYPE_BG) {
		roomptr = rooms;
		roomnum = rooms[0];

		while (roomnum != -1) {
			if (roomnum < g_TileNumRooms) {
				start = g_TileFileData.u8 + g_TileRooms[roomnum];
				end = g_TileFileData.u8 + g_TileRooms[roomnum + 1];

				if (cdTestAToBGeolist(start, end, pos, coord2, &sp27c, geoflags, checkvertical, arg6, ymax, ymin) == 0) {
					cdSetObstacleProp(NULL);
					return false;
				}
			}

			roomptr++;
			roomnum = *roomptr;
		}
	}

	roomGetProps(rooms, propnums, 256);

	propnumptr = propnums;

	while (*propnumptr >= 0) {
		struct prop *prop = &g_Vars.props[*propnumptr];

		if (propIsOfCdType(prop, types)
				&& propUpdateGeometry(prop, &start, &end)
				&& cdTestAToBGeolist(start, end, pos, coord2, &sp27c, geoflags, checkvertical, arg6, ymax, ymin) == 0) {
			cdSetObstacleProp(prop);
			return false;
		}

		propnumptr++;
	}

	return true;
}

s32 cdExamAToB(struct coord *arg0, struct coord *arg1, s16 *rooms, s32 types, u16 geoflags, bool checkvertical, s32 arg6, f32 ymax, f32 ymin);

s32 cdExamAToB(struct coord *arg0, struct coord *arg1, s16 *rooms, s32 types, u16 geoflags, bool checkvertical, s32 arg6, f32 ymax, f32 ymin)
{
	s32 roomnum;
	s16 *roomptr;
	u8 *start;
	u8 *end;
	struct coord sp2c4;
	bool sp2c0 = false;
	struct coord sp2b4;
	struct coord sp2a8;
	struct coord sp29c;
	f32 sp298 = 4294967296;
	struct geo *sp294;
	s16 *propnumptr;
	s16 propnums[256];

	sp2c4.x = arg1->x - arg0->x;
	sp2c4.y = arg1->y - arg0->y;
	sp2c4.z = arg1->z - arg0->z;

	if (types & CDTYPE_BG) {
		roomptr = rooms;
		roomnum = rooms[0];

		while (roomnum != -1) {
			if (roomnum < g_TileNumRooms) {
				start = g_TileFileData.u8 + g_TileRooms[roomnum];
				end = g_TileFileData.u8 + g_TileRooms[roomnum + 1];

				if (!cdExamAToBGeolist(start, end, arg0, arg1, &sp2c4, geoflags, checkvertical, arg6, ymax, ymin, &sp298, &sp2b4, &sp2a8, &sp29c, &sp294, roomnum)) {
					sp2c0 = true;
					cdSetObstacleVtxColPropFltGeo(&sp2a8, &sp29c, &sp2b4, NULL, sp298, sp294);
				}
			}

			roomptr++;
			roomnum = *roomptr;
		}
	}

	roomGetProps(rooms, propnums, 256);
	propnumptr = propnums;

	while (*propnumptr >= 0) {
		struct prop *prop = &g_Vars.props[*propnumptr];

		if (propIsOfCdType(prop, types)
				&& propUpdateGeometry(prop, &start, &end)
				&& !cdExamAToBGeolist(start, end, arg0, arg1, &sp2c4, geoflags, checkvertical, arg6, ymax, ymin, &sp298, &sp2b4, &sp2a8, &sp29c, &sp294, -999)) {
			sp2c0 = true;
			cdSetObstacleVtxColPropFltGeo(&sp2a8, &sp29c, &sp2b4, prop, sp298, sp294);
		}

		propnumptr++;
	}

	return !sp2c0;
}

bool cdTestCylMove01(struct coord *pos, s16 *rooms, struct coord *targetpos, u32 types, u32 arg4, f32 ymax, f32 ymin)
{
	s16 sp44[21];
	s16 sp34[8];

	portal00018148(pos, targetpos, rooms, sp34, sp44, 20);

	return cdTestAToB(pos, targetpos, sp44, types, GEOFLAG_WALL, CHECKVERTICAL_NO, arg4, ymax, ymin);
}

s32 cdTestCylMove02(struct coord *pos, s16 *rooms, struct coord *coord2, s16 *rooms2, u32 types, s32 arg5, f32 ymax, f32 ymin)
{
	s32 result;
	s16 sp44[20];
	s16 sp34[8];

	func0f065d1c(pos, rooms, coord2, sp34, sp44, 20);

	if (arrayIntersects(sp34, rooms2)) {
		result = cdTestAToB(pos, coord2, sp44, types, GEOFLAG_WALL, CHECKVERTICAL_NO, arg5, ymax, ymin);
	} else {
		result = CDRESULT_COLLISION;
	}

	return result;
}

s32 cdExamCylMove03(struct coord *pos, s16 *rooms, struct coord *arg2, u32 types, u32 arg4, f32 ymax, f32 ymin)
{
	s16 sp44[21];
	s16 sp34[8];

	portal00018148(pos, arg2, rooms, sp34, sp44, 20);

	return cdExamAToB(pos, arg2, sp44, types, GEOFLAG_WALL, CHECKVERTICAL_NO, arg4, ymax, ymin);
}

s32 cdTestCylMove04(struct coord *arg0, s16 *arg1, struct coord *arg2, s16 *arg3, u32 types, s32 arg5, f32 ymax, f32 ymin)
{
	s16 rooms[21];

	portal00018148(arg0, arg2, arg1, arg3, rooms, 20);

	return cdTestAToB(arg0, arg2, rooms, types, GEOFLAG_WALL, CHECKVERTICAL_NO, arg5, ymax, ymin);
}

s32 cdExamCylMove05(struct coord *pos, s16 *rooms, struct coord *pos2, s16 *rooms2, s32 types, bool arg5, f32 ymax, f32 ymin)
{
	s16 sp44[21];
	s16 sp34[8];
	s32 result;

	func0f065d1c(pos, rooms, pos2, sp34, sp44, 20);

	result = cdExamAToB(pos, pos2, sp44, types, GEOFLAG_WALL, CHECKVERTICAL_NO, arg5, ymax, ymin);

	if (result != CDRESULT_COLLISION && !arrayIntersects(sp34, rooms2)) {
		cdClearResults();
		result = CDRESULT_ERROR;
	}

	return result;
}

s32 cdExamCylMove06(struct coord *arg0, s16 *arg1, struct coord *arg2, s16 *arg3, f32 width, s32 types, s32 arg6, f32 ymax, f32 ymin)
{
	s16 sp5c[21];
	s16 sp4c[8];
	struct coord sp40;
	s32 result;

	func0f065d1c(arg0, arg1, arg2, sp4c, sp5c, 20);

	result = cdExamAToB(arg0, arg2, sp5c, types, GEOFLAG_WALL, CHECKVERTICAL_NO, arg6, ymax, ymin);

	if (result == CDRESULT_COLLISION) {
		sp40.x = arg2->x - arg0->x;
		sp40.y = arg2->y - arg0->y;
		sp40.z = arg2->z - arg0->z;

		cd000250cc(arg0, &sp40, width);
	} else if (!arrayIntersects(sp4c, arg3)) {
		cdClearResults();
		result = CDRESULT_ERROR;
	}

	return result;
}

s32 cdExamCylMove07(struct coord *arg0, s16 *arg1, struct coord *arg2, s16 *arg3, u32 types, s32 arg5, f32 ymax, f32 ymin)
{
	s16 rooms[21];

	portal00018148(arg0, arg2, arg1, arg3, rooms, 20);

	return cdExamAToB(arg0, arg2, rooms, types, GEOFLAG_WALL, CHECKVERTICAL_NO, arg5, ymax, ymin);
}

s32 cdExamCylMove08(struct coord *arg0, s16 *arg1, struct coord *arg2, s16 *arg3, f32 width, u32 types, s32 arg6, f32 ymax, f32 ymin)
{
	s16 rooms[21];
	struct coord sp40;
	s32 result;

	portal00018148(arg0, arg2, arg1, arg3, rooms, 20);

	result = cdExamAToB(arg0, arg2, rooms, types, GEOFLAG_WALL, CHECKVERTICAL_NO, arg6, ymax, ymin);

	if (result == CDRESULT_COLLISION) {
		sp40.x = arg2->x - arg0->x;
		sp40.y = arg2->y - arg0->y;
		sp40.z = arg2->z - arg0->z;

		cd000250cc(arg0, &sp40, width);
	}

	return result;
}

bool cdTestLos03(struct coord *viewpos, s16 *rooms, struct coord *targetpos, u32 types, u16 geoflags)
{
	s16 sp44[21];
	s16 sp34[8];

	portal00018148(viewpos, targetpos, rooms, sp34, sp44, 20);

	return cdTestAToB(viewpos, targetpos, sp44, types, geoflags, CHECKVERTICAL_YES, 1, 0, 0);
}

bool cdTestLos04(struct coord *frompos, s16 *fromrooms, struct coord *topos, s32 types)
{
	return cdTestLos03(frompos, fromrooms, topos, types, GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT);
}

bool cdTestLos05(struct coord *coord, s16 *rooms, struct coord *coord2, s16 *rooms2, s32 types, u16 geoflags)
{
	bool result;
	s16 sp44[20];
	s16 sp34[8];

	func0f065d1c(coord, rooms, coord2, sp34, sp44, 20);

	if (arrayIntersects(sp34, rooms2)) {
		result = cdTestAToB(coord, coord2, sp44, types, geoflags, CHECKVERTICAL_YES, 1, 0, 0);
	} else {
		result = false;
	}

	return result;
}

bool cdTestLos06(struct coord *arg0, s16 *rooms1, struct coord *arg2, s16 *rooms2, u32 types)
{
	return cdTestLos05(arg0, rooms1, arg2, rooms2, types, GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT);
}

bool cdTestLos07(struct coord *pos, s16 *rooms, struct coord *pos2, s16 *rooms2, s16 *rooms3, u32 types, u16 geoflags)
{
	bool result;
	s16 sp34[20];

	func0f065d1c(pos, rooms, pos2, rooms3, sp34, 20);

	if (arrayIntersects(rooms3, rooms2)) {
		result = cdTestAToB(pos, pos2, sp34, types, geoflags, CHECKVERTICAL_YES, 1, 0, 0);
	} else {
		result = false;
	}

	return result;
}

s32 cdExamLos08(struct coord *pos, s16 *rooms, struct coord *pos2, u32 types, u16 geoflags)
{
	s16 sp44[21];
	s16 sp34[8];

	portal00018148(pos, pos2, rooms, sp34, sp44, 20);

	return cdExamAToB(pos, pos2, sp44, types, geoflags, CHECKVERTICAL_YES, 1, 0, 0);
}

s32 cdExamLos09(struct coord *pos, s16 *rooms, struct coord *pos2, u32 types)
{
	return cdExamLos08(pos, rooms, pos2, types, GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT);
}

s32 cdTestLos10(struct coord *arg0, s16 *arg1, struct coord *arg2, s16 *arg3, u32 types, u16 geoflags)
{
	s16 rooms[21];

	portal00018148(arg0, arg2, arg1, arg3, rooms, 20);

	return cdTestAToB(arg0, arg2, rooms, types, geoflags, CHECKVERTICAL_YES, 1, 0, 0);
}

s32 cdTestLos11(struct coord *arg0, s16 *arg1, struct coord *arg2, s16 *arg3, u32 types)
{
	return cdTestLos10(arg0, arg1, arg2, arg3, types, GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT);
}

bool cd0002ded8(struct coord *arg0, struct coord *arg1, struct prop *prop)
{
	u8 *start;
	u8 *end;
	struct coord sp7c;
	bool result = false;
	struct coord sp6c;
	struct coord sp60;
	struct coord sp54;
	f32 sp50 = 4294967296;
	struct geo *geo;

	sp7c.x = arg1->x - arg0->x;
	sp7c.y = arg1->y - arg0->y;
	sp7c.z = arg1->z - arg0->z;

	if (propUpdateGeometry(prop, &start, &end)) {
		if (!cdExamAToBGeolist(start, end, arg0, arg1, &sp7c,
					GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT,
					CHECKVERTICAL_YES, 1, 0, 0, &sp50, &sp6c, &sp60, &sp54, &geo, -999)) {
			result = true;
			cdSetObstacleVtxColPropFltGeo(&sp60, &sp54, &sp6c, prop, sp50, geo);
		}
	}

	return !result;
}

/**
 * Return true if both blocks are not intersecting on the X/Z plane.
 */
bool cdBlockExcludesBlockLaterally(struct geoblock *block1, struct geoblock *block2)
{
	u32 stack[4];
	f32 zero = 0.0f;
	s32 numvertices0 = block1->header.numvertices;
	s32 numvertices1 = block2->header.numvertices;
	s32 i;

	for (i = 0; i < numvertices0; i++) {
		s32 next = (i + 1) % numvertices0;
		f64 diff1;
		f64 diff2;

		diff1 = block1->vertices[next][1] - (f64)block1->vertices[i][1];
		diff2 = block1->vertices[i][0] - (f64)block1->vertices[next][0];

		if (diff1 == zero && diff2 == zero) {
			if (cdIs2dPointInBlock(block2, block1->vertices[i][0], block1->vertices[i][1])) {
				return false;
			}
		} else {
			f64 sum1 = block1->vertices[i][0] * diff1 + block1->vertices[i][1] * diff2;
			f64 sum2;
			s32 j = (next + 1) % numvertices0;
			s32 k;

			while (j != i) {
				sum2 = block1->vertices[j][0] * diff1 + block1->vertices[j][1] * diff2;

				if (1);
				if (1);
				if (1);

				if (sum2 != sum1) {
					break;
				}

				j = (j + 1) % numvertices0;
			}

			for (k = 0; k < numvertices1; k++) {
				f64 sum3 = block2->vertices[k][0] * diff1 + block2->vertices[k][1] * diff2;

				if (sum2 == sum1) {
					sum2 = sum1 - sum3 + sum1;
				}

				if ((sum3 < sum1 && sum2 < sum1) || (sum3 > sum1 && sum2 > sum1)) {
					break;
				}
			}

			if (k == numvertices1) {
				return true;
			}
		}
	}

	return false;
}

s32 cdTestBlockOverlapsGeolist(u8 *start, u8 *end, struct geoblock *block, u16 geoflags)
{
	struct geo *geo = (struct geo *) start;

	while (geo < (struct geo *) end) {
		if (geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *) geo;
			geo = (struct geo *)((u32)geo + tile->header.numvertices * 6 + 0xe);
		} else if (geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *) geo;
			geo = (struct geo *)((u32)geo + tile->header.numvertices * 0xc + 0x10);
		} else if (geo->type == GEOTYPE_BLOCK) {
			struct geoblock *thisblock = (struct geoblock *) geo;

			if ((geoflags & (GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT))
					&& thisblock->ymax >= block->ymin
					&& thisblock->ymin <= block->ymax) {
				// Tiles are overlapping vertically
				s32 i;

				for (i = 0; i < block->header.numvertices; i++) {
					if (cdIs2dPointInBlock(thisblock, block->vertices[i][0], block->vertices[i][1])) {
						return CDRESULT_COLLISION;
					}
				}

				for (i = 0; i < thisblock->header.numvertices; i++) {
					if (cdIs2dPointInBlock(block, thisblock->vertices[i][0], thisblock->vertices[i][1])) {
						return CDRESULT_COLLISION;
					}
				}

				// This is a bit wasteful...
				// If A excludes B, there's no point checking if B excludes A.
				if (!cdBlockExcludesBlockLaterally(block, thisblock) && !cdBlockExcludesBlockLaterally(thisblock, block)) {
					return CDRESULT_COLLISION;
				}
			}

			geo = (struct geo *)((u32)geo + 0x4c);
		} else if (geo->type == GEOTYPE_CYL) {
			struct geocyl *cyl = (struct geocyl *) geo;

			if ((geoflags & geo->flags)
					&& cyl->ymax >= block->ymin
					&& cyl->ymin <= block->ymax
					&& cd000274e0Block(block, cyl->x, cyl->z, cyl->radius, NULL, NULL)) {
				return CDRESULT_COLLISION;
			}

			geo = (struct geo *)((u32)geo + 0x18);
		}
	}

	return CDRESULT_NOCOLLISION;
}

/**
 * Test if the given block overlaps any prop. Set the saved obstacle prop if so.
 *
 * The BG tests are pointless and not used, as cdTestBlockOverlapsGeolist only
 * tests blocks and cylinders.
 *
 * The function is used to check if a door is being blocked by another prop,
 * and is also used in a sanity check to make sure a moved object hasn't moved
 * into the player's position.
 */
s32 cdTestBlockOverlapsAnyProp(struct geoblock *geo, s16 *rooms, u32 types)
{
	s32 result = CDRESULT_NOCOLLISION;
	s32 roomnum;
	u8 *start;
	u8 *end;
	s16 *roomptr;
	s16 propnums[256];
	s16 *propnumptr;

	// Check BG
	if (types & CDTYPE_BG) {
		roomptr = rooms;
		roomnum = rooms[0];

		while (roomnum != -1) {
			if (roomnum < g_TileNumRooms) {
				start = g_TileFileData.u8 + g_TileRooms[roomnum];
				end = g_TileFileData.u8 + g_TileRooms[roomnum + 1];

				result = cdTestBlockOverlapsGeolist(start, end, geo, GEOFLAG_WALL);

				if (result == CDRESULT_COLLISION) {
					cdSetObstacleProp(NULL);
					break;
				}
			}

			roomptr++;
			roomnum = *roomptr;
		}
	}

	// Check props
	if (result != CDRESULT_COLLISION) {
		roomGetProps(rooms, propnums, 256);
		propnumptr = propnums;

		while (*propnumptr >= 0) {
			struct prop *prop = &g_Vars.props[*propnumptr];

			if (propIsOfCdType(prop, types) && propUpdateGeometry(prop, &start, &end)) {
				result = cdTestBlockOverlapsGeolist(start, end, geo, GEOFLAG_WALL);

				if (result == CDRESULT_COLLISION) {
					cdSetObstacleProp(prop);
					break;
				}
			}

			propnumptr++;
		}
	}

	return result;
}

bool cd0002e680IntTile(struct geotilei *tile, s32 numvertices, struct coord *verts, struct coord *diffs, struct prop *prop, struct geoblock *block)
{
	bool result = false;
	s32 i;
	s32 next;
	s32 curr;
	struct coord sp84;
	struct coord sp78;
	struct coord sp6c;

	for (i = 0; i < numvertices; i++) {
		if (var8005f030) {
			s32 remaining = numvertices - i;
			next = (remaining + numvertices - 2) % numvertices;
			curr = remaining - 1;
		} else {
			next = (i + 1) % numvertices;
			curr = i;
		}

		if (cd0002ac70IntTile((struct coord *)((u32)verts + curr * sizeof(struct coord)),
					(struct coord *)((u32)verts + next * sizeof(struct coord)),
					(struct coord *)((u32)diffs + curr * sizeof(struct coord)),
					tile, &sp6c, &sp84, &sp78, 0, 0.0f, 0.0f)) {
			cdSetObstacleVtxColProp(&sp84, &sp78, &sp6c, prop);
			cdSetSavedPos((struct coord *)((u32)verts + curr * sizeof(struct coord)), (struct coord *)((u32)verts + next * sizeof(struct coord)));
			cdSetSavedBlock(block);
			result = true;
			break;
		}
	}

	return result;
}

bool cd0002e82cIntTile(struct geotilef *tile, s32 numvertices, struct coord *verts, struct coord *diffs, struct prop *prop, struct geoblock *block)
{
	bool result = false;
	s32 i;
	s32 next;
	s32 curr;
	struct coord sp84;
	struct coord sp78;
	struct coord sp6c;

	for (i = 0; i < numvertices; i++) {
		if (var8005f030) {
			s32 remaining = numvertices - i;
			next = (remaining + numvertices - 2) % numvertices;
			curr = remaining - 1;
		} else {
			next = (i + 1) % numvertices;
			curr = i;
		}

		if (cd0002b128FltTile((struct coord *)((u32)verts + curr * sizeof(struct coord)),
					(struct coord *)((u32)verts + next * sizeof(struct coord)),
					(struct coord *)((u32)diffs + curr * sizeof(struct coord)),
					tile, &sp6c, &sp84, &sp78, 0, 0.0f, 0.0f)) {
			cdSetObstacleVtxColProp(&sp84, &sp78, &sp6c, prop);
			cdSetSavedPos((struct coord *)((u32)verts + curr * sizeof(struct coord)), (struct coord *)((u32)verts + next * sizeof(struct coord)));
			cdSetSavedBlock(block);
			result = true;
			break;
		}
	}

	return result;
}

bool cd0002e9d8Block(struct geoblock *thisblock, s32 numvertices, struct coord *verts, struct coord *diffs, struct prop *prop, struct geoblock *block)
{
	bool result = false;
	s32 i;
	s32 next;
	s32 curr;
	struct coord sp84;
	struct coord sp78;
	struct coord sp6c;

	for (i = 0; i < numvertices; i++) {
		if (var8005f030) {
			s32 remaining = numvertices - i;
			next = (remaining + numvertices - 2) % numvertices;
			curr = remaining - 1;
		} else {
			next = (i + 1) % numvertices;
			curr = i;
		}

		if (cd0002b560Block((struct coord *)((u32)verts + curr * sizeof(struct coord)),
					(struct coord *)((u32)verts + next * sizeof(struct coord)),
					(struct coord *)((u32)diffs + curr * sizeof(struct coord)),
					thisblock, &sp6c, &sp84, &sp78, 0, 0.0f, 0.0f)) {
			cdSetObstacleVtxColProp(&sp84, &sp78, &sp6c, prop);
			cdSetSavedPos((struct coord *)((u32)verts + curr * sizeof(struct coord)), (struct coord *)((u32)verts + next * sizeof(struct coord)));
			cdSetSavedBlock(block);
			result = true;
			break;
		}
	}

	return result;
}

bool cd0002eb84Cyl(struct geocyl *cyl, s32 numvertices, struct coord *arg2, struct coord *arg3, struct prop *prop, struct geoblock *block)
{
	bool result = false;
	s32 i;
	s32 next;
	s32 curr;
	struct coord sp84;
	struct coord sp78;
	struct coord sp6c;

	for (i = 0; i < numvertices; i++) {
		if (var8005f030) {
			s32 remaining = numvertices - i;
			next = (remaining + numvertices - 2) % numvertices;
			curr = remaining - 1;
		} else {
			next = (i + 1) % numvertices;
			curr = i;
		}

		if (cd0002b954Cyl((struct coord *)((u32)arg2 + curr * sizeof(struct coord)),
					(struct coord *)((u32)arg2 + next * sizeof(struct coord)),
					(struct coord *)((u32)arg3 + curr * sizeof(struct coord)),
					cyl, &sp6c, &sp84, &sp78, 0, 0.0f, 0.0f)) {
			cdSetObstacleVtxColProp(&sp84, &sp78, &sp6c, prop);
			cdSetSavedPos((struct coord *)((u32)arg2 + curr * sizeof(struct coord)), (struct coord *)((u32)arg2 + next * sizeof(struct coord)));
			cdSetSavedBlock(block);
			result = true;
			break;
		}
	}

	return result;
}

bool cd0002ed30(u8 *start, u8 *end, struct geoblock *block, s32 numvertices, struct coord *verts, struct coord *diffs, u16 geoflags, struct prop *prop)
{
	struct geo *geo = (struct geo *) start;

	while (geo < (struct geo *) end) {
		if (1);

		if (geo->type == GEOTYPE_TILE_I) {
			struct geotilei *tile = (struct geotilei *)geo;

			if ((geoflags & geo->flags)
					&& *(s16 *)(tile->ymax + (u32)tile) >= block->ymin
					&& *(s16 *)(tile->ymin + (u32)tile) <= block->ymax
					&& cd0002e680IntTile(tile, numvertices, verts, diffs, prop, block)) {
				return false;
			}

			geo = (struct geo *)((u32)geo + tile->header.numvertices * 6 + 0xe);
		} else if (geo->type == GEOTYPE_TILE_F) {
			struct geotilef *tile = (struct geotilef *)geo;

			if ((geoflags & geo->flags)
					&& tile->vertices[tile->ymax].y >= block->ymin
					&& tile->vertices[tile->ymin].y <= block->ymax
					&& cd0002e82cIntTile(tile, numvertices, verts, diffs, prop, block)) {
				return false;
			}

			geo = (struct geo *)((u32)geo + (u32)(tile->header.numvertices - 0x40) * 0xc + 0x310);
		} else if (geo->type == GEOTYPE_BLOCK) {
			struct geoblock *block2 = (struct geoblock *)geo;

			if ((geoflags & (GEOFLAG_WALL | GEOFLAG_BLOCK_SIGHT | GEOFLAG_BLOCK_SHOOT))
					&& block2->ymax >= block->ymin
					&& block2->ymin <= block->ymax
					&& cd0002e9d8Block(block2, numvertices, verts, diffs, prop, block)) {
				return false;
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geoblock));
		} else if (geo->type == GEOTYPE_CYL) {
			struct geocyl *cyl = (struct geocyl *)geo;

			if ((geoflags & geo->flags)
					&& cyl->ymax >= block->ymin
					&& cyl->ymin <= block->ymax
					&& cd0002eb84Cyl(cyl, numvertices, verts, diffs, prop, block)) {
				return false;
			}

			geo = (struct geo *)((u32)geo + sizeof(struct geocyl));
		}
	}

	return true;
}

bool cd0002f02c(struct geoblock *block, s16 *rooms, s32 types)
{
	s32 numvertices = block->header.numvertices;
	s32 i;
	u8 *start;
	u8 *end;
	s32 next;
	s16 propnums[256];
	s16 *propnumptr;
	bool result = true;
	struct coord verts[8];
	struct coord diffs[8];

	for (i = 0; i < numvertices; i++) {
		verts[i].x = block->vertices[i][0];
		verts[i].y = block->ymax;
		verts[i].z = block->vertices[i][1];
	}

	for (i = 0; i < numvertices; i++) {
		next = (i + 1) % numvertices;

		diffs[i].x = verts[next].x - verts[i].x;
		diffs[i].y = verts[next].y - verts[i].y;
		diffs[i].z = verts[next].z - verts[i].z;
	}

	if (types & CDTYPE_BG) {
		s16 *roomsptr = rooms;
		s32 roomnum = *roomsptr;

		while (roomnum != -1) {
			if (roomnum < g_TileNumRooms) {
				start = g_TileFileData.u8 + g_TileRooms[roomnum];
				end = g_TileFileData.u8 + g_TileRooms[roomnum + 1];

				result = cd0002ed30(start, end, block, numvertices, verts, diffs, GEOFLAG_WALL, NULL);

				if (!result) {
					break;
				}
			}

			roomsptr++;
			roomnum = *roomsptr;
		}
	}

	if (result) {
		roomGetProps(rooms, propnums, 256);

		propnumptr = propnums;

		while (*propnumptr >= 0) {
			struct prop *prop = &g_Vars.props[*propnumptr];

			if (propIsOfCdType(prop, types)) {
				if (propUpdateGeometry(prop, &start, &end)) {
					result = cd0002ed30(start, end, block, numvertices, verts, diffs, GEOFLAG_WALL, prop);

					if (!result) {
						break;
					}
				}
			}

			propnumptr++;
		}
	}

	return result;
}

#if VERSION < VERSION_NTSC_1_0
struct debugtri *cdReadIntTileVertices(struct debugtri *ptr, s32 *remaining, struct geotilei *tile)
{
	if (tile->header.flags & (GEOFLAG_FLOOR1 | GEOFLAG_WALL)) {
		s32 i;
		s32 numvertices = tile->header.numvertices;
		s16 vertices[16][3];

		if (numvertices > 16) {
			numvertices = 16;
		}

		for (i = 0; i < numvertices; i++) {
			vertices[i][0] = tile->vertices[i][0] - g_Vars.currentplayer->globaldrawworldoffset.x;
			vertices[i][1] = tile->vertices[i][1] - g_Vars.currentplayer->globaldrawworldoffset.y;
			vertices[i][2] = tile->vertices[i][2] - g_Vars.currentplayer->globaldrawworldoffset.z;
		}

		for (i = 2; i < numvertices; i++) {
			if (*remaining > 0) {
				ptr->vertices[0][0] = vertices[0][0];
				ptr->vertices[0][1] = vertices[0][1];
				ptr->vertices[0][2] = vertices[0][2];
				ptr->vertices[1][0] = vertices[i - 1][0];
				ptr->vertices[1][1] = vertices[i - 1][1];
				ptr->vertices[1][2] = vertices[i - 1][2];
				ptr->vertices[2][0] = vertices[i][0];
				ptr->vertices[2][1] = vertices[i][1];
				ptr->vertices[2][2] = vertices[i][2];
				ptr->unk12 = 0;

				ptr++;
			}

			*remaining -= 1;
		}
	}

	return ptr;
}
#endif

#if VERSION < VERSION_NTSC_1_0
struct debugtri *cdReadFltTileVertices(struct debugtri *ptr, s32 *remaining, struct geotilef *tile)
{
	if (tile->header.flags & (GEOFLAG_FLOOR1 | GEOFLAG_WALL)) {
		s32 i;
		s32 numvertices = tile->header.numvertices;
		s16 vertices[16][3];

		if (numvertices > 16) {
			numvertices = 16;
		}

		for (i = 0; i < numvertices; i++) {
			vertices[i][0] = tile->vertices[i].x - g_Vars.currentplayer->globaldrawworldoffset.x;
			vertices[i][1] = tile->vertices[i].y - g_Vars.currentplayer->globaldrawworldoffset.y;
			vertices[i][2] = tile->vertices[i].z - g_Vars.currentplayer->globaldrawworldoffset.z;
		}

		for (i = 2; i < numvertices; i++) {
			if (*remaining > 0) {
				ptr->vertices[0][0] = vertices[0][0];
				ptr->vertices[0][1] = vertices[0][1];
				ptr->vertices[0][2] = vertices[0][2];
				ptr->vertices[1][0] = vertices[i - 1][0];
				ptr->vertices[1][1] = vertices[i - 1][1];
				ptr->vertices[1][2] = vertices[i - 1][2];
				ptr->vertices[2][0] = vertices[i][0];
				ptr->vertices[2][1] = vertices[i][1];
				ptr->vertices[2][2] = vertices[i][2];
				ptr->unk12 = 0;

				ptr++;
			}

			*remaining -= 1;
		}
	}

	return ptr;
}
#endif

Gfx *cdRender(Gfx *gdl, u32 arg1, u32 arg2, u32 arg3)
{
	return gdl;
}

void cd0002f2fc(u32 arg0, u32 arg1)
{
	// empty
}

bool cdIsNearlyInSightWithFlags(struct coord *viewpos, s16 *rooms, struct coord *targetpos, f32 distance, s32 types, u16 geoflags)
{
	struct coord diff;
	f32 x;
	f32 z;
	struct coord vector;

	if (cdTestLos03(viewpos, rooms, targetpos, types, geoflags)) {
		return true;
	}

	vector.x = targetpos->x - viewpos->x;
	vector.y = 0;
	vector.z = targetpos->z - viewpos->z;

	guNormalize(&vector.x, &vector.y, &vector.z);

	x = vector.f[0] * distance;
	z = vector.f[2] * distance;

	diff.x = targetpos->x - z;
	diff.y = targetpos->y;
	diff.z = targetpos->z + x;

	if (cdTestLos03(viewpos, rooms, &diff, types, geoflags)) {
		return true;
	}

	diff.x = targetpos->x + z;
	diff.y = targetpos->y;
	diff.z = targetpos->z - x;

	if (cdTestLos03(viewpos, rooms, &diff, types, geoflags)) {
		return true;
	}

	return false;
}

bool cdIsNearlyInSight(struct coord *viewpos, s16 *rooms, struct coord *targetpos, f32 distance, s32 types)
{
	return cdIsNearlyInSightWithFlags(viewpos, rooms, targetpos, distance, types, GEOFLAG_BLOCK_SIGHT);
}
