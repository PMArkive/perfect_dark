#include <ultra64.h>
#include "constants.h"
#include "game/game_006900.h"
#include "game/bondgun.h"
#include "game/tex.h"
#include "game/savebuffer.h"
#include "game/menugfx.h"
#include "game/menu.h"
#include "game/game_1531a0.h"
#include "game/gfxmemory.h"
#include "game/file.h"
#include "game/utils.h"
#include "bss.h"
#include "lib/vi.h"
#include "lib/main.h"
#include "lib/rng.h"
#include "lib/mtx.h"
#include "data.h"
#include "types.h"

#define NUM_SUCCESS_PARTICLES 280

#define BLURIMG_WIDTH  40
#define BLURIMG_HEIGHT 30
#define SAMPLE_WIDTH  8
#define SAMPLE_HEIGHT 8
#define PXTOBYTES(val) ((val) * 2)

/**
 * Blur the gameplay background for the pause menu.
 *
 * The blurred image is 30x40 pixels at 16 bits per pixel. At standard
 * resolution this is 1/8th the size of the framebuffer.
 *
 * This function reads the framebuffer in blocks of 8x8 pixels. Each block's
 * R/G/B components are averaged and used to set a pixel in the blurred buffer.
 *
 * If hi-res is being used, every second horizontal pixel on the framebuffer is
 * read instead. The blurred image is the same size regardless of hi-res.
 *
 * The transition effect when pausing and unpausing is implemented elsewhere.
 * It's a simple fade between the source framebuffer and the blurred image.
 * Only one blurred image is made.
 */
void menugfxCreateBlur(void)
{
	u8 *fb = (u8 *) viGetFrontBuffer();
	s32 dstx;
	s32 dsty;
#if PAL
	s32 fbwidthinbytes = viGetWidth() * 2;
	f32 scale = viGetWidth() / 320.0f;
#else
	s32 fbwidthinbytes = PXTOBYTES(viGetWidth());
#endif
	s32 srcx;
	s32 srcy;
	u32 r;
	u32 g;
	u32 b;
	u16 colour;

#if PAL
	g_ScaleX = 1;
#else
	g_ScaleX = (g_ViRes == VIRES_HI) ? 2 : 1;
#endif

	fb = (u8 *) viGetFrontBuffer();

	for (dsty = 0; dsty < BLURIMG_HEIGHT; dsty++) {

		for (dstx = 0; dstx < BLURIMG_WIDTH; dstx++) {
			s32 dstindex = PXTOBYTES(dsty * BLURIMG_WIDTH) + PXTOBYTES(dstx);

#if PAL
			s32 samplestartindex = (((s32) ((f32) dstx * 2 * 4 * 2 * scale) + (s32) (dsty * fbwidthinbytes * 8)) & 0xfffffffe);
#else
			s32 samplestartindex = PXTOBYTES(dstx * SAMPLE_WIDTH) * g_ScaleX + dsty * fbwidthinbytes * SAMPLE_HEIGHT;
#endif

			r = g = b = 0;

			for (srcx = 0; srcx < SAMPLE_WIDTH; srcx++) {
				for (srcy = 0; srcy < SAMPLE_HEIGHT; srcy++) {
#if PAL
					s32 index = (samplestartindex + (s32) (PXTOBYTES((f32) srcx) * scale) + srcy * fbwidthinbytes) & 0xfffffffe;
#else
					s32 index = samplestartindex + PXTOBYTES(srcx) * g_ScaleX + srcy * fbwidthinbytes;
#endif

					colour = fb[index] << 8 | fb[index + 1];

					r = r + (colour >> 11 & 0x1f);
					g = g + (colour >> 6 & 0x1f);
					b = b + (colour >> 1 & 0x1f);
				}
			}

			r /= SAMPLE_WIDTH * SAMPLE_HEIGHT;
			g /= SAMPLE_WIDTH * SAMPLE_HEIGHT;
			b /= SAMPLE_WIDTH * SAMPLE_HEIGHT;

			g_BlurBuffer[dstindex + 0] = ((r << 3) | (g >> 2)) & 0xffff;
			g_BlurBuffer[dstindex + 1] = ((g << 6) | ((b & 0x1f) << 1)) & 0xffff;
		}
	}

	g_ScaleX = 1;
}

Gfx *menugfxRenderBgBlur(Gfx *gdl, u32 colour, s16 arg2, s16 arg3)
{
	u32 *colours;
	struct gfxvtx *vertices;
#if VERSION >= VERSION_PAL_BETA
	s32 width;
	s32 height;
#endif

	if (IS4MB()) {
		return menugfxRenderGradient(gdl, 0, 0, viGetWidth(), viGetHeight(), 0xff, 0xff, 0xff);
	}

	colours = gfxAllocateColours(1);
	vertices = gfxAllocateVertices(4);

	gDPPipeSync(gdl++);
	gSPTexture(gdl++, 0xffff, 0xffff, 0, G_TX_RENDERTILE, G_ON);

	gDPLoadTextureBlock(gdl++, g_BlurBuffer, G_IM_FMT_RGBA, G_IM_SIZ_16b, 40, 30, 0,
			G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP,
			G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);
	gDPSetTextureFilter(gdl++, G_TF_BILERP);

	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

#if VERSION >= VERSION_JPN_FINAL
	width = viGetWidth() * 10;
	height = viGetHeight() * 10;

	*(u16 *)&vertices[0].x = arg2;
	*(u16 *)&vertices[0].y = arg3;
	vertices[0].z = -10;
	*(u16 *)&vertices[1].x = arg2 + 320 * 10u + 40;
	*(u16 *)&vertices[1].y = arg3;
	vertices[1].z = -10;
	*(u16 *)&vertices[2].x = arg2 + 320 * 10u + 40;
	*(u16 *)&vertices[2].y = arg3 + 240 * 10u + 50;
	vertices[2].z = -10;
	*(u16 *)&vertices[3].x = arg2;
	*(u16 *)&vertices[3].y = arg3 + 240 * 10u + 50;
	vertices[3].z = -10;
#elif PAL
	width = viGetWidth() * 10;
	height = viGetHeight() * 10;

	*(u16 *)&vertices[0].x = arg2;
	*(u16 *)&vertices[0].y = arg3;
	vertices[0].z = -10;
	*(u16 *)&vertices[1].x = (s32)width + arg2 + 40;
	*(u16 *)&vertices[1].y = arg3;
	vertices[1].z = -10;
	*(u16 *)&vertices[2].x = (s32)width + arg2 + 40;
	*(u16 *)&vertices[2].y = (s32)height + arg3 + 50;
	vertices[2].z = -10;
	*(u16 *)&vertices[3].x = arg2;
	*(u16 *)&vertices[3].y = (s32)height + arg3 + 50;
	vertices[3].z = -10;
#else
	*(u16 *)&vertices[0].x = arg2;
	*(u16 *)&vertices[0].y = arg3;
	vertices[0].z = -10;
	*(u16 *)&vertices[1].x = arg2 + 320 * 10u + 40;
	*(u16 *)&vertices[1].y = arg3;
	vertices[1].z = -10;
	*(u16 *)&vertices[2].x = arg2 + 320 * 10u + 40;
	*(u16 *)&vertices[2].y = arg3 + 240 * 10u + 50;
	vertices[2].z = -10;
	*(u16 *)&vertices[3].x = arg2;
	*(u16 *)&vertices[3].y = arg3 + 240 * 10u + 50;
	vertices[3].z = -10;
#endif

	vertices[0].s = 0;
	vertices[0].t = 0;
	vertices[1].s = 1280;
	vertices[1].t = 0;
	vertices[2].s = 1280;
	vertices[2].t = 960;
	vertices[3].s = 0;
	vertices[3].t = 960;

	vertices[0].colour = 0;
	vertices[1].colour = 0;
	vertices[2].colour = 0;
	vertices[3].colour = 0;

	colours[0] = colour;

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 1);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 4);

	gDPTri2(gdl++, 0, 1, 2, 2, 3, 0);

	return gdl;
}

void func0f0e0cbc(s32 arg0, s32 arg1, s16 arg2, s16 arg3, struct gfxvtx *vertex, Mtxf *arg5)
{
	struct coord sp24;

	sp24.x = (arg2 - arg0 + 100) * 0.25f;
	sp24.y = (arg3 - arg1 + 100) * 0.25f;
	sp24.z = 0;

	vertex->x = arg2 * 10;
	vertex->y = arg3 * 10;
	vertex->z = -10;

	vertex->colour = 0;

	mtx4TransformVecInPlace(arg5, &sp24);

	vertex->s = sp24.x * 32;
	vertex->t = sp24.y * 32;
}

Gfx *menugfxRenderDialogBackground(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, struct menudialog *dialog, u32 colour1, u32 colour2, f32 arg8)
{
	u32 stack[1];
	u32 leftcolour;
	u32 rightcolour;

	// Render the dialog's background fill
	gdl = textSetPrimColour(gdl, colour1);

	gDPFillRectangleScaled(gdl++, x1, y1, x2, y2);

	gdl = text0f153838(gdl);

	if (dialog->transitionfrac < 0.0f) {
		leftcolour = g_MenuColourPalettes[dialog->type].unk00;
	} else {
		leftcolour = colourBlend(g_MenuColourPalettes[dialog->type2].unk00, g_MenuColourPalettes[dialog->type].unk00, dialog->colourweight);
	}

	if (dialog->transitionfrac < 0.0f) {
		rightcolour = g_MenuColourPalettes[dialog->type].unk08;
	} else {
		rightcolour = colourBlend(g_MenuColourPalettes[dialog->type2].unk08, g_MenuColourPalettes[dialog->type].unk08, dialog->colourweight);
	}

	// Right, left, bottom border
	gdl = menugfxDrawDialogBorderLine(gdl, x2 - 1, y1, x2, y2, rightcolour, rightcolour);
	gdl = menugfxDrawDialogBorderLine(gdl, x1, y1, x1 + 1, y2, leftcolour, leftcolour);
	gdl = menugfxDrawDialogBorderLine(gdl, x1, y2 - 1, x2, y2, leftcolour, rightcolour);

	return gdl;
}

/**
 * This unused function renders an experimental menu background.
 *
 * The background consists of two layers of a green hazy texture.
 * Both layers spin slowly in opposite directions.
 */
Gfx *menugfxRenderBgGreenHaze(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2)
{
	s32 i;
	u32 *colours;
	struct gfxvtx *vertices;
	u32 alphas[2];
	s16 t5;
	s16 s0;
	s16 s2;
	s16 s3;
	f32 f20;
	f32 f22;
	f32 f24;
	f32 f26;
	f32 f0;
	f32 f2;
	u32 stack;

	colours = gfxAllocateColours(4);
	vertices = gfxAllocateVertices(8);

	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);
	gDPSetTextureFilter(gdl++, G_TF_BILERP);

	texSelect(&gdl, &g_TexGeneralConfigs[6], 2, 0, 2, 1, NULL);

	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

	vertices[4].x = vertices[0].x = x1 * 10;
	vertices[4].y = vertices[0].y = y1 * 10;
	vertices[4].z = vertices[0].z = -10;
	vertices[5].x = vertices[1].x = x2 * 10;
	vertices[5].y = vertices[1].y = y1 * 10;
	vertices[5].z = vertices[1].z = -10;
	vertices[6].x = vertices[2].x = x2 * 10;
	vertices[6].y = vertices[2].y = y2 * 10;
	vertices[6].z = vertices[2].z = -10;
	vertices[7].x = vertices[3].x = x1 * 10;
	vertices[7].y = vertices[3].y = y2 * 10;
	vertices[7].z = vertices[3].z = -10;

	for (i = 0; i < 2; i++) {
		s16 tmp = i * 256;
		f0 = g_20SecIntervalFrac;
		f26 = M_BADTAU * g_20SecIntervalFrac;

		if (i == 1) {
			f26 = -f26;
		}

		if (i == 1) {
			f0 += 0.5f;
		}

		if (f0 > 1.0f) {
			f0 -= 1.0f;
		}

		f2 = 1.0f - f0;

		if (f0 < 0.2f) {
			alphas[i] = f0 / 0.2f * 127.0f;
		} else if (f0 > 0.9f) {
			alphas[i] = f2 / 0.1f * 127.0f;
		} else {
			alphas[i] = 0x7f;
		}

		f20 = (f2 + 0.1f) * 15.0f;
		f22 = (x2 - x1) / 2 * f20;
		f24 = (y2 - y1) / 2 * f20;

		s2 = sinf(f26) * f22;
		s3 = cosf(f26) * f24;
		s0 = cosf(f26) * f22;
		t5 = -sinf(f26) * f24;

		vertices[i * 4 + 0].s = tmp - s2 - s0;
		vertices[i * 4 + 0].t = tmp - s3 - t5;
		vertices[i * 4 + 1].s = tmp + s2 - s0;
		vertices[i * 4 + 1].t = tmp + s3 - t5;
		vertices[i * 4 + 2].s = tmp + s2 + s0;
		vertices[i * 4 + 2].t = tmp + s3 + t5;
		vertices[i * 4 + 3].s = tmp - s2 + s0;
		vertices[i * 4 + 3].t = tmp - s3 + t5;
	}

	vertices[0].colour = 0;
	vertices[1].colour = 0;
	vertices[2].colour = 4;
	vertices[3].colour = 4;
	vertices[4].colour = 12;
	vertices[5].colour = 12;
	vertices[6].colour = 8;
	vertices[7].colour = 8;

	colours[0] = 0x00af0000 | alphas[0];
	colours[1] = 0xffff0000 | alphas[0];
	colours[2] = 0x00af0000 | alphas[1];
	colours[3] = 0xffff0000 | alphas[1];

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 4);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 8);

	if (g_20SecIntervalFrac > 0.5f) {
		gDPTri4(gdl++, 4, 5, 6, 6, 7, 4, 0, 1, 2, 2, 3, 0);
	} else {
		gDPTri4(gdl++, 0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4);
	}

	return gdl;
}

Gfx *menugfxDrawDropdownBackground(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2)
{
	u32 *colours = gfxAllocateColours(3);
	struct gfxvtx *vertices = gfxAllocateVertices(6);
	u32 colour1;
	u32 colour2;

	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);
	gDPSetTextureFilter(gdl++, G_TF_BILERP);

	texSelect(&gdl, NULL, 2, 0, 2, true, NULL);

	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

	vertices[0].x = x1 * 10;
	vertices[0].y = y1 * 10;
	vertices[0].z = -10;

	vertices[1].x = x2 * 10;
	vertices[1].y = y1 * 10;
	vertices[1].z = -10;

	vertices[2].x = x1 * 10;
	vertices[2].y = (y2 + y1) / 2 * 10;
	vertices[2].z = -10;

	vertices[3].x = x2 * 10;
	vertices[3].y = (y2 + y1) / 2 * 10;
	vertices[3].z = -10;

	vertices[4].x = x1 * 10;
	vertices[4].y = y2 * 10;
	vertices[4].z = -10;

	vertices[5].x = x2 * 10;
	vertices[5].y = y2 * 10;
	vertices[5].z = -10;

	vertices[0].colour = 0;
	vertices[1].colour = 0;
	vertices[2].colour = 4;
	vertices[3].colour = 4;
	vertices[4].colour = 8;
	vertices[5].colour = 8;

	colour1 = text0f1543ac((x1 + x2) / 2, (y2 + y1) / 2, 0xffffffff) & 0xff;
	colour2 = (text0f1543ac((x1 + x2) / 2, (y2 + y1) / 2, 0xffffff7f) & 0xff) | 0x00006f00;

	colours[0] = colour1 | 0x00006f00;
	colours[1] = colour2;
	colours[2] = colour1 | 0x00003f00;

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 3);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 6);

	gDPTri4(gdl++, 0, 1, 3, 3, 2, 0, 2, 3, 4, 4, 3, 5);

	return gdl;
}

#if VERSION >= VERSION_NTSC_1_0
// NTSC beta is non-matching, but a close match is shown here
#if VERSION >= VERSION_NTSC_1_0
Gfx *menugfxDrawListGroupHeader(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, u8 alpha)
#else
Gfx *menugfxDrawListGroupHeader(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3)
#endif
{
	u32 *colours = gfxAllocateColours(7);
	struct gfxvtx *vertices = gfxAllocateVertices(9);
	u32 alpha1;
	u32 alpha2;

	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);
	gDPSetTextureFilter(gdl++, G_TF_BILERP);

	texSelect(&gdl, NULL, 2, 0, 2, 1, NULL);

	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

	vertices[0].x = x1 * 10;
	vertices[0].y = y1 * 10;
	vertices[0].z = -10;
	vertices[1].x = x2 * 10;
	vertices[1].y = y1 * 10;
	vertices[1].z = -10;
	vertices[2].x = x1 * 10;
	vertices[2].y = (y2 + y1) / 2 * 10;
	vertices[2].z = -10;
	vertices[3].x = x2 * 10;
	vertices[3].y = (y2 + y1) / 2 * 10;
	vertices[3].z = -10;
	vertices[4].x = x1 * 10;
	vertices[4].y = y2 * 10;
	vertices[4].z = -10;
	vertices[5].x = x2 * 10;
	vertices[5].y = y2 * 10;
	vertices[5].z = -10;
	vertices[6].x = x3 * 10;
	vertices[6].y = y1 * 10;
	vertices[6].z = -10;
	vertices[7].x = x3 * 10;
	vertices[7].y = (y2 + y1) / 2 * 10;
	vertices[7].z = -10;
	vertices[8].x = x3 * 10;
	vertices[8].y = y2 * 10;
	vertices[8].z = -10;

	vertices[0].colour = 0;
	vertices[1].colour = 24;
	vertices[2].colour = 4;
	vertices[3].colour = 4;
	vertices[4].colour = 8;
	vertices[5].colour = 8;
	vertices[6].colour = 12;
	vertices[7].colour = 16;
	vertices[8].colour = 20;

#if VERSION >= VERSION_NTSC_1_0
	alpha1 = alpha;
	alpha2 = alpha;
#else
	alpha1 = text0f1543ac((x2 + x1) / 2, (y2 + y1) / 2, 0xffffffff) & 0xff;
	alpha2 = text0f1543ac((x2 + x1) / 2, (y2 + y1) / 2, 0xffffff7f) & 0xff;
#endif

	colours[0] = 0x00006f00 | alpha1;
	colours[1] = 0x00006f00 | alpha2;
	colours[2] = 0x00003f00 | alpha2;
	colours[3] = 0xffffff00;
	colours[4] = (0x00006f00 | alpha2) & 0xffffff00;
	colours[5] = (0x00003f00 | alpha1) & 0xffffff00;
	colours[6] = 0x6f6f6f00 | alpha1;

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 7);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 9);

	gDPTri4(gdl++, 0, 1, 3, 3, 2, 0, 2, 3, 4, 4, 3, 5);
	gDPTri4(gdl++, 1, 6, 7, 7, 3, 1, 3, 7, 8, 8, 5, 3);

	gdl = menugfxDrawShimmer(gdl, x1, y1, x2, y1 + 1, (alpha1 & 0xff) >> 2, 1, 0x28, 0);
	gdl = menugfxDrawShimmer(gdl, x1, y2, x2, y2 + 1, (alpha1 & 0xff) >> 2, 0, 0x28, 1);

	return gdl;
}
#else
GLOBAL_ASM(
glabel menugfxDrawListGroupHeader
/*  f0ded8c:	27bdff70 */ 	addiu	$sp,$sp,-144
/*  f0ded90:	afbf003c */ 	sw	$ra,0x3c($sp)
/*  f0ded94:	afa40090 */ 	sw	$a0,0x90($sp)
/*  f0ded98:	afb20038 */ 	sw	$s2,0x38($sp)
/*  f0ded9c:	afb10034 */ 	sw	$s1,0x34($sp)
/*  f0deda0:	afb00030 */ 	sw	$s0,0x30($sp)
/*  f0deda4:	afa50094 */ 	sw	$a1,0x94($sp)
/*  f0deda8:	afa60098 */ 	sw	$a2,0x98($sp)
/*  f0dedac:	afa7009c */ 	sw	$a3,0x9c($sp)
/*  f0dedb0:	0fc588c3 */ 	jal	gfxAllocateColours
/*  f0dedb4:	24040007 */ 	addiu	$a0,$zero,0x7
/*  f0dedb8:	00408825 */ 	or	$s1,$v0,$zero
/*  f0dedbc:	0fc588a9 */ 	jal	gfxAllocateVertices
/*  f0dedc0:	24040009 */ 	addiu	$a0,$zero,0x9
/*  f0dedc4:	8fae0090 */ 	lw	$t6,0x90($sp)
/*  f0dedc8:	3c18e700 */ 	lui	$t8,0xe700
/*  f0dedcc:	00408025 */ 	or	$s0,$v0,$zero
/*  f0dedd0:	25cf0008 */ 	addiu	$t7,$t6,0x8
/*  f0dedd4:	afaf0090 */ 	sw	$t7,0x90($sp)
/*  f0dedd8:	adc00004 */ 	sw	$zero,0x4($t6)
/*  f0deddc:	add80000 */ 	sw	$t8,0x0($t6)
/*  f0dede0:	8fb90090 */ 	lw	$t9,0x90($sp)
/*  f0dede4:	3c0fba00 */ 	lui	$t7,0xba00
/*  f0dede8:	35ef1402 */ 	ori	$t7,$t7,0x1402
/*  f0dedec:	272e0008 */ 	addiu	$t6,$t9,0x8
/*  f0dedf0:	afae0090 */ 	sw	$t6,0x90($sp)
/*  f0dedf4:	af200004 */ 	sw	$zero,0x4($t9)
/*  f0dedf8:	af2f0000 */ 	sw	$t7,0x0($t9)
/*  f0dedfc:	8fb80090 */ 	lw	$t8,0x90($sp)
/*  f0dee00:	3c0eb900 */ 	lui	$t6,0xb900
/*  f0dee04:	35ce0002 */ 	ori	$t6,$t6,0x2
/*  f0dee08:	27190008 */ 	addiu	$t9,$t8,0x8
/*  f0dee0c:	afb90090 */ 	sw	$t9,0x90($sp)
/*  f0dee10:	af000004 */ 	sw	$zero,0x4($t8)
/*  f0dee14:	af0e0000 */ 	sw	$t6,0x0($t8)
/*  f0dee18:	8faf0090 */ 	lw	$t7,0x90($sp)
/*  f0dee1c:	3c19fc12 */ 	lui	$t9,0xfc12
/*  f0dee20:	37397e24 */ 	ori	$t9,$t9,0x7e24
/*  f0dee24:	25f80008 */ 	addiu	$t8,$t7,0x8
/*  f0dee28:	afb80090 */ 	sw	$t8,0x90($sp)
/*  f0dee2c:	240ef9fc */ 	addiu	$t6,$zero,-1540
/*  f0dee30:	adee0004 */ 	sw	$t6,0x4($t7)
/*  f0dee34:	adf90000 */ 	sw	$t9,0x0($t7)
/*  f0dee38:	8faf0090 */ 	lw	$t7,0x90($sp)
/*  f0dee3c:	3c19b600 */ 	lui	$t9,0xb600
/*  f0dee40:	240e3000 */ 	addiu	$t6,$zero,0x3000
/*  f0dee44:	25f80008 */ 	addiu	$t8,$t7,0x8
/*  f0dee48:	afb80090 */ 	sw	$t8,0x90($sp)
/*  f0dee4c:	adee0004 */ 	sw	$t6,0x4($t7)
/*  f0dee50:	adf90000 */ 	sw	$t9,0x0($t7)
/*  f0dee54:	8faf0090 */ 	lw	$t7,0x90($sp)
/*  f0dee58:	3c19ba00 */ 	lui	$t9,0xba00
/*  f0dee5c:	37390c02 */ 	ori	$t9,$t9,0xc02
/*  f0dee60:	25f80008 */ 	addiu	$t8,$t7,0x8
/*  f0dee64:	afb80090 */ 	sw	$t8,0x90($sp)
/*  f0dee68:	240e2000 */ 	addiu	$t6,$zero,0x2000
/*  f0dee6c:	adee0004 */ 	sw	$t6,0x4($t7)
/*  f0dee70:	adf90000 */ 	sw	$t9,0x0($t7)
/*  f0dee74:	240f0002 */ 	addiu	$t7,$zero,0x2
/*  f0dee78:	24180001 */ 	addiu	$t8,$zero,0x1
/*  f0dee7c:	afb80014 */ 	sw	$t8,0x14($sp)
/*  f0dee80:	afaf0010 */ 	sw	$t7,0x10($sp)
/*  f0dee84:	afa00018 */ 	sw	$zero,0x18($sp)
/*  f0dee88:	27a40090 */ 	addiu	$a0,$sp,0x90
/*  f0dee8c:	00002825 */ 	or	$a1,$zero,$zero
/*  f0dee90:	24060002 */ 	addiu	$a2,$zero,0x2
/*  f0dee94:	0fc2c5c8 */ 	jal	texSelect
/*  f0dee98:	00003825 */ 	or	$a3,$zero,$zero
/*  f0dee9c:	8fab0094 */ 	lw	$t3,0x94($sp)
/*  f0deea0:	2404000a */ 	addiu	$a0,$zero,0xa
/*  f0deea4:	8fac0098 */ 	lw	$t4,0x98($sp)
/*  f0deea8:	01640019 */ 	multu	$t3,$a0
/*  f0deeac:	8fad009c */ 	lw	$t5,0x9c($sp)
/*  f0deeb0:	8fb90090 */ 	lw	$t9,0x90($sp)
/*  f0deeb4:	8faa00a0 */ 	lw	$t2,0xa0($sp)
/*  f0deeb8:	3c0fb900 */ 	lui	$t7,0xb900
/*  f0deebc:	272e0008 */ 	addiu	$t6,$t9,0x8
/*  f0deec0:	afae0090 */ 	sw	$t6,0x90($sp)
/*  f0deec4:	3c180050 */ 	lui	$t8,0x50
/*  f0deec8:	37184240 */ 	ori	$t8,$t8,0x4240
/*  f0deecc:	35ef031d */ 	ori	$t7,$t7,0x31d
/*  f0deed0:	00002812 */ 	mflo	$a1
/*  f0deed4:	af2f0000 */ 	sw	$t7,0x0($t9)
/*  f0deed8:	af380004 */ 	sw	$t8,0x4($t9)
/*  f0deedc:	01840019 */ 	multu	$t4,$a0
/*  f0deee0:	03201825 */ 	or	$v1,$t9,$zero
/*  f0deee4:	014c9021 */ 	addu	$s2,$t2,$t4
/*  f0deee8:	2402fff6 */ 	addiu	$v0,$zero,-10
/*  f0deeec:	a6050000 */ 	sh	$a1,0x0($s0)
/*  f0deef0:	a6020004 */ 	sh	$v0,0x4($s0)
/*  f0deef4:	a6020010 */ 	sh	$v0,0x10($s0)
/*  f0deef8:	a6050018 */ 	sh	$a1,0x18($s0)
/*  f0deefc:	a602001c */ 	sh	$v0,0x1c($s0)
/*  f0def00:	a6020028 */ 	sh	$v0,0x28($s0)
/*  f0def04:	00003012 */ 	mflo	$a2
/*  f0def08:	a6060002 */ 	sh	$a2,0x2($s0)
/*  f0def0c:	a606000e */ 	sh	$a2,0xe($s0)
/*  f0def10:	01a40019 */ 	multu	$t5,$a0
/*  f0def14:	a6050030 */ 	sh	$a1,0x30($s0)
/*  f0def18:	a6020034 */ 	sh	$v0,0x34($s0)
/*  f0def1c:	a6020040 */ 	sh	$v0,0x40($s0)
/*  f0def20:	240f0018 */ 	addiu	$t7,$zero,0x18
/*  f0def24:	24050008 */ 	addiu	$a1,$zero,0x8
/*  f0def28:	2418000c */ 	addiu	$t8,$zero,0xc
/*  f0def2c:	00003812 */ 	mflo	$a3
/*  f0def30:	a607000c */ 	sh	$a3,0xc($s0)
/*  f0def34:	06410003 */ 	bgez	$s2,.NB0f0def44
/*  f0def38:	0012c843 */ 	sra	$t9,$s2,0x1
/*  f0def3c:	26410001 */ 	addiu	$at,$s2,0x1
/*  f0def40:	0001c843 */ 	sra	$t9,$at,0x1
.NB0f0def44:
/*  f0def44:	03240019 */ 	multu	$t9,$a0
/*  f0def48:	a6070024 */ 	sh	$a3,0x24($s0)
/*  f0def4c:	a607003c */ 	sh	$a3,0x3c($s0)
/*  f0def50:	03209025 */ 	or	$s2,$t9,$zero
/*  f0def54:	24190010 */ 	addiu	$t9,$zero,0x10
/*  f0def58:	00004012 */ 	mflo	$t0
/*  f0def5c:	a608001a */ 	sh	$t0,0x1a($s0)
/*  f0def60:	a6080026 */ 	sh	$t0,0x26($s0)
/*  f0def64:	01440019 */ 	multu	$t2,$a0
/*  f0def68:	00001812 */ 	mflo	$v1
/*  f0def6c:	a6030032 */ 	sh	$v1,0x32($s0)
/*  f0def70:	a603003e */ 	sh	$v1,0x3e($s0)
/*  f0def74:	8fae00a4 */ 	lw	$t6,0xa4($sp)
/*  f0def78:	a20f0013 */ 	sb	$t7,0x13($s0)
/*  f0def7c:	a606004a */ 	sh	$a2,0x4a($s0)
/*  f0def80:	01c40019 */ 	multu	$t6,$a0
/*  f0def84:	24040004 */ 	addiu	$a0,$zero,0x4
/*  f0def88:	a204001f */ 	sb	$a0,0x1f($s0)
/*  f0def8c:	a204002b */ 	sb	$a0,0x2b($s0)
/*  f0def90:	016d2021 */ 	addu	$a0,$t3,$t5
/*  f0def94:	a2050037 */ 	sb	$a1,0x37($s0)
/*  f0def98:	a2050043 */ 	sb	$a1,0x43($s0)
/*  f0def9c:	240e0014 */ 	addiu	$t6,$zero,0x14
/*  f0defa0:	a602004c */ 	sh	$v0,0x4c($s0)
/*  f0defa4:	a6080056 */ 	sh	$t0,0x56($s0)
/*  f0defa8:	00004812 */ 	mflo	$t1
/*  f0defac:	a6090048 */ 	sh	$t1,0x48($s0)
/*  f0defb0:	a6090054 */ 	sh	$t1,0x54($s0)
/*  f0defb4:	a6020058 */ 	sh	$v0,0x58($s0)
/*  f0defb8:	a6090060 */ 	sh	$t1,0x60($s0)
/*  f0defbc:	a6030062 */ 	sh	$v1,0x62($s0)
/*  f0defc0:	a6020064 */ 	sh	$v0,0x64($s0)
/*  f0defc4:	a2000007 */ 	sb	$zero,0x7($s0)
/*  f0defc8:	a218004f */ 	sb	$t8,0x4f($s0)
/*  f0defcc:	a219005b */ 	sb	$t9,0x5b($s0)
/*  f0defd0:	a20e0067 */ 	sb	$t6,0x67($s0)
/*  f0defd4:	04810003 */ 	bgez	$a0,.NB0f0defe4
/*  f0defd8:	00047843 */ 	sra	$t7,$a0,0x1
/*  f0defdc:	24810001 */ 	addiu	$at,$a0,0x1
/*  f0defe0:	00017843 */ 	sra	$t7,$at,0x1
.NB0f0defe4:
/*  f0defe4:	01e02025 */ 	or	$a0,$t7,$zero
/*  f0defe8:	afaf0050 */ 	sw	$t7,0x50($sp)
/*  f0defec:	afb20044 */ 	sw	$s2,0x44($sp)
/*  f0deff0:	02402825 */ 	or	$a1,$s2,$zero
/*  f0deff4:	0fc53a82 */ 	jal	text0f1543ac
/*  f0deff8:	2406ffff */ 	addiu	$a2,$zero,-1
/*  f0deffc:	8fa40050 */ 	lw	$a0,0x50($sp)
/*  f0df000:	305200ff */ 	andi	$s2,$v0,0xff
/*  f0df004:	8fa50044 */ 	lw	$a1,0x44($sp)
/*  f0df008:	0fc53a82 */ 	jal	text0f1543ac
/*  f0df00c:	2406ff7f */ 	addiu	$a2,$zero,-129
/*  f0df010:	2406ff00 */ 	addiu	$a2,$zero,-256
/*  f0df014:	36586f00 */ 	ori	$t8,$s2,0x6f00
/*  f0df018:	304300ff */ 	andi	$v1,$v0,0xff
/*  f0df01c:	3c016f6f */ 	lui	$at,0x6f6f
/*  f0df020:	ae380000 */ 	sw	$t8,0x0($s1)
/*  f0df024:	34796f00 */ 	ori	$t9,$v1,0x6f00
/*  f0df028:	36453f00 */ 	ori	$a1,$s2,0x3f00
/*  f0df02c:	34216f00 */ 	ori	$at,$at,0x6f00
/*  f0df030:	03267024 */ 	and	$t6,$t9,$a2
/*  f0df034:	00a67824 */ 	and	$t7,$a1,$a2
/*  f0df038:	0241c025 */ 	or	$t8,$s2,$at
/*  f0df03c:	ae390004 */ 	sw	$t9,0x4($s1)
/*  f0df040:	ae250008 */ 	sw	$a1,0x8($s1)
/*  f0df044:	ae26000c */ 	sw	$a2,0xc($s1)
/*  f0df048:	ae2e0010 */ 	sw	$t6,0x10($s1)
/*  f0df04c:	ae2f0014 */ 	sw	$t7,0x14($s1)
/*  f0df050:	ae380018 */ 	sw	$t8,0x18($s1)
/*  f0df054:	8fb90090 */ 	lw	$t9,0x90($sp)
/*  f0df058:	3c0f0718 */ 	lui	$t7,0x718
/*  f0df05c:	35ef001c */ 	ori	$t7,$t7,0x1c
/*  f0df060:	272e0008 */ 	addiu	$t6,$t9,0x8
/*  f0df064:	afae0090 */ 	sw	$t6,0x90($sp)
/*  f0df068:	af2f0000 */ 	sw	$t7,0x0($t9)
/*  f0df06c:	02202025 */ 	or	$a0,$s1,$zero
/*  f0df070:	0c013100 */ 	jal	osVirtualToPhysical
/*  f0df074:	afb90060 */ 	sw	$t9,0x60($sp)
/*  f0df078:	8fa70060 */ 	lw	$a3,0x60($sp)
/*  f0df07c:	3c0e0480 */ 	lui	$t6,0x480
/*  f0df080:	35ce006c */ 	ori	$t6,$t6,0x6c
/*  f0df084:	ace20004 */ 	sw	$v0,0x4($a3)
/*  f0df088:	8fb10090 */ 	lw	$s1,0x90($sp)
/*  f0df08c:	02002025 */ 	or	$a0,$s0,$zero
/*  f0df090:	26390008 */ 	addiu	$t9,$s1,0x8
/*  f0df094:	afb90090 */ 	sw	$t9,0x90($sp)
/*  f0df098:	0c013100 */ 	jal	osVirtualToPhysical
/*  f0df09c:	ae2e0000 */ 	sw	$t6,0x0($s1)
/*  f0df0a0:	ae220004 */ 	sw	$v0,0x4($s1)
/*  f0df0a4:	8faf0090 */ 	lw	$t7,0x90($sp)
/*  f0df0a8:	3c19b100 */ 	lui	$t9,0xb100
/*  f0df0ac:	3c0e3432 */ 	lui	$t6,0x3432
/*  f0df0b0:	25f80008 */ 	addiu	$t8,$t7,0x8
/*  f0df0b4:	afb80090 */ 	sw	$t8,0x90($sp)
/*  f0df0b8:	35ce2310 */ 	ori	$t6,$t6,0x2310
/*  f0df0bc:	37395403 */ 	ori	$t9,$t9,0x5403
/*  f0df0c0:	adf90000 */ 	sw	$t9,0x0($t7)
/*  f0df0c4:	adee0004 */ 	sw	$t6,0x4($t7)
/*  f0df0c8:	8faf0090 */ 	lw	$t7,0x90($sp)
/*  f0df0cc:	3c0e5873 */ 	lui	$t6,0x5873
/*  f0df0d0:	3c19b100 */ 	lui	$t9,0xb100
/*  f0df0d4:	25f80008 */ 	addiu	$t8,$t7,0x8
/*  f0df0d8:	afb80090 */ 	sw	$t8,0x90($sp)
/*  f0df0dc:	37393817 */ 	ori	$t9,$t9,0x3817
/*  f0df0e0:	35ce3761 */ 	ori	$t6,$t6,0x3761
/*  f0df0e4:	adee0004 */ 	sw	$t6,0x4($t7)
/*  f0df0e8:	adf90000 */ 	sw	$t9,0x0($t7)
/*  f0df0ec:	8fa60098 */ 	lw	$a2,0x98($sp)
/*  f0df0f0:	325000ff */ 	andi	$s0,$s2,0xff
/*  f0df0f4:	0010c882 */ 	srl	$t9,$s0,0x2
/*  f0df0f8:	240f0028 */ 	addiu	$t7,$zero,0x28
/*  f0df0fc:	240e0001 */ 	addiu	$t6,$zero,0x1
/*  f0df100:	24d80001 */ 	addiu	$t8,$a2,0x1
/*  f0df104:	afb80010 */ 	sw	$t8,0x10($sp)
/*  f0df108:	afae0018 */ 	sw	$t6,0x18($sp)
/*  f0df10c:	afaf001c */ 	sw	$t7,0x1c($sp)
/*  f0df110:	03208025 */ 	or	$s0,$t9,$zero
/*  f0df114:	afb90014 */ 	sw	$t9,0x14($sp)
/*  f0df118:	afa00020 */ 	sw	$zero,0x20($sp)
/*  f0df11c:	8fa7009c */ 	lw	$a3,0x9c($sp)
/*  f0df120:	8fa50094 */ 	lw	$a1,0x94($sp)
/*  f0df124:	0fc37fce */ 	jal	menugfxDrawShimmer
/*  f0df128:	8fa40090 */ 	lw	$a0,0x90($sp)
/*  f0df12c:	8fa600a0 */ 	lw	$a2,0xa0($sp)
/*  f0df130:	24190028 */ 	addiu	$t9,$zero,0x28
/*  f0df134:	240e0001 */ 	addiu	$t6,$zero,0x1
/*  f0df138:	24d80001 */ 	addiu	$t8,$a2,0x1
/*  f0df13c:	afa20090 */ 	sw	$v0,0x90($sp)
/*  f0df140:	afb80010 */ 	sw	$t8,0x10($sp)
/*  f0df144:	afae0020 */ 	sw	$t6,0x20($sp)
/*  f0df148:	afb9001c */ 	sw	$t9,0x1c($sp)
/*  f0df14c:	00402025 */ 	or	$a0,$v0,$zero
/*  f0df150:	8fa50094 */ 	lw	$a1,0x94($sp)
/*  f0df154:	8fa7009c */ 	lw	$a3,0x9c($sp)
/*  f0df158:	afb00014 */ 	sw	$s0,0x14($sp)
/*  f0df15c:	0fc37fce */ 	jal	menugfxDrawShimmer
/*  f0df160:	afa00018 */ 	sw	$zero,0x18($sp)
/*  f0df164:	8fbf003c */ 	lw	$ra,0x3c($sp)
/*  f0df168:	8fb00030 */ 	lw	$s0,0x30($sp)
/*  f0df16c:	8fb10034 */ 	lw	$s1,0x34($sp)
/*  f0df170:	8fb20038 */ 	lw	$s2,0x38($sp)
/*  f0df174:	03e00008 */ 	jr	$ra
/*  f0df178:	27bd0090 */ 	addiu	$sp,$sp,0x90
);
#endif

Gfx *menugfxRenderGradient(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, u32 colourstart, u32 colourmid, u32 colourend)
{
	u32 *colours = gfxAllocateColours(3);
	struct gfxvtx *vertices = gfxAllocateVertices(6);
	s32 ymid;

	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);
	gDPSetTextureFilter(gdl++, G_TF_BILERP);

	texSelect(&gdl, NULL, 2, 0, 2, 1, NULL);

	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

	ymid = (y1 + y2) / 2;

	if (x2 - x1 < ymid * 2) {
		ymid <<= 0;
	}

	// 0 = top left
	// 1 = top right
	// 2 = bottom right
	// 3 = bottom left
	// 4 = mid left
	// 5 = mid right

	vertices[0].z = -10;
	vertices[1].z = -10;
	vertices[2].z = -10;
	vertices[3].z = -10;
	vertices[4].z = -10;
	vertices[5].z = -10;

	vertices[0].x = x1 * 10;
	vertices[0].y = y1 * 10;
	vertices[0].colour = 0;

	vertices[1].x = x2 * 10;
	vertices[1].y = y1 * 10;
	vertices[1].colour = 0;

	vertices[2].x = x2 * 10;
	vertices[2].y = y2 * 10;
	vertices[2].colour = 4;

	vertices[3].x = x1 * 10;
	vertices[3].y = y2 * 10;
	vertices[3].colour = 4;

	vertices[4].x = x1 * 10;
	vertices[4].y = ymid * 10;
	vertices[4].colour = 8;

	vertices[5].x = x2 * 10;
	vertices[5].y = ymid * 10;
	vertices[5].colour = 8;

	colours[0] = colourstart;
	colours[2] = colourmid;
	colours[1] = colourend;

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 3);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 6);
	gDPTri4(gdl++, 0, 1, 5, 5, 4, 0, 2, 3, 4, 4, 5, 2);

	return gdl;
}

Gfx *menugfxRenderSlider(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, s32 markerx, u32 colour)
{
	u32 *colours = gfxAllocateColours(3);
	struct gfxvtx *vertices = gfxAllocateVertices(6);

	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);
	gDPSetTextureFilter(gdl++, G_TF_BILERP);

	texSelect(&gdl, NULL, 2, 0, 2, 1, NULL);

	gDPSetRenderMode(gdl++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);
	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

	// Marker triangle
	vertices[0].x = markerx * 10 - 40;
	vertices[0].y = y2 * 10 - 80;
	vertices[0].z = -10;
	vertices[1].x = markerx * 10 + 40;
	vertices[1].y = y2 * 10 - 80;
	vertices[1].z = -10;
	vertices[2].x = markerx * 10;
	vertices[2].y = y2 * 10 + 10;
	vertices[2].z = -10;

	vertices[0].colour = 0;
	vertices[1].colour = 0;
	vertices[2].colour = 4;

	// Background triangle
	vertices[3].x = x1 * 10;
	vertices[3].y = y2 * 10;
	vertices[3].z = -10;
	vertices[4].x = (s16)(x2 * 10) - 40;
	vertices[4].y = y1 * 10;
	vertices[4].z = -10;
	vertices[5].x = x2 * 10;
	vertices[5].y = y2 * 10;
	vertices[5].z = -10;

	vertices[3].colour = 8;
	vertices[4].colour = 8;
	vertices[5].colour = 8;

	colours[0] = (colour & 0xffffff00) | 0x4f;
	colours[1] = 0xffffffff;
	colours[2] = 0x0000ff4f;

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 3);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 6);

	gDPTri1(gdl++, 3, 4, 5);

	gDPPipeSync(gdl++);
	gDPSetRenderMode(gdl++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

	gDPTri1(gdl++, 0, 1, 2);

	gDPPipeSync(gdl++);
	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

	// Line to the left of the marker: blue -> white gradient
	gdl = menugfxDrawLine(gdl, x1, y2, markerx, y2 + 1, 0x0000ffff, 0xffffffff);

	// Line to the right of the marker: solid blue
	gdl = menugfxDrawLine(gdl, markerx, y2, x2, y2 + 1, 0x0000ffff, 0x0000ffff);

	return gdl;
}

Gfx *menugfx0f0e2348(Gfx *gdl)
{
	gSPSetGeometryMode(gdl++, G_CULL_BACK);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPPipelineMode(gdl++, G_PM_1PRIMITIVE);
	gDPSetTextureLOD(gdl++, G_TL_TILE);
	gDPSetTextureLUT(gdl++, G_TT_NONE);
	gDPSetTextureDetail(gdl++, G_TD_CLAMP);
	gDPSetTexturePersp(gdl++, G_TP_PERSP);
	gDPSetTextureFilter(gdl++, G_TF_BILERP);
	gDPSetTextureConvert(gdl++, G_TC_FILT);
	gDPSetCombineMode(gdl++, G_CC_SHADE, G_CC_SHADE);
	gDPSetCombineKey(gdl++, G_CK_NONE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetRenderMode(gdl++, G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2);
	gDPSetColorDither(gdl++, G_CD_MAGICSQ);
	gSPSetGeometryMode(gdl++, G_ZBUFFER);

	return gdl;
}

Gfx *menugfx0f0e2498(Gfx *gdl)
{
	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);

	texSelect(&gdl, 0, 2, 0, 2, 1, NULL);

	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

	return gdl;
}

Gfx *menugfxDrawTri2(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, u32 colour1, u32 colour2, bool arg7)
{
	struct gfxvtx *vertices;
	u32 *colours;

	colours = gfxAllocateColours(2);
	vertices = gfxAllocateVertices(4);

	vertices[0].x = x1 * 10;
	vertices[0].y = y1 * 10;
	vertices[0].z = -10;

	vertices[1].x = x2 * 10;
	vertices[1].y = y1 * 10;
	vertices[1].z = -10;

	vertices[2].x = x2 * 10;
	vertices[2].y = y2 * 10;
	vertices[2].z = -10;

	vertices[3].x = x1 * 10;
	vertices[3].y = y2 * 10;
	vertices[3].z = -10;

	if (!arg7) {
		vertices[0].colour = 0;
		vertices[1].colour = 4;
		vertices[2].colour = 4;
		vertices[3].colour = 0;
	} else {
		vertices[0].colour = 0;
		vertices[1].colour = 0;
		vertices[2].colour = 4;
		vertices[3].colour = 4;
	}

	colours[0] = colour1;
	colours[1] = colour2;

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 2);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 4);
	gDPTri2(gdl++, 0, 1, 2, 2, 3, 0);

	return gdl;
}

Gfx *menugfxDrawLine(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, u32 colour1, u32 colour2)
{
	gdl = menugfx0f0e2498(gdl);
	gdl = menugfxDrawTri2(gdl, x1, y1, x2, y2, colour1, colour2, false);

	return gdl;
}

Gfx *menugfxDrawProjectedLine(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, u32 colour1, u32 colour2)
{
	s32 numfullblocks;
	s32 i;
	u32 partcolourtop;
	u32 partcolourbottom;
	s32 partbottom;
	s32 parttop;
	u32 stack[2];

	if (textHasDiagonalBlend()) {
		if (x2 - x1 < y2 - y1) {
			// Portrait
			numfullblocks = (y2 - y1) / 15;
			parttop = y1;
			partcolourtop = textApplyProjectionColour(x1, y1, colour1);

			for (i = 0; i < numfullblocks; i++) {
				partbottom = y1 + i * 15;

				if (y2 - partbottom < 3) {
					partbottom = y2;
					partcolourbottom = textApplyProjectionColour(x2, partbottom, colour2);
				} else {
					partcolourbottom = colourBlend(colour2, colour1, (partbottom - y1) * 255 / (y2 - y1));
					// @bug: y1 should be x1
					partcolourbottom = textApplyProjectionColour(y1, partbottom, partcolourbottom);
				}

				gdl = menugfxDrawTri2(gdl, x1, parttop, x2, partbottom, partcolourtop, partcolourbottom, false);

				parttop = partbottom;
				partcolourtop = partcolourbottom;
			}

			partcolourbottom = textApplyProjectionColour(x2, y2, colour2);
			gdl = menugfxDrawTri2(gdl, x1, parttop, x2, y2, partcolourtop, partcolourbottom, false);
		} else {
			// Landscape
			s32 numfullblocks;
			s32 i;
			u32 partcolourleft;
			u32 partcolourright;
			s32 partright;
			s32 partleft;
			u32 stack[2];

			numfullblocks = (x2 - x1) / 15;
			partleft = x1;
			partcolourleft = textApplyProjectionColour(x1, y1, colour1);

			for (i = 0; i < numfullblocks; i++) {
				partright = x1 + i * 15;

				if (x2 - partright < 3) {
					partright = x2;
					partcolourright = textApplyProjectionColour(x2, y2, colour2);
				} else {
					partcolourright = colourBlend(colour2, colour1, (partright - x1) * 255 / (x2 - x1));
					partcolourright = textApplyProjectionColour(partright, y1, partcolourright);
				}

				gdl = menugfxDrawTri2(gdl, partleft, y1, partright, y2, partcolourleft, partcolourright, false);

				partleft = partright;
				partcolourleft = partcolourright;
			}

			if (partcolourtop);

			partcolourright = textApplyProjectionColour(x2, y2, colour2);
			gdl = menugfxDrawTri2(gdl, partleft, y1, x2, y2, partcolourleft, partcolourright, false);
		}
	} else {
		gdl = menugfxDrawTri2(gdl, x1, y1, x2, y2, colour1, colour2, false);
	}

	return gdl;
}

/**
 * Consider rendering a shimmer effect along a line, based on timing.
 *
 * Also known as the white comets that travel along the dialog borders and
 * within some elements in the dialog.
 *
 * The given coordinates are the coordinates of the full line. Colour is the
 * natural colour of the line.
 *
 * The shimmer will go right to left or bottom to top unless the reverse
 * argument is set to true.
 */
Gfx *menugfxDrawShimmer(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, u32 colour, bool arg6, s32 arg7, bool reverse)
{
	s32 shimmerleft;
	s32 shimmertop;
	s32 shimmerright;
	s32 shimmerbottom;
	s32 v0;
	s32 alpha;
	s32 minalpha;
	u32 tailcolour;

	alpha = 0;
	minalpha = 0;

	if (reverse) {
		v0 = 6.0f * g_20SecIntervalFrac * 600.0f;
	} else {
		v0 = (1.0f - g_20SecIntervalFrac) * 6.0f * 600.0f;
	}

	if (y2 - y1 < x2 - x1) {
		// Horizontal
		v0 += (u32)(y1 + x1);
		v0 %= 600;
		shimmerleft = x1 + v0 - arg7;
		shimmerright = shimmerleft + arg7;

		if (shimmerleft < x1) {
			alpha = x1 - shimmerleft;
			shimmerleft = x1;
		}

		if (shimmerright > x2) {
			minalpha = shimmerright - x2;
			shimmerright = x2;
		}

		if (alpha < minalpha) {
			alpha = minalpha;
		}

		alpha = alpha * 255 / arg7;

		if (alpha > 255) {
			alpha = 255;
		}

		if (x1 <= shimmerright && x2 >= shimmerleft) {
			if (arg6) {
				gdl = menugfx0f0e2498(gdl);
			}

			tailcolour = ((((colour & 0xff) * (0xff - alpha)) / 255) & 0xff) | 0xffffff00;

			if (reverse) {
				// Left to right
				gdl = menugfxDrawTri2(gdl, shimmerleft, y1, shimmerright, y2, 0xffffff00, tailcolour, 0);
			} else {
				// Right to left
				gdl = menugfxDrawTri2(gdl, shimmerleft, y1, shimmerright, y2, tailcolour, 0xffffff00, 0);
			}
		}
	} else {
		// Vertical
		v0 += (u32)(y1 + x1);
		v0 %= 600;
		shimmertop = y1 + v0 - arg7;
		shimmerbottom = shimmertop + arg7;

		if (shimmertop < y1) {
			alpha = y1 - shimmertop;
			shimmertop = y1;
		}

		if (shimmerbottom > y2) {
			minalpha = shimmerbottom - y2;
			shimmerbottom = y2;
		}

		if (alpha < minalpha) {
			alpha = minalpha;
		}

		alpha = alpha * 255 / arg7;

		if (alpha > 255) {
			alpha = 255;
		}

		if (y1 <= shimmerbottom && y2 >= shimmertop) {
			if (arg6) {
				gdl = menugfx0f0e2498(gdl);
			}

			tailcolour = ((((colour & 0xff) * (0xff - alpha)) / 255) & 0xff) | 0xffffff00;

			if (reverse) {
				// Top to bottom
				gdl = menugfxDrawTri2(gdl, x1, shimmertop, x2, shimmerbottom, 0xffffff00, tailcolour, 1);
			} else {
				// Bottom to top
				gdl = menugfxDrawTri2(gdl, x1, shimmertop, x2, shimmerbottom, tailcolour, 0xffffff00, 1);
			}
		}
	}

	return gdl;
}

Gfx *menugfxDrawDialogBorderLine(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, u32 colour1, u32 colour2)
{
	gdl = menugfxDrawLine(gdl, x1, y1, x2, y2, colour1, colour2);
	gdl = menugfxDrawShimmer(gdl, x1, y1, x2, y2, colour1, 0, 10, false);

	return gdl;
}

Gfx *menugfxDrawFilledRect(Gfx *gdl, s32 x1, s32 y1, s32 x2, s32 y2, u32 colour1, u32 colour2)
{
	gdl = menugfx0f0e2498(gdl);
	gdl = menugfxDrawProjectedLine(gdl, x1, y1, x2, y2, colour1, colour2);
	gdl = menugfxDrawShimmer(gdl, x1, y1, x2, y2, colour1, 0, 10, false);

	return gdl;
}

/**
 * Draw a chevron/arrow using a simple triangle.
 *
 * Used for the left/right arrows on the Combat Simulator head/body selectors.
 *
 * The x and y arguments refer to the apex of the chevron. Size is the distance
 * from the apex to the opposite side.
 */
Gfx *menugfxDrawCarouselChevron(Gfx *gdl, s32 x, s32 y, s32 size, s32 direction, u32 colour1, u32 colour2)
{
	struct gfxvtx *vertices;
	u32 *colours;
	s16 halfwidth;
	s16 halfheight;
	s16 relx;
	s16 rely;

	relx = 0;
	rely = 0;
	halfwidth = 0;
	halfheight = 0;

	size *= 10;

	switch (direction) {
	case 0: // Down
		rely = -size;
		halfwidth = -size / 2;
		break;
	case 1: // Left
		relx = size;
		halfheight = -size / 2;
		break;
	case 2: // Up
		rely = size;
		halfwidth = size / 2;
		break;
	case 3: // Right
		relx = size * -1;
		halfheight = size / 2;
		break;
	}

	colours = gfxAllocateColours(2);
	vertices = gfxAllocateVertices(3);

	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);

	texSelect(&gdl, NULL, 2, 0, 2, 1, NULL);

	gDPSetRenderMode(gdl++, G_RM_XLU_SURF, G_RM_XLU_SURF2);

	vertices[0].x = x * 10;
	vertices[0].y = y * 10;
	vertices[0].z = -10;

	vertices[1].x = x * 10 + relx + halfwidth;
	vertices[1].y = y * 10 + rely + halfheight;
	vertices[1].z = -10;

	vertices[2].x = x * 10 + relx - halfwidth;
	vertices[2].y = y * 10 + rely - halfheight;
	vertices[2].z = -10;

	vertices[0].colour = 0;
	vertices[1].colour = 4;
	vertices[2].colour = 4;

	colours[0] = colour1;
	colours[1] = colour2;

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 2);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 3);
	gDPTri1(gdl++, 0, 1, 2);

	return gdl;
}

/**
 * Draw a chevron/arrow for dialogs.
 *
 * These chevons are drawn with two triangles, and the middle rear vertex's
 * position can be adjusted on each frame to create an animated chevron.
 *
 * The x and y arguments refer to the apex of the chevron. Size is the distance
 * from the apex to the opposite side.
 */
Gfx *menugfxDrawDialogChevron(Gfx *gdl, s32 x, s32 y, s32 size, s32 direction, u32 colour1, u32 colour2, f32 arg7)
{
	u32 stack;
	u32 *colours;
	struct gfxvtx *vertices;
	s16 halfwidth;
	s16 halfheight;
	s16 relx;
	s16 rely;

	relx = 0;
	rely = 0;
	halfwidth = 0;
	halfheight = 0;

	size *= 10;

	switch (direction) {
	case 0: // Down
		rely = -size;
		halfwidth = -((s32)(size * arg7 * 0.5f) + size) / 2;
		break;
	case 1: // Left
		relx = size;
		halfheight = -((s32)(size * arg7 * 0.5f) + size) / 2;
		break;
	case 2: // Up
		rely = size;
		halfwidth = ((s32)(size * arg7 * 0.5f) + size) / 2;
		break;
	case 3: // Right
		relx = -size;
		halfheight = ((s32)(size * arg7 * 0.5f) + size) / 2;
		break;
	}

	colours = gfxAllocateColours(2);
	vertices = gfxAllocateVertices(4);

	gDPPipeSync(gdl++);
	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetCombineMode(gdl++, G_CC_MODULATEI, G_CC_MODULATEI);
	gSPClearGeometryMode(gdl++, G_CULL_BOTH);

	texSelect(&gdl, NULL, 2, 0, 2, 1, NULL);

	gDPSetRenderMode(gdl++, G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2);

	// Apex
	vertices[0].x = x * 10;
	vertices[0].y = y * 10;
	vertices[0].z = -10;

	vertices[1].x = x * 10 + relx + halfwidth;
	vertices[1].y = y * 10 + rely + halfheight;
	vertices[1].z = -10;

	vertices[2].x = x * 10 + relx - halfwidth;
	vertices[2].y = y * 10 + rely - halfheight;
	vertices[2].z = -10;

	relx = (relx / 3.0f + (relx * 1.5f * (1.0f - arg7)) / 3.0f);
	rely = rely / 3.0f + (rely * 1.5f * (1.0f - arg7)) / 3.0f;

	// Middle vertex
	vertices[3].x = x * 10 + relx;
	vertices[3].y = y * 10 + rely;
	vertices[3].z = -10;

	vertices[0].colour = 0;
	vertices[1].colour = 4;
	vertices[2].colour = 4;
	vertices[3].colour = 4;

	colours[0] = colour1;
	colours[1] = colour2;

	gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 2);
	gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 4);
	gDPTri2(gdl++, 0, 1, 3, 3, 2, 0);

	return gdl;
}

Gfx *menugfxDrawCheckbox(Gfx *gdl, s32 x, s32 y, s32 size, bool fill, u32 bordercolour, u32 fillcolour)
{
	if (fill) {
		gdl = textSetPrimColour(gdl, fillcolour);
		gDPFillRectangleScaled(gdl++, x, y, x + size, y + size);
		gdl = text0f153838(gdl);
	}

	gdl = textSetPrimColour(gdl, bordercolour);

	gDPFillRectangleScaled(gdl++, x, y, x + size + 1, y + 1);
	gDPFillRectangleScaled(gdl++, x, y + size, x + size + 1, y + size + 1);
	gDPFillRectangleScaled(gdl++, x, y + 1, x + 1, y + size);
	gDPFillRectangleScaled(gdl++, x + size, y + 1, x + size + 1, y + size);

	gdl = text0f153838(gdl);

	return gdl;
}

Gfx *menugfxRenderBgFailure(Gfx *gdl)
{
	f32 spb4;
	s32 i;
	f32 angle;
	s32 s0;
	s32 s1;
	s32 s2;
	s32 s3;
	s32 s6;
	s32 s7;
	u32 alpha1;
	u32 alpha2;

	spb4 = M_TAU * g_20SecIntervalFrac;

	var8009de98 = var8009de9c = 0;

	gdl = func0f0d4a3c(gdl, 0);

	var8009de90 = -100000;
	var8009de94 = 100000;

	gdl = menugfxDrawPlane(gdl, -10000, 0, 10000, 0, 0x00007f7f, 0x00007f7f, MENUPLANE_04);
	gdl = menugfxDrawPlane(gdl, -10000, 300, 10000, 300, 0x00007f7f, 0x00007f7f, MENUPLANE_04);

	for (i = 0; i < 3; i++) {
		angle = (2.0f * i * M_PI) / 3.0f + spb4;
		s6 = sinf(angle) * 600.0f;
		s3 = cosf(angle) * 600.0f;

		angle += 0.52359879016876f;
		s2 = sinf(angle) * 600.0f;
		s0 = cosf(angle) * 600.0f;

		angle += 0.52359879016876f;
		s1 = sinf(angle) * 600.0f;
		s7 = cosf(angle) * 600.0f;

		s6 += 160;
		s2 += 160;
		s1 += 160;
		s3 += 120;
		s0 += 120;
		s7 += 120;

		gdl = menugfxDrawPlane(gdl, s6, s3, s2, s0, 0xff000040, 0xff00007f, MENUPLANE_02);
		gdl = menugfxDrawPlane(gdl, s2, s0, s1, s7, 0xff00007f, 0xff000040, MENUPLANE_03);

		angle = -2.0f * spb4 + (2.0f * i * M_PI) / 3.0f;
		s6 = sinf(angle) * 600.0f;
		s3 = cosf(angle) * 600.0f;

		angle += 0.17453293502331f;
		s2 = sinf(angle) * 600.0f;
		s0 = cosf(angle) * 600.0f;

		angle += 0.099733099341393f;
		s1 = sinf(angle) * 600.0f;
		s7 = cosf(angle) * 600.0f;

		s6 += 160;
		s2 += 160;
		s1 += 160;
		s3 += 120;
		s0 += 120;
		s7 += 120;

		alpha1 = menuGetCosOscFrac(4) * 127.0f;
		alpha2 = menuGetCosOscFrac(4) * 55.0f;

		gdl = menugfxDrawPlane(gdl, s6, s3, s2, s0, 0xffff0000 | alpha2, 0xffff0000 | alpha1, MENUPLANE_02);
		gdl = menugfxDrawPlane(gdl, s2, s0, s1, s7, 0xffff0000 | alpha1, 0xffff0000 | alpha2, MENUPLANE_03);

		angle = -2.0f * spb4 + (2.0f * i * M_PI) / 3.0f + M_PI;
		s6 = sinf(angle) * 600.0f;
		s3 = cosf(angle) * 600.0f;

		angle += 0.17453293502331f;
		s2 = sinf(angle) * 600.0f;
		s0 = cosf(angle) * 600.0f;

		angle += 0.099733099341393f;
		s1 = sinf(angle) * 600.0f;
		s7 = cosf(angle) * 600.0f;

		s6 += 160;
		s2 += 160;
		s1 += 160;
		s3 += 120;
		s0 += 120;
		s7 += 120;

		alpha1 = (1.0f - menuGetCosOscFrac(4)) * 99.0f;
		alpha2 = (1.0f - menuGetCosOscFrac(4)) * 33.0f;

		gdl = menugfxDrawPlane(gdl, s6, s3, s2, s0, 0xffffff00 | alpha2, 0xffffff00 | alpha1, MENUPLANE_02);
		gdl = menugfxDrawPlane(gdl, s2, s0, s1, s7, 0xffffff00 | alpha1, 0xffffff00 | alpha2, MENUPLANE_03);
	}

	gdl = func0f0d4c80(gdl);

	return gdl;
}

/**
 * Draw the background for the Combat Simulator menus.
 *
 * The background has two "cones" being projected into the camera's eye and
 * rotating in opposite directions.
 */
Gfx *menugfxRenderBgCone(Gfx *gdl)
{
	f32 baseangle;
	f32 angle;
	s32 x1;
	s32 y1;
	s32 x2;
	s32 y2;
	s32 i;
	u32 colourupper;
	u32 colour;

	// Cone 1
	baseangle = M_TAU * g_20SecIntervalFrac * 2.0f;
	colourupper = (u32) (menuGetSinOscFrac(1.0f) * 255.0f) << 16;

	gdl = func0f0d4a3c(gdl, 0);

	if (1);

	var8009de90 = -100000;
	var8009de94 = 100000;

	for (i = 0; i < 8; i++) {
		angle = baseangle + i * 2.0f * M_PI * 0.125f;

		x1 = 600.0f * sinf(angle);
		y1 = 600.0f * cosf(angle);
		x2 = 600.0f * sinf(angle + 0.78539818525314f);
		y2 = 600.0f * cosf(angle + 0.78539818525314f);

		x1 += 160;
		x2 += 160;
		y1 += 120;
		y2 += 120;

		colour = colourupper | 0xff00007f;

		gdl = menugfxDrawPlane(gdl, x1, y1, x2, y2, colour, colour, MENUPLANE_08);
	}

	// Cone 2
	colourupper = (u32) (255.0f - menuGetCosOscFrac(1.0f) * 255.0f) << 16;

	baseangle = M_TAU * g_20SecIntervalFrac;

	if (1);

	for (i = 0; i < 8; i++) {
		if (gdl && gdl);

		angle = -baseangle + 2.0f * i * M_PI * 0.125f;

		x1 = 600.0f * sinf(angle);
		y1 = 600.0f * cosf(angle);
		x2 = 600.0f * sinf(angle + 0.78539818525314f);
		y2 = 600.0f * cosf(angle + 0.78539818525314f);

		x1 += 160;
		x2 += 160;
		y1 += 120;
		y2 += 120;

		colour = colourupper | 0xff00007f;

		gdl = menugfxDrawPlane(gdl, x1, y1, x2, y2, colour, colour, MENUPLANE_09);
	}

	gdl = func0f0d4c80(gdl);

	return gdl;
}

/**
 * Fill the framebuffer with a transparent green overlay.
 *
 * The amount of transparency varies from frame to frame. It's quite volatile.
 *
 * This function is not called.
 */
Gfx *func0f0e458c(Gfx *gdl)
{
	var8009de98 = 0;
	var8009de9c = -20 - viGetHeight() / 2;

	gdl = func0f0d4a3c(gdl, 0);

	var8009de90 = -100000;
	var8009de94 = 100000;

	gdl = menugfxDrawPlane(gdl, -1000, viGetHeight() + 10, 2000, viGetHeight() + 10, 0x00ff00bf, 0x00ff00bf, MENUPLANE_06);
	gdl = menugfxDrawPlane(gdl, -1000, viGetHeight() + 10, 2000, viGetHeight() + 10, 0xffff00af, 0xffff00af, MENUPLANE_05);
	gdl = func0f0d4c80(gdl);

	var8009de98 = var8009de9c = 0;

	return gdl;
}

Gfx *func0f0e46b0(Gfx *gdl, f32 arg1)
{
	s32 y1 = (var8009de9c + 120) + arg1 * (0.0f - (var8009de9c + 120));
	s32 y2 = (var8009de9c + 120) + arg1 * (240.0f - (var8009de9c + 120));

	gdl = func0f0d4a3c(gdl, 0);

	var8009de90 = -100000;
	var8009de94 = 100000;

	gdl = menugfxDrawPlane(gdl, -1000, y1, 2000, y1, 0xffff007f, 0xffff007f, MENUPLANE_05);
	gdl = menugfxDrawPlane(gdl, -1000, y1, 2000, y1, 0x00aa007f, 0x00aa007f, MENUPLANE_06);
	gdl = menugfxDrawPlane(gdl, -1000, y2, 2000, y2, 0xffff007f, 0xffff007f, MENUPLANE_05);
	gdl = menugfxDrawPlane(gdl, -1000, y2, 2000, y2, 0x00aa007f, 0x00aa007f, MENUPLANE_06);

	gdl = func0f0d4c80(gdl);

	return gdl;
}

/**
 * This is an exact copy of menugfxRenderBgFailure.
 */
Gfx *menugfxRenderBgFailureCopy(Gfx *gdl)
{
	f32 spb4;
	s32 i;
	f32 angle;
	s32 s0;
	s32 s1;
	s32 s2;
	s32 s3;
	s32 s6;
	s32 s7;
	u32 alpha1;
	u32 alpha2;

	spb4 = M_TAU * g_20SecIntervalFrac;

	var8009de98 = var8009de9c = 0;

	gdl = func0f0d4a3c(gdl, 0);

	var8009de90 = -100000;
	var8009de94 = 100000;

	gdl = menugfxDrawPlane(gdl, -10000, 0, 10000, 0, 0x00007f7f, 0x00007f7f, MENUPLANE_04);
	gdl = menugfxDrawPlane(gdl, -10000, 300, 10000, 300, 0x00007f7f, 0x00007f7f, MENUPLANE_04);

	for (i = 0; i < 3; i++) {
		angle = (2.0f * i * M_PI) / 3.0f + spb4;
		s6 = sinf(angle) * 600.0f;
		s3 = cosf(angle) * 600.0f;

		angle += 0.52359879016876f;
		s2 = sinf(angle) * 600.0f;
		s0 = cosf(angle) * 600.0f;

		angle += 0.52359879016876f;
		s1 = sinf(angle) * 600.0f;
		s7 = cosf(angle) * 600.0f;

		s6 += 160;
		s2 += 160;
		s1 += 160;
		s3 += 120;
		s0 += 120;
		s7 += 120;

		gdl = menugfxDrawPlane(gdl, s6, s3, s2, s0, 0xff000040, 0xff00007f, MENUPLANE_02);
		gdl = menugfxDrawPlane(gdl, s2, s0, s1, s7, 0xff00007f, 0xff000040, MENUPLANE_03);

		angle = -2.0f * spb4 + (2.0f * i * M_PI) / 3.0f;
		s6 = sinf(angle) * 600.0f;
		s3 = cosf(angle) * 600.0f;

		angle += 0.17453293502331f;
		s2 = sinf(angle) * 600.0f;
		s0 = cosf(angle) * 600.0f;

		angle += 0.099733099341393f;
		s1 = sinf(angle) * 600.0f;
		s7 = cosf(angle) * 600.0f;

		s6 += 160;
		s2 += 160;
		s1 += 160;
		s3 += 120;
		s0 += 120;
		s7 += 120;

		alpha1 = menuGetCosOscFrac(4) * 127.0f;
		alpha2 = menuGetCosOscFrac(4) * 55.0f;

		gdl = menugfxDrawPlane(gdl, s6, s3, s2, s0, 0xffff0000 | alpha2, 0xffff0000 | alpha1, MENUPLANE_02);
		gdl = menugfxDrawPlane(gdl, s2, s0, s1, s7, 0xffff0000 | alpha1, 0xffff0000 | alpha2, MENUPLANE_03);

		angle = -2.0f * spb4 + (2.0f * i * M_PI) / 3.0f + M_PI;
		s6 = sinf(angle) * 600.0f;
		s3 = cosf(angle) * 600.0f;

		angle += 0.17453293502331f;
		s2 = sinf(angle) * 600.0f;
		s0 = cosf(angle) * 600.0f;

		angle += 0.099733099341393f;
		s1 = sinf(angle) * 600.0f;
		s7 = cosf(angle) * 600.0f;

		s6 += 160;
		s2 += 160;
		s1 += 160;
		s3 += 120;
		s0 += 120;
		s7 += 120;

		alpha1 = (1.0f - menuGetCosOscFrac(4)) * 99.0f;
		alpha2 = (1.0f - menuGetCosOscFrac(4)) * 33.0f;

		gdl = menugfxDrawPlane(gdl, s6, s3, s2, s0, 0xffffff00 | alpha2, 0xffffff00 | alpha1, MENUPLANE_02);
		gdl = menugfxDrawPlane(gdl, s2, s0, s1, s7, 0xffffff00 | alpha1, 0xffffff00 | alpha2, MENUPLANE_03);
	}

	gdl = func0f0d4c80(gdl);

	return gdl;
}

struct coord *g_MenuParticles = NULL;

/**
 * Menu particles are stored in gunmem. Gunmem doesn't need to be freed because
 * the memory gets reused for other things, so all that needs to happen here is
 * clearing the pointer that the menugfx system maintains.
 */
void menugfxFreeParticles(void)
{
	g_MenuParticles = NULL;
}

u32 menugfxGetParticleArraySize(void)
{
	return align16(NUM_SUCCESS_PARTICLES * sizeof(struct coord));
}

/**
 * Render the "success" background, which is used on the solo mission completed
 * endscreen.
 *
 * The endscreen features two haze layers, and a bunch of particles (stars?)
 * flying towards the camera.
 *
 * In the Defense stage, everything is gray and the particles move slower.
 */
Gfx *menugfxRenderBgSuccess(Gfx *gdl)
{
	Mtxf sp110;
	Mtxf *modelmtx;
	u32 *colours;
	u32 *ptr;
	s32 i;
	s32 j;
	f32 f0;
	f32 speed = 5.0f;
	bool gray = false;

	if (g_StageIndex == STAGEINDEX_DEFENSE) {
		speed = 2.0f;
		gray = true;
	}

	// Initialise particles if they haven't been already
	if (g_MenuParticles == NULL) {
		g_MenuParticles = (struct coord *) bgunGetGunMem();

		if (g_MenuParticles == NULL) {
			return gdl;
		}

		for (i = 0; i < NUM_SUCCESS_PARTICLES; i++) {
			do {
				g_MenuParticles[i].x = RANDOMFRAC() * 10000.0f - 5000.0f;
				g_MenuParticles[i].y = RANDOMFRAC() * 10000.0f - 5000.0f;
			} while (g_MenuParticles[i].x < 640.0f && g_MenuParticles[i].x > -640.0f
					&& g_MenuParticles[i].y < 480.0f && g_MenuParticles[i].y > -480.0f);

			g_MenuParticles[i].z = -RANDOMFRAC() * 8000.0f;
		}
	}

	// Move the particles closer, and reset the ones which have gone behind the camera
	for (i = 0; i < NUM_SUCCESS_PARTICLES; i++) {
		s32 mult = (i % 5) + 1;

#if VERSION >= VERSION_PAL_BETA
		g_MenuParticles[i].z += mult * g_Vars.diffframe240freal * speed;
#else
		g_MenuParticles[i].z += mult * g_Vars.diffframe240f * speed;
#endif

		if (g_MenuParticles[i].z > 0.0f) {
			do {
				g_MenuParticles[i].x = RANDOMFRAC() * 10000.0f - 5000.0f;
				g_MenuParticles[i].y = RANDOMFRAC() * 10000.0f - 5000.0f;
			} while (g_MenuParticles[i].x < 640.0f && g_MenuParticles[i].x > -640.0f
					&& g_MenuParticles[i].y < 480.0f && g_MenuParticles[i].y > -480.0f);

			g_MenuParticles[i].z = -8000.0f - RANDOMFRAC() * 500.0f;
		}
	}

	f0 = menuGetLinearIntervalFrac(10.0f);

	// Draw the two haze layers
	var8009de98 = 0;
	var8009de9c = 0;

	gdl = func0f0d4a3c(gdl, 0);

	var8009de90 = -100000;
	var8009de94 = 100000;

	if (gray) {
		gdl = menugfxDrawPlane(gdl, -1000, -10, 2000, -10, 0x6060607f, 0x6060607f, MENUPLANE_05);
		gdl = menugfxDrawPlane(gdl, -1000, viGetHeight() + 10, 2000, viGetHeight() + 10, 0x9090907f, 0x9090907f, MENUPLANE_05);
	} else {
		gdl = menugfxDrawPlane(gdl, -1000, -10, 2000, -10, 0x0000947f, 0x0000947f, MENUPLANE_10);
		gdl = menugfxDrawPlane(gdl, -1000, viGetHeight() + 10, 2000, viGetHeight() + 10, 0x6200947f, 0x6200947f, MENUPLANE_06);
	}

	// Prepare stuff for drawing the particles
	gdl = func0f0d4c80(gdl);
	gdl = func0f0d49c8(gdl);

	texSelect(&gdl, &g_TexGeneralConfigs[1], 2, 1, 2, true, NULL);

	gDPSetCycleType(gdl++, G_CYC_1CYCLE);
	gDPSetColorDither(gdl++, G_CD_DISABLE);
	gDPSetRenderMode(gdl++, G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2);
	gDPSetAlphaCompare(gdl++, G_AC_NONE);
	gDPSetTextureLOD(gdl++, G_TL_TILE);
	gDPSetTextureConvert(gdl++, G_TC_FILT);
	gDPSetCombineLERP(gdl++, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0);
	gDPSetTextureFilter(gdl++, G_TF_BILERP);
	gDPSetTexturePersp(gdl++, G_TP_PERSP);

	mtx4LoadIdentity(&sp110);

	modelmtx = gfxAllocateMatrix();

	mtx00016054(&sp110, modelmtx);

	gSPMatrix(gdl++, osVirtualToPhysical(modelmtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

	colours = gfxAllocateColours(20);

	if (gray) {
		for (j = 0, ptr = colours; j < 5;) {
			ptr[0] = 0xffffff00 | ((5 - j) * 127u / 5);
			ptr += 4;
			j++;
		}

		for (j = 0, ptr = colours; j < 5;) {
			ptr[1] = 0xaaaaaa00 | ((5 - j) * 16u / 5);
			ptr += 4;
			j++;
		}

		for (j = 0, ptr = colours; j < 5;) {
			ptr[2] = 0xffffff00 | ((5 - j) * 127u / 5);
			ptr += 4;
			j++;
		}

		for (j = 0, ptr = colours; j < 5;) {
			ptr[3] = 0xaaaaaa00 | ((5 - j) * 16u / 5);
			ptr += 4;
			j++;
		}
	} else {
		for (j = 0, ptr = colours; j < 5;) {
			ptr[0] = 0xffffff00 | ((5 - j) * 127u / 5);
			ptr += 4;
			j++;
		}

		for (j = 0, ptr = colours; j < 5;) {
			ptr[1] = 0xaaaaff00 | ((5 - j) * 16u / 5);
			ptr += 4;
			j++;
		}

		for (j = 0, ptr = colours; j < 5;) {
			ptr[2] = 0xffffff00 | ((5 - j) * 127u / 5);
			ptr += 4;
			j++;
		}

		for (j = 0, ptr = colours; j < 5;) {
			if (colours);
			ptr[3] = 0xffaaff00 | ((5 - j) * 16u / 5);
			ptr += 4;
			j++;
		}
	}

	{
		struct coord pos;
		u32 stack[5];

		gDPSetColorArray(gdl++, osVirtualToPhysical(colours), 20);

		// Draw the particles
		for (i = NUM_SUCCESS_PARTICLES - 1; i >= 0; i--) {
			s32 s3 = 0;
			f32 sine = sinf(f0 * M_BADTAU + M_BADTAU * (i / 15.0f));
			f32 cosine = cosf(f0 * M_BADTAU + M_BADTAU * (i / 15.0f));

			pos = g_MenuParticles[i];

			if (pos.z < -6600.0f) {
				s3 = -(pos.f[2] + 6600.0f) / 1400.0f * 5.0f;
			}

			if (pos.z);

			if (pos.z < 0.0f && s3 < 6) {
				f32 invsine2 = -sine;
				f32 invsine = -sine;
				f32 invcosine = -cosine;
				struct gfxvtx *vertices = gfxAllocateVertices(5);

				vertices[0].x = pos.f[0];
				vertices[0].y = pos.f[1];
				vertices[0].z = pos.f[2];

				vertices[1].x = pos.f[0] + 25.0f * (sine + cosine);
				vertices[1].y = pos.f[1] + 25.0f * (cosine + invsine);
				vertices[1].z = pos.f[2];

				vertices[2].x = pos.f[0] + 25.0f * (sine - cosine);
				vertices[2].y = pos.f[1] + 25.0f * (cosine - invsine);
				vertices[2].z = pos.f[2];

				vertices[3].x = pos.f[0] + 25.0f * (invsine - cosine);
				vertices[3].y = pos.f[1] + 25.0f * (invcosine - invsine);
				vertices[3].z = pos.f[2];

				vertices[4].x = pos.f[0] + 25.0f * (invsine2 + cosine);
				vertices[4].y = pos.f[1] + 25.0f * (invcosine + invsine);
				vertices[4].z = pos.f[2];

				vertices[0].colour = (s3 * 4 + (i % 2) * 2) * 4;
				vertices[1].colour = (s3 * 4 + (i % 2) * 2 + 1) * 4;
				vertices[2].colour = (s3 * 4 + (i % 2) * 2 + 1) * 4;
				vertices[3].colour = (s3 * 4 + (i % 2) * 2 + 1) * 4;
				vertices[4].colour = (s3 * 4 + (i % 2) * 2 + 1) * 4;

				gDPSetVerticeArray(gdl++, osVirtualToPhysical(vertices), 5);

				gDPTri4(gdl++, 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1);
			}
		}
	}

	gdl = func0f0d479c(gdl);

	return gdl;
}
