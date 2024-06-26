#include "./include/core/types.h"
#include "printf.h"
#include "string.h"

#include "RGBImage.h"

int VBE_SetMode( ulong mode );
int VBE_Setup(int w, int h);
double floor(double x);

extern ulong vbe_lfb_addr;
extern ulong vbe_selected_mode;
extern ulong vbe_bytes;

int HSVtoRGB(int _h, int _s, int _v)
{
double h = (double)_h / 255.0, s = (double)_s / 255.0, v = (double)_v / 255.0;
double r = 0;
double g = 0;
double b = 0;

if (s == 0)
{
    r = v;
    g = v;
    b = v;
}
else
{
    double varH = h * 6;
    double varI = floor(varH);
    double var1 = v * (1 - s);
    double var2 = v * (1 - (s * (varH - varI)));
    double var3 = v * (1 - (s * (1 - (varH - varI))));

    if (varI == 0)
    {
        r = v;
        g = var3;
        b = var1;
    }
    else if (varI == 1)
    {
        r = var2;
        g = v;
        b = var1;
    }
    else if (varI == 2)
    {
        r = var1;
        g = v;
        b = var3;
    }
    else if (varI == 3)
    {
        r = var1;
        g = var2;
        b = v;
    }
    else if (varI == 4)
    {
        r = var3;
        g = var1;
        b = v;
    }
    else
    {
        r = v;
        g = var1;
        b = var2;
    }
  }
  return ((int)(r * 255) << 16) | ((int)(g * 255) << 8) | (int)(b * 255);
}
 

void DrawFractal(void)
{
    int x = 0, y = 0, w= 800, h = 600;

    // int color;

    // color = HSVtoRGB(0, 0, 1000);
    // printf("COLOR #%x\n", color);

    // return;

    if (!VBE_Setup(w, h))
	    return;

    printf("DEBUG1");

    if (!VBE_SetMode(vbe_selected_mode | 0x4000)) {
        printf("VBE_SetMode error");
        return;
    }

    printf("VBE_LFB_ADDR: %d\n", vbe_lfb_addr);

    double cRe, cIm;
    double newRe, newIm, oldRe, oldIm;
    double zoom = 1, moveX = 0, moveY = 0;
    int color;
    int maxIterations = 300;

    cRe = -0.7;
    cIm = 0.27015;

    // for (int i = 0; i < 48; i++) {
    //     for (int j = 0; j < 48; j++) {
    //         *(int *)((char *)vbe_lfb_addr + j * w * vbe_bytes + i * vbe_bytes + 0) = 0x00A0FF;
    //     }
    // }

    // return;

    // color = HSVtoRGB(0, 0, 1000);
    // printf("COLOR %d\n", color);

    // 9216

    unsigned int rgb_map[9216] = {0};
    unsigned int rgb_map_size = 0;

    int skip_index = 0;

    for (int i = 0; i < 9216; i++) {
        if (skip_index == 3) {
            skip_index = 0;
            continue;
        }

        rgb_map[rgb_map_size] = bird[i];

        skip_index++;
        rgb_map_size++;
    }

    int colors[9126] = {0};
    unsigned int colors_size = 0;

    int color_index = 0;

    for (int i = 0; i < rgb_map_size; i++) {

        if (color_index == 2) {
            color_index = 0;
            colors_size++;
            continue;
        }

        colors[colors_size] = (rgb_map[i] << (color_index * 8)) | colors[colors_size];
        color_index++;
    }

    int output_index = 0;

    for(y = 0; y < 48; y++)
    for(x = 0; x < 48; x++)
    {

        *(int *)((char *)vbe_lfb_addr + y * w * vbe_bytes + x * vbe_bytes + 0) = colors[output_index++];

        // newRe = 1.5 * (x - w / 2) / (0.5 * zoom * w) + moveX;
        // newIm = (y - h / 2) / (0.5 * zoom * h) + moveY;

        // int i;
        // for(i = 0; i < maxIterations; i++)
        // {
        //     oldRe = newRe;
        //     oldIm = newIm;
        //     newRe = oldRe * oldRe - oldIm * oldIm + cRe;
        //     newIm = 2 * oldRe * oldIm + cIm;
        //     if((newRe * newRe + newIm * newIm) > 4) break;
        // }


	// Draw pixel
        // *(int *)((char *)vbe_lfb_addr + y * w * vbe_bytes + x * vbe_bytes + 0) = 0xFFFFFF;
    }
}