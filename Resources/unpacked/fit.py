import xml.etree.ElementTree as ET
from PIL import Image
import copy

fitted_image = None
count = 0

def convert(image):
    global count, fitted_image
    rows = image.height // 16
    columns = image.width // 16
    for row in range(rows):
        for column in range(columns):
            box = ( column * 16, row * 16, column * 16 + 16,row * 16 + 16)
            tile = image.crop(box)
            fitted_image.paste(tile, ((count % 40) * 16, (count // 40) * 16))
            count += 1

def parse_tmx(file_path):
    tree = ET.parse(file_path)
    root = tree.getroot()
    tilecount = 0
    columns = 40

    for tileset in root.findall('tileset'):   # count tiles
        print(tileset.attrib["tilecount"])
        tilecount += int(tileset.attrib["tilecount"])

    global fitted_image
    fitted_image = Image.new("RGBA", (16 * columns, 16 * int((tilecount - 1) / columns + 1)), (0, 0, 0, 0)) # create a blank image
    print(((tilecount - 1) / columns) + 1)

    for tileset in root.findall('tileset'):
        for images in tileset.findall('image'):
            print(images.attrib["source"])
            image_source = images.attrib["source"]
            if image_source[-3: ] != "png":
                image_source += ".png"
            image = Image.open(image_source)
            convert(image)  # combine images into one png


        

    new_tileset = ET.Element('tileset', {
        'firstgid': '1',
        'name': 'cocos_fitted', 
        'tilewidth': '16', 
        'tileheight': '16', 
        'tilecount': f"{tilecount}", 
        'columns': f"{columns}"
    })
    new_image_element = ET.SubElement(new_tileset, 'image', {
        'source': 'new_image.png', 
        'width': '256', 
        'height': f'{16 * tilecount / columns}'
    })
    # 找到第一个 tileset 节点
    first_tileset = root.find('tileset')

    for tileset in root.findall('tileset'):
        for tile in tileset.findall("tile"):
            for animation in tile.findall("animation"):
                tile.remove(animation)
            new_tileset.append(tile)
        
    if first_tileset is not None:
        # 获取所有子元素的列表
        children = list(root)
        # 找到第一个 tileset 节点的索引
        index = children.index(first_tileset)
        # 在该索引位置插入新的 tileset 节点
        root.insert(index, new_tileset)
    else:
        root.append(new_tileset)

    skip = 1
    for tileset in root.findall('tileset'):
        if skip:
            skip = 0
            continue
        root.remove(tileset)    # remove former tilesets
    
    # save tmx and png
    tree.write(file_path, encoding='utf-8', xml_declaration=True)        
    fitted_image.save("new_image.png")

# 使用示例

# path = input("print in your tmx filename(do not need \".tmx\"):") + ".tmx"
path = "newnewFarm.tmx"
parse_tmx(path)