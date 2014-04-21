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


def create_blank_tower(rows):
    t = [];
    i = 0;
    while (i < rows):
        t.append([False, False, False])
        i += 1
    return t;



def get_subtower(blocks):
    col = [];
    row = [];
    blk_avgs = []
    for x1, y1, x2, y2 in blocks:
        yavg = (y1+y2)/2;
        xavg = (x1+x2)/2;
        blk_avgs.append([yavg, xavg])
        i = 0;
        in_rows = False
        while (i < len(row)):
            y_row = row[i];
            if (abs(y_row - yavg) < 5):
                in_rows = True
            i += 1
        if (not in_rows):
            row.append(yavg);
        i = 0;
        in_cols = False;
        while (i < len(col)):
            x_col = col[i]
            if (abs(x_col-xavg) < 15):
                in_cols = True
            i += 1
        if (not in_cols):
            col.append(xavg)
    col.sort();
    row.sort();
    row.reverse();
    #print row;
    print('col is');
    print col;
    t = create_blank_tower(len(row));
    for yavg, xavg in blk_avgs:
        i = 0;
        while (i<(len(row))):
            if (abs(yavg-row[i]) < 5):
                j = 0;
                while (j < 3):
                    if (abs(xavg-col[j]) < 15):
                        t[i][j] = True
                    j += 1
            i += 1
    return (t, row, col)



left_tower = [55, 15, 192, 286];
right_tower = [154, 15, 285, 286];

def main(sfilename, dfilename, tower_location):#source then destination
    real_img = cv2.imread(sfilename, cv2.CV_LOAD_IMAGE_COLOR);
    bw = cv2.imread(sfilename,cv2.CV_LOAD_IMAGE_GRAYSCALE)
    #cv2.adaptiveBilateralFilter(bw, (7, 7), 2)
    bw = cv2.GaussianBlur(bw, (15,15), 1);#works solidly well
    #edges = cv2.Canny(bw, 12.75,31.875);
    edges = cv2.Canny(bw, 70, 175);
    #edges = cv2.Canny(bw, 100, 200);
    cfile_name = sfilename[:-4] + '_canny' + '.JPG';
    cv2.imwrite(cfile_name, edges);
    startx, starty, endx, endy = tower_location;
    edges = edges[starty:endy, startx:endx];
    cascadeb = cv2.CascadeClassifier('./detectors/blocks.xml');
    maxSizb= (60,45)
    cascadel = cv2.CascadeClassifier('./detectors/logo.xml');
    maxSizl = (65, 34);

    rectsb = detect(edges, cascadeb, maxSizb);
    rectsl = detect(edges, cascadel, maxSizl);
    #print(rectsl);
    rectsb = remove_common(rectsb, rectsl);
    #print(rectsb);
    (t, row, col) = get_subtower(rectsb);
    print(t);
    print (row);
    print (col);
    rectsb2 = [];
    rectsl2 = [];
    for x1, y1, x2, y2 in rectsb:
        rectsb2.append([x1+startx, y1+starty, x2+startx, y2+starty]);
    for x1, y1, x2, y2 in rectsl:
        rectsl2.append([x1+startx, y1+starty, x2+startx, y2+starty]);

    draw_rects(real_img, rectsb2, (0, 255, 0));
    draw_rects(real_img, rectsl2, (255, 0, 0));

    cv2.imwrite(dfilename, real_img);
    return (t, row, col);


main('./left.JPG', './results/left.JPG', left_tower);
main('./right.JPG', './results/right.JPG', right_tower);


