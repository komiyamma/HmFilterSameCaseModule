// 2004.12.15 ���n�߁B
// �u�ҏW�E�ϊ��v�z���̃R�}���h��DLL�ɒǂ��o���B���łɁA�����I�ɑ����̕ϊ��֐����v���O�C���I�ɃT�|�[�g����B
// 2008.2.25
//  HIDEMARUFILTERFUNCA�͎g���Ȃ��̂ō폜�B
// 2016.11.25
//  WM_HIDEMARUINFO���̓}�N���w���v�ɋL�ڂ��āA��������͍폜�B

typedef HGLOBAL (_cdecl* HIDEMARUFILTERFUNC)( HWND hwndHidemaru, WCHAR* pwszIn, char* pszParam, int cbParamBuffer );

struct HIDEMARUFILTERINFO {
    UINT                cbStructSize;
    const char*         pszExportName;
    const char*         pszNameJapan;
    const char*         pszNameUs;
    BYTE                chAccel;        // �A�N�Z�����[�^�L�[
    BYTE                fMustLineUnit;  // �s�P�ʂłȂ��Ǝ��s�ł��Ȃ��t���O
    BYTE                abReserve[2];
};


// extern "C" HIDEMARUFILTERINFO* _cdecl EnumHidemaruFilter();
// extern "C" DWORD _cdecl HidemaruFilterGetVersion();

