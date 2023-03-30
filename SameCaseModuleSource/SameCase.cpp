// SameCase.hmfおよびSamCase.hmf64
// 秀丸エディタ用の変換モジュールです。
// 置換コマンドで呼び出して使う用です。「編集 - 変換」メニューから選択して使うことは出来ません。
//
// 初回作成： 2023/03/30   by 秀まるお
//
// このモジュールもソースコードも好きに使ってかまいません。勝手にソースコード流用してらっても大丈夫です。
//
//

#include "pch.h"
#include "HmFilter.h"

extern "C" HGLOBAL _cdecl SameCase(HWND hwndHidemaru, WCHAR * pwsz, char* pszParam, int cbParamBuffer);

struct HIDEMARUFILTERINFO aFilterInfo[14] = {
	{ sizeof(HIDEMARUFILTERINFO), "SameCase", "大文字/小文字同期置換(置換コマンド用)", "Same case convert (for replace use)", 'S', 0, 0, 0 },
	{ 0, NULL, NULL, NULL, NULL, 0, 0, 0 }
};

extern "C" HIDEMARUFILTERINFO * _cdecl EnumHidemaruFilter() {
	return aFilterInfo;
}

extern "C" DWORD _cdecl HidemaruFilterGetVersion() {
	return (1 << 16) + (0 * 10) + 0;		// 1.00
}



BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//入力された文字列をそのままHGLOBALに入れて返す。NULLを返すと全置換が途中で止まってしまうので。
HGLOBAL ReturnNotConvert( WCHAR* pwsz ) {
	int cbCopy = ((int)wcslen( pwsz ) + 1) * sizeof(WCHAR);
	HGLOBAL hmem = GlobalAlloc( GMEM_MOVEABLE, cbCopy );
	WCHAR*	pwszDest = (WCHAR*)GlobalLock( hmem );
	memcpy( pwszDest, pwsz, cbCopy );
	GlobalUnlock( hmem );
	return hmem;
}

extern "C" HGLOBAL _cdecl SameCase( HWND hwndHidemaru, WCHAR* pwsz, char* pszParam, int cbParamBuffer ) {
	if( *pszParam == '\0' ) {
		//パラメータが指定されてない場合はエラーにするしか無い。
Label_ErrorExit: ;
		MessageBoxW( GetLastActivePopup( hwndHidemaru )
					, L"SameCase変換モジュールからのエラーメッセージ:\n"
					  L"パラメーターが指定されていないか、またはパラメータが不正です。パラメーターに置換先文字列の英単語を指定してください。\n"
					  L"例(\"Source\"を\"Dest\"に置換する例):\n"
					  L"検索文字列= (source)\n"
					  L"置換文字列= \\(1,SameCase,SameCase,dest)\n"
					  L"のように指定すると、\n"
					  L"    Source → Dest\n"
					  L"    source → dest\n"
					  L"    SOURCE → DEST\n"
					  L"のように置換します。\n"
					, L"SameCase変換モジュール"
					, MB_OK | MB_ICONHAND );
		return NULL;
	}
	//全部小文字、全部大文字、先頭だけ大文字の３パターンの判断をする。それ以外は変換しない。
	//
	BOOL	fTopUpperCase = FALSE;
	int		fBodyUpperCase = -1;
	if( *pwsz >= 'a' && *pwsz <= 'z' ) {
		//小文字でスタートの場合は全部小文字
	} else if( *pwsz >= 'A' && *pwsz <= 'Z' ) {
		fTopUpperCase = TRUE;
	} else {
		return ReturnNotConvert( pwsz );		//変換しない。
	}
	WCHAR*	pwszStart = pwsz;
	pwsz ++;

	// 単語の長さが1文字しか無かった場合は --> 全部大文字か全部小文字に変換する。
	if( *pwsz == 0 ) {
		//	//単語の長さ2文字以上必要
		//	goto Label_ErrorExit;
		fBodyUpperCase = fTopUpperCase;
	} else {
		while( *pwsz != 0 ) {
			if( *pwsz >= 'a' && *pwsz <= 'z' ) {
				if( fBodyUpperCase <= 0 ) {
					fBodyUpperCase = 0;
				} else {
					return ReturnNotConvert( pwszStart );		//2文字目以降が大文字/小文字混じりの場合は変換しない。
				}
			} else if( *pwsz >= 'A' && *pwsz <= 'Z' ) {
				if( fBodyUpperCase != 0 ) {		//-1または1の場合ならOK。
					fBodyUpperCase = 1;
				} else {
					return ReturnNotConvert( pwszStart );		//2文字目以降が大文字/小文字混じりの場合は変換しない。
				}
			}
			pwsz ++;
		}
	}
	int	cchWord = (int)strlen( pszParam );
	HGLOBAL	hmem = GlobalAlloc( GMEM_MOVEABLE, (cchWord + 1) * sizeof(WCHAR) );
	WCHAR*	pwszDest = (WCHAR*)GlobalLock( hmem );
	if( isalpha( *pszParam ) ) {
		if( fTopUpperCase ) {
			*pwszDest = (*pszParam & 0xDF);		//大文字
		} else {
			*pwszDest = (*pszParam & 0xDF) | 0x20;		//小文字
		}
	} else if( (*pszParam & 0x80) != 0 ) {		//全角文字はパラメータエラー。
		goto Label_ErrorExit;
	} else {
		*pwszDest = *pszParam;
	}
	pwszDest ++;
	pszParam ++;
	while( *pszParam != 0 ) {
		if( isalpha( *pszParam ) ) {
			if( fBodyUpperCase ) {
				*pwszDest = (*pszParam & 0xDF);		//大文字
			} else {
				*pwszDest = (*pszParam & 0xDF) | 0x20;		//小文字
			}
		} else if( (*pszParam & 0x80) != 0 ) {		//全角文字はパラメータエラー。
			goto Label_ErrorExit;
		} else {
			*pwszDest = *pszParam;
		}
		pszParam ++;
		pwszDest ++;
	}
	*pwszDest = 0;
	GlobalUnlock( hmem );
	return hmem;
}
