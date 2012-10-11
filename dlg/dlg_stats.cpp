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

#include "../std.h"

#include "dlg_stats.h"

#include "../anime_db.h"
#include "../common.h"
#include "../gfx.h"
#include "../myanimelist.h"
#include "../resource.h"
#include "../stats.h"
#include "../string.h"

class StatsDialog StatsDialog;

// =============================================================================

StatsDialog::StatsDialog()
    : header_font_(nullptr) {
}

StatsDialog::~StatsDialog() {
  if (header_font_) {
    ::DeleteObject(header_font_);
    header_font_ = nullptr;
  }
}

// =============================================================================

BOOL StatsDialog::OnInitDialog() {
  // Create bold header font
  if (!header_font_) {
    LOGFONT lFont;
    ::GetObject(GetFont(), sizeof(LOGFONT), &lFont);
    lFont.lfCharSet = DEFAULT_CHARSET;
    lFont.lfWeight = FW_BOLD;
    header_font_ = ::CreateFontIndirect(&lFont);
  }
  // Set new font for headers
  for (int i = 0; i < 3; i++) {
    SendDlgItemMessage(IDC_STATIC_HEADER1 + i, WM_SETFONT, 
      reinterpret_cast<WPARAM>(header_font_), FALSE);
  }

  // Calculate and display statistics
  Stats.CalculateAll();
  Refresh();

  return TRUE;
}

INT_PTR StatsDialog::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_DRAWITEM: {
      // Draw score bars
      if (wParam == IDC_STATIC_ANIME_STAT2) {
        LPDRAWITEMSTRUCT dis = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
        win32::Rect rect = dis->rcItem;
        win32::Dc dc = dis->hDC;
        
        int bar_height = GetTextHeight(dc.Get());
        int bar_left = rect.left;
        int bar_max = rect.Width();
        
        for (int i = 10; i > 0; i--) {
          int bar_width = static_cast<int>(bar_max * Stats.score_distribution[i]);
          if (i < 10) rect.top += bar_height;
          rect.bottom = rect.top + bar_height - 2;
          rect.right = rect.left + bar_width;
          dc.FillRect(rect, mal::COLOR_DARKBLUE);
        }
        
        dc.DetachDC();
        return TRUE;
      }
      break;
    }
  }
  
  return DialogProcDefault(hwnd, uMsg, wParam, lParam);
}


// =============================================================================

void StatsDialog::Refresh() {
  if (!IsWindow()) return;
  
  // Anime list
  wstring text;
  text += ToWstr(Stats.anime_count) + L"\n";
  text += ToWstr(Stats.episode_count) + L"\n";
  text += Stats.life_spent_watching + L"\n";
  text += ToWstr(Stats.score_mean, 2) + L"\n";
  text += ToWstr(Stats.score_deviation, 2);
  SetDlgItemText(IDC_STATIC_ANIME_STAT1, text.c_str());

  // Score distribution
  RedrawWindow();

  // Database
  text.clear();
  text += ToWstr(static_cast<int>(AnimeDatabase.items.size())) + L"\n";
  vector<wstring> file_list;
  text += ToWstr(PopulateFiles(file_list, anime::GetImagePath())) + L" (" + 
    ToSizeString(GetFolderSize(anime::GetImagePath(), false)) + L")";
  SetDlgItemText(IDC_STATIC_ANIME_STAT3, text.c_str());
}