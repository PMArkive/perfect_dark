#include <ultra64.h>
#include "constants.h"
#include "game/chraction.h"
#include "game/chr.h"
#include "game/body.h"
#include "game/prop.h"
#include "game/atan2f.h"
#include "game/modelmgr.h"
#include "game/lv.h"
#include "game/modeldef.h"
#include "game/mplayer/mplayer.h"
#include "game/pad.h"
#include "game/propobj.h"
#include "bss.h"
#include "lib/memp.h"
#include "lib/model.h"
#include "lib/mema.h"
#include "lib/rng.h"
#include "lib/mtx.h"
#include "lib/anim.h"
#include "lib/collision.h"
#include "data.h"
#include "types.h"

s32 g_NumActiveHeadsPerGender;
u32 var8009cd24;
u32 g_ActiveMaleHeads[8];
u32 g_ActiveFemaleHeads[8];

s32 g_NumBondBodies = 0;
s32 g_NumMaleGuardHeads = 0;
s32 g_NumFemaleGuardHeads = 0;
s32 g_NumMaleGuardTeamHeads = 0;
s32 g_NumFemaleGuardTeamHeads = 0;
s32 var80062b14 = 0;
s32 var80062b18 = 0;

s32 g_BondBodies[] = {
	BODY_DJBOND,
	BODY_CONNERY,
	BODY_DALTON,
	BODY_MOORE,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

s32 g_MaleGuardHeads[] = {
	HEAD_BEAU1,
	HEAD_CHRIST,
	HEAD_DARLING,
	HEAD_JON,
	HEAD_ROSS,
	HEAD_RUSS,
	HEAD_MARK2,
	HEAD_JAMIE,
	HEAD_DUNCAN2,
	HEAD_BRIAN,
	HEAD_STEVEM,
	HEAD_KEITH,
	HEAD_GRANT,
	HEAD_PENNY,
	HEAD_DAVEC,
	HEAD_JONES,
	HEAD_GRAHAM,
	HEAD_SHAUN,
	HEAD_NEIL2,
	HEAD_EDMCG,
	HEAD_MATT_C,
	HEAD_PEER_S,
	HEAD_ANDY_R,
	HEAD_BEN_R,
	HEAD_STEVE_K,
	HEAD_SCOTT_H,
	HEAD_SANCHEZ,
	HEAD_COOK,
	HEAD_PRYCE,
	HEAD_SILKE,
	HEAD_SMITH,
	HEAD_GARETH,
	HEAD_MURCHIE,
	HEAD_WONG,
	HEAD_CARTER,
	HEAD_TINTIN,
	HEAD_MUNTON,
	HEAD_PHELPS,
	HEAD_KEN,
	HEAD_JOEL,
	HEAD_TIM,
	HEAD_ROBIN,
	-1,
};

s32 g_MaleGuardTeamHeads[] = {
	HEAD_BEAU1,
	HEAD_CHRIST,
	HEAD_DARLING,
	HEAD_JON,
	HEAD_ROSS,
	HEAD_RUSS,
	HEAD_MARK2,
	HEAD_JAMIE,
	HEAD_DUNCAN2,
	HEAD_BRIAN,
	HEAD_STEVEM,
	HEAD_KEITH,
	HEAD_GRANT,
	HEAD_PENNY,
	HEAD_DAVEC,
	HEAD_JONES,
	-1,
};

s32 g_FemaleGuardHeads[] = {
	HEAD_LESLIE_S,
	HEAD_ANKA,
	HEAD_EILEEN_T,
	HEAD_EILEEN_H,
	-1,
};

s32 g_FemaleGuardTeamHeads[] = {
	HEAD_LESLIE_S,
	HEAD_ANKA,
	HEAD_EILEEN_T,
	HEAD_EILEEN_H,
	-1,
};

s32 var80062c80 = 0;
s32 g_ActiveMaleHeadsIndex = 0;
s32 g_ActiveFemaleHeadsIndex = 0;

s32 g_FemGuardHeads[3] = {
	HEAD_ALEX,
	HEAD_JULIANNE,
	HEAD_LAURA,
};

u32 bodyGetRace(s32 bodynum)
{
	return RACE_HUMAN;
}

bool bodyLoad(s32 bodynum)
{
	if (!g_HeadsAndBodies[bodynum].filedata) {
		g_HeadsAndBodies[bodynum].filedata = modeldefLoadToNew(g_HeadsAndBodies[bodynum].filenum);
		return true;
	}

	return false;
}

struct model *body0f02ce8c(s32 bodynum, s32 headnum, struct modelfiledata *bodyfiledata, struct modelfiledata *headfiledata, bool sunglasses, struct model *model, bool isplayer, u8 varyheight)
{
	f32 scale = g_HeadsAndBodies[bodynum].scale * 0.10000001f;
	f32 animscale = g_HeadsAndBodies[bodynum].animscale;
	struct modelnode *node = NULL;
	u32 stack[2];

	if (bodyfiledata == NULL) {
		if (g_HeadsAndBodies[bodynum].filedata == NULL) {
			g_HeadsAndBodies[bodynum].filedata = modeldefLoadToNew(g_HeadsAndBodies[bodynum].filenum);
		}

		bodyfiledata = g_HeadsAndBodies[bodynum].filedata;
	}

	modelCalculateRwDataLen(bodyfiledata);

	if (!g_HeadsAndBodies[bodynum].unk00_01) {
		if (bodyfiledata->skel == &g_SkelChr) {
			node = modelGetPart(bodyfiledata, MODELPART_CHR_HEADSPOT);

			if (node != NULL) {
				if (headnum < 0) {
					headfiledata = func0f18e57c(-1 - headnum, &headnum);
					bodyfiledata->rwdatalen += headfiledata->rwdatalen;
				} else if (headnum > 0) {
					if (headfiledata == NULL) {
						if (g_Vars.normmplayerisrunning && !IS4MB()) {
							headfiledata = modeldefLoadToNew(g_HeadsAndBodies[headnum].filenum);
							g_HeadsAndBodies[headnum].filedata = headfiledata;
							g_FileInfo[g_HeadsAndBodies[headnum].filenum].loadedsize = 0;
							bodyCalculateHeadOffset(headfiledata, headnum, bodynum);
						} else {
							if (g_HeadsAndBodies[headnum].filedata == NULL) {
								g_HeadsAndBodies[headnum].filedata = modeldefLoadToNew(g_HeadsAndBodies[headnum].filenum);
							}

							headfiledata = g_HeadsAndBodies[headnum].filedata;
						}
					}

					modelCalculateRwDataLen(headfiledata);

					bodyfiledata->rwdatalen += headfiledata->rwdatalen;

					if (g_HeadsAndBodies[bodynum].canvaryheight && varyheight) {
						// Set height to between 95% and 115%
						f32 frac = RANDOMFRAC() * 0.05f;
						scale *= 2.0f * frac - 0.05f + 1.0f;
					}
				}
			}
		}
	}

	if (model) {
		if (model->rwdatalen < bodyfiledata->rwdatalen);
	} else {
		model = modelmgrInstantiateModelWithAnim(bodyfiledata);
	}

	if (model) {
		modelSetScale(model, scale);
		modelSetAnimScale(model, animscale);

		if (headfiledata && !g_HeadsAndBodies[bodynum].unk00_01) {
			bodyfiledata->rwdatalen -= headfiledata->rwdatalen;

			modelmgr0f0b32a0(model, node, headfiledata);

			if ((s16)*(s32 *)&headfiledata->skel == SKEL_HEAD) {
				struct modelnode *node2;

				if (!sunglasses) {
					node2 = modelGetPart(headfiledata, MODELPART_HEAD_SUNGLASSES);

					if (node2) {
						union modelrwdata *rwdata = modelGetNodeRwData(model, node2);
						rwdata->toggle.visible = false;
					}
				}

				node2 = modelGetPart(headfiledata, MODELPART_HEAD_HUDPIECE);

				if (node2) {
					union modelrwdata *rwdata = modelGetNodeRwData(model, node2);
					rwdata->toggle.visible = false;
				}
			}
		}
	}

	return model;
}

struct model *body0f02d338(s32 bodynum, s32 headnum, struct modelfiledata *bodyfiledata, struct modelfiledata *headfiledata, bool sunglasses, u8 varyheight)
{
	return body0f02ce8c(bodynum, headnum, bodyfiledata, headfiledata, sunglasses, NULL, false, varyheight);
}

struct model *bodyAllocateModel(s32 bodynum, s32 headnum, u32 spawnflags)
{
	bool sunglasses = false;
	u8 varyheight = true;

	if (spawnflags & SPAWNFLAG_FORCESUNGLASSES) {
		sunglasses = true;
	} else if (spawnflags & SPAWNFLAG_MAYBESUNGLASSES) {
		sunglasses = random() % 2 == 0;
	}

	if (spawnflags & SPAWNFLAG_FIXEDHEIGHT) {
		varyheight = false;
	}

	return body0f02d338(bodynum, headnum, NULL, NULL, sunglasses, varyheight);
}

s32 body0f02d3f8(void)
{
	return g_BondBodies[var80062c80];
}

s32 bodyChooseHead(s32 bodynum)
{
	s32 head;

	if (g_HeadsAndBodies[bodynum].ismale) {
		head = g_ActiveMaleHeads[g_ActiveMaleHeadsIndex++];

		if (g_ActiveMaleHeadsIndex == g_NumActiveHeadsPerGender) {
			g_ActiveMaleHeadsIndex = 0;
		}
	} else if (bodynum == BODY_FEM_GUARD) {
		head = g_FemGuardHeads[random() % 3];
	} else {
		head = g_ActiveFemaleHeads[g_ActiveFemaleHeadsIndex++];

		if (g_ActiveFemaleHeadsIndex == g_NumActiveHeadsPerGender) {
			g_ActiveFemaleHeadsIndex = 0;
		}
	}

	return head;
}

/**
 * Read a "packed" chr definition and create a runtime chr from it.
 *
 * Chr definitions are stored in a packed format in each stage's setup file.
 * The packed format is used for space saving reasons.
 */
void bodyAllocateChr(s32 stagenum, struct packedchr *packed, s32 cmdindex)
{
	struct pad pad;
	s16 rooms[2];
	struct chrdata *chr;
	struct modelfiledata *headfiledata;
	struct model *model;
	struct prop *prop;
	s32 bodynum;
	s32 headnum;
	f32 angle;
	s32 index;

	padUnpack(packed->padnum, PADFIELD_POS | PADFIELD_LOOK | PADFIELD_ROOM, &pad);

	rooms[0] = pad.room;
	rooms[1] = -1;

	if (cdTestVolume(&pad.pos, 20, rooms, CDTYPE_ALL, CHECKVERTICAL_YES, 200, -200) == CDRESULT_COLLISION
			&& packed->chair == -1
			&& (packed->spawnflags & SPAWNFLAG_IGNORECOLLISION) == 0) {
		return;
	}

	if (packed->spawnflags & (SPAWNFLAG_ONLYONA | SPAWNFLAG_ONLYONSA | SPAWNFLAG_ONLYONPA)) {
		if ((packed->spawnflags & (SPAWNFLAG_ONLYONA | SPAWNFLAG_ONLYONSA | SPAWNFLAG_ONLYONPA)) == 0) {
			return;
		}

		if (((packed->spawnflags & SPAWNFLAG_ONLYONA) && lvGetDifficulty() == DIFF_A)
				|| ((packed->spawnflags & SPAWNFLAG_ONLYONSA) && lvGetDifficulty() == DIFF_SA)
				|| ((packed->spawnflags & SPAWNFLAG_ONLYONPA) && lvGetDifficulty() == DIFF_PA)) {
			// ok
		} else {
			return;
		}
	}

	headnum = -55555;
	headfiledata = NULL;

	if (packed->bodynum == 255) {
		bodynum = body0f02d3f8();
	} else {
		bodynum = packed->bodynum;
	}

	if (!g_HeadsAndBodies[bodynum].unk00_01) {
		if (packed->headnum >= 0) {
			headnum = packed->headnum;
		} else if (headnum == -55555) {
			headnum = bodyChooseHead(bodynum);
		}
	}

	if (headnum < 0) {
		index = -1 - headnum;

		if (index >= 0 && index < 22) {
			headfiledata = func0f18e57c(index, &headnum);
		}

		model = body0f02ce8c(bodynum, headnum, NULL, headfiledata, false, NULL, false, false);
	} else {
		model = bodyAllocateModel(bodynum, headnum, packed->spawnflags);
	}

	if (model != NULL) {
		angle = atan2f(pad.look.x, pad.look.z);
		prop = chrAllocate(model, &pad.pos, rooms, angle);

		if (prop != NULL) {
			propActivate(prop);
			propEnable(prop);

			chr = prop->chr;
			chrSetChrnum(chr, packed->chrnum);
			chr->hearingscale = packed->hearscale / 1000.0f;
			chr->visionrange = packed->viewdist;
			chr->padpreset1 = packed->padpreset;
			chr->chrpreset1 = packed->chrpreset;
			chr->headnum = headnum;
			chr->bodynum = bodynum;
			chr->race = bodyGetRace(chr->bodynum);

			chr->rtracked = false;

			if (bodynum == BODY_DRCAROLL) {
				chr->drcarollimage_left = 0;
				chr->drcarollimage_right = 0;
				chr->height = 185;
				chr->radius = 30;
			} else if (bodynum == BODY_CHICROB) {
				chr->unk348[0] = mempAlloc(sizeof(struct fireslotthing), MEMPOOL_STAGE);
				chr->unk348[1] = mempAlloc(sizeof(struct fireslotthing), MEMPOOL_STAGE);
				chr->unk348[0]->beam = mempAlloc(ALIGN16(sizeof(struct beam)), MEMPOOL_STAGE);
				chr->unk348[1]->beam = mempAlloc(ALIGN16(sizeof(struct beam)), MEMPOOL_STAGE);
				chr->unk348[0]->beam->age = -1;
				chr->unk348[1]->beam->age = -1;
				chr->height = 200;
				chr->radius = 42;
			}

			if (packed->spawnflags & SPAWNFLAG_INVINCIBLE) {
				chr->chrflags |= CHRCFLAG_INVINCIBLE;
			}

			if (packed->spawnflags & SPAWNFLAG_00000200) {
				chr->hidden |= CHRHFLAG_00400000;
			}

			if (packed->spawnflags & SPAWNFLAG_ANTINONINTERACTABLE) {
				chr->hidden |= CHRHFLAG_ANTINONINTERACTABLE;
			}

			if (packed->spawnflags & SPAWNFLAG_DONTSHOOTME) {
				chr->hidden |= CHRHFLAG_DONTSHOOTME;
			}

			if (packed->spawnflags & SPAWNFLAG_HIDDEN) {
				chr->chrflags |= CHRCFLAG_HIDDEN;
			}

			if (packed->spawnflags & SPAWNFLAG_RTRACKED) {
				chr->rtracked = true;
			}

			if (packed->spawnflags & SPAWNFLAG_NOBLOOD) {
				chr->noblood = true;
			}

			if (packed->spawnflags & SPAWNFLAG_BLUESIGHT) {
				chr->hidden2 |= CHRH2FLAG_BLUESIGHT;
			}

			chr->flags = packed->flags;
			chr->flags2 = packed->flags2;
			chr->team = packed->team;
			chr->squadron = packed->squadron;
			chr->aibot = NULL;

			if (packed->tude != 4) {
				chr->tude = packed->tude;
			} else {
				chr->tude = random() % 4;
			}

			chr->voicebox = random() % 3;

			if (!g_HeadsAndBodies[chr->bodynum].ismale) {
				chr->voicebox = VOICEBOX_FEMALE;
			}

			chr->naturalanim = packed->naturalanim;
			chr->myspecial = packed->chair;
			chr->yvisang = packed->yvisang;

			packed->chrindex = chr - g_ChrSlots;

			chr->teamscandist = packed->teamscandist;
			chr->convtalk = packed->convtalk;

			if (chr->flags & CHRFLAG0_CAN_HEARSPAWN) {
				chr->chrflags |= CHRCFLAG_CLONEABLE;
			}

			if (random() % 5 == 0) {
				// Make chr punch slower
				chr->flags2 |= CHRFLAG1_ADJUSTPUNCHSPEED;
			}
		}
	}
}

void body0f02ddbf(void)
{
	// empty
}

/**
 * Tweak the head's Y offset to suit the body.
 *
 * By default, heads and their matching bodies align perfectly and don't need
 * any tweaking. This function is used in multiplayer where players can put any
 * heads on any bodies.
 */
void bodyCalculateHeadOffset(struct modelfiledata *headfiledata, s32 headnum, s32 bodynum)
{
	struct modelnode *node;
	struct modelnode *prev;
	Gfx *gdl;
	s32 offset;
	struct modelrodata_bbox *bbox;
	s32 i;

#if VERSION >= VERSION_JPN_FINAL
	offset = 0;

	switch (headnum) {
	case HEAD_DARK_COMBAT:
	case HEAD_DARK_FROCK:
	case HEAD_DARKAQUA:
	case HEAD_DARK_SNOW:
		switch (bodynum) {
		case BODY_DARK_COMBAT:
		case BODY_DARK_FROCK:
		case BODY_DARK_TRENCH:
		case BODY_DARK_RIPPED:
		case BODY_DARK_AF1:
		case BODY_DARKWET:
		case BODY_DARKAQUALUNG:
		case BODY_DARKSNOW:
		case BODY_DARKLAB:
		case BODY_DARK_LEATHER:
		case BODY_DARK_NEGOTIATOR:
			break;
		default:
			offset = -12;
			break;
		}
		break;
	}
#endif

	if ((s16)(*(s32 *)&headfiledata->skel) == SKEL_HEAD) {
#if VERSION >= VERSION_JPN_FINAL
		if (g_HeadsAndBodies[headnum].type == g_HeadsAndBodies[bodynum].type && offset == 0) {
			return;
		}
#else
		if (g_HeadsAndBodies[headnum].type == g_HeadsAndBodies[bodynum].type) {
			return;
		}
#endif

#if VERSION >= VERSION_JPN_FINAL
		switch (g_HeadsAndBodies[headnum].type) {
		default:
		case HEADBODYTYPE_FEMALE:
			offset += 0;
			break;
		case HEADBODYTYPE_MAIAN:
			offset += 0;
			break;
		case HEADBODYTYPE_DEFAULT:
			offset -= 35;
			break;
		case HEADBODYTYPE_MRBLONDE:
			offset += 0;
			break;
		case HEADBODYTYPE_CASS:
			offset -= 20;
			break;
		case HEADBODYTYPE_FEMALEGUARD:
			offset -= 40;
			break;
		}
#else
		// Same as JPN, but sets the value rather than adjusts
		switch (g_HeadsAndBodies[headnum].type) {
		default:
		case HEADBODYTYPE_FEMALE:
			offset = 0;
			break;
		case HEADBODYTYPE_MAIAN:
			offset = 0;
			break;
		case HEADBODYTYPE_DEFAULT:
			offset = -35;
			break;
		case HEADBODYTYPE_MRBLONDE:
			offset = 0;
			break;
		case HEADBODYTYPE_CASS:
			offset = -20;
			break;
		case HEADBODYTYPE_FEMALEGUARD:
			offset = -40;
			break;
		}
#endif

		switch (g_HeadsAndBodies[bodynum].type) {
		case HEADBODYTYPE_FEMALE:
			break;
		case HEADBODYTYPE_MAIAN:
			offset -= 30;
			break;
		case HEADBODYTYPE_DEFAULT:
			offset += 35;
			break;
		case HEADBODYTYPE_MRBLONDE:
			break;
		case HEADBODYTYPE_CASS:
			offset += 20;
			break;
		case HEADBODYTYPE_FEMALEGUARD:
			offset += 40;
			break;
		}

		if (g_HeadsAndBodies[bodynum].type == HEADBODYTYPE_FEMALE) {
			if (g_HeadsAndBodies[headnum].type == HEADBODYTYPE_DEFAULT
					|| g_HeadsAndBodies[headnum].type == HEADBODYTYPE_MRBLONDE) {
				offset -= 10;
			} else if (g_HeadsAndBodies[headnum].type == HEADBODYTYPE_CASS
					|| g_HeadsAndBodies[headnum].type == HEADBODYTYPE_FEMALEGUARD) {
				offset -= 5;
			}
		} else if (g_HeadsAndBodies[bodynum].type == HEADBODYTYPE_CASS
				&& (g_HeadsAndBodies[headnum].type == HEADBODYTYPE_DEFAULT
					|| g_HeadsAndBodies[headnum].type == HEADBODYTYPE_MRBLONDE)) {
			offset -= 5;
		}

		// Apply the offset
		if (offset != 0) {
			node = NULL;

			do {
				prev = node;

				modelIterateDisplayLists(headfiledata, &node, &gdl);

				if (node && node != prev && node->type == MODELNODETYPE_DL) {
					struct modelrodata_dl *rodata = &node->rodata->dl;

					for (i = 0; i < rodata->numvertices; i++) {
						rodata->vertices[i].y += offset;
					}
				}
			} while (node);

			bbox = modelFileDataFindBboxRodata(headfiledata);

			if (bbox != NULL) {
				bbox->ymin += offset;
				bbox->ymax += offset;
			}
		}
	}
}
