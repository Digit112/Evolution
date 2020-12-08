from PIL import Image, ImageDraw, ImageFont
import struct

steps = 4000
frames = []

width = 800
height = 800

fnt = ImageFont.truetype("arial.ttf", 20)

for f in range(0, steps, 10):
	frames.append(Image.new("RGB", (width, height), (255, 255, 255)))
	draw = ImageDraw.Draw(frames[int(f/10)])
	
	draw.text((360, 770), "Speed", font=fnt, fill=(0, 0, 0))
	
	temp = Image.new("RGB", (60, 20), (255, 255, 255))
	tempd = ImageDraw.Draw(temp)
	tempd.text((0, 0), "Sense", font=fnt, fill=(0, 0, 0))
	temp = temp.rotate(270, Image.NEAREST, expand=1)
	Image.Image.paste(frames[int(f/10)], temp, (15, 360))
	
	draw.text((35, 750), "0", font=fnt, fill=(0, 0, 0))
	draw.text((740, 750), "10", font=fnt, fill=(0, 0, 0))
	draw.text((10, 40), "100", font=fnt, fill=(0, 0, 0))
	
	draw.line((50, 50, 50, 750), fill=(0, 0, 0))
	draw.line((50, 750, 750, 750), fill=(0, 0, 0))
	
	fp = open("animation/%d.bin" % f, "rb")
	pop = struct.unpack("i", fp.read(4))[0]
	food = struct.unpack("i", fp.read(4))[0]
	for p in range(0, pop):
		energy = int(struct.unpack("i", fp.read(4))[0])
		speed  = int(struct.unpack("i", fp.read(4))[0])*70 + 50
		sense  = int(struct.unpack("i", fp.read(4))[0])*7  + 50
		x = int(struct.unpack("d", fp.read(8))[0])+400
		y = int(struct.unpack("d", fp.read(8))[0])+400
		sx = int(struct.unpack("d", fp.read(8))[0])+400
		sy = int(struct.unpack("d", fp.read(8))[0])+400
		cx = int(struct.unpack("d", fp.read(8))[0])+400
		cy = int(struct.unpack("d", fp.read(8))[0])+400
		dx = int(struct.unpack("d", fp.read(8))[0])+400
		dy = int(struct.unpack("d", fp.read(8))[0])+400
		
		draw.ellipse((speed-5, sense-5, speed+5, sense+5), fill=(0, 0, 0))
		
	for p in range(0, food):
		x = int(struct.unpack("d", fp.read(8))[0])+400
		y = int(struct.unpack("d", fp.read(8))[0])+400
	
frames[0].save("out.gif", save_all=True, append_images=frames[1:], loop=0, duration=40)










