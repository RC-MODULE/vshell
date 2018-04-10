    //--------------------------------------------------------------------
#ifndef _AVIWRITER_H_
#define _AVIWRITER_H_
    //--------------------------------------------------------------------
#include <vfw.h>
    //--------------------------------------------------------------------
class C_AVIWriter
{
private:
    PAVIFILE m_pAF;
    PGETFRAME m_pGF;
    PAVISTREAM m_pAS;
    int m_nSizeImage;

    BOOL Close();
public:
    C_AVIWriter();
    ~C_AVIWriter();

    BOOL Open(LPSTR szFileName, LPBITMAPINFOHEADER pBIH);
    BOOL WriteFrame(BYTE *pbImage);
};
    //--------------------------------------------------------------------
#endif  // _AVIWRITER_H_