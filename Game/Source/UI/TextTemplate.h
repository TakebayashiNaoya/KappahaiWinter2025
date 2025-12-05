#pragma once
#include "stdafx.h"

/// <summary>
/// テキストの情報を保持する構造体です。
/// </summary>
struct ResultInfo
{
	std::string text;	// 文字列。（※半角のみ）
	Vector3 position;	// 表示位置。
	float scale;		// 大きさ。
	Vector4 color;		// 色。
};

/// <summary>
/// フォントレンダーの設定。
/// </summary>
/// <param name="pos_x">描画するx座標。</param>
/// <param name="pos_y">描画するy座標。</param>
/// <param name="scale">描画するサイズ。</param>
/// <param name="color">描画する色。</param>
/// <param name="fontRender">設定するフォントレンダー。</param>
/// <param name="format">描画する文字列。</param>
/// <param name="...">描画する文字列を無限に追加できる。</param>
static void SetTextOption(FontRender* fontRender, ResultInfo info, const wchar_t* format, ...)
{
	fontRender->SetPosition(info.position);
	fontRender->SetScale(info.scale);
	fontRender->SetColor(info.color);
	va_list _ArgList;
	__crt_va_start(_ArgList, format);
	wchar_t wcsbuf[256];
	vswprintf_s(wcsbuf, 256, format, _ArgList);
	__crt_va_end(_ArgList);
	fontRender->SetText(wcsbuf);
}