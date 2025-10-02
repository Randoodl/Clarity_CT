from PIL import Image
import os

"""
Just a quick and dirty script to turn square .pngs into icon strings
I decided to include it in case you're partial to editing the icons in the ToolBar
But let's face it: if you're looking here, you probably already know of a more efficent way
"""

if __name__ == '__main__':

    #The amount of pixels per side, i.e. 16 for a 256 pixel image
    n_Pixels = 16

    for FILE in os.listdir():
        if ".png" in FILE:
        
            #Load the image
            CurrentImage = Image.open(FILE)

            #Build icon binary string here
            IconString = ""

            #Get the edge length and the centre YX offset value for each pixel
            SideLen    = CurrentImage.height
            PixLen     = SideLen/n_Pixels
            PixOffset  = PixLen/2

            #Loop through all the YX centre points of each pixel
            for i_Row in range(n_Pixels):
                for i_Col in range(n_Pixels):

                    #if pixel has 0 alpha, store a 0, otherwise store a 1 in SetString
                    if CurrentImage.getpixel(((PixLen * i_Col) + PixOffset, (PixLen * i_Row) + PixOffset))[-1] == 0:
                        IconString += "0"
                    else:
                        IconString += "1"

            print(FILE, "\n  ", IconString)