from PIL import Image, ImageDraw
import struct

steps = 400
frames = []

width = 800
height = 800

def lerp(a, b, t):
	return [(b[0]-a[0])*t+a[0], (b[1]-a[1])*t+a[1]]

def bezier(s, c, d, t):
	return lerp(lerp(s, c, t), lerp(c, d, t), t)

for f in range(0, steps):
	frames.append(Image.new("RGB", (width, height)))
	draw = ImageDraw.Draw(frames[f])
	
	draw.ellipse((0, 0, width, height), fill=(60, 200, 60))
	
	fp = open("animation/%d.bin" % f, "rb")
	pop = struct.unpack("i", fp.read(4))[0]
	food = struct.unpack("i", fp.read(4))[0]
	for p in range(0, pop):
		energy = int(struct.unpack("i", fp.read(4))[0])
		speed  = int(struct.unpack("i", fp.read(4))[0])
		sense  = int(struct.unpack("i", fp.read(4))[0])
		x = int(struct.unpack("d", fp.read(8))[0])+400
		y = int(struct.unpack("d", fp.read(8))[0])+400
		sx = int(struct.unpack("d", fp.read(8))[0])+400
		sy = int(struct.unpack("d", fp.read(8))[0])+400
		cx = int(struct.unpack("d", fp.read(8))[0])+400
		cy = int(struct.unpack("d", fp.read(8))[0])+400
		dx = int(struct.unpack("d", fp.read(8))[0])+400
		dy = int(struct.unpack("d", fp.read(8))[0])+400
		print("%.2f, %.2f" % (x, y))
		
		draw.ellipse((x-5, y-5, x+5, y+5), fill=(int(energy / 21000 * 255), int(energy / 21000 * 255), int(energy / 21000 * 255)))
		
#		draw.ellipse((sx-5, sy-5, sx+5, sy+5), fill=(120, 40, 40))
#		draw.ellipse((cx-5, cy-5, cx+5, cy+5), fill=(40, 40, 120))
#		draw.ellipse((dx-5, dy-5, dx+5, dy+5), fill=(40, 120, 40))
		
#		draw.line((sx, sy, cx, cy), fill=(60, 60, 60))
#		draw.line((cx, cy, dx, dy), fill=(60, 60, 60))
		
	for p in range(0, food):
		x = int(struct.unpack("d", fp.read(8))[0])+400
		y = int(struct.unpack("d", fp.read(8))[0])+400
		
		draw.ellipse((x-2, y-2, x+2, y+2), fill=(120, 20, 120))
	
frames[0].save("out.gif", save_all=True, append_images=frames[1:], loop=0, duration=40)










