import cv2

def main(source):
    img = cv2.imread(source, 0) #make it black and white
    edges = cv2.Canny(img, 100, 200) #threshold values for canny alg
    cv2.imshow("edges", edges)
    return

main('./sources/output0.JPG')

