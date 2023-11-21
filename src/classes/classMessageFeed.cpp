
#include <classMessageFeed.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;

classMessageFeed::classMessageFeed(classTile *tile) : classPopUpContainer(1)
{
  _callingTile = tile;
  lv_obj_del(_panel);

  lv_obj_t *_flexGrid = lv_obj_create(_ovlPanel);
  lv_obj_remove_style_all(_flexGrid);
  lv_obj_set_size(_flexGrid, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
  lv_obj_set_style_radius(_flexGrid, 5, 0);
  lv_obj_align(_flexGrid, LV_ALIGN_TOP_MID, 0, 5);
  lv_obj_set_flex_flow(_flexGrid, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_style_pad_row(_flexGrid, 10, 0);
  lv_obj_add_flag(_flexGrid, LV_OBJ_FLAG_SCROLLABLE);

  std::list<post>::iterator _feedIterator = _callingTile->getFeedIterator();

  for (int i = _callingTile->getFeedSize(); i > 0; i--)
  {
    lv_obj_t *_postContainer = lv_obj_create(_flexGrid);
    lv_obj_remove_style_all(_postContainer);
    lv_obj_set_style_pad_all(_postContainer, 10, 0);
    lv_obj_set_size(_postContainer, SCREEN_WIDTH - 10, LV_SIZE_CONTENT);
    lv_obj_set_style_radius(_postContainer, 5, 0);
    lv_obj_align(_postContainer, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_clear_flag(_postContainer, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(_postContainer, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(_postContainer, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

    post *_post = _feedIterator.operator->();
    _feedIterator++;

    lv_obj_t *_labelHead = lv_label_create(_postContainer);
    lv_label_set_text(_labelHead, "");
    if (!_post->head.empty())
    {
      lv_obj_align(_labelHead, LV_ALIGN_TOP_LEFT, 0, 0);
      lv_obj_set_size(_labelHead, SCREEN_WIDTH - 30, LV_SIZE_CONTENT);
      lv_obj_set_style_pad_all(_labelHead, 5, 0);
      lv_obj_set_style_radius(_labelHead, 5, 0);
      lv_obj_set_style_bg_color(_labelHead, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(_labelHead, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_text_font(_labelHead, &lv_font_montserrat_20, 0);
      lv_label_set_recolor(_labelHead, true);

      lv_label_set_text(_labelHead, _post->head.c_str());
    }

    if (!_post->body.empty())
    {
      lv_obj_t *_labelBody = lv_label_create(_postContainer);
      lv_obj_set_style_pad_all(_labelBody, 5, 0);
      lv_obj_set_size(_labelBody, SCREEN_WIDTH - 30, LV_SIZE_CONTENT);
      lv_obj_set_style_text_color(_labelBody, lv_color_hex(0x808080), 0);
      lv_label_set_recolor(_labelBody, true);

      lv_label_set_text(_labelBody, _post->body.c_str());

      if (_post->head.empty())
        lv_obj_align(_labelBody, LV_ALIGN_TOP_LEFT, 0, 0);
      else
        lv_obj_align_to(_labelBody, _labelHead, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 8);
    }
  }
}

