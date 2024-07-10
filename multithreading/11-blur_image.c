#include "multithreading.h"

#define THREAD_POOL_SZ 10

/**
 * blurPortionThreadEntry - thread entry program to blur a portion
 * of an image
 * this function exits the thread if the input portion is NULL
 * @portion: a pointer to a blur_portion_t structure containing image
 *           data to blur
 * Return: NULL after completing the blurring process or on exit
 */

void *blurPortionThreadEntry(blur_portion_t *portion)
{
	if (!portion)
		pthread_exit(NULL);

	blur_portion(portion);

	pthread_exit(NULL);
}

/**
 * portionImage - program that divides an image into multiple portions
 * for parallel processing
 * each portion covers a part of the image to be processed separately
 * in multithreading
 * @img_blur: the output image structure where the blurred image
 *            will be stored
 * @img: the input image structure to be blurred
 * @kernel: the convolution kernel used for blurring
 * @portion_ct: the number of portions to divide the image into
 * Return: a pointer to an array of blur_portion_t structures,
 *         each representing a portion of the image
 */

blur_portion_t *portionImage(img_t *img_blur, const img_t *img,
			     const kernel_t *kernel, size_t portion_ct)
{
	blur_portion_t *portions = NULL;
	size_t i, extra_rows, next_y;

	if (!img_blur || !img || !kernel ||
	    !img_blur->pixels || !img->pixels || !kernel->matrix)
		return (NULL);

	portions = malloc(sizeof(blur_portion_t) * portion_ct);

	if (!portions)
		return (NULL);

	for (i = 0, next_y = 0, extra_rows = img->h % portion_ct;
	     i < portion_ct; i++)
	{
		portions[i].img = img;
		portions[i].img_blur = img_blur;
		portions[i].x = 0;
		portions[i].y = next_y;
		portions[i].w = img->w;
		portions[i].h = img->h / portion_ct;

		if (extra_rows)
		{
			(portions[i].h)++;
			extra_rows--;
		}

		portions[i].kernel = kernel;

		next_y += portions[i].h;
	}

	return (portions);
}

/**
 * blur_image - program that blurs an entire image using multithreading
 * by dividing the image into smaller portions
 * the function uses a thread pool to process each portion concurrently
 * @img_blur: a pointer to the output image data structure
 * @img: a pointer to the input image data structure
 * @kernel: a pointer to the convolution kernel used for blurring
 * Return: nothing (void)
 */

void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	pthread_t t_ids[THREAD_POOL_SZ] = {0};
	blur_portion_t *portions = NULL;
	pthread_attr_t attr;
	size_t i, j;

	if (!img_blur || !img || !kernel ||
	    !img_blur->pixels || !img->pixels || !kernel->matrix)
		return;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	portions = portionImage(img_blur, img, kernel, THREAD_POOL_SZ);

	if (!portions)
	{
		fprintf(stderr, "blur_image: portionImage failed\n");
		return;
	}
	for (i = 0; i < THREAD_POOL_SZ; i++)
	{
		if (pthread_create(t_ids + i, &attr,
				   (void *(*)(void *))blurPortionThreadEntry,
				   portions + i) != 0)
		{
			fprintf(stderr, "blur_image: pthread_create failed\n");
			for (j = 0; j < i; j++)
				pthread_kill(t_ids[j], SIGTERM);
			free(portions);
			return;
		}
	}
	pthread_attr_destroy(&attr);

	for (i = 0; i < THREAD_POOL_SZ; i++)
	{
		if (pthread_join(t_ids[i], NULL) != 0)
		{
			fprintf(stderr, "blur_image: pthread_join failed\n");
			free(portions);
			return;
		}
	}
	free(portions);
}
