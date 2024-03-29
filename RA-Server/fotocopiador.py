import sys
import cv2
import numpy as np
import os.path
import multiprocessing
from itertools import starmap

NBHD_SIZE = 19
UNSHARP_T = 48
ADAPT_T   = 24
INVERT = False
#ASPECT = 8.5/11.0  #carta
#ASPECT = 8.5/13.0  #oficio
#ASPECT = 8.5/14.0  #legal
ASPECT = 210/297  #A4
#ASPECT = 297/420  #A3

def processImage(fname):
		print "Processing %s" % fname
		source = cv2.imread(fname, cv2.CV_LOAD_IMAGE_GRAYSCALE)
		#cv2.imwrite('01.png', source)  ##################################################################
		# source = cv2.imread(fname, cv2.CV_LOAD_IMAGE_UNCHANGED )
		if INVERT:
			source = 255 - source
		return bitone(warpSheet(source))

def bitone(image):
    '''
    Convert a greyscale image to a bitone image,
    in such a way that we preserve as much detail as possible,
    and have the least amount of speckles.
    '''
    # First, sharpen the image: unsharp mask w/ threshold.
    blur = cv2.blur(image, (NBHD_SIZE, NBHD_SIZE))
    cv2.imwrite('02.png', blur)  ##################################################################
    diff = cv2.absdiff(image, blur)
    cv2.imwrite('03.png', diff)  ##################################################################
	
    # Apparently OpenCV doesn't have a way to
    # apply a mask to a weighted sum, so we do it ourselves.
    _,mask = cv2.threshold(blur, UNSHARP_T, 1, cv2.THRESH_BINARY)
    cv2.imwrite('04.png', mask)  ##################################################################
    blur = cv2.multiply(blur, mask)
    cv2.imwrite('05.png', blur)  ##################################################################
    sharpened = cv2.addWeighted(image, 2, blur, -1, 0)
    cv2.imwrite('sharp.png', sharpened)
    # Now threshold the sharpened image.
    thresh = cv2.adaptiveThreshold(sharpened, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, NBHD_SIZE, ADAPT_T)
    cv2.imwrite('07.png', thresh)  ##################################################################
    return thresh

def findPaper(image):
    '''
    Try to find a sheet of paper contained in the image.
    Return the contour or raise ValueError if none found.
    '''
    squares = []    
	# Blur image to emphasize bigger features.
    blur = cv2.blur(image, (2, 2))		## CESAR - Le cambie de 2,2  a  4,4 porque asi identifica mejor el cuadrado
    cv2.imwrite('blurFindPaper.png', blur)  ##################################################################
    retval, edges = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
    cv2.imwrite('edges.png', edges)  ###############################################################3
	
    contours, hierarchy = cv2.findContours(edges, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
	
    for c in contours:
        clen = cv2.arcLength(c, True)
        c = cv2.approxPolyDP(c, 0.02*clen, True)
        area = abs(cv2.contourArea(c))
        if len(c) == 4 and \
           0.1 * edges.size <= area <= 0.9 * edges.size and \
           cv2.isContourConvex(c):
                squares.append(c)
    return max(squares, key=lambda s: cv2.arcLength(s,True))

def warpSheet(image):
    '''
    Automatically crops an image to paper size if possible.
    '''
    try:
        sheet = findPaper(image)        
    except ValueError:
        return image
    h, w = image.shape
    src = sheet[::,0,::].astype('float32')
    # Compute distances from topleft corner (0,0)
    # to find topleft and bottomright
    d = np.sum(np.abs(src)**2,axis=-1)**0.5
    t_l = np.argmin(d)
    b_r = np.argmax(d)
    # Compute distances from topright corner (w,0)
    # to find topright and bottomleft
    y = np.array([[w,0],]*4)
    d = np.sum(np.abs(src-y)**2,axis=-1)**0.5
    t_r = np.argmin(d)
    b_l = np.argmax(d)
    #Now assemble these together
    if h >= w:
        destH, destW = h, int(h*ASPECT)
    else:
        destW, destH = h, int(h*ASPECT)
    dest = np.zeros(src.shape,dtype='float32')
    dest[t_l] = np.array([0,0])
    dest[t_r] = np.array([destW,0])
    dest[b_l] = np.array([0,destH])
    dest[b_r] = np.array([destW,destH])
    transform = cv2.getPerspectiveTransform(src, dest)
    return cv2.warpPerspective(image, transform, (destW, destH))

def rename(originalName):
    d,f = os.path.split(originalName)
    f,ext = os.path.splitext(f)
    return os.path.join(d,'p_%s.png' %f)

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print "Usage: fotocopiador.py [-i] files"
        print "-i inverts images."
    else:
	if sys.argv[1] == "-i":
	    INVERT = True
	    files = sys.argv[2:]
        else:
            files = sys.argv[1:]
    pool = multiprocessing.Pool()
    processed = pool.map(processImage, files)
    newnames = map(rename, files)
    for n,i in zip(newnames, processed):
	cv2.imwrite(n, i)