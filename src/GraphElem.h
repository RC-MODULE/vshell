    //--------------------------------------------------------------------
#ifndef _GRAPHELEM_H_
#define _GRAPHELEM_H_
    //--------------------------------------------------------------------
const int GRAPH_MAX_PARAMS  = 16;

const int GRAPH_PIXEL       = 1;
const int GRAPH_LINE        = 2;
const int GRAPH_POLYLINE    = 3;
const int GRAPH_POLYBEZIER  = 4;
const int GRAPH_ARC         = 5;
const int GRAPH_RECTANGLE   = 6;
const int GRAPH_POLYGON     = 7;
const int GRAPH_ELLIPSE     = 8;
const int GRAPH_CHORD       = 9;
const int GRAPH_PIE         = 10;  

const int GRAPH_PIXEL_F     = 11;
const int GRAPH_LINE_F      = 12;
const int GRAPH_POLYLINE_F  = 13;
const int GRAPH_POLYBEZIER_F= 14;
const int GRAPH_ARC_F       = 15;
const int GRAPH_RECTANGLE_F = 16;
const int GRAPH_POLYGON_F   = 17;
const int GRAPH_ELLIPSE_F   = 18;
const int GRAPH_CHORD_F     = 19;
const int GRAPH_PIE_F       = 20;

const int GRAPH_TEXTOUT     = 21;
    //--------------------------------------------------------------------
class C_GraphElem
{
protected:
public:
    int pnParams[GRAPH_MAX_PARAMS];

    C_GraphElem();
    virtual ~C_GraphElem() {};
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK) = 0;
};
    //--------------------------------------------------------------------
class C_Pixel : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_Line : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_Polyline : public C_GraphElem
{
public:
    virtual ~C_Polyline() {delete [] (int*)pnParams[0];} 
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_PolyBezier : public C_GraphElem
{
public:
    virtual ~C_PolyBezier() {delete [] (int*)pnParams[0];} 
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_Arc : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_Rectangle : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_Polygon : public C_GraphElem
{
public:
    virtual ~C_Polygon() {delete [] (int*)pnParams[0];} 
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_Ellipse : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_Chord : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_Pie : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_PixelF : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_LineF : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_PolylineF : public C_GraphElem
{
public:
    virtual ~C_PolylineF() {delete [] (float*)pnParams[0];} 
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_PolyBezierF : public C_GraphElem
{
public:
    virtual ~C_PolyBezierF() {delete [] (float*)pnParams[0];} 
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_ArcF : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_RectangleF : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_PolygonF : public C_GraphElem
{
public:
    virtual ~C_PolygonF() {delete [] (int*)pnParams[0];} 
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_EllipseF : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_ChordF : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_PieF : public C_GraphElem
{
public:
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
class C_TextOut : public C_GraphElem
{
    virtual ~C_TextOut() {delete [] (char*)pnParams[0];} 
    virtual void Draw(HDC hDC, int nX, int nY, int nW, int nH, int nK);
};
    //--------------------------------------------------------------------
C_GraphElem *CreateGraphElem(int nType);
    //--------------------------------------------------------------------
#endif