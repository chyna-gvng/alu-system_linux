#include "multithreading.h"

/**
 * blur_portion - program that applies a Gaussian blur to a specified portion
 * of an image
 * this function iterates over the specified portion of the image;
 * for each pixel in this region, it applies a Gaussian blur using the provided
 * convolution kernel;
 * the blur is achieved by computing a weighted average of the pixel's
 * color value and the color values of its neighboring pixels;
 * the weights are determined by the convolution kernel;
 * this process modifies the color values of the pixels in the specified
 * portion of the image, resulting in a blurred effect;
 * the function handles boundary conditions and ensures that pixel
 * manipulations stay within the bounds of the image;
 * the resultant blurred image is stored in img_blur
 * @portion: a pointer to a 'blur_portion_t' structure.
 *           This structure includes all necessary information:
 *           - img: a pointer to the original image (img_t)
 *           - img_blur: a pointer to the destination image for storing
 *                       blurred results (img_t)
 *           - x, y: the top-left coordinates of the portion of the image
 *                   to be blurred
 *           - w, h: the width and height of the portion to be blurred
 *           - kernel: a pointer to the convolution kernel to be used
 *                     for blurring (kernel_t)
 * Return: nothing (void)
 */

void blur_portion(blur_portion_t const *portion)
{
	size_t end_x = portion->x + portion->w, end_y = portion->y + portion->h;
	size_t x, y;
	float weight;
	size_t kx, ky;
	int pixel_x, pixel_y, adjusted_kx, adjusted_ky;
	pixel_t current_pixel, *blurred_pixel;

	for (y = portion->y; y < end_y; y++)
	{
		for (x = portion->x; x < end_x; x++)
		{
			float totalR = 0, totalG = 0, totalB = 0, totalWeight = 0;

			for (ky = 0; ky < portion->kernel->size; ky++)
			{
				adjusted_ky = ky - portion->kernel->size / 2;
				for (kx = 0; kx < portion->kernel->size; kx++)
				{
					adjusted_kx = kx - portion->kernel->size / 2;
					pixel_x = x + adjusted_kx;
					pixel_y = y + adjusted_ky;
					if (pixel_x >= 0 && pixel_x < (int)portion->img->w &&
					pixel_y >= 0 && pixel_y < (int)portion->img->h)
					{
						current_pixel = portion->img->pixels
							[pixel_y * portion->img->w + pixel_x];
						weight = portion->kernel->matrix[ky][kx];
						totalR += current_pixel.r * weight;
						totalG += current_pixel.g * weight;
						totalB += current_pixel.b * weight;
						totalWeight += weight;
					}
				}
			}
			blurred_pixel = &portion->img_blur->pixels[y * portion->img_blur->w + x];
			blurred_pixel->r = totalR / totalWeight;
			blurred_pixel->g = totalG / totalWeight;
			blurred_pixel->b = totalB / totalWeight;
		}
	}
}
