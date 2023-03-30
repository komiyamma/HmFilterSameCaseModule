// 2004.12.15 作り始め。
// 「編集・変換」配下のコマンドをDLLに追い出す。ついでに、将来的に多くの変換関数をプラグイン的にサポートする。
// 2008.2.25
//  HIDEMARUFILTERFUNCAは使われないので削除。
// 2016.11.25
//  WM_HIDEMARUINFO等はマクロヘルプに記載して、ここからは削除。

typedef HGLOBAL (_cdecl* HIDEMARUFILTERFUNC)( HWND hwndHidemaru, WCHAR* pwszIn, char* pszParam, int cbParamBuffer );

struct HIDEMARUFILTERINFO {
    UINT                cbStructSize;
    const char*         pszExportName;
    const char*         pszNameJapan;
    const char*         pszNameUs;
    BYTE                chAccel;        // アクセラレータキー
    BYTE                fMustLineUnit;  // 行単位でないと実行できないフラグ
    BYTE                abReserve[2];
};


// extern "C" HIDEMARUFILTERINFO* _cdecl EnumHidemaruFilter();
// extern "C" DWORD _cdecl HidemaruFilterGetVersion();

