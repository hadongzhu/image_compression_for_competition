import subprocess
import time

if __name__ == '__main__':
    for index in range(1 , 34):
        print('compress sample%02d.bmp ...' % (index))
        result = subprocess.run(['./cmake-build-debug-visual-studio/image_compression_for_competition.exe', '-en', './res/sample%02d.bmp' % (index)], stdout=subprocess.PIPE)
        with open('./res/sample%02d_%s.txt' % (index, time.strftime("%Y%m%d%H%M%S", time.localtime())), 'w') as f:
            f.write(result.stdout.decode('utf-8'))

    for index in range(1, 34):
        print('decompress sample%02d.bmp.jlcd ...' % (index))
        result = subprocess.run(['./cmake-build-debug-visual-studio/image_compression_for_competition.exe', '-de', './res/sample%02d.bmp.jlcd' % (index), './res/sample%02dtest.bmp' % (index)], stdout=subprocess.PIPE)
        print(result.stdout.decode('utf-8'))

    for index in range(1, 34):
        print('compare sample%02d.bmp ...' % (index))
        result = subprocess.run(['./cmake-build-debug-visual-studio/image_compression_for_competition.exe', '-cp', './res/sample%02d.bmp' % (index), './res/sample%02dtest.bmp' % (index)], stdout=subprocess.PIPE)
        print(result.stdout.decode('utf-8'))