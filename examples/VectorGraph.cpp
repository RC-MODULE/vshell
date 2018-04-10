//------------------------------------------------------------------------
//  VShell example: vec graphic 
//
//  Author: Sergey Landyshev
//
//  Copyright (c) 2005 RC Module Inc.
//------------------------------------------------------------------------
#include <windows.h>
#include "VShell.h"

static const int WIDTH = 256;
static const int HEIGHT = 256;
static const int BLOCK_SIZE = 8;
static const int VECTORS = (WIDTH * HEIGHT) / (BLOCK_SIZE * BLOCK_SIZE);

int main()
{

    VS_Init();
    VS_CreateImage("Vector Graphic", 0, WIDTH, HEIGHT, VS_RGB8, NULL);
    VS_CreateImage("Float vec Graphic", 1, WIDTH, HEIGHT, VS_RGB8, NULL);
    
    S_VS_Point *pVectors = new S_VS_Point[VECTORS];
    int i, j, k;
    for(k=0, i=0; i<WIDTH / BLOCK_SIZE; i++)
    {
        for(j=0; j<HEIGHT / BLOCK_SIZE; j++, k++)
        {
            pVectors[k].nX = k % 8;
            pVectors[k].nY = k % 8;
        }
    }
    pVectors[4].nX = VS_NULL_VEC;
    pVectors[6].nX = VS_NULL_VEC;
    
    S_VS_Point pPolyline[8] = 
    {
        {200, 200}, 
        {220, 180}, 
        {220, 200},
        {230, 200},
        {220, 210}, 
        {220, 230},
        {200, 220},
        {200, 230}
    };

    S_VS_Point pPolygon[8] = 
    {
        {250, 250}, 
        {270, 230}, 
        {270, 250},
        {280, 250},
        {270, 260}, 
        {270, 280},
        {250, 270},
        {250, 280}
    };
    S_VS_PointF pPolylineF[8] = 
    {
        {200.5f, 200.5f}, 
        {220.5f, 180.5f}, 
        {220.5f, 200.5f}, 
        {230.5f, 200.5f}, 
        {220.5f, 210.5f}, 
        {220.5f, 230.5f}, 
        {200.5f, 220.5f}, 
        {200.5f, 230.5f}
    };

    VS_Text("Пример демонстрирует построение изображений\r\n");
    VS_Text("с использованием векторной графики.\r\n");
    VS_Text("Источник изображений не задаётся.\r\n");
    VS_Text("Векторная графика накладывается на пустой буфер.\r\n");
    VS_Text("В примере для вывода графики используются 2 окна:\r\n");
    VS_Text("в первое выводиться целочисленная векторная графика,\r\n");
    VS_Text("во второе - графика с числами с плавающей точкой.\r\n");
    VS_Text("Для того, чтобы увидеть особенности векторной графики\r\n");
    VS_Text("с плавающей точкой необходимо увеличить изображение.\r\n");

    while(VS_Run())
    {
        for(i=0; i<HEIGHT; i+=8)
        {
            for(j=0; j<WIDTH; j+=8)
            {
                VS_Rectangle(0, j+1, i+1, j+6, i+6, RGB(200, 0, 0), RGB(255, 255, 0));
                VS_Ellipse(0, j+2, i+2, j+4, i+4, RGB(0, 100, 0), RGB(0, 255, 255));
                VS_Line(0, j+4, i+2, j+4, i+6, RGB(100, 0, 0));
                VS_Line(0, j+2, i+4, j+6, i+2, RGB(255, 0, 0));
            }
        }
        VS_Rectangle(0, 120, 120, 140, 140, RGB(200, 0, 0), RGB(255, 255, 0));
        VS_Ellipse(0, 140, 140, 150, 145, RGB(0, 255, 0), RGB(0, 255, 255));
        VS_SetPixel(0, 50, 50, RGB(255, 150, 255));
        VS_SetPixel(0, 50, 60, RGB(255, 150, 255));
        VS_SetPixel(0, 60, 60, RGB(255, 150, 255));
        VS_SetPixel(0, 60, 50, RGB(255, 150, 255));
        VS_Line(0, 100, 100, 105, 105, RGB(255, 0, 0));
        VS_Polyline(0, pPolyline, 8, RGB(100, 255, 100));
        VS_Polygon(0, pPolygon, 8, RGB(255, 255, 0), RGB(100, 50, 0));
        VS_Pie(0, 0, 0, 80, 80, 50, 0, 0, 50, RGB(255, 255, 0), RGB(255, 0, 255));
        VS_Chord(0, 80, 0, 160, 80, 120, 0, 0, 50, RGB(255, 255, 0), RGB(255, 0, 255));
        VS_Arc(0, 160, 0, 240, 80, 200, 0, 0, 50, RGB(255, 255, 0));

        VS_SetVectors(0, BLOCK_SIZE, pVectors, NULL);
        
        VS_Line(0, 160, 170, 105, 105, RGB(0, 255, 0));
        VS_Line(0, 160, 170, 105, 115, RGB(0, 255, 0));
        VS_Line(0, 160, 170, 105, 125, RGB(0, 255, 0));
        VS_Line(0, 160, 170, 105, 135, RGB(0, 255, 0));
        VS_Line(0, 160, 170, 105, 145, RGB(0, 255, 0));

		VS_Line(0, 160, 170, 185, 105, RGB(0, 255, 220));
        VS_Line(0, 160, 170, 185, 115, RGB(0, 255, 220));
        VS_Line(0, 160, 170, 185, 125, RGB(0, 255, 220));
        VS_Line(0, 160, 170, 185, 185, RGB(0, 255, 0));
        VS_Line(0, 160, 170, 125, 185, RGB(220, 255, 0));

        VS_TextOut(0, 50, 50, 10, RGB(255, 255, 255), "Vector graphic %d", 12345);

        for(i=0; i<HEIGHT; i+=8)
        {
            for(j=0; j<WIDTH; j+=8)
            {
                VS_RectangleF(1, j+1.5f, i+1.5f, j+6.5f, i+6.5f, RGB(200, 0, 0), RGB(255, 255, 0));
                VS_EllipseF(1, j+2.5f, i+2.5f, j+4.5f, i+4.5f, RGB(0, 100, 0), RGB(0, 255, 255));
                VS_LineF(1, j+4.5f, i+2.5f, j+4.5f, i+6.5f, RGB(100, 0, 0));
                VS_LineF(1, j+2.5f, i+4.5f, j+6.5f, i+2.5f, RGB(255, 0, 0));
            }
        }
        VS_RectangleF(1, 120.5f, 120.5f, 140.5f, 140.5f, RGB(200, 0, 0), RGB(255, 255, 0));
        VS_EllipseF(1, 140.5f, 140.5f, 150.5f, 145.5f, RGB(0, 255, 0), RGB(0, 255, 255));
        VS_SetPixelF(1, 50.5f, 50.5f, RGB(255, 150, 255));
        VS_SetPixelF(1, 50.5f, 60.5f, RGB(255, 150, 255));
        VS_SetPixelF(1, 60.5f, 60.5f, RGB(255, 150, 255));
        VS_SetPixelF(1, 60.5f, 50.5f, RGB(255, 150, 255));
        VS_LineF(1, 100.5f, 100.5f, 105.5f, 105.5f, RGB(255, 0, 0));
        VS_PolylineF(1, pPolylineF, 8, RGB(100, 255, 100));
        VS_PolygonF(1, pPolylineF, 8, RGB(255, 255, 0), RGB(100, 50, 0));
        VS_PieF(1, 0.5f, 0.5f, 80.5f, 80.5f, 50.5f, 0.5f, 0.5f, 50.5f, RGB(255, 255, 0), RGB(255, 0, 255));
        VS_ChordF(1, 80.5f, 0.5f, 160.5f, 80.5f, 120.5f, 0.5f, 0.5f, 50.5f, RGB(255, 255, 0), RGB(255, 0, 255));
        VS_ArcF(1, 160.5f, 0.5f, 240.5f, 80.5f, 200.5f, 0.5f, 0.5f, 50.5f, RGB(255, 255, 0));

        VS_SetVectors(1, BLOCK_SIZE, pVectors, NULL);
        
        VS_LineF(1, 160.5f, 170.5f, 105.5f, 105.5f, RGB(0, 255, 0));
        VS_LineF(1, 160.5f, 170.5f, 105.5f, 115.5f, RGB(0, 255, 0));
        VS_LineF(1, 160.5f, 170.5f, 105.5f, 125.5f, RGB(0, 255, 0));
        VS_LineF(1, 160.5f, 170.5f, 105.5f, 135.5f, RGB(0, 255, 0));
        VS_LineF(1, 160.5f, 170.5f, 105.5f, 145.5f, RGB(0, 255, 0));

		VS_LineF(1, 160.5f, 170.5f, 185.5f, 105.5f, RGB(0, 255, 220));
        VS_LineF(1, 160.5f, 170.5f, 185.5f, 115.5f, RGB(0, 255, 220));
        VS_LineF(1, 160.5f, 170.5f, 185.5f, 125.5f, RGB(0, 255, 220));
        VS_LineF(1, 160.5f, 170.5f, 185.5f, 185.5f, RGB(0, 255, 0));
        VS_LineF(1, 160.5f, 170.5f, 125.5f, 185.5f, RGB(220, 255, 0));

        
        VS_Draw(VS_DRAW_ALL);
        Sleep(100);
    }
    
    delete [] pVectors;
    return 0;
}
//-------------------------------------------------------------------
