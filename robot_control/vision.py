import cv2
import cv2.cv as cv

def detect(img, cascade, maxSiz):
    rects = cascade.detectMultiScale(img, maxSize=maxSiz);
    if len(rects) == 0:
        return []
    rects[:,2:] += rects[:,:2];
    return rects




def draw_rects(img, rects, color):
    for x1, y1, x2, y2 in rects:
        cv2.rectangle(img, (x1, y1), (x2, y2), color);
    return;

def remove_common(blocks, logos):
    result = [];
    for x1, y1, x2, y2 in blocks:
        b_height= (y1+y2)/2;
        conflict = False;
        for xl1, yl1, xl2, yl2 in logos:
            l_height = (yl1+yl2) / 2;
            if (abs(l_height - b_height) < 7):
                conflict = True;
        if (not conflict):
            result.append([x1, y1, x2, y2]);
    return result;



def main(sfilename, dfilename):#source then destination
    real_img = cv2.imread(sfilename, cv2.CV_LOAD_IMAGE_COLOR);
    bw = cv2.imread(sfilename,cv2.CV_LOAD_IMAGE_GRAYSCALE)
    #cv2.adaptiveBilateralFilter(bw, (7, 7), 2)
    bw = cv2.GaussianBlur(bw, (3,3), .4);#works solidly well
    #edges = cv2.Canny(bw, 12.75,31.875);
    edges = cv2.Canny(bw, 70, 175);
    #edges = cv2.Canny(bw, 100, 200);
    cv2.imwrite('./results/canny0.JPG', edges);
    starty = 65;
    endy = starty + 223;
    startx = startx = 120;
    endx = startx + 180;
    edges = edges[starty:endy, startx:endx];
    cascadeb = cv2.CascadeClassifier('./detectors/blocks.xml');
    maxSizb= (51,33)
    cascadel = cv2.CascadeClassifier('./detectors/logo.xml');
    maxSizl = (65, 34);

    rectsb = detect(edges, cascadeb, maxSizb);
    rectsl = detect(edges, cascadel, maxSizl);
    print(rectsl);
    rectsb = remove_common(rectsb, rectsl);
    print(rectsb);
    rectsb2 = [];
    rectsl2 = [];
    for x1, y1, x2, y2 in rectsb:
        rectsb2.append([x1+startx, y1+starty, x2+startx, y2+starty]);
    for x1, y1, x2, y2 in rectsl:
        rectsl2.append([x1+startx, y1+starty, x2+startx, y2+starty]);

    draw_rects(real_img, rectsb2, (0, 255, 0));
    draw_rects(real_img, rectsl2, (255, 0, 0));

    cv2.imwrite(dfilename, real_img);
    return;


main('./sources/output38.JPG', './results/output38.JPG');


