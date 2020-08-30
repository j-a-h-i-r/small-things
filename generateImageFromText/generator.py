import os
import warnings
from PIL import Image, ImageDraw, ImageFont

def generateImage(
        text, 
        width=200, 
        height=120, 
        color=(0,0,0), 
        background=(255,255,255), 
        fontsize=40,
        savedir='',
        fontname="impact.ttf"
    ):
    blankImage = Image.new('RGB', (width, height), background)
    font = ImageFont.truetype(fontname, fontsize)
    drawing = ImageDraw.Draw(blankImage)
    textWidth, textHeight = drawing.textsize(text, font=font)
    if textWidth > width:
        warnings.warn('Calculated text width greater than image width. Image will be clipped')
    if textHeight > height:
        warnings.warn('Calculated text height greater than image height. Image will be clipped')
    textPosW, textPosH = ((width - textWidth)//2), ((height - textHeight)//2)
    drawing.text((textPosW, textPosH), text, fill=color, font=font)
    filename_with_extension = f'{text}.png'
    image_save_path = os.path.join(savedir, filename_with_extension)
    blankImage.save(image_save_path)


if __name__ == '__main__':
    SAVE_DIR = 'generated'
    if not os.path.exists(SAVE_DIR):
        os.makedirs(SAVE_DIR)
    generateImage('100', fontsize=64, savedir=SAVE_DIR, color=(250, 120, 120), background=(120,120,120))
