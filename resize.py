import cv2
import argparse


def parse_opt():
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', type=str,
                        help='path of input file that we want to resize')
    parser.add_argument(
        '-s', type=str, help='path of the output file that we want to resize input image into')
    parser.add_argument('-o', type=str, help='output path')
    return parser.parse_args()


if __name__ == '__main__':
    opt = parse_opt()
    img = cv2.imread(opt.input, cv2.IMREAD_UNCHANGED)
    resized = cv2.resize(img, tuple(map(lambda x: (x[1], x[0]), [cv2.imread(
        opt.s, cv2.IMREAD_UNCHANGED).shape[:-1]]))[0], interpolation=cv2.INTER_AREA)
    if len(resized.shape) > 2 and resized.shape[2] == 4:
        resized = cv2.cvtColor(resized, cv2.COLOR_BGRA2BGR)
    cv2.imwrite(opt.o, resized)
    print(f"Image saved => {opt.o}")
