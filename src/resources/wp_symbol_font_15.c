/*******************************************************************************
 * Size: 15 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef WP_SYMBOL_FONT_15
#define WP_SYMBOL_FONT_15 1
#endif

#if WP_SYMBOL_FONT_15

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F054 "" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x9d, 0x10, 0x0,
    0x0, 0x1f, 0xfd, 0x10, 0x0, 0x0, 0x5f, 0xfd,
    0x10, 0x0, 0x0, 0x5f, 0xfd, 0x10, 0x0, 0x0,
    0x5f, 0xfd, 0x10, 0x0, 0x0, 0x5f, 0xfd, 0x10,
    0x0, 0x0, 0xbf, 0xf7, 0x0, 0x0, 0x9f, 0xfb,
    0x0, 0x0, 0x9f, 0xfb, 0x0, 0x0, 0x9f, 0xfb,
    0x0, 0x0, 0x9f, 0xfb, 0x0, 0x0, 0x1f, 0xfb,
    0x0, 0x0, 0x0, 0x59, 0x0, 0x0, 0x0,

    /* U+F078 "" */
    0x6, 0x0, 0x0, 0x0, 0x0, 0x6, 0x1a, 0xfc,
    0x0, 0x0, 0x0, 0xa, 0xfc, 0x7f, 0xfc, 0x0,
    0x0, 0xb, 0xff, 0x90, 0x7f, 0xfc, 0x0, 0xb,
    0xff, 0x90, 0x0, 0x7f, 0xfc, 0x1b, 0xff, 0x90,
    0x0, 0x0, 0x7f, 0xff, 0xff, 0x90, 0x0, 0x0,
    0x0, 0x7f, 0xff, 0x90, 0x0, 0x0, 0x0, 0x0,
    0x7f, 0x90, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20,
    0x0, 0x0, 0x0,

    /* U+F142 "" */
    0x0, 0x0, 0x0, 0x7f, 0xd3, 0x2f, 0xff, 0xc3,
    0xff, 0xfd, 0xa, 0xff, 0x50, 0x2, 0x10, 0x5,
    0xcb, 0x12, 0xff, 0xfb, 0x3f, 0xff, 0xd0, 0xcf,
    0xf7, 0x0, 0x43, 0x0, 0x4, 0x30, 0xc, 0xff,
    0x63, 0xff, 0xfd, 0x2f, 0xff, 0xb0, 0x5c, 0xb2
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 150, .box_w = 9, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 63, .adv_w = 210, .box_w = 13, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 122, .adv_w = 90, .box_w = 5, .box_h = 16, .ofs_x = 0, .ofs_y = -2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x24, 0xee
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61524, .range_length = 239, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 3, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t wp_symbol_font_15 = {
#else
lv_font_t wp_symbol_font_15 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -5,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if WP_SYMBOL_FONT_15*/

