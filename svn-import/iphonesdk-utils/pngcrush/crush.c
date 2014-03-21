#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include "png.h"
#define PNG_FLAG_ZLIB_CUSTOM_WINDOW_BITS  0x0008

#define RDERR(...) { debuglog(LEVEL_ERROR, infilename, __VA_ARGS__); goto out; }
#define WRERR(...) { debuglog(LEVEL_ERROR, inplace?infilename:outfilename, __VA_ARGS__); goto out; }
#define INFO(level, ...) { if(verbose) debuglog(level, infilename, __VA_ARGS__); }

#ifndef PATH_MAX
#include <linux/limits.h>
#endif

#include <getopt.h>

enum debuglevels {
	LEVEL_ERROR = -1,
	LEVEL_NONE = 0,
	LEVEL_INFO, LEVEL_RIDICULOUS, LEVEL_INSANE
};

static bool inplace = true;
static unsigned int verbose = 0;
static bool chunked = true;
static unsigned int global_num_rows = 8;

void debuglog(int level, char *filename, const char *format, ...) {
	if(level > verbose && level != LEVEL_ERROR) return;
	va_list args;
	va_start(args, format);
	char *out = (char *)malloc(PATH_MAX);
	vsprintf(out, format, args);
	if(!filename)
		fprintf(stderr, "%s", out);
	else
		fprintf(stderr, "%s: %s", filename, out);
	if(out[strlen(out)-1] != '\n')
		fprintf(stderr, "\n");
	free(out);
	va_end(args);
}

void png_warning_cb(png_structp ptr, png_const_charp msg) {
	char *filename = (char *)png_get_error_ptr(ptr);
	if(verbose)
		debuglog(LEVEL_INFO, filename, msg);
}

void copy(char *inf, char *outf) {
	FILE *in, *out;
	in = fopen(inf, "rb");
	out = fopen(outf, "wb");

	fseek(in, 0, SEEK_END);
	int len = ftell(in);
	rewind(in);

	char *filebuf = malloc(len);
	fread(filebuf, 1, len, in);
	fclose(in);
	fwrite(filebuf, 1, len, out);
	fclose(out);
	free(filebuf);
}

int unknown_chunk_read_cb(png_structp ptr, png_unknown_chunkp chunk) {
	//png_uint_32 *my_chunk_data = (png_uint_32*)png_get_user_chunk_ptr(ptr);
	if(!strncmp((char*)chunk->name, "CgBI", 4)) {
		fprintf(stderr, "Error: This file is already crushed and how the hell did you get here?\n");
		exit(1);
	}
	return 1;
}

void swap_and_premultiply_alpha_transform(png_structp ptr, png_row_infop row_info, png_bytep data) {
    unsigned int x = 0;
	for(x = 0; x < row_info->width * 4; x += 4) {
		png_byte r, g, b, a;
		r = data[x+0]; g = data[x+1]; b = data[x+2]; a = data[x+3];
		data[x+0] = (b*a) / 0xff;
		data[x+1] = (g*a) / 0xff;
		data[x+2] = (r*a) / 0xff;
	}
}

void crush(char *infilename, char *outfilename) {
    printf("crush PNG: %s\n", infilename);
	if(!outfilename) {
		outfilename = (char *)malloc(PATH_MAX);
		sprintf(outfilename, "%s.crush", infilename);
	}

	FILE *fp_in = NULL, *fp_out = NULL;

	{
		debuglog(LEVEL_INFO, NULL, "Crushing %s.\n", infilename);
		fp_in = fopen(infilename, "rb");
		if(!fp_in) {
			RDERR("Error: could not open.\n", infilename);
		}

		png_byte header[8];
		fread(header, 1, 8, fp_in);
		if(png_sig_cmp(header, 0, 8) != 0) {
			RDERR("Error: Not a valid PNG file.\n", infilename);
		}

		fseek(fp_in, 12, SEEK_SET);
		png_byte cgbi[4];
		fread(cgbi, 1, 4, fp_in);
		if(!strncmp((char*)cgbi, "CgBI", 4)) {
			fclose(fp_in); fp_in = NULL;
			INFO(LEVEL_INFO, "Warning: This file is already crushed. Doing nothing.\n");
			if(!inplace)
				copy(infilename, outfilename);
			goto out;
		}
		fseek(fp_in, 8, SEEK_SET);

		png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(!read_ptr) RDERR("Error: failed to init libpng for reading.\n");
		png_set_error_fn(read_ptr, (void*)infilename, NULL, png_warning_cb);

		png_infop read_info = png_create_info_struct(read_ptr);
		if(!read_info) {
			png_destroy_read_struct(&read_ptr, NULL, NULL);
			RDERR("Error: failed to init libpng for reading.\n");
		}

		png_infop read_end = png_create_info_struct(read_ptr);
		if(!read_end) {
			png_destroy_read_struct(&read_ptr, &read_info, NULL);
			RDERR("Error: failed to init libpng for reading.\n");
		}

		if(setjmp(png_jmpbuf(read_ptr))) {
			RDERR("Error: error reading PNG.\n");
			goto out;
		}

		png_init_io(read_ptr, fp_in);
		png_set_sig_bytes(read_ptr, 8); // Already read the signature.

		fp_out = fopen(outfilename, "wb");
		if(!fp_out) {
			WRERR("Error: could not open for writing.\n", outfilename);
		}
		png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(!write_ptr) WRERR("Error: failed to init libpng for writing.\n");
		png_set_error_fn(read_ptr, (void*)(inplace?infilename:outfilename), NULL, png_warning_cb);

		png_infop write_info = png_create_info_struct(write_ptr);
		if(!write_info) {
			png_destroy_write_struct(&write_ptr, NULL);
			WRERR("Error: failed to init libpng for writing.\n");
		}

		if(setjmp(png_jmpbuf(write_ptr))) {
			unlink(outfilename);
			WRERR("Error: error writing PNG.\n");
			goto out;
		}

		// If we don't write the header first, png_write_chunk (for our custom CgBI chunk) doesn't add a header.
		// png_write_sig() supposedly exists, even in the header file, but I can't for the life of me use it?
		fwrite(header, 1, 8, fp_out);
		png_init_io(write_ptr, fp_out);
		png_set_sig_bytes(write_ptr, 8);

		png_set_filter(write_ptr, 0, PNG_FILTER_NONE);

		// The default window size is 15 bits. Setting it to -15 causes zlib to discard the header and crc information.
		// This is critical to making a proper CgBI PNG
		png_set_compression_window_bits(write_ptr, -15);
        /*write_ptr->flags |= PNG_FLAG_ZLIB_CUSTOM_WINDOW_BITS;
        write_ptr->zlib_window_bits = -15;*/

		// Handle ALL unknown chunks.
		png_set_keep_unknown_chunks(read_ptr, PNG_HANDLE_CHUNK_ALWAYS, NULL, 0);
		png_set_read_user_chunk_fn(read_ptr, NULL, unknown_chunk_read_cb);

		png_read_info(read_ptr, read_info);

		png_uint_32 width, height;
		int color_type, bitdepth, filter_method, compression_type, interlace_type;
		int orig_color_type;
		double gamma = 0.45455;
		png_get_IHDR(read_ptr, read_info, &width, &height, &bitdepth, &color_type, &interlace_type, &compression_type, &filter_method);
		orig_color_type = color_type;

		if(color_type == PNG_COLOR_TYPE_PALETTE) {
			INFO(LEVEL_INFO, "Converting Palette to RGB\n");
			png_set_palette_to_rgb(read_ptr);
		} else if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
			if(bitdepth < 8)
				png_set_expand_gray_1_2_4_to_8(read_ptr);
			INFO(LEVEL_INFO, "Converting Greyscale image to RGB\n");
			png_set_gray_to_rgb(read_ptr);
		}
		if(png_get_valid(read_ptr, read_info, PNG_INFO_tRNS)) {
			INFO(LEVEL_INFO, "Converting tRNS chunk to Alpha values\n");
			png_set_tRNS_to_alpha(read_ptr);
		}
		else if(!(color_type & PNG_COLOR_MASK_ALPHA)) {
			// Expand, adding an opaque alpha channel.
			INFO(LEVEL_INFO, "Adding opaque alpha channel.\n");
			png_set_add_alpha(read_ptr, 0xff, PNG_FILLER_AFTER);
		}
		if(bitdepth == 16) {
			INFO(LEVEL_INFO, "Stripping 16-bit samples.\n");
			png_set_strip_16(read_ptr);
		}

		png_set_read_user_transform_fn(read_ptr, swap_and_premultiply_alpha_transform);
		png_read_update_info(read_ptr, read_info);
		png_set_user_transform_info(read_ptr, NULL, 8, 4);
		if(png_get_valid(read_ptr, read_info, PNG_INFO_gAMA)) png_get_gAMA(read_ptr, read_info, &gamma);
		if(png_get_valid(read_ptr, read_info, PNG_INFO_cHRM)) {
			double wx,wy,rx,ry,gx,gy,bx,by;
			png_get_cHRM(read_ptr, read_info, &wx, &wy, &rx, &ry, &gx, &gy, &bx, &by);
			INFO(LEVEL_RIDICULOUS, "cHRM (chromaticities): %f, %f, %f, %f, %f, %f, %f, %f\n", wx,wy,rx,ry,gx,gy,bx,by);
		}
		if(png_get_valid(read_ptr, read_info, PNG_INFO_sRGB)) {
			int intent;
			png_get_sRGB(read_ptr, read_info, &intent);
			INFO(LEVEL_RIDICULOUS, "sRGB intent: %d\n", intent);
		}
		if(png_get_valid(read_ptr, read_info, PNG_INFO_pHYs)) {
			png_uint_32 res_x, res_y;
			int unit_type;
			png_get_pHYs(read_ptr, read_info, &res_x, &res_y, &unit_type);
			INFO(LEVEL_RIDICULOUS, "pHYs info: xres %u yres %u unit type %d\n", res_x, res_y, unit_type);
		}

		// re-read the info
		png_get_IHDR(read_ptr, read_info, &width, &height, &bitdepth, &color_type, &interlace_type, &compression_type, &filter_method);
		INFO(LEVEL_INFO, "Dimensions: %ux%u.\n", height, width);
		if(interlace_type != 0)
			INFO(LEVEL_INFO, "Interlaced. Overriding chunked mode.\n");
		//INFO("Bit Depth: %d\n", bitdepth);
		//INFO("Colour Mode: %d\n", color_type);

		png_set_IHDR(write_ptr, write_info, width, height, bitdepth, color_type, interlace_type, compression_type, filter_method);

		// Standard Gamma
		png_set_gAMA(write_ptr, write_info, 0.45455);

		// Primary Chromaticities white_xy, red_xy, blue_xy, green_xy, in that order.
		png_set_cHRM(write_ptr, write_info, 0.312700, 0.329000, 0.640000, 0.330000, 0.300000, 0.600000, 0.150000, 0.060000);

		// Apple's PNGs have an sRGB intent of 0.
		png_set_sRGB(write_ptr, write_info, 0);
		// CgBI = 0x50 00 20 06

		//png_unknown_chunkp cgbi = (png_unknown_chunkp)png_malloc(write_ptr, sizeof(png_unknown_chunk));
		png_byte cname[] = {'C', 'g', 'B', 'I', '\0'};
		png_byte cdata[] = {0x50, 0x00, 0x20, 0x06};
		if(orig_color_type & PNG_COLOR_MASK_ALPHA || orig_color_type == PNG_COLOR_TYPE_PALETTE) {
			// I'm not sure, but if the input colortype is alpha anything, CgBI[3] is 0x02 instead of 0x06.
			// Strange, because 0x06 means RGBA and 0x02 does not.
			// But, Mimick this behaviour. Otherwise, our alpha channel is ignored.
			cdata[3] = 0x02;
		}
		png_write_chunk(write_ptr, cname, cdata, 4);
		png_write_info(write_ptr, write_info);

		int rowbytes = png_get_rowbytes(read_ptr, read_info); // We're always outputting 4bpp.
		INFO(LEVEL_RIDICULOUS, "There are %d bytes per row. Don't let anybody tell you otherwise.\n", rowbytes);
		if(interlace_type != 0 || !chunked) {
			INFO(LEVEL_INSANE, "Reading whole image into memory: %d bytes.\n", rowbytes*height);
			png_bytep read_data = (png_bytep)malloc(rowbytes * height);
			png_bytep read_rows[height];
			int bpr = png_get_rowbytes(read_ptr, read_info);
            unsigned int i = 0;
			for(i = 0; i < height; i++) {
					read_rows[i] = read_data + i*bpr;
			}
			png_read_image(read_ptr, read_rows);
			png_write_image(write_ptr, read_rows);
			free(read_data);
		} else {
			int number_of_passes = 1;
			if(interlace_type == PNG_INTERLACE_ADAM7) {
				number_of_passes = png_set_interlace_handling(read_ptr);
				png_set_interlace_handling(write_ptr);
			}
			int remaining_rows = height;
			unsigned int numrows = remaining_rows > global_num_rows ? global_num_rows : remaining_rows;
			INFO(LEVEL_INSANE, "Reading image into memory %d rows at a time: %d bytes.\n", numrows, rowbytes*numrows);
			png_bytep row = png_malloc(read_ptr, rowbytes * numrows);
			png_bytep rowpointers[numrows];
            unsigned int i = 0;
			for(i = 0; i < numrows; i++)
				rowpointers[i] = row+(i*rowbytes);
            int pass = 0;
			for(pass = 0; pass < number_of_passes; pass++) {
				while(remaining_rows > 0) {
					png_read_rows(read_ptr, rowpointers, NULL, numrows);
					png_write_rows(write_ptr, rowpointers, numrows);
					remaining_rows -= numrows;
					numrows = remaining_rows > global_num_rows ? global_num_rows : remaining_rows;
				}
			}
			png_free(read_ptr, row);
		}

		png_read_end(read_ptr, read_end);
		png_write_end(write_ptr, write_info);
		fclose(fp_in); fp_in = NULL;
		fclose(fp_out); fp_out = NULL;
		png_destroy_read_struct(&read_ptr, &read_info, &read_end);
		png_destroy_write_struct(&write_ptr, &write_info);

		if(inplace) {
			unlink(infilename);
			rename(outfilename, infilename);
		}
	}

out:
	if(fp_in) fclose(fp_in);
	if(fp_out) fclose(fp_out);
	free(outfilename);
}

#if 0
void usage(char *argv0) {
	printf("pincrush\n");
	printf("Syntax: %s [-v] [-c#] -i <infile> [infile ...]\n", argv0);
	printf("        %s [-v] [-c#] <infile> <outfile>\n\n", argv0);
	printf("  -i	In-place mode. One of -i or outfile is required.\n");
	printf("  -v	Verbose mode. More 'v's = more verbose! Up to three. Five is right out.\n");
	printf("  -c#	Process # rows at a time. The default is 8.\n");
	printf("     	Use 0 to disable chunk mode (uses more memory, reads the entire image into memory).\n");
	printf("  -h	Display this help text.\n\n");
	printf("pincrush comes with no warranty, neither express nor implied. If it blows up\nyour themes, eats your children, or causes you bodily harm,\nall you'll get is a little pity and maybe an apology.\n");
	printf("Please report any pincrush bugs to <dustin@howett.net>, possibly including\na copy of the offending PNG file.\n");
}

int main(int argc, char **argv, char **envp) {
	char *argv0 = argv[0];
	char optflag;
	while((optflag = getopt(argc, argv, "ivhc:")) != -1) {
		switch(optflag) {
			case 'i':
				inplace = true;
				break;
			case 'v':
				verbose++;
				break;
			case 'c':
				chunked = true;
				global_num_rows = (unsigned int)strtoul(optarg, NULL, 0);
				if(global_num_rows == 0)
					chunked = false;
				break;
			case '?':
			case 'h':
			default:
				usage(argv0);
				exit(1);
		}
	}
	argc -= optind;
	argv += optind;

	if(argc < 1 || (!inplace && argc < 2)) {
		usage(argv0);
		exit(1);
	}

	if(inplace) {
        int i = 0;
		for(i = 0; i < argc; i++)
			crush(argv[i], NULL);
	} else
		crush(argv[0], strdup(argv[1]));

	return 0;
}
#endif
