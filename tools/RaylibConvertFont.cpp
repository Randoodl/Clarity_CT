#include <raylib.h>

//Convert a font.ttf/otf into an EmbeddedFont header file using raylib's ExportFontAsCode() function
//If you don't trust the pile of unsigned char data (which in good practice, you probably shouldn't), you can export your own

int main(int argc, char* argv[])
{
    //It seems to throw a fit if I don't init a window, so here it goes
    InitWindow(10, 10, "");

    Font LoadedFont = LoadFontEx(argv[1], 50, 0, 0);
    ExportFontAsCode(LoadedFont, "EmbeddedFont.h");
    UnloadFont(LoadedFont);

    CloseWindow();

    return 0;
}