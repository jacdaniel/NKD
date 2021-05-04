#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _ND
#include <callback.h>
#endif
#include <poisson_fourier.h>
#include <fftw3.h>
#include <util.h>

//#undef PI
//#define PI         3.141592653589793

typedef struct _POISSON_FOURIER
{
	fftw_plan p1;
	fftw_plan p2;
	void* spatial_in;
	void* spatial_out;
	void* freq;

	double hx, hz;

	long transform;

	double* inv_lap;

	long width_poisson;
	long depth_poisson;
	long size_poisson;
	int threads_nbre;
}	POISSON_FOURIER;


//static double * inverse_laplacian_create(long x1, long x2, long z1, long z2, long dxt, long dzt, double lambda)
//{
//	long x, z, dx, dz;
//	double * filter = NULL;
//
//	dx = x2+1-x1;
//	dz = z2+1-z1;
//	filter = (double*)CALLOC(dx*dz, double);
//	for (x=x1; x<=x2; x++)
//		for (z=z1; z<=z2; z++)
//			if ( x == 0 && z == 0 )
//				filter[0] = 0.0;
//			else
//				filter[(z-z1)*dx+x-x1] = 1.0/(-4.0+2.0*(cos(2.0*PI*(double)x/(double)dxt)+cos(2*PI*(double)z/(double)dzt)-lambda));				
//	return filter;
//}

double* poisson_fourier_inverse_laplacian_create(long x1, long x2, long z1, long z2, long dxt, long dzt, double lambda, double mu)
{
	long x, z, dx, dz;
	double* filter = NULL, wx, wz, l1, l2, val;

	dx = x2 + 1 - x1;
	dz = z2 + 1 - z1;
	filter = (double*)CALLOC(dx * dz, double);
	for (x = x1; x <= x2; x++)
		for (z = z1; z <= z2; z++)
			if (x == 0 && z == 0)
				filter[0] = 0.0;
			else
			{
				wx = 2.0 * PI * (double)x / (double)dxt;
				wz = 2.0 * PI * (double)z / (double)dzt;
				l1 = -4.0 + 2.0 * cos(wx) + 2.0 * cos(wz);
				l2 = 20.0 - 16.0 * cos(wx) - 16.0 * cos(wz) + 8.0 * cos(wx) * cos(wz) + 2.0 * cos(2.0 * wx) + 2.0 * cos(2.0 * wz);
				val = l1 - mu * l2 - lambda;
				filter[(z - z1) * dx + x - x1] = 1.0 / val;
			}
	return filter;
}

// mu : controle le laplacien carre pour limiter la courbure
// lambda : coefficient du terme d'attache aux donn�es
void* poisson_fourier_init(void* poisson_fourier, long width, long depth, double hx, double hz, long transform, double lambda, double mu, long nbthreads, long no)
{
	POISSON_FOURIER* data = (POISSON_FOURIER*)poisson_fourier;
	long width_org = width, depth_org = depth;

	if (poisson_fourier == NULL)
		data = (POISSON_FOURIER*)CALLOC(1, POISSON_FOURIER);
	else
	{
		// test des tailles et maj
	}

	/*
	if ( width > 52 && depth > 52 )
	{
		fftw_init_threads();
		fftw_plan_with_nthreads(threads_nbre);
	}
	*/


	// TODO
	if (nbthreads > 1)
	{
		if (no == 0)
		{
			//			fftw_init_threads();			
		}
		//		fftw_plan_with_nthreads(nbthreads);
	}



	data->threads_nbre = nbthreads;
	data->hx = hx;
	data->hz = hz;
	data->transform = transform;
	if (transform == TRANSFORM_DCT1 || transform == TRANSFORM_DCTT1)
	{
		if (transform == TRANSFORM_DCT1)
		{
			if (width > 1) width = 2 * (width + 2);
			if (depth > 1) depth = 2 * (depth + 2);
		}
		else
		{
			if (width > 1) width = 2 * width;
			if (depth > 1) depth = 2 * depth;
		}
		data->spatial_in = fftw_malloc(sizeof(fftw_complex) * width * depth);
		data->freq = fftw_malloc(sizeof(fftw_complex) * width * depth);
		data->spatial_out = fftw_malloc(sizeof(fftw_complex) * width * depth);
		if (width_org == 1)
		{
			data->p1 = fftw_plan_dft_1d(depth, (fftw_complex*)data->spatial_in, (fftw_complex*)data->freq, FFTW_FORWARD, FFTW_ESTIMATE);
			data->p2 = fftw_plan_dft_1d(depth, (fftw_complex*)data->freq, (fftw_complex*)data->spatial_out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else if (depth_org == 1)
		{
			data->p1 = fftw_plan_dft_1d(width, (fftw_complex*)data->spatial_in, (fftw_complex*)data->freq, FFTW_FORWARD, FFTW_ESTIMATE);
			data->p2 = fftw_plan_dft_1d(width, (fftw_complex*)data->freq, (fftw_complex*)data->spatial_out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			data->p1 = fftw_plan_dft_2d(depth, width, (fftw_complex*)data->spatial_in, (fftw_complex*)data->freq, FFTW_FORWARD, FFTW_ESTIMATE);
			data->p2 = fftw_plan_dft_2d(depth, width, (fftw_complex*)data->freq, (fftw_complex*)data->spatial_out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		data->inv_lap = poisson_fourier_inverse_laplacian_create(0, width - 1, 0, depth - 1, width, depth, lambda, mu);
	}
	else if (transform == TRANSFORM_DCT2 || transform == TRANSFORM_DCTT2)
	{
		if (transform == TRANSFORM_DCT2)
		{
			if (width > 1) width += 2;
			if (depth > 1) depth += 2;
		}
		data->spatial_in = fftw_malloc(sizeof(double) * width * depth);
		data->freq = fftw_malloc(sizeof(double) * width * depth);
		data->spatial_out = fftw_malloc(sizeof(double) * width * depth);
		data->p1 = fftw_plan_r2r_2d(depth, width, (double*)data->spatial_in, (double*)data->freq, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE);
		data->p2 = fftw_plan_r2r_2d(depth, width, (double*)data->freq, (double*)data->spatial_out, FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);
		data->inv_lap = poisson_fourier_inverse_laplacian_create(0, width - 1, 0, depth - 1, width * 2, depth * 2, lambda, mu);
	}
	else if (transform == TRANSFORM_DST1)
	{
		if (width > 1) width = 2 * (width - 1);
		if (depth > 1) depth = 2 * (depth - 1);
		data->spatial_in = fftw_malloc(sizeof(fftw_complex) * width * depth);
		data->freq = fftw_malloc(sizeof(fftw_complex) * width * depth);
		data->spatial_out = fftw_malloc(sizeof(fftw_complex) * width * depth);
		if (width_org == 1)
		{
			data->p1 = fftw_plan_dft_1d(depth, (fftw_complex*)data->spatial_in, (fftw_complex*)data->freq, FFTW_FORWARD, FFTW_ESTIMATE);
			data->p2 = fftw_plan_dft_1d(depth, (fftw_complex*)data->freq, (fftw_complex*)data->spatial_out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else if (depth_org == 1)
		{
			data->p1 = fftw_plan_dft_1d(width, (fftw_complex*)data->spatial_in, (fftw_complex*)data->freq, FFTW_FORWARD, FFTW_ESTIMATE);
			data->p2 = fftw_plan_dft_1d(width, (fftw_complex*)data->freq, (fftw_complex*)data->spatial_out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		else
		{
			data->p1 = fftw_plan_dft_2d(depth, width, (fftw_complex*)data->spatial_in, (fftw_complex*)data->freq, FFTW_FORWARD, FFTW_ESTIMATE);
			data->p2 = fftw_plan_dft_2d(depth, width, (fftw_complex*)data->freq, (fftw_complex*)data->spatial_out, FFTW_BACKWARD, FFTW_ESTIMATE);
		}
		data->inv_lap = poisson_fourier_inverse_laplacian_create(0, width - 1, 0, depth - 1, width, depth, lambda, mu);
	}
	else
	{
		if (width > 1) width -= 2;
		if (depth > 1) depth -= 2;
		data->spatial_in = fftw_malloc(sizeof(fftw_complex) * width * depth);
		data->freq = fftw_malloc(sizeof(fftw_complex) * width * depth);
		data->spatial_out = fftw_malloc(sizeof(fftw_complex) * width * depth);
		if (width_org == 1)
		{
			data->p1 = fftw_plan_r2r_1d(depth, (double*)data->spatial_in, (double*)data->freq, FFTW_RODFT00, FFTW_ESTIMATE);
			data->p2 = fftw_plan_r2r_1d(depth, (double*)data->freq, (double*)data->spatial_out, FFTW_RODFT00, FFTW_ESTIMATE);
		}
		else if (depth_org == 1)
		{
			data->p1 = fftw_plan_r2r_1d(width, (double*)data->spatial_in, (double*)data->freq, FFTW_RODFT00, FFTW_ESTIMATE);
			data->p2 = fftw_plan_r2r_1d(width, (double*)data->freq, (double*)data->spatial_out, FFTW_RODFT00, FFTW_ESTIMATE);
		}
		else
		{
			data->p1 = fftw_plan_r2r_2d(depth, width, (double*)data->spatial_in, (double*)data->freq, FFTW_RODFT00, FFTW_RODFT00, FFTW_ESTIMATE);
			data->p2 = fftw_plan_r2r_2d(depth, width, (double*)data->freq, (double*)data->spatial_out, FFTW_RODFT00, FFTW_RODFT00, FFTW_ESTIMATE);
		}
		if (width_org == 1)
			data->inv_lap = poisson_fourier_inverse_laplacian_create(0, 0, 1, depth, 1, 2 * (depth + 2) - 2, lambda, mu);
		else if (depth_org == 1)
			data->inv_lap = poisson_fourier_inverse_laplacian_create(1, width, 0, 0, 2 * (width + 2) - 2, 1, lambda, mu);
		else
			data->inv_lap = poisson_fourier_inverse_laplacian_create(1, width, 1, depth, 2 * (width + 2) - 2, 2 * (depth + 2) - 2, lambda, mu);
	}
	data->width_poisson = width;
	data->depth_poisson = depth;
	data->size_poisson = width * depth;
	return (void*)data;
}


void* poisson_fourier_release(void* poisson_fourier)
{
	POISSON_FOURIER* data = (POISSON_FOURIER*)poisson_fourier;

	if (data == NULL) return NULL;
	fftw_destroy_plan(data->p1);
	fftw_destroy_plan(data->p2);
	if (data->spatial_in != NULL) fftw_free(data->spatial_in);
	if (data->freq != NULL) fftw_free(data->freq);
	if (data->spatial_out != NULL) fftw_free(data->spatial_out);
	FREE(data->inv_lap)
		FREE(data)
		/*fftw_cleanup_threads();*/
		return NULL;
}


static void dct2_fill_in(double* f, long width, long depth, void* in)
{
	long add, size = 1;
	double* img = (double*)in;

	if (width > 1) size *= width + 2;
	if (depth > 1) size *= depth + 2;
	for (add = 0; add < size; add++) img[add] = f[add];
}


static void dct1_fill_in(double* f, long width, long depth, void* in)
{
	long x, z, add, size = 1;
	double v;
	fftw_complex* spatial_in = (fftw_complex*)in;
	double* in_small = NULL;

	if (width > 1) size *= (width + 2) * 2;
	if (depth > 1) size *= (depth + 2) * 2;
	for (add = 0; add < size; add++) spatial_in[add][1] = 0.0;
	in_small = (double*)CALLOC(size, double); // volontairement trop grand
	dct2_fill_in(f, width, depth, (void*)in_small);
	if (width == 1)
		for (z = 0; z < depth + 2; z++)
			spatial_in[z][0] = spatial_in[2 * (depth + 2) - 1 - z][0] = in_small[z];
	else if (depth == 1)
		for (x = 0; x < width + 2; x++)
			spatial_in[x][0] = spatial_in[2 * (width + 2) - 1 - x][0] = in_small[x];
	else
		for (z = 0; z < depth + 2; z++)
			for (x = 0; x < width + 2; x++)
			{
				v = in_small[z * (width + 2) + x];
				spatial_in[z * 2 * (width + 2) + x][0] = v;																	//HG
				spatial_in[z * 2 * (width + 2) + 2 * (width + 2) - 1 - x][0] = v;										//HD
				spatial_in[(2 * (depth + 2) - 1 - z) * 2 * (width + 2) + x][0] = v;									//BG
				spatial_in[(2 * (depth + 2) - 1 - z) * 2 * (width + 2) + 2 * (width + 2) - 1 - x][0] = v;		//BD
			}
	FREE(in_small)
}


static void dst2_fill_in(double* f, long width, long depth, float* boundary, void* in)
{
	long x, z, add, size = 1;
	double v;
	double* img = (double*)in;

	if (width > 1) size *= width - 2;
	if (depth > 1) size *= depth - 2;
	for (add = 0; add < size; add++) img[add] = 0.0;
	if (width == 1)
		for (z = 1; z < depth - 1; z++)
		{
			v = f[z];
			if (boundary != NULL)
			{
				if (z == depth - 2) v -= (double)boundary[1];
			}
			img[z - 1] = v;
		}
	else if (depth == 1)
		for (x = 1; x < width - 1; x++)
		{
			v = f[x];
			if (boundary != NULL)
			{
				if (x == width - 2) v -= (double)boundary[1];
			}
			img[x - 1] = v;
		}
	else
		for (z = 1; z < depth - 1; z++)
			for (x = 1; x < width - 1; x++)
			{
				v = f[z * width + x];
				if (boundary != NULL)
				{
					if (z == 1)
					{
						v -= (double)boundary[x];
						if (x == 1)
							v -= (double)boundary[2 * width + 2 * depth - 5];
						if (x == width - 2)
							v -= (double)boundary[width];
					}
					else if (z == depth - 2)
					{
						v -= (double)boundary[2 * width + depth - 3 - x];
						if (x == 1)
							v -= (double)boundary[2 * width + depth - 2];
						if (x == width - 2)
							v -= (double)boundary[width + depth - 3];
					}
					else if (x == width - 2)
						v -= (double)boundary[width - 1 + z];
					else if (x == 1)
						v -= (double)boundary[2 * width + 2 * depth - 4 - z];
				}
				img[(z - 1) * (width - 2) + x - 1] = v;
			}
}



static void dst1_fill_in(double* f, long width, long depth, float* boundary, void* in)
{
	long x, z, add, size = 1;
	double v;
	fftw_complex* spatial_in = (fftw_complex*)in;
	double* in_small = NULL;

	if (width > 1) size *= (width - 1) * 2;
	if (depth > 1) size *= (depth - 1) * 2;
	for (add = 0; add < size; add++) spatial_in[add][0] = spatial_in[add][1] = 0.0;
	in_small = (double*)CALLOC(size, double); // volontairement trop grand
	dst2_fill_in(f, width, depth, boundary, (void*)in_small);
	if (width == 1)
		for (z = 1; z < depth - 1; z++)
		{
			v = in_small[z - 1];
			spatial_in[z][0] = v;
			spatial_in[(depth - 1) * 2 - z][0] = -v;
		}
	else if (depth == 1)
		for (x = 1; x < width - 1; x++)
		{
			v = in_small[x - 1];
			spatial_in[x][0] = v;
			spatial_in[(width - 1) * 2 - x][0] = -v;
		}
	else
		for (z = 1; z < depth - 1; z++)
			for (x = 1; x < width - 1; x++)
			{
				v = in_small[(z - 1) * (width - 2) + x - 1];
				spatial_in[z * (width - 1) * 2 + x][0] = v;																	//HG
				spatial_in[z * (width - 1) * 2 + (width - 1) * 2 - x][0] = -v;										//HD
				spatial_in[((depth - 1) * 2 - z) * (width - 1) * 2 + x][0] = -v;									//BG
				spatial_in[((depth - 1) * 2 - z) * (width - 1) * 2 + (width - 1) * 2 - x][0] = v;				//BD						
			}
	FREE(in_small)
}


static void dctt2_fill_in(double* f, long width, long depth, void* in)
{
	long add, size = 1;
	double* img = (double*)in;

	if (width > 1) size *= width;
	if (depth > 1) size *= depth;
	for (add = 0; add < size; add++) img[add] = f[add];
}


static void dctt1_fill_in(double* f, long width, long depth, void* in)
{
	long x, z, add, size = 1;
	double v;
	fftw_complex* spatial_in = (fftw_complex*)in;
	double* in_small = NULL;

	if (width > 1) size *= width * 2;
	if (depth > 1) size *= depth * 2;
	for (add = 0; add < size; add++) spatial_in[add][1] = 0.0;
	in_small = (double*)CALLOC(size, double); // volontairement trop grand
	dctt2_fill_in(f, width, depth, (void*)in_small);
	if (width == 1)
		for (z = 0; z < depth; z++)
			spatial_in[z][0] = spatial_in[2 * depth - 1 - z][0] = in_small[z];
	else if (depth == 1)
		for (x = 0; x < width; x++)
			spatial_in[x][0] = spatial_in[2 * width - 1 - x][0] = in_small[x];
	else
		for (z = 0; z < depth; z++)
			for (x = 0; x < width; x++)
			{
				v = in_small[z * width + x];
				spatial_in[z * 2 * width + x][0] = v;													//HG
				spatial_in[z * 2 * width + 2 * width - 1 - x][0] = v;								//HD
				spatial_in[(2 * depth - 1 - z) * 2 * width + x][0] = v;							//BG
				spatial_in[(2 * depth - 1 - z) * 2 * width + 2 * width - 1 - x][0] = v;		//BD
			}
	FREE(in_small)
}

void poisson_fourier_do(void* poisson_fourier,
	long width, long depth,
	double* f, double* u, float* boundary)
{
	long x, z, add, transform;
	POISSON_FOURIER* data = (POISSON_FOURIER*)poisson_fourier;

	transform = data->transform;
	if (transform == TRANSFORM_DCT1 || transform == TRANSFORM_DCT2)
	{
		if (transform == TRANSFORM_DCT1)
			dct1_fill_in(f, width, depth, data->spatial_in);
		else
			dct2_fill_in(f, width, depth, data->spatial_in);
	}
	else if (transform == TRANSFORM_DST1 || transform == TRANSFORM_DST2)
	{
		if (transform == TRANSFORM_DST1)
			dst1_fill_in(f, width, depth, boundary, data->spatial_in);
		else
			dst2_fill_in(f, width, depth, boundary, data->spatial_in);
	}
	else
	{
		if (transform == TRANSFORM_DCTT1)
			dctt1_fill_in(f, width, depth, data->spatial_in);
		else
			dctt2_fill_in(f, width, depth, data->spatial_in);
	}
	/*fftw_init_threads();*/ fftw_execute(data->p1);
	if (transform == TRANSFORM_DCT1 || transform == TRANSFORM_DST1 || transform == TRANSFORM_DCTT1)
	{
		fftw_complex* freq = (fftw_complex*)data->freq;
		for (add = 0; add < data->size_poisson; add++)
		{
			freq[add][0] *= data->inv_lap[add];
			freq[add][1] *= data->inv_lap[add];
		}
	}
	else if (transform == TRANSFORM_DCT2 || transform == TRANSFORM_DST2 || transform == TRANSFORM_DCTT2)
	{
		double* freq = (double*)data->freq;

		for (add = 0; add < data->size_poisson; add++)
			freq[add] *= data->inv_lap[add];
	}
	/*fftw_init_threads();*/ fftw_execute(data->p2);
	if (transform == TRANSFORM_DCT1)
	{
		fftw_complex* spatial_out = (fftw_complex*)data->spatial_out;

		if (width == 1)
			for (z = 0; z < depth; z++)
				u[z] = spatial_out[z + 1][0] / data->size_poisson;
		else if (depth == 1)
			for (x = 0; x < width; x++)
				u[x] = spatial_out[x + 1][0] / data->size_poisson;
		else
			for (z = 0; z < depth; z++)
				for (x = 0; x < width; x++)
					u[z * width + x] = spatial_out[(z + 1) * data->width_poisson + x + 1][0] / data->size_poisson;
	}
	else if (transform == TRANSFORM_DCT2)
	{
		double* spatial_out = (double*)data->spatial_out;

		if (width == 1)
			for (z = 0; z < depth; z++)
				u[z] = spatial_out[z + 1] / (4.0 * data->size_poisson);
		else if (depth == 1)
			for (x = 0; x < width; x++)
				u[x] = spatial_out[x + 1] / (4.0 * data->size_poisson);
		else
			for (z = 0; z < depth; z++)
				for (x = 0; x < width; x++)
					u[z * width + x] = spatial_out[(z + 1) * data->width_poisson + x + 1] / (4.0 * data->size_poisson);
	}
	else if (transform == TRANSFORM_DST1 || transform == TRANSFORM_DST2)
	{
		for (add = 0; add < width * depth; add++) u[add] = 0.0;

		if (transform == TRANSFORM_DST1)
		{
			fftw_complex* spatial_out = (fftw_complex*)data->spatial_out;

			if (width == 1)
				for (z = 1; z < depth - 1; z++)
					u[z] = spatial_out[z][0] / data->size_poisson;
			else if (depth == 1)
				for (x = 1; x < width - 1; x++)
					u[x] = spatial_out[x][0] / data->size_poisson;
			else
				for (z = 1; z < depth - 1; z++)
					for (x = 1; x < width - 1; x++)
						u[z * width + x] = spatial_out[z * data->width_poisson + x][0] / data->size_poisson;
		}
		else
		{
			double* spatial_out = (double*)data->spatial_out;

			if (width == 1)
				for (z = 1; z < depth - 1; z++)
					u[z] = spatial_out[z - 1] / (2.0 * data->size_poisson);
			else if (depth == 1)
				for (x = 1; x < width - 1; x++)
					u[x] = spatial_out[x - 1] / (2.0 * data->size_poisson);
			else
				for (z = 1; z < depth - 1; z++)
					for (x = 1; x < width - 1; x++)
						u[z * width + x] = spatial_out[(z - 1) * data->width_poisson + x - 1] / (4.0 * data->size_poisson);
		}
		if (width == 1)
		{
			if (boundary != NULL)
			{
				u[0] = (double)boundary[0];
				u[depth - 1] = (double)boundary[1];
			}
		}
		else if (depth == 1)
		{
			if (boundary != NULL)
			{
				u[0] = (double)boundary[0];
				u[width - 1] = (double)boundary[1];
			}
		}
		else
		{
			if (boundary != NULL)
			{
				for (x = 0; x < width; x++)
				{
					u[x] = (double)boundary[x];
					u[(depth - 1) * width + x] = (double)boundary[2 * width + depth - 3 - x];
				}
				for (z = 1; z < depth - 1; z++)
				{
					u[z * width] = (double)boundary[2 * width + 2 * depth - 4 - z];
					u[z * width + width - 1] = (double)boundary[width - 1 + z];
				}
			}
		}
	}
	else if (transform == TRANSFORM_DCTT1)
	{
		fftw_complex* spatial_out = (fftw_complex*)data->spatial_out;

		if (width == 1)
			for (z = 0; z < depth; z++)
				u[z] = spatial_out[z][0] / data->size_poisson;
		else if (depth == 1)
			for (x = 0; x < width; x++)
				u[x] = spatial_out[x][0] / data->size_poisson;
		else
			for (z = 0; z < depth; z++)
				for (x = 0; x < width; x++)
					u[z * width + x] = spatial_out[z * data->width_poisson + x][0] / data->size_poisson;
	}
	else
	{
		double* spatial_out = (double*)data->spatial_out;

		if (width == 1)
			for (z = 0; z < depth; z++)
				u[z] = spatial_out[z] / (4.0 * data->size_poisson);
		else if (depth == 1)
			for (x = 0; x < width; x++)
				u[x] = spatial_out[x] / (4.0 * data->size_poisson);
		else
			for (z = 0; z < depth; z++)
				for (x = 0; x < width; x++)
					u[z * width + x] = spatial_out[z * data->width_poisson + x] / (4.0 * data->size_poisson);
	}
}

void poisson_fourier_laplace_interpolation(long width, long depth, float* boundary, double* u)
{
	void* poisson_fourier = NULL;
	double* f = NULL;

	f = (double*)calloc(width * depth, sizeof(double));
	poisson_fourier = poisson_fourier_init(NULL, width, depth, 1.0, 1.0, TRANSFORM_DST2, 0.0, 0.0, 0, 0);
	poisson_fourier_do(poisson_fourier, width, depth, f, u, boundary);
	poisson_fourier = poisson_fourier_release(poisson_fourier);
	FREE(f)
}