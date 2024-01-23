
#include "face_dect.hpp"

// int face_dect_display()
// {

//     //获取一帧
//     camera_fb_t *fb = NULL;
//     int res = SUCCESS;
//     fb = esp_camera_fb_get();

//     if (!fb)
//     {
//         Serial.println("Camera capture failed");
//         return FAIL;
//     }

//     size_t out_len, out_width, out_height;
//     uint8_t *out_buf;
//     bool s;

//     int face_id = 0;
//     if (fb->width > 400)
//     {
//         esp_camera_fb_return(fb);
//         return res;
//     }

//     if (fb->format == PIXFORMAT_RGB565)
//     {

//         HumanFaceDetectMSR01 s1(0.3F, 0.5F, 10, 0.2F);
//         std::list<dl::detect::result_t> &results = s1.infer((uint16_t *)fb->buf, {(int)fb->height, (int)fb->width, 3});

//         if (results.size() > 0)
//         {
//             fb_data_t rfb;
//             rfb.width = fb->width;
//             rfb.height = fb->height;
//             rfb.data = fb->buf;
//             rfb.bytes_per_pixel = 2;
//             rfb.format = FB_RGB565;

//             draw_face_boxes(&rfb, &results, face_id);
//         }

//         esp_camera_fb_return(fb);
//     }
//     return res;
// }


void draw_face_boxes(fb_data_t *fb, std::list<dl::detect::result_t> *results, int face_id)
{
    int x, y, w, h;
    uint32_t color = FACE_COLOR_YELLOW;
    if (face_id < 0)
    {
        color = FACE_COLOR_RED;
    }
    else if (face_id > 0)
    {
        color = FACE_COLOR_GREEN;
    }
    if(fb->bytes_per_pixel == 2){
        //color = ((color >> 8) & 0xF800) | ((color >> 3) & 0x07E0) | (color & 0x001F);
        color = ((color >> 16) & 0x001F) | ((color >> 3) & 0x07E0) | ((color << 8) & 0xF800);
    }
    int i = 0;
    for (std::list<dl::detect::result_t>::iterator prediction = results->begin(); prediction != results->end(); prediction++, i++)
    {
        // rectangle box
        x = (int)prediction->box[0];
        y = (int)prediction->box[1];
        w = (int)prediction->box[2] - x + 1;
        h = (int)prediction->box[3] - y + 1;
        if((x + w) > fb->width){
            w = fb->width - x;
        }
        if((y + h) > fb->height){
            h = fb->height - y;
        }
        fb_gfx_drawFastHLine(fb, x, y, w, color);
        fb_gfx_drawFastHLine(fb, x, y + h - 1, w, color);
        fb_gfx_drawFastVLine(fb, x, y, h, color);
        fb_gfx_drawFastVLine(fb, x + w - 1, y, h, color);
    }
}