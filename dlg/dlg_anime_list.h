/*
** Taiga, a lightweight client for MyAnimeList
** Copyright (C) 2010-2012, Eren Okka
** 
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DLG_ANIME_LIST_H
#define DLG_ANIME_LIST_H

#include "../std.h"
#include "../win32/win_control.h"
#include "../win32/win_dialog.h"

// =============================================================================

class AnimeListDialog : public win32::Dialog {
public:
  AnimeListDialog() {}
  virtual ~AnimeListDialog() {}

  INT_PTR DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  BOOL OnInitDialog();
  LRESULT OnListNotify(LPARAM lParam);
  LRESULT OnListCustomDraw(LPARAM lParam);
  LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);
  void OnSize(UINT uMsg, UINT nType, SIZE size);
  LRESULT OnTabNotify(LPARAM lParam);

  int GetListIndex(int anime_id);
  void RefreshList(int index = -1);
  void RefreshTabs(int index = -1, bool redraw = true);

public:
  // List-view control
  class ListView : public win32::ListView {
  public:
    ListView() { dragging = false; };
    int GetSortType(int column);
    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool dragging;
    win32::ImageList drag_image;
    AnimeListDialog* parent;
  } listview;

  // Other controls
  win32::Tab tab;
};

extern class AnimeListDialog AnimeListDialog;

#endif // DLG_ANIME_LIST_H