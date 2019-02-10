#pragma once

#include <stdlib.h> /* malloc(), free() */
#include <string.h> /* memset(), memcpy() */
#include <stdio.h> /* fopen(), fwrite(), fread(), fclose() */
using namespace std;

// 读取一张bmp图
int bmpDecode(
	const char *filename, unsigned char **imageData, int *width, int *height, int components)
{
	FILE *f = fopen(filename, "rb");

	if (!f)
		return -1;

	unsigned char file_header[14];
	unsigned char info_header[40];
	unsigned char pad[3];
	
	unsigned int w, h;
	unsigned char *data = NULL;

	unsigned int x, y, i, padding;

	memset(file_header, 0, sizeof(file_header));
	memset(info_header, 0, sizeof(info_header));

	// 判断是否读入文件合法
	if (fread(file_header, sizeof(file_header), 1, f) == 0)
	{
		fclose(f);
		return -1;
	}

	if (fread(info_header, sizeof(info_header), 1, f) == 0)
	{
		fclose(f);
		return -1;
	}

	if ((file_header[0] != 'B') || (file_header[1] != 'M'))
	{
		fclose(f);
		return -1;
	}

	if ((info_header[14] != 24) && (info_header[14] != 32))
	{
		fclose(f);
		return -1;
	}

	// 从info头中读入信息
	w = (info_header[4] + (info_header[5] << 8) + (info_header[6] << 16) + (info_header[7] << 24));
	h = (info_header[8] + (info_header[9] << 8) + (info_header[10] << 16) + (info_header[11] << 24));
	
	// 如果这个图能够进行读取
	if ((w > 0) && (h > 0))
	{
		data = (unsigned char*)malloc(w * h * components);
		
		if (!data)
		{
			fclose(f);
			return -1;
		}

		// RGB和BGR是倒着来的
		for (y = (h - 1); y != -1; y--)
		{
			for (x = 0; x < w; x++)
			{
				i = (x + y * w) * components;

				if (fread(data + i, 3, 1, f) == 0)
				{
					free(data);

					fclose(f);
					return -1;
				}
				unsigned char temp = data[i];
				data[i] = data[i + 2];
				data[i + 2] = temp;

				if (components == 4)
					data[i + 3] = 255;
			}

			// 留空
			padding = ((4 - (w * 3) % 4) % 4);

			if (fread(pad, 1, padding, f) != padding)
			{
				free(data);
				fclose(f);
				return -1;
			}
		}
	}
	(*width) = w;
	(*height) = h;
	(*imageData) = data; // 更新
	fclose(f);

	return 0;
}

// 输出一张bmp图
int bmpEncode(
	const char *filename, const unsigned char *imageData, unsigned int width, unsigned int height, unsigned int components)
{
	FILE *f = fopen(filename, "wb");

	if (!f)
		return -1;

	// 初始化头
	unsigned char file_header[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0 };
	unsigned char info_header[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };
	const unsigned char pad[3] = { 0, 0, 0 };

	const unsigned int size = 54 + width * height * 3; // 3 as the BMP format uses 3 channels (red, green, blue and NO alpha)

	unsigned int x, y, i, padding;

	unsigned char pixel[3];

	// 记入信息
	file_header[2] = (unsigned char)(size);
	file_header[3] = (unsigned char)(size >> 8);
	file_header[4] = (unsigned char)(size >> 16);
	file_header[5] = (unsigned char)(size >> 24);

	info_header[4] = (unsigned char)(width);
	info_header[5] = (unsigned char)(width >> 8);
	info_header[6] = (unsigned char)(width >> 16);
	info_header[7] = (unsigned char)(width >> 24);

	info_header[8] = (unsigned char)(height);
	info_header[9] = (unsigned char)(height >> 8);
	info_header[10] = (unsigned char)(height >> 16);
	info_header[11] = (unsigned char)(height >> 24);

	if (fwrite(file_header, 14, 1, f) == 0)
	{
		fclose(f);
		return -1;
	}

	if (fwrite(info_header, 40, 1, f) == 0)
	{
		fclose(f);
		return -1;
	}

	// 倒着来，RGB和BGR，太鬼畜了
	for (y = (height - 1); y != -1; y--)
	{
		for (x = 0; x < width; x++)
		{
			i = (x + y * width) * components;

			memcpy(pixel, imageData + i, sizeof(pixel));
			unsigned char temp = pixel[0];
			pixel[0] = pixel[2];
			pixel[2] = temp;

			if (fwrite(pixel, sizeof(pixel), 1, f) == 0)
			{
				fclose(f);
				return -1;
			}
		}
		// 留空
		padding = ((4 - (width * 3) % 4) % 4);

		if (fwrite(pad, 1, padding, f) != padding)
		{
			fclose(f);
			return -1;
		}
	}
	fclose(f);

	return 0;
}

