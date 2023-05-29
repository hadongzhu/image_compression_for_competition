# 比赛用图像压缩

### 性能比较



| 算法                  | sample01.bmp | sample02.bmp | sample03.bmp |
|:--------------------|:-------------|:-------------|:-------------|
| LZMA                | 7.08032%     | 16.7472%     | 53.901%      |
| JPEG-LS（含头）         | 21.8635%     | 33.6453%     | 49.2618%     |
| JPEG-LS（含头） + LZMA  | 23.8156%     | 35.8255%     | 51.1689%     |
| JPEG-LS（不含头）        | 5.45728%     | 17.239%      | 32.8556%     |
| JPEG-LS（不含头） + LZMA | 9.65596%     | 21.8506%      | 39.1786%     |
| 混合（JPEG-LS（不含头）、LZMA）   | 4.92183%     | 14.9336%      | 33.2063%     |

### 纯透明空块压缩效果（8\*8\*4bytes）

| 算法             | 原始大小为256 |
|:---------------|:---------|
| LZMA           | 13       |
| JPEG-LS（含头）    | 45       |
| JPEG-LS（含头） + LZMA | 13       |
| JPEG-LS（不含头）    | 3        |
| JPEG-LS（不含头） + LZMA |          |

在压缩数据较少使LZMA再压缩会导致数据反而变大。

通用算法容器
当块长度小于255时，则采用如下的压缩方法：
| 字节数          | 说明          |
|:-------------|:------------|
| 1            | 算法编号(type)        |
| 不定            | 采用对应压缩算法得到的压缩数据      |
当块长度为256时，图片未经压缩，直接写入数据。

算法1（适用于普通的图片）
采用JPEG-LS（不含头）

算法2（适用复杂通用情况）
采用LZMA压缩

算法3（复合，测试下仅少量可压缩）
采用JPEG-LS（不含头）压缩，再采用LZMA压缩

算法4（适用于空白块）
仅做标记，无任何数据，代表所有数据均为0x00

算法5（猜测适用于连续色块且颜色种类多，未实现）
将相同的像素压缩为size(1byte)和color(4byte)，如此直至块结束

算法6（猜测适用于颜色少的大量重复颜色块，未实现）
将出现的颜色编写为字典（index(1byte)和color(4byte)，通过重复的index判断是否开始编码），将颜色块压缩为index(1byte)和size(1byte)，如此直至块结束

混合算法表现
|样本|混合算法（算法1-4）压缩率|JPEG-LS压缩率|YUV+JPEG-LS|color1+JPEG-LS|混合算法（算法1-3,5-6）压缩率|
|:-------------|:------------|:-------------|:------------|:-------------|:------------|
|sample01.bmp|4.92%|5.46%|8.30%|8.32%|4.12%|
|sample02.bmp|14.93%|17.19%|17.84%|17.44%|14.80%|
|sample03.bmp|33.21%|32.86%|31.68%|31.68%|31.03%|
|sample04.bmp|38.65%|38.48%|32.87%|32.97%|32.64%|
|sample05.bmp|33.48%|33.35%|31.29%|31.41%|30.83%|
|sample06.bmp|46.82%|46.44%|42.96%|42.74%|42.63%|
|sample07.bmp|43.66%|43.47%|38.27%|38.51%|38.34%|
|sample08.bmp|48.22%|48.45%|38.54%|38.65%|38.30%|
|sample09.bmp|47.91%|47.92%|39.58%|39.62%|39.42%|
|sample10.bmp|25.49%|25.38%|24.80%|24.71%|23.68%|
|sample11.bmp|22.28%|22.41%|22.17%|22.16%|21.07%|
|sample12.bmp|35.62%|35.40%|32.97%|33.17%|32.67%|
|sample13.bmp|46.66%|46.83%|38.65%|38.70%|38.40%|
|sample14.bmp|24.87%|25.38%|23.19%|23.30%|22.53%|
|sample15.bmp|44.07%|43.99%|37.36%|37.48%|37.16%|
|sample16.bmp|25.56%|25.76%|23.33%|23.42%|22.85%|
|sample17.bmp|32.03%|32.52%|27.92%|27.95%|27.47%|
|sample18.bmp|19.70%|20.00%|18.88%|18.92%|18.06%|
|sample19.bmp|37.69%|37.91%|30.97%|31.11%|30.70%|
|sample20.bmp|32.48%|32.48%|29.25%|29.25%|28.94%|
|sample21.bmp|13.65%|17.13%|16.46%|16.68%|13.55%|
|sample22.bmp|9.06%|10.81%|11.18%|10.99%|9.04%|
|sample23.bmp|17.03%|17.32%|17.36%|17.09%|16.10%|
|sample24.bmp|14.05%|14.90%|14.10%|13.83%|13.15%|
|sample25.bmp|40.25%|40.10%|33.94%|33.96%|33.83%|
|sample26.bmp|56.46%|56.15%|48.63%|48.59%|48.11%|
|sample27.bmp|43.20%|43.21%|35.43%|35.50%|35.27%|
|sample28.bmp|39.28%|39.32%|39.81%|39.72%|37.51%|
|sample29.bmp|50.23%|49.94%|48.08%|47.99%|47.12%|
|sample30.bmp|48.97%|48.62%|44.94%|45.42%|44.79%|
|sample31.bmp|51.81%|52.16%|49.04%|49.49%|48.29%|
|sample32.bmp|21.25%|23.09%|25.79%|25.30%|21.12%|
|sample33.bmp|60.69%|61.00%|57.07%|57.79%|56.17%|
|平均压缩率|34.07%|34.41%|31.29%|31.33%|30.29%|
|最大压缩率|4.92%|5.46%|8.30%|8.32%|4.12%|
|最小压缩率|60.69%|61.00%|57.07%|57.79%|56.17%|




