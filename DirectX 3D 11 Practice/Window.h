#pragma once

#include "WinMake.h"


/* Calculates the required size o a window rectangle, based on
	desired client size.*/
void AdjustWindowClientSize(RECT* win_rect, int width, int height) {
	win_rect->left = 0;
	win_rect->right = width + win_rect->left;
	win_rect->top = 0;
	win_rect->bottom = height + win_rect->top;
	/*Function passes in rect, styles, and bool menu toggle. */
	AdjustWindowRect(win_rect,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
}

static LRESULT CALLBACK HandleMsgSetup(
	HWND Window, UINT msg, 
	WPARAM w_param, LPARAM l_param) noexcept;

static LRESULT CALLBACK HandleMsgThunk(
	HWND Window, UINT msg, 
	WPARAM w_param, LPARAM l_param) noexcept;

LRESULT HandleMsg(
	HWND Window, UINT msg, 
	WPARAM w_param, LPARAM l_param) noexcept;

