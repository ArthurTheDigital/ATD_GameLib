/**
 * @file      
 * @brief     Wrap around OpenGL types, constants and functions.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

#pragma once

#include <stddef.h>


namespace ATD {

class Gl
{
public:
	/* Types: */

	typedef unsigned int   Enum;
	typedef unsigned char  Boolean;
	typedef unsigned int   Bitfield;
	typedef void           Void;
	typedef signed char    Byte;
	typedef short          Short;
	typedef int            Int;
	typedef unsigned char  Ubyte;
	typedef unsigned short Ushort;
	typedef unsigned int   Uint;
	typedef int            Sizei;
	typedef float          Float;
	typedef float          Clampf;
	typedef double         Double;
	typedef double         Clampd;

	typedef ptrdiff_t      Sizeiptr;
	typedef ptrdiff_t      Intptr;
	typedef char           Char;

	/* Constants: */

	/* Boolean values: */
	static const Boolean FALSE;
	static const Boolean TRUE;

	/* Data types: */
	static const Enum BYTE;
	static const Enum UNSIGNED_BYTE;
	static const Enum SHORT;
	static const Enum UNSIGNED_SHORT;
	static const Enum INT;
	static const Enum UNSIGNED_INT;
	static const Enum FLOAT;
	static const Enum _2_BYTES;
	static const Enum _3_BYTES;
	static const Enum _4_BYTES;
	static const Enum DOUBLE;

	/* Primitives: */
	static const Enum POINTS;
	static const Enum LINES;
	static const Enum LINE_LOOP;
	static const Enum LINE_STRIP;
	static const Enum TRIANGLES;
	static const Enum TRIANGLE_STRIP;
	static const Enum TRIANGLE_FAN;
	static const Enum QUADS;
	static const Enum QUAD_STRIP;
	static const Enum POLYGON;

	/* Vertex arrays: */
	static const Enum VERTEX_ARRAY;
	static const Enum NORMAL_ARRAY;
	static const Enum COLOR_ARRAY;
	static const Enum TEXTURE_COORD_ARRAY;
	static const Enum EDGE_FLAG_ARRAY;
	static const Enum VERTEX_ARRAY_SIZE;
	static const Enum VERTEX_ARRAY_TYPE;
	static const Enum VERTEX_ARRAY_STRIDE;
	static const Enum NORMAL_ARRAY_TYPE;
	static const Enum NORMAL_ARRAY_STRIDE;
	static const Enum COLOR_ARRAY_SIZE;
	static const Enum COLOR_ARRAY_TYPE;
	static const Enum COLOR_ARRAY_STRIDE;
	static const Enum INDEX_ARRAY_TYPE;
	static const Enum INDEX_ARRAY_STRIDE;
	static const Enum TEXTURE_COORD_ARRAY_SIZE;
	static const Enum TEXTURE_COORD_ARRAY_TYPE;
	static const Enum TEXTURE_COORD_ARRAY_STRIDE;
	static const Enum EDGE_FLAG_ARRAY_STRIDE;
	static const Enum VERTEX_ARRAY_POINTER;
	static const Enum NORMAL_ARRAY_POINTER;
	static const Enum COLOR_ARRAY_POINTER;
	static const Enum TEXTURE_COORD_ARRAY_POINTER;
	static const Enum EDGE_FLAG_ARRAY_POINTER;
	static const Enum V2F;
	static const Enum V3F;
	static const Enum C4UB_V2F;
	static const Enum C4UB_V3F;
	static const Enum C3F_V3F;
	static const Enum N3F_V3F;
	static const Enum C4F_N3F_V3F;
	static const Enum T2F_V3F;
	static const Enum T4F_V4F;
	static const Enum T2F_C4UB_V3F;
	static const Enum T2F_C3F_V3F;
	static const Enum T2F_N3F_V3F;
	static const Enum T2F_C4F_N3F_V3F;
	static const Enum T4F_C4F_N3F_V4F;

	/* Matrix Mode */
	static const Enum MATRIX_MODE;
	static const Enum MODELVIEW;
	static const Enum PROJECTION;
	static const Enum TEXTURE;

	/* Points */
	static const Enum POINT_SMOOTH;
	static const Enum POINT_SIZE;
	static const Enum POINT_SIZE_GRANULARITY;
	static const Enum POINT_SIZE_RANGE;

	/* Lines */
	static const Enum LINE_SMOOTH;
	static const Enum LINE_STIPPLE;
	static const Enum LINE_STIPPLE_PATTERN;
	static const Enum LINE_STIPPLE_REPEAT;
	static const Enum LINE_WIDTH;
	static const Enum LINE_WIDTH_GRANULARITY;
	static const Enum LINE_WIDTH_RANGE;

	/* Polygons */
	static const Enum POINT;
	static const Enum LINE;
	static const Enum FILL;
	static const Enum CW;
	static const Enum CCW;
	static const Enum FRONT;
	static const Enum BACK;
	static const Enum POLYGON_MODE;
	static const Enum POLYGON_SMOOTH;
	static const Enum POLYGON_STIPPLE;
	static const Enum EDGE_FLAG;
	static const Enum CULL_FACE;
	static const Enum CULL_FACE_MODE;
	static const Enum FRONT_FACE;
	static const Enum POLYGON_OFFSET_FACTOR;
	static const Enum POLYGON_OFFSET_UNITS;
	static const Enum POLYGON_OFFSET_POINT;
	static const Enum POLYGON_OFFSET_LINE;
	static const Enum POLYGON_OFFSET_FILL;

	/* Display Lists */
	static const Enum COMPILE;
	static const Enum COMPILE_AND_EXECUTE;
	static const Enum LIST_BASE;
	static const Enum LIST_INDEX;
	static const Enum LIST_MODE;

	/* Depth buffer */
	static const Enum NEVER;
	static const Enum LESS;
	static const Enum EQUAL;
	static const Enum LEQUAL;
	static const Enum GREATER;
	static const Enum NOTEQUAL;
	static const Enum GEQUAL;
	static const Enum ALWAYS;
	static const Enum DEPTH_TEST;
	static const Enum DEPTH_BITS;
	static const Enum DEPTH_CLEAR_VALUE;
	static const Enum DEPTH_FUNC;
	static const Enum DEPTH_RANGE;
	static const Enum DEPTH_WRITEMASK;
	static const Enum DEPTH_COMPONENT;

	/* Lighting */
	static const Enum LIGHTING;
	static const Enum LIGHT0;
	static const Enum LIGHT1;
	static const Enum LIGHT2;
	static const Enum LIGHT3;
	static const Enum LIGHT4;
	static const Enum LIGHT5;
	static const Enum LIGHT6;
	static const Enum LIGHT7;
	static const Enum SPOT_EXPONENT;
	static const Enum SPOT_CUTOFF;
	static const Enum CONSTANT_ATTENUATION;
	static const Enum LINEAR_ATTENUATION;
	static const Enum QUADRATIC_ATTENUATION;
	static const Enum AMBIENT;
	static const Enum DIFFUSE;
	static const Enum SPECULAR;
	static const Enum SHININESS;
	static const Enum EMISSION;
	static const Enum POSITION;
	static const Enum SPOT_DIRECTION;
	static const Enum AMBIENT_AND_DIFFUSE;
	static const Enum COLOR_INDEXES;
	static const Enum LIGHT_MODEL_TWO_SIDE;
	static const Enum LIGHT_MODEL_LOCAL_VIEWER;
	static const Enum LIGHT_MODEL_AMBIENT;
	static const Enum FRONT_AND_BACK;
	static const Enum SHADE_MODEL;
	static const Enum FLAT;
	static const Enum SMOOTH;
	static const Enum COLOR_MATERIAL;
	static const Enum COLOR_MATERIAL_FACE;
	static const Enum COLOR_MATERIAL_PARAMETER;
	static const Enum NORMALIZE;

	/* User clipping planes */
	static const Enum CLIP_PLANE0;
	static const Enum CLIP_PLANE1;
	static const Enum CLIP_PLANE2;
	static const Enum CLIP_PLANE3;
	static const Enum CLIP_PLANE4;
	static const Enum CLIP_PLANE5;

	/* Accumulation buffer */
	static const Enum ACCUM_RED_BITS;
	static const Enum ACCUM_GREEN_BITS;
	static const Enum ACCUM_BLUE_BITS;
	static const Enum ACCUM_ALPHA_BITS;
	static const Enum ACCUM_CLEAR_VALUE;
	static const Enum ACCUM;
	static const Enum ADD;
	static const Enum LOAD;
	static const Enum MULT;
	static const Enum RETURN;

	/* Alpha testing */
	static const Enum ALPHA_TEST;
	static const Enum ALPHA_TEST_REF;
	static const Enum ALPHA_TEST_FUNC;

	/* Blending */
	static const Enum BLEND;
	static const Enum BLEND_SRC;
	static const Enum BLEND_DST;
	static const Enum ZERO;
	static const Enum ONE;
	static const Enum SRC_COLOR;
	static const Enum ONE_MINUS_SRC_COLOR;
	static const Enum SRC_ALPHA;
	static const Enum ONE_MINUS_SRC_ALPHA;
	static const Enum DST_ALPHA;
	static const Enum ONE_MINUS_DST_ALPHA;
	static const Enum DST_COLOR;
	static const Enum ONE_MINUS_DST_COLOR;
	static const Enum SRC_ALPHA_SATURATE;

	/* Render Mode */
	static const Enum FEEDBACK;
	static const Enum RENDER;
	static const Enum SELECT;

	/* Feedback */
	static const Enum _2D;
	static const Enum _3D;
	static const Enum _3D_COLOR;
	static const Enum _3D_COLOR_TEXTURE;
	static const Enum _4D_COLOR_TEXTURE;
	static const Enum POINT_TOKEN;
	static const Enum LINE_TOKEN;
	static const Enum LINE_RESET_TOKEN;
	static const Enum POLYGON_TOKEN;
	static const Enum BITMAP_TOKEN;
	static const Enum DRAW_PIXEL_TOKEN;
	static const Enum COPY_PIXEL_TOKEN;
	static const Enum PASS_THROUGH_TOKEN;
	static const Enum FEEDBACK_BUFFER_POINTER;
	static const Enum FEEDBACK_BUFFER_SIZE;
	static const Enum FEEDBACK_BUFFER_TYPE;

	/* Selection */
	static const Enum SELECTION_BUFFER_POINTER;
	static const Enum SELECTION_BUFFER_SIZE;

	/* Fog */
	static const Enum FOG;
	static const Enum FOG_MODE;
	static const Enum FOG_DENSITY;
	static const Enum FOG_COLOR;
	static const Enum FOG_INDEX;
	static const Enum FOG_START;
	static const Enum FOG_END;
	static const Enum LINEAR;
	static const Enum EXP;
	static const Enum EXP2;

	/* Logic Ops */
	static const Enum LOGIC_OP;
	static const Enum INDEX_LOGIC_OP;
	static const Enum COLOR_LOGIC_OP;
	static const Enum LOGIC_OP_MODE;
	static const Enum CLEAR;
	static const Enum SET;
	static const Enum COPY;
	static const Enum COPY_INVERTED;
	static const Enum NOOP;
	static const Enum INVERT;
	static const Enum AND;
	static const Enum NAND;
	static const Enum OR;
	static const Enum NOR;
	static const Enum XOR;
	static const Enum EQUIV;
	static const Enum AND_REVERSE;
	static const Enum AND_INVERTED;
	static const Enum OR_REVERSE;
	static const Enum OR_INVERTED;

	/* Stencil */
	static const Enum STENCIL_BITS;
	static const Enum STENCIL_TEST;
	static const Enum STENCIL_CLEAR_VALUE;
	static const Enum STENCIL_FUNC;
	static const Enum STENCIL_VALUE_MASK;
	static const Enum STENCIL_FAIL;
	static const Enum STENCIL_PASS_DEPTH_FAIL;
	static const Enum STENCIL_PASS_DEPTH_PASS;
	static const Enum STENCIL_REF;
	static const Enum STENCIL_WRITEMASK;
	static const Enum STENCIL_INDEX;
	static const Enum KEEP;
	static const Enum REPLACE;
	static const Enum INCR;
	static const Enum DECR;

	/* Buffers, Pixel Drawing/Reading */
	static const Enum NONE;
	static const Enum LEFT;
	static const Enum RIGHT;
	/* FRONT; */
	/* BACK; */
	/* FRONT_AND_BACK; */
	static const Enum FRONT_LEFT;
	static const Enum FRONT_RIGHT;
	static const Enum BACK_LEFT;
	static const Enum BACK_RIGHT;
	static const Enum AUX0;
	static const Enum AUX1;
	static const Enum AUX2;
	static const Enum AUX3;
	static const Enum COLOR_INDEX;
	static const Enum RED;
	static const Enum GREEN;
	static const Enum BLUE;
	static const Enum ALPHA;
	static const Enum LUMINANCE;
	static const Enum LUMINANCE_ALPHA;
	static const Enum ALPHA_BITS;
	static const Enum RED_BITS;
	static const Enum GREEN_BITS;
	static const Enum BLUE_BITS;
	static const Enum INDEX_BITS;
	static const Enum SUBPIXEL_BITS;
	static const Enum AUX_BUFFERS;
	static const Enum READ_BUFFER;
	static const Enum DRAW_BUFFER;
	static const Enum DOUBLEBUFFER;
	static const Enum STEREO;
	static const Enum BITMAP;
	static const Enum COLOR;
	static const Enum DEPTH;
	static const Enum STENCIL;
	static const Enum DITHER;
	static const Enum RGB;
	static const Enum RGBA;

	/* Implementation limits */
	static const Enum MAX_LIST_NESTING;
	static const Enum MAX_EVAL_ORDER;
	static const Enum MAX_LIGHTS;
	static const Enum MAX_CLIP_PLANES;
	static const Enum MAX_TEXTURE_SIZE;
	static const Enum MAX_PIXEL_MAP_TABLE;
	static const Enum MAX_ATTRIB_STACK_DEPTH;
	static const Enum MAX_MODELVIEW_STACK_DEPTH;
	static const Enum MAX_NAME_STACK_DEPTH;
	static const Enum MAX_PROJECTION_STACK_DEPTH;
	static const Enum MAX_TEXTURE_STACK_DEPTH;
	static const Enum MAX_VIEWPORT_DIMS;
	static const Enum MAX_CLIENT_ATTRIB_STACK_DEPTH;

	/* Gets */
	static const Enum ATTRIB_STACK_DEPTH;
	static const Enum CLIENT_ATTRIB_STACK_DEPTH;
	static const Enum COLOR_CLEAR_VALUE;
	static const Enum COLOR_WRITEMASK;
	static const Enum CURRENT_INDEX;
	static const Enum CURRENT_COLOR;
	static const Enum CURRENT_NORMAL;
	static const Enum CURRENT_RASTER_COLOR;
	static const Enum CURRENT_RASTER_DISTANCE;
	static const Enum CURRENT_RASTER_INDEX;
	static const Enum CURRENT_RASTER_POSITION;
	static const Enum CURRENT_RASTER_TEXTURE_COORDS;
	static const Enum CURRENT_RASTER_POSITION_VALID;
	static const Enum CURRENT_TEXTURE_COORDS;
	static const Enum INDEX_CLEAR_VALUE;
	static const Enum INDEX_MODE;
	static const Enum INDEX_WRITEMASK;
	static const Enum MODELVIEW_MATRIX;
	static const Enum MODELVIEW_STACK_DEPTH;
	static const Enum NAME_STACK_DEPTH;
	static const Enum PROJECTION_MATRIX;
	static const Enum PROJECTION_STACK_DEPTH;
	static const Enum RENDER_MODE;
	static const Enum RGBA_MODE;
	static const Enum TEXTURE_MATRIX;
	static const Enum TEXTURE_STACK_DEPTH;
	static const Enum VIEWPORT;

	/* Evaluators */
	static const Enum AUTO_NORMAL;
	static const Enum MAP1_COLOR_4;
	static const Enum MAP1_INDEX;
	static const Enum MAP1_NORMAL;
	static const Enum MAP1_TEXTURE_COORD_1;
	static const Enum MAP1_TEXTURE_COORD_2;
	static const Enum MAP1_TEXTURE_COORD_3;
	static const Enum MAP1_TEXTURE_COORD_4;
	static const Enum MAP1_VERTEX_3;
	static const Enum MAP1_VERTEX_4;
	static const Enum MAP2_COLOR_4;
	static const Enum MAP2_INDEX;
	static const Enum MAP2_NORMAL;
	static const Enum MAP2_TEXTURE_COORD_1;
	static const Enum MAP2_TEXTURE_COORD_2;
	static const Enum MAP2_TEXTURE_COORD_3;
	static const Enum MAP2_TEXTURE_COORD_4;
	static const Enum MAP2_VERTEX_3;
	static const Enum MAP2_VERTEX_4;
	static const Enum MAP1_GRID_DOMAIN;
	static const Enum MAP1_GRID_SEGMENTS;
	static const Enum MAP2_GRID_DOMAIN;
	static const Enum MAP2_GRID_SEGMENTS;
	static const Enum COEFF;
	static const Enum ORDER;
	static const Enum _DOMAIN; /* FIXME: Conflicts with something? */

	/* Hints */
	static const Enum PERSPECTIVE_CORRECTION_HINT;
	static const Enum POINT_SMOOTH_HINT;
	static const Enum LINE_SMOOTH_HINT;
	static const Enum POLYGON_SMOOTH_HINT;
	static const Enum FOG_HINT;
	static const Enum DONT_CARE;
	static const Enum FASTEST;
	static const Enum NICEST;

	/* Scissor box */
	static const Enum SCISSOR_BOX;
	static const Enum SCISSOR_TEST;

	/* Pixel Mode / Transfer */
	static const Enum MAP_COLOR;
	static const Enum MAP_STENCIL;
	static const Enum INDEX_SHIFT;
	static const Enum INDEX_OFFSET;
	static const Enum RED_SCALE;
	static const Enum RED_BIAS;
	static const Enum GREEN_SCALE;
	static const Enum GREEN_BIAS;
	static const Enum BLUE_SCALE;
	static const Enum BLUE_BIAS;
	static const Enum ALPHA_SCALE;
	static const Enum ALPHA_BIAS;
	static const Enum DEPTH_SCALE;
	static const Enum DEPTH_BIAS;
	static const Enum PIXEL_MAP_S_TO_S_SIZE;
	static const Enum PIXEL_MAP_I_TO_I_SIZE;
	static const Enum PIXEL_MAP_I_TO_R_SIZE;
	static const Enum PIXEL_MAP_I_TO_G_SIZE;
	static const Enum PIXEL_MAP_I_TO_B_SIZE;
	static const Enum PIXEL_MAP_I_TO_A_SIZE;
	static const Enum PIXEL_MAP_R_TO_R_SIZE;
	static const Enum PIXEL_MAP_G_TO_G_SIZE;
	static const Enum PIXEL_MAP_B_TO_B_SIZE;
	static const Enum PIXEL_MAP_A_TO_A_SIZE;
	static const Enum PIXEL_MAP_S_TO_S;
	static const Enum PIXEL_MAP_I_TO_I;
	static const Enum PIXEL_MAP_I_TO_R;
	static const Enum PIXEL_MAP_I_TO_G;
	static const Enum PIXEL_MAP_I_TO_B;
	static const Enum PIXEL_MAP_I_TO_A;
	static const Enum PIXEL_MAP_R_TO_R;
	static const Enum PIXEL_MAP_G_TO_G;
	static const Enum PIXEL_MAP_B_TO_B;
	static const Enum PIXEL_MAP_A_TO_A;
	static const Enum PACK_ALIGNMENT;
	static const Enum PACK_LSB_FIRST;
	static const Enum PACK_ROW_LENGTH;
	static const Enum PACK_SKIP_PIXELS;
	static const Enum PACK_SKIP_ROWS;
	static const Enum PACK_SWAP_BYTES;
	static const Enum UNPACK_ALIGNMENT;
	static const Enum UNPACK_LSB_FIRST;
	static const Enum UNPACK_ROW_LENGTH;
	static const Enum UNPACK_SKIP_PIXELS;
	static const Enum UNPACK_SKIP_ROWS;
	static const Enum UNPACK_SWAP_BYTES;
	static const Enum ZOOM_X;
	static const Enum ZOOM_Y;

	/* Texture mapping */
	static const Enum TEXTURE_ENV;
	static const Enum TEXTURE_ENV_MODE;
	static const Enum TEXTURE_1D;
	static const Enum TEXTURE_2D;
	static const Enum TEXTURE_WRAP_S;
	static const Enum TEXTURE_WRAP_T;
	static const Enum TEXTURE_MAG_FILTER;
	static const Enum TEXTURE_MIN_FILTER;
	static const Enum TEXTURE_ENV_COLOR;
	static const Enum TEXTURE_GEN_S;
	static const Enum TEXTURE_GEN_T;
	static const Enum TEXTURE_GEN_R;
	static const Enum TEXTURE_GEN_Q;
	static const Enum TEXTURE_GEN_MODE;
	static const Enum TEXTURE_BORDER_COLOR;
	static const Enum TEXTURE_WIDTH;
	static const Enum TEXTURE_HEIGHT;
	static const Enum TEXTURE_BORDER;
	static const Enum TEXTURE_COMPONENTS;
	static const Enum TEXTURE_RED_SIZE;
	static const Enum TEXTURE_GREEN_SIZE;
	static const Enum TEXTURE_BLUE_SIZE;
	static const Enum TEXTURE_ALPHA_SIZE;
	static const Enum TEXTURE_LUMINANCE_SIZE;
	static const Enum TEXTURE_INTENSITY_SIZE;
	static const Enum NEAREST_MIPMAP_NEAREST;
	static const Enum NEAREST_MIPMAP_LINEAR;
	static const Enum LINEAR_MIPMAP_NEAREST;
	static const Enum LINEAR_MIPMAP_LINEAR;
	static const Enum OBJECT_LINEAR;
	static const Enum OBJECT_PLANE;
	static const Enum EYE_LINEAR;
	static const Enum EYE_PLANE;
	static const Enum SPHERE_MAP;
	static const Enum DECAL;
	static const Enum MODULATE;
	static const Enum NEAREST;
	static const Enum REPEAT;
	static const Enum CLAMP;
	static const Enum S;
	static const Enum T;
	static const Enum R;
	static const Enum Q;

	/* Utility */
	static const Enum VENDOR;
	static const Enum RENDERER;
	static const Enum VERSION;
	static const Enum EXTENSIONS;

	/* Errors */
	static const Enum NO_ERROR;
	static const Enum INVALID_ENUM;
	static const Enum INVALID_VALUE;
	static const Enum INVALID_OPERATION;
	static const Enum STACK_OVERFLOW;
	static const Enum STACK_UNDERFLOW;
	static const Enum OUT_OF_MEMORY;

	/* glPush/PopAttrib bits */
	static const Bitfield CURRENT_BIT;
	static const Bitfield POINT_BIT;
	static const Bitfield LINE_BIT;
	static const Bitfield POLYGON_BIT;
	static const Bitfield POLYGON_STIPPLE_BIT;
	static const Bitfield PIXEL_MODE_BIT;
	static const Bitfield LIGHTING_BIT;
	static const Bitfield FOG_BIT;
	static const Bitfield DEPTH_BUFFER_BIT;
	static const Bitfield ACCUM_BUFFER_BIT;
	static const Bitfield STENCIL_BUFFER_BIT;
	static const Bitfield VIEWPORT_BIT;
	static const Bitfield TRANSFORM_BIT;
	static const Bitfield ENABLE_BIT;
	static const Bitfield COLOR_BUFFER_BIT;
	static const Bitfield HINT_BIT;
	static const Bitfield EVAL_BIT;
	static const Bitfield LIST_BIT;
	static const Bitfield TEXTURE_BIT;
	static const Bitfield SCISSOR_BIT;
	static const Bitfield ALL_ATTRIB_BITS;

	/* OpenGL 1.1 */
	static const Enum PROXY_TEXTURE_1D;
	static const Enum PROXY_TEXTURE_2D;
	static const Enum TEXTURE_PRIORITY;
	static const Enum TEXTURE_RESIDENT;
	static const Enum TEXTURE_BINDING_1D;
	static const Enum TEXTURE_BINDING_2D;
	static const Enum TEXTURE_INTERNAL_FORMAT;
	static const Enum ALPHA4;
	static const Enum ALPHA8;
	static const Enum ALPHA12;
	static const Enum ALPHA16;
	static const Enum LUMINANCE4;
	static const Enum LUMINANCE8;
	static const Enum LUMINANCE12;
	static const Enum LUMINANCE16;
	static const Enum LUMINANCE4_ALPHA4;
	static const Enum LUMINANCE6_ALPHA2;
	static const Enum LUMINANCE8_ALPHA8;
	static const Enum LUMINANCE12_ALPHA4;
	static const Enum LUMINANCE12_ALPHA12;
	static const Enum LUMINANCE16_ALPHA16;
	static const Enum INTENSITY;
	static const Enum INTENSITY4;
	static const Enum INTENSITY8;
	static const Enum INTENSITY12;
	static const Enum INTENSITY16;
	static const Enum R3_G3_B2;
	static const Enum RGB4;
	static const Enum RGB5;
	static const Enum RGB8;
	static const Enum RGB10;
	static const Enum RGB12;
	static const Enum RGB16;
	static const Enum RGBA2;
	static const Enum RGBA4;
	static const Enum RGB5_A1;
	static const Enum RGBA8;
	static const Enum RGB10_A2;
	static const Enum RGBA12;
	static const Enum RGBA16;
	static const Bitfield CLIENT_PIXEL_STORE_BIT;
	static const Bitfield CLIENT_VERTEX_ARRAY_BIT;
	static const Bitfield ALL_CLIENT_ATTRIB_BITS;
	static const Bitfield CLIENT_ALL_ATTRIB_BITS;

	/* OpenGL 1.2 */
	static const Enum RESCALE_NORMAL;
	static const Enum CLAMP_TO_EDGE;
	static const Enum MAX_ELEMENTS_VERTICES;
	static const Enum MAX_ELEMENTS_INDICES;
	static const Enum BGR;
	static const Enum BGRA;
	static const Enum UNSIGNED_BYTE_3_3_2;
	static const Enum UNSIGNED_BYTE_2_3_3_REV;
	static const Enum UNSIGNED_SHORT_5_6_5;
	static const Enum UNSIGNED_SHORT_5_6_5_REV;
	static const Enum UNSIGNED_SHORT_4_4_4_4;
	static const Enum UNSIGNED_SHORT_4_4_4_4_REV;
	static const Enum UNSIGNED_SHORT_5_5_5_1;
	static const Enum UNSIGNED_SHORT_1_5_5_5_REV;
	static const Enum UNSIGNED_INT_8_8_8_8;
	static const Enum UNSIGNED_INT_8_8_8_8_REV;
	static const Enum UNSIGNED_INT_10_10_10_2;
	static const Enum UNSIGNED_INT_2_10_10_10_REV;
	static const Enum LIGHT_MODEL_COLOR_CONTROL;
	static const Enum SINGLE_COLOR;
	static const Enum SEPARATE_SPECULAR_COLOR;
	static const Enum TEXTURE_MIN_LOD;
	static const Enum TEXTURE_MAX_LOD;
	static const Enum TEXTURE_BASE_LEVEL;
	static const Enum TEXTURE_MAX_LEVEL;
	static const Enum SMOOTH_POINT_SIZE_RANGE;
	static const Enum SMOOTH_POINT_SIZE_GRANULARITY;
	static const Enum SMOOTH_LINE_WIDTH_RANGE;
	static const Enum SMOOTH_LINE_WIDTH_GRANULARITY;
	static const Enum ALIASED_POINT_SIZE_RANGE;
	static const Enum ALIASED_LINE_WIDTH_RANGE;
	static const Enum PACK_SKIP_IMAGES;
	static const Enum PACK_IMAGE_HEIGHT;
	static const Enum UNPACK_SKIP_IMAGES;
	static const Enum UNPACK_IMAGE_HEIGHT;
	static const Enum TEXTURE_3D;
	static const Enum PROXY_TEXTURE_3D;
	static const Enum TEXTURE_DEPTH;
	static const Enum TEXTURE_WRAP_R;
	static const Enum MAX_3D_TEXTURE_SIZE;
	static const Enum TEXTURE_BINDING_3D;

	/* GL_ARB_imaging */
	static const Enum CONSTANT_COLOR;
	static const Enum ONE_MINUS_CONSTANT_COLOR;
	static const Enum CONSTANT_ALPHA;
	static const Enum ONE_MINUS_CONSTANT_ALPHA;
	static const Enum COLOR_TABLE;
	static const Enum POST_CONVOLUTION_COLOR_TABLE;
	static const Enum POST_COLOR_MATRIX_COLOR_TABLE;
	static const Enum PROXY_COLOR_TABLE;
	static const Enum PROXY_POST_CONVOLUTION_COLOR_TABLE;
	static const Enum PROXY_POST_COLOR_MATRIX_COLOR_TABLE;
	static const Enum COLOR_TABLE_SCALE;
	static const Enum COLOR_TABLE_BIAS;
	static const Enum COLOR_TABLE_FORMAT;
	static const Enum COLOR_TABLE_WIDTH;
	static const Enum COLOR_TABLE_RED_SIZE;
	static const Enum COLOR_TABLE_GREEN_SIZE;
	static const Enum COLOR_TABLE_BLUE_SIZE;
	static const Enum COLOR_TABLE_ALPHA_SIZE;
	static const Enum COLOR_TABLE_LUMINANCE_SIZE;
	static const Enum COLOR_TABLE_INTENSITY_SIZE;
	static const Enum CONVOLUTION_1D;
	static const Enum CONVOLUTION_2D;
	static const Enum SEPARABLE_2D;
	static const Enum CONVOLUTION_BORDER_MODE;
	static const Enum CONVOLUTION_FILTER_SCALE;
	static const Enum CONVOLUTION_FILTER_BIAS;
	static const Enum REDUCE;
	static const Enum CONVOLUTION_FORMAT;
	static const Enum CONVOLUTION_WIDTH;
	static const Enum CONVOLUTION_HEIGHT;
	static const Enum MAX_CONVOLUTION_WIDTH;
	static const Enum MAX_CONVOLUTION_HEIGHT;
	static const Enum POST_CONVOLUTION_RED_SCALE;
	static const Enum POST_CONVOLUTION_GREEN_SCALE;
	static const Enum POST_CONVOLUTION_BLUE_SCALE;
	static const Enum POST_CONVOLUTION_ALPHA_SCALE;
	static const Enum POST_CONVOLUTION_RED_BIAS;
	static const Enum POST_CONVOLUTION_GREEN_BIAS;
	static const Enum POST_CONVOLUTION_BLUE_BIAS;
	static const Enum POST_CONVOLUTION_ALPHA_BIAS;
	static const Enum CONSTANT_BORDER;
	static const Enum REPLICATE_BORDER;
	static const Enum CONVOLUTION_BORDER_COLOR;
	static const Enum COLOR_MATRIX;
	static const Enum COLOR_MATRIX_STACK_DEPTH;
	static const Enum MAX_COLOR_MATRIX_STACK_DEPTH;
	static const Enum POST_COLOR_MATRIX_RED_SCALE;
	static const Enum POST_COLOR_MATRIX_GREEN_SCALE;
	static const Enum POST_COLOR_MATRIX_BLUE_SCALE;
	static const Enum POST_COLOR_MATRIX_ALPHA_SCALE;
	static const Enum POST_COLOR_MATRIX_RED_BIAS;
	static const Enum POST_COLOR_MATRIX_GREEN_BIAS;
	static const Enum POST_COLOR_MATRIX_BLUE_BIAS;
	static const Enum POST_COLOR_MATRIX_ALPHA_BIAS;
	static const Enum HISTOGRAM;
	static const Enum PROXY_HISTOGRAM;
	static const Enum HISTOGRAM_WIDTH;
	static const Enum HISTOGRAM_FORMAT;
	static const Enum HISTOGRAM_RED_SIZE;
	static const Enum HISTOGRAM_GREEN_SIZE;
	static const Enum HISTOGRAM_BLUE_SIZE;
	static const Enum HISTOGRAM_ALPHA_SIZE;
	static const Enum HISTOGRAM_LUMINANCE_SIZE;
	static const Enum HISTOGRAM_SINK;
	static const Enum MINMAX;
	static const Enum MINMAX_FORMAT;
	static const Enum MINMAX_SINK;
	static const Enum TABLE_TOO_LARGE;
	static const Enum BLEND_EQUATION;
	static const Enum MIN;
	static const Enum MAX;
	static const Enum FUNC_ADD;
	static const Enum FUNC_SUBTRACT;
	static const Enum FUNC_REVERSE_SUBTRACT;
	static const Enum BLEND_COLOR;

	/* OpenGL 1.3 */
	/* multitexture */
	static const Enum TEXTURE0;
	static const Enum TEXTURE1;
	static const Enum TEXTURE2;
	static const Enum TEXTURE3;
	static const Enum TEXTURE4;
	static const Enum TEXTURE5;
	static const Enum TEXTURE6;
	static const Enum TEXTURE7;
	static const Enum TEXTURE8;
	static const Enum TEXTURE9;
	static const Enum TEXTURE10;
	static const Enum TEXTURE11;
	static const Enum TEXTURE12;
	static const Enum TEXTURE13;
	static const Enum TEXTURE14;
	static const Enum TEXTURE15;
	static const Enum TEXTURE16;
	static const Enum TEXTURE17;
	static const Enum TEXTURE18;
	static const Enum TEXTURE19;
	static const Enum TEXTURE20;
	static const Enum TEXTURE21;
	static const Enum TEXTURE22;
	static const Enum TEXTURE23;
	static const Enum TEXTURE24;
	static const Enum TEXTURE25;
	static const Enum TEXTURE26;
	static const Enum TEXTURE27;
	static const Enum TEXTURE28;
	static const Enum TEXTURE29;
	static const Enum TEXTURE30;
	static const Enum TEXTURE31;
	static const Enum ACTIVE_TEXTURE;
	static const Enum CLIENT_ACTIVE_TEXTURE;
	static const Enum MAX_TEXTURE_UNITS;
	/* texture_cube_map */
	static const Enum NORMAL_MAP;
	static const Enum REFLECTION_MAP;
	static const Enum TEXTURE_CUBE_MAP;
	static const Enum TEXTURE_BINDING_CUBE_MAP;
	static const Enum TEXTURE_CUBE_MAP_POSITIVE_X;
	static const Enum TEXTURE_CUBE_MAP_NEGATIVE_X;
	static const Enum TEXTURE_CUBE_MAP_POSITIVE_Y;
	static const Enum TEXTURE_CUBE_MAP_NEGATIVE_Y;
	static const Enum TEXTURE_CUBE_MAP_POSITIVE_Z;
	static const Enum TEXTURE_CUBE_MAP_NEGATIVE_Z;
	static const Enum PROXY_TEXTURE_CUBE_MAP;
	static const Enum MAX_CUBE_MAP_TEXTURE_SIZE;
	/* texture_compression */
	static const Enum COMPRESSED_ALPHA;
	static const Enum COMPRESSED_LUMINANCE;
	static const Enum COMPRESSED_LUMINANCE_ALPHA;
	static const Enum COMPRESSED_INTENSITY;
	static const Enum COMPRESSED_RGB;
	static const Enum COMPRESSED_RGBA;
	static const Enum TEXTURE_COMPRESSION_HINT;
	static const Enum TEXTURE_COMPRESSED_IMAGE_SIZE;
	static const Enum TEXTURE_COMPRESSED;
	static const Enum NUM_COMPRESSED_TEXTURE_FORMATS;
	static const Enum COMPRESSED_TEXTURE_FORMATS;
	/* multisample */
	static const Enum MULTISAMPLE;
	static const Enum SAMPLE_ALPHA_TO_COVERAGE;
	static const Enum SAMPLE_ALPHA_TO_ONE;
	static const Enum SAMPLE_COVERAGE;
	static const Enum SAMPLE_BUFFERS;
	static const Enum SAMPLES;
	static const Enum SAMPLE_COVERAGE_VALUE;
	static const Enum SAMPLE_COVERAGE_INVERT;
	static const Bitfield MULTISAMPLE_BIT;
	/* transpose_matrix */
	static const Enum TRANSPOSE_MODELVIEW_MATRIX;
	static const Enum TRANSPOSE_PROJECTION_MATRIX;
	static const Enum TRANSPOSE_TEXTURE_MATRIX;
	static const Enum TRANSPOSE_COLOR_MATRIX;
	/* texture_env_combine */
	static const Enum COMBINE;
	static const Enum COMBINE_RGB;
	static const Enum COMBINE_ALPHA;
	static const Enum SOURCE0_RGB;
	static const Enum SOURCE1_RGB;
	static const Enum SOURCE2_RGB;
	static const Enum SOURCE0_ALPHA;
	static const Enum SOURCE1_ALPHA;
	static const Enum SOURCE2_ALPHA;
	static const Enum OPERAND0_RGB;
	static const Enum OPERAND1_RGB;
	static const Enum OPERAND2_RGB;
	static const Enum OPERAND0_ALPHA;
	static const Enum OPERAND1_ALPHA;
	static const Enum OPERAND2_ALPHA;
	static const Enum RGB_SCALE;
	static const Enum ADD_SIGNED;
	static const Enum INTERPOLATE;
	static const Enum SUBTRACT;
	static const Enum CONSTANT;
	static const Enum PRIMARY_COLOR;
	static const Enum PREVIOUS;
	/* texture_env_dot3 */
	static const Enum DOT3_RGB;
	static const Enum DOT3_RGBA;
	/* texture_border_clamp */
	static const Enum CLAMP_TO_BORDER;

	/* OpenGL 1.5 */
	static const Enum BUFFER_SIZE;
	static const Enum BUFFER_USAGE;
	static const Enum QUERY_COUNTER_BITS;
	static const Enum CURRENT_QUERY;
	static const Enum QUERY_RESULT;
	static const Enum QUERY_RESULT_AVAILABLE;
	static const Enum ARRAY_BUFFER;
	static const Enum ELEMENT_ARRAY_BUFFER;
	static const Enum ARRAY_BUFFER_BINDING;
	static const Enum ELEMENT_ARRAY_BUFFER_BINDING;
	static const Enum VERTEX_ATTRIB_ARRAY_BUFFER_BINDING;
	static const Enum READ_ONLY;
	static const Enum WRITE_ONLY;
	static const Enum READ_WRITE;
	static const Enum BUFFER_ACCESS;
	static const Enum BUFFER_MAPPED;
	static const Enum BUFFER_MAP_POINTER;
	static const Enum STREAM_DRAW;
	static const Enum STREAM_READ;
	static const Enum STREAM_COPY;
	static const Enum STATIC_DRAW;
	static const Enum STATIC_READ;
	static const Enum STATIC_COPY;
	static const Enum DYNAMIC_DRAW;
	static const Enum DYNAMIC_READ;
	static const Enum DYNAMIC_COPY;
	static const Enum SAMPLES_PASSED;
	static const Enum SRC1_ALPHA;
	static const Enum VERTEX_ARRAY_BUFFER_BINDING;
	static const Enum NORMAL_ARRAY_BUFFER_BINDING;
	static const Enum COLOR_ARRAY_BUFFER_BINDING;
	static const Enum INDEX_ARRAY_BUFFER_BINDING;
	static const Enum TEXTURE_COORD_ARRAY_BUFFER_BINDING;
	static const Enum EDGE_FLAG_ARRAY_BUFFER_BINDING;
	static const Enum SECONDARY_COLOR_ARRAY_BUFFER_BINDING;
	static const Enum FOG_COORDINATE_ARRAY_BUFFER_BINDING;
	static const Enum WEIGHT_ARRAY_BUFFER_BINDING;
	static const Enum FOG_COORD_SRC;
	static const Enum FOG_COORD;
	static const Enum CURRENT_FOG_COORD;
	static const Enum FOG_COORD_ARRAY_TYPE;
	static const Enum FOG_COORD_ARRAY_STRIDE;
	static const Enum FOG_COORD_ARRAY_POINTER;
	static const Enum FOG_COORD_ARRAY;
	static const Enum FOG_COORD_ARRAY_BUFFER_BINDING;
	static const Enum SRC0_RGB;
	static const Enum SRC1_RGB;
	static const Enum SRC2_RGB;
	static const Enum SRC0_ALPHA;
	static const Enum SRC2_ALPHA;

	/* OpenGL 2.0 */
	static const Enum BLEND_EQUATION_RGB;
	static const Enum VERTEX_ATTRIB_ARRAY_ENABLED;
	static const Enum VERTEX_ATTRIB_ARRAY_SIZE;
	static const Enum VERTEX_ATTRIB_ARRAY_STRIDE;
	static const Enum VERTEX_ATTRIB_ARRAY_TYPE;
	static const Enum CURRENT_VERTEX_ATTRIB;
	static const Enum VERTEX_PROGRAM_POINT_SIZE;
	static const Enum VERTEX_ATTRIB_ARRAY_POINTER;
	static const Enum STENCIL_BACK_FUNC;
	static const Enum STENCIL_BACK_FAIL;
	static const Enum STENCIL_BACK_PASS_DEPTH_FAIL;
	static const Enum STENCIL_BACK_PASS_DEPTH_PASS;
	static const Enum MAX_DRAW_BUFFERS;
	static const Enum DRAW_BUFFER0;
	static const Enum DRAW_BUFFER1;
	static const Enum DRAW_BUFFER2;
	static const Enum DRAW_BUFFER3;
	static const Enum DRAW_BUFFER4;
	static const Enum DRAW_BUFFER5;
	static const Enum DRAW_BUFFER6;
	static const Enum DRAW_BUFFER7;
	static const Enum DRAW_BUFFER8;
	static const Enum DRAW_BUFFER9;
	static const Enum DRAW_BUFFER10;
	static const Enum DRAW_BUFFER11;
	static const Enum DRAW_BUFFER12;
	static const Enum DRAW_BUFFER13;
	static const Enum DRAW_BUFFER14;
	static const Enum DRAW_BUFFER15;
	static const Enum BLEND_EQUATION_ALPHA;
	static const Enum MAX_VERTEX_ATTRIBS;
	static const Enum VERTEX_ATTRIB_ARRAY_NORMALIZED;
	static const Enum MAX_TEXTURE_IMAGE_UNITS;
	static const Enum FRAGMENT_SHADER;
	static const Enum VERTEX_SHADER;
	static const Enum MAX_FRAGMENT_UNIFORM_COMPONENTS;
	static const Enum MAX_VERTEX_UNIFORM_COMPONENTS;
	static const Enum MAX_VARYING_FLOATS;
	static const Enum MAX_VERTEX_TEXTURE_IMAGE_UNITS;
	static const Enum MAX_COMBINED_TEXTURE_IMAGE_UNITS;
	static const Enum SHADER_TYPE;
	static const Enum FLOAT_VEC2;
	static const Enum FLOAT_VEC3;
	static const Enum FLOAT_VEC4;
	static const Enum INT_VEC2;
	static const Enum INT_VEC3;
	static const Enum INT_VEC4;
	static const Enum BOOL;
	static const Enum BOOL_VEC2;
	static const Enum BOOL_VEC3;
	static const Enum BOOL_VEC4;
	static const Enum FLOAT_MAT2;
	static const Enum FLOAT_MAT3;
	static const Enum FLOAT_MAT4;
	static const Enum SAMPLER_1D;
	static const Enum SAMPLER_2D;
	static const Enum SAMPLER_3D;
	static const Enum SAMPLER_CUBE;
	static const Enum SAMPLER_1D_SHADOW;
	static const Enum SAMPLER_2D_SHADOW;
	static const Enum DELETE_STATUS;
	static const Enum COMPILE_STATUS;
	static const Enum LINK_STATUS;
	static const Enum VALIDATE_STATUS;
	static const Enum INFO_LOG_LENGTH;
	static const Enum ATTACHED_SHADERS;
	static const Enum ACTIVE_UNIFORMS;
	static const Enum ACTIVE_UNIFORM_MAX_LENGTH;
	static const Enum SHADER_SOURCE_LENGTH;
	static const Enum ACTIVE_ATTRIBUTES;
	static const Enum ACTIVE_ATTRIBUTE_MAX_LENGTH;
	static const Enum FRAGMENT_SHADER_DERIVATIVE_HINT;
	static const Enum SHADING_LANGUAGE_VERSION;
	static const Enum CURRENT_PROGRAM;
	static const Enum POINT_SPRITE_COORD_ORIGIN;
	static const Enum LOWER_LEFT;
	static const Enum UPPER_LEFT;
	static const Enum STENCIL_BACK_REF;
	static const Enum STENCIL_BACK_VALUE_MASK;
	static const Enum STENCIL_BACK_WRITEMASK;
	static const Enum VERTEX_PROGRAM_TWO_SIDE;
	static const Enum POINT_SPRITE;
	static const Enum COORD_REPLACE;
	static const Enum MAX_TEXTURE_COORDS;

	/* OpenGL 2.1 */
	static const Enum PIXEL_PACK_BUFFER;
	static const Enum PIXEL_UNPACK_BUFFER;
	static const Enum PIXEL_PACK_BUFFER_BINDING;
	static const Enum PIXEL_UNPACK_BUFFER_BINDING;
	static const Enum FLOAT_MAT2x3;
	static const Enum FLOAT_MAT2x4;
	static const Enum FLOAT_MAT3x2;
	static const Enum FLOAT_MAT3x4;
	static const Enum FLOAT_MAT4x2;
	static const Enum FLOAT_MAT4x3;
	static const Enum SRGB;
	static const Enum SRGB8;
	static const Enum SRGB_ALPHA;
	static const Enum SRGB8_ALPHA8;
	static const Enum COMPRESSED_SRGB;
	static const Enum COMPRESSED_SRGB_ALPHA;
	static const Enum CURRENT_RASTER_SECONDARY_COLOR;
	static const Enum SLUMINANCE_ALPHA;
	static const Enum SLUMINANCE8_ALPHA8;
	static const Enum SLUMINANCE;
	static const Enum SLUMINANCE8;
	static const Enum COMPRESSED_SLUMINANCE;
	static const Enum COMPRESSED_SLUMINANCE_ALPHA;

	/* OpenGL 3.0 */
	static const Enum COMPARE_REF_TO_TEXTURE;
	static const Enum CLIP_DISTANCE0;
	static const Enum CLIP_DISTANCE1;
	static const Enum CLIP_DISTANCE2;
	static const Enum CLIP_DISTANCE3;
	static const Enum CLIP_DISTANCE4;
	static const Enum CLIP_DISTANCE5;
	static const Enum CLIP_DISTANCE6;
	static const Enum CLIP_DISTANCE7;
	static const Enum MAX_CLIP_DISTANCES;
	static const Enum MAJOR_VERSION;
	static const Enum MINOR_VERSION;
	static const Enum NUM_EXTENSIONS;
	static const Enum CONTEXT_FLAGS;
	static const Enum COMPRESSED_RED;
	static const Enum COMPRESSED_RG;
	static const Enum CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
	static const Enum RGBA32F;
	static const Enum RGB32F;
	static const Enum RGBA16F;
	static const Enum RGB16F;
	static const Enum VERTEX_ATTRIB_ARRAY_INTEGER;
	static const Enum MAX_ARRAY_TEXTURE_LAYERS;
	static const Enum MIN_PROGRAM_TEXEL_OFFSET;
	static const Enum MAX_PROGRAM_TEXEL_OFFSET;
	static const Enum CLAMP_READ_COLOR;
	static const Enum FIXED_ONLY;
	static const Enum MAX_VARYING_COMPONENTS;
	static const Enum TEXTURE_1D_ARRAY;
	static const Enum PROXY_TEXTURE_1D_ARRAY;
	static const Enum TEXTURE_2D_ARRAY;
	static const Enum PROXY_TEXTURE_2D_ARRAY;
	static const Enum TEXTURE_BINDING_1D_ARRAY;
	static const Enum TEXTURE_BINDING_2D_ARRAY;
	static const Enum R11F_G11F_B10F;
	static const Enum UNSIGNED_INT_10F_11F_11F_REV;
	static const Enum RGB9_E5;
	static const Enum UNSIGNED_INT_5_9_9_9_REV;
	static const Enum TEXTURE_SHARED_SIZE;
	static const Enum TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH;
	static const Enum TRANSFORM_FEEDBACK_BUFFER_MODE;
	static const Enum MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS;
	static const Enum TRANSFORM_FEEDBACK_VARYINGS;
	static const Enum TRANSFORM_FEEDBACK_BUFFER_START;
	static const Enum TRANSFORM_FEEDBACK_BUFFER_SIZE;
	static const Enum PRIMITIVES_GENERATED;
	static const Enum TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN;
	static const Enum RASTERIZER_DISCARD;
	static const Enum MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS;
	static const Enum MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS;
	static const Enum INTERLEAVED_ATTRIBS;
	static const Enum SEPARATE_ATTRIBS;
	static const Enum TRANSFORM_FEEDBACK_BUFFER;
	static const Enum TRANSFORM_FEEDBACK_BUFFER_BINDING;
	static const Enum RGBA32UI;
	static const Enum RGB32UI;
	static const Enum RGBA16UI;
	static const Enum RGB16UI;
	static const Enum RGBA8UI;
	static const Enum RGB8UI;
	static const Enum RGBA32I;
	static const Enum RGB32I;
	static const Enum RGBA16I;
	static const Enum RGB16I;
	static const Enum RGBA8I;
	static const Enum RGB8I;
	static const Enum RED_INTEGER;
	static const Enum GREEN_INTEGER;
	static const Enum BLUE_INTEGER;
	static const Enum RGB_INTEGER;
	static const Enum RGBA_INTEGER;
	static const Enum BGR_INTEGER;
	static const Enum BGRA_INTEGER;
	static const Enum SAMPLER_1D_ARRAY;
	static const Enum SAMPLER_2D_ARRAY;
	static const Enum SAMPLER_1D_ARRAY_SHADOW;
	static const Enum SAMPLER_2D_ARRAY_SHADOW;
	static const Enum SAMPLER_CUBE_SHADOW;
	static const Enum UNSIGNED_INT_VEC2;
	static const Enum UNSIGNED_INT_VEC3;
	static const Enum UNSIGNED_INT_VEC4;
	static const Enum INT_SAMPLER_1D;
	static const Enum INT_SAMPLER_2D;
	static const Enum INT_SAMPLER_3D;
	static const Enum INT_SAMPLER_CUBE;
	static const Enum INT_SAMPLER_1D_ARRAY;
	static const Enum INT_SAMPLER_2D_ARRAY;
	static const Enum UNSIGNED_INT_SAMPLER_1D;
	static const Enum UNSIGNED_INT_SAMPLER_2D;
	static const Enum UNSIGNED_INT_SAMPLER_3D;
	static const Enum UNSIGNED_INT_SAMPLER_CUBE;
	static const Enum UNSIGNED_INT_SAMPLER_1D_ARRAY;
	static const Enum UNSIGNED_INT_SAMPLER_2D_ARRAY;
	static const Enum QUERY_WAIT;
	static const Enum QUERY_NO_WAIT;
	static const Enum QUERY_BY_REGION_WAIT;
	static const Enum QUERY_BY_REGION_NO_WAIT;
	static const Enum BUFFER_ACCESS_FLAGS;
	static const Enum BUFFER_MAP_LENGTH;
	static const Enum BUFFER_MAP_OFFSET;
	static const Enum DEPTH_COMPONENT32F;
	static const Enum DEPTH32F_STENCIL8;
	static const Enum FLOAT_32_UNSIGNED_INT_24_8_REV;
	static const Enum INVALID_FRAMEBUFFER_OPERATION;
	static const Enum FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING;
	static const Enum FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE;
	static const Enum FRAMEBUFFER_ATTACHMENT_RED_SIZE;
	static const Enum FRAMEBUFFER_ATTACHMENT_GREEN_SIZE;
	static const Enum FRAMEBUFFER_ATTACHMENT_BLUE_SIZE;
	static const Enum FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE;
	static const Enum FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE;
	static const Enum FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE;
	static const Enum FRAMEBUFFER_DEFAULT;
	static const Enum FRAMEBUFFER_UNDEFINED;
	static const Enum DEPTH_STENCIL_ATTACHMENT;
	static const Enum MAX_RENDERBUFFER_SIZE;
	static const Enum DEPTH_STENCIL;
	static const Enum UNSIGNED_INT_24_8;
	static const Enum DEPTH24_STENCIL8;
	static const Enum TEXTURE_STENCIL_SIZE;
	static const Enum TEXTURE_RED_TYPE;
	static const Enum TEXTURE_GREEN_TYPE;
	static const Enum TEXTURE_BLUE_TYPE;
	static const Enum TEXTURE_ALPHA_TYPE;
	static const Enum TEXTURE_DEPTH_TYPE;
	static const Enum UNSIGNED_NORMALIZED;
	static const Enum FRAMEBUFFER_BINDING;
	static const Enum DRAW_FRAMEBUFFER_BINDING;
	static const Enum RENDERBUFFER_BINDING;
	static const Enum READ_FRAMEBUFFER;
	static const Enum DRAW_FRAMEBUFFER;
	static const Enum READ_FRAMEBUFFER_BINDING;
	static const Enum RENDERBUFFER_SAMPLES;
	static const Enum FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE;
	static const Enum FRAMEBUFFER_ATTACHMENT_OBJECT_NAME;
	static const Enum FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL;
	static const Enum FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE;
	static const Enum FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER;
	static const Enum FRAMEBUFFER_COMPLETE;
	static const Enum FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
	static const Enum FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
	static const Enum FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER;
	static const Enum FRAMEBUFFER_INCOMPLETE_READ_BUFFER;
	static const Enum FRAMEBUFFER_UNSUPPORTED;
	static const Enum MAX_COLOR_ATTACHMENTS;
	static const Enum COLOR_ATTACHMENT0;
	static const Enum COLOR_ATTACHMENT1;
	static const Enum COLOR_ATTACHMENT2;
	static const Enum COLOR_ATTACHMENT3;
	static const Enum COLOR_ATTACHMENT4;
	static const Enum COLOR_ATTACHMENT5;
	static const Enum COLOR_ATTACHMENT6;
	static const Enum COLOR_ATTACHMENT7;
	static const Enum COLOR_ATTACHMENT8;
	static const Enum COLOR_ATTACHMENT9;
	static const Enum COLOR_ATTACHMENT10;
	static const Enum COLOR_ATTACHMENT11;
	static const Enum COLOR_ATTACHMENT12;
	static const Enum COLOR_ATTACHMENT13;
	static const Enum COLOR_ATTACHMENT14;
	static const Enum COLOR_ATTACHMENT15;
	static const Enum COLOR_ATTACHMENT16;
	static const Enum COLOR_ATTACHMENT17;
	static const Enum COLOR_ATTACHMENT18;
	static const Enum COLOR_ATTACHMENT19;
	static const Enum COLOR_ATTACHMENT20;
	static const Enum COLOR_ATTACHMENT21;
	static const Enum COLOR_ATTACHMENT22;
	static const Enum COLOR_ATTACHMENT23;
	static const Enum COLOR_ATTACHMENT24;
	static const Enum COLOR_ATTACHMENT25;
	static const Enum COLOR_ATTACHMENT26;
	static const Enum COLOR_ATTACHMENT27;
	static const Enum COLOR_ATTACHMENT28;
	static const Enum COLOR_ATTACHMENT29;
	static const Enum COLOR_ATTACHMENT30;
	static const Enum COLOR_ATTACHMENT31;
	static const Enum DEPTH_ATTACHMENT;
	static const Enum STENCIL_ATTACHMENT;
	static const Enum FRAMEBUFFER;
	static const Enum RENDERBUFFER;
	static const Enum RENDERBUFFER_WIDTH;
	static const Enum RENDERBUFFER_HEIGHT;
	static const Enum RENDERBUFFER_INTERNAL_FORMAT;
	static const Enum STENCIL_INDEX1;
	static const Enum STENCIL_INDEX4;
	static const Enum STENCIL_INDEX8;
	static const Enum STENCIL_INDEX16;
	static const Enum RENDERBUFFER_RED_SIZE;
	static const Enum RENDERBUFFER_GREEN_SIZE;
	static const Enum RENDERBUFFER_BLUE_SIZE;
	static const Enum RENDERBUFFER_ALPHA_SIZE;
	static const Enum RENDERBUFFER_DEPTH_SIZE;
	static const Enum RENDERBUFFER_STENCIL_SIZE;
	static const Enum FRAMEBUFFER_INCOMPLETE_MULTISAMPLE;
	static const Enum MAX_SAMPLES;
	static const Enum INDEX;
	static const Enum TEXTURE_LUMINANCE_TYPE;
	static const Enum TEXTURE_INTENSITY_TYPE;
	static const Enum FRAMEBUFFER_SRGB;
	static const Enum HALF_FLOAT;
	static const Enum MAP_READ_BIT;
	static const Enum MAP_WRITE_BIT;
	static const Enum MAP_INVALIDATE_RANGE_BIT;
	static const Enum MAP_INVALIDATE_BUFFER_BIT;
	static const Enum MAP_FLUSH_EXPLICIT_BIT;
	static const Enum MAP_UNSYNCHRONIZED_BIT;
	static const Enum COMPRESSED_RED_RGTC1;
	static const Enum COMPRESSED_SIGNED_RED_RGTC1;
	static const Enum COMPRESSED_RG_RGTC2;
	static const Enum COMPRESSED_SIGNED_RG_RGTC2;
	static const Enum RG;
	static const Enum RG_INTEGER;
	static const Enum R8;
	static const Enum R16;
	static const Enum RG8;
	static const Enum RG16;
	static const Enum R16F;
	static const Enum R32F;
	static const Enum RG16F;
	static const Enum RG32F;
	static const Enum R8I;
	static const Enum R8UI;
	static const Enum R16I;
	static const Enum R16UI;
	static const Enum R32I;
	static const Enum R32UI;
	static const Enum RG8I;
	static const Enum RG8UI;
	static const Enum RG16I;
	static const Enum RG16UI;
	static const Enum RG32I;
	static const Enum RG32UI;
	static const Enum VERTEX_ARRAY_BINDING;
	static const Enum CLAMP_VERTEX_COLOR;
	static const Enum CLAMP_FRAGMENT_COLOR;
	static const Enum ALPHA_INTEGER;


	/* Function types: */

	typedef Enum(GetErrorFunc)();

	typedef void(ViewportFunc)(Int x, Int y, Sizei width, Sizei height);
	typedef void(ClearFunc)(Bitfield mask);
	typedef void(GetBooleanvFunc)(Enum paramName, Boolean *paramsRet);
	typedef void(GetFloatvFunc)(Enum paramName, Float *paramsRet);
	typedef void(GetIntegervFunc)(Enum paramName, Int *paramsRet);

	typedef void(GenBuffersFunc)(Sizei n, Uint *buffers);
	typedef void(DeleteBuffersFunc)(Sizei n, const Uint *buffers);
	typedef void(BindBufferFunc)(Enum target, Uint buffer);
	typedef void(BufferDataFunc)(Enum target, Sizeiptr size, 
			const void *data, Enum usage);
	typedef void *(MapBufferFunc)(Enum target, Enum access);
	typedef Boolean (UnmapBufferFunc)(Enum target);
	typedef void(VertexAttribPointerFunc)(Uint index, Int size, Enum type, 
			Boolean normalized, Sizei stride, const void *pointer);
	typedef void(EnableVertexAttribArrayFunc)(Uint index);
	typedef void(DisableVertexAttribArrayFunc)(Uint index);
	typedef void(DrawArraysFunc)(Enum mode, Int first, Sizei count);
	typedef void(DrawElementsFunc)(Enum mode, Sizei count, Enum type, 
			const void *indexes);

	typedef Uint(CreateShaderFunc)(Enum shaderType);
	typedef void(DeleteShaderFunc)(Uint shader);
	typedef void(ShaderSourceFunc)(Uint shader, Sizei count, 
			const Char *const *strings, const Int *lengths);
	typedef void(CompileShaderFunc)(Uint shader);
	typedef void(AttachShaderFunc)(Uint program, Uint shader);
	typedef void(GetShaderivFunc)(Uint shader, Enum paramName, 
			Int *paramRet);
	typedef void(GetShaderInfoLogFunc)(Uint shader, Sizei maxLength, 
			Sizei *logLenRet, Char *logStrRet);

	typedef Uint(CreateProgramFunc)();
	typedef void(DeleteProgramFunc)(Uint program);
	typedef void(LinkProgramFunc)(Uint program);
	typedef void(ValidateProgramFunc)(Uint program);
	typedef void(UseProgramFunc)(Uint program);
	typedef void(GetProgramivFunc)(Uint program, Enum paramName, 
			Int *paramRet);
	typedef void(GetProgramInfoLogFunc)(Uint program, Sizei maxLength, 
			Sizei *logLenRet, Char *logStrRet);	

	typedef Int(GetUniformLocationFunc)(Uint program, 
			const Char *uniformName);
	typedef void(GetActiveUniformFunc)(Uint program, Uint index, 
			Sizei bufSize, Sizei *length, Int *size, Enum *type, Char *name);
	typedef void(GetActiveAttribFunc)(Uint program, Uint index, 
			Sizei bufSize, Sizei *length, Int *size, Enum *type, Char *name);
	typedef void(Uniform1fFunc)(Int location, Float v0);
	typedef void(Uniform2fFunc)(Int location, Float v0, Float v1);
	typedef void(Uniform3fFunc)(Int location, Float v0, Float v1, Float v2);
	typedef void(Uniform4fFunc)(Int location, Float v0, Float v1, Float v2, 
			Float v3);
	typedef void(Uniform1iFunc)(Int location, Int v0);
	typedef void(Uniform2iFunc)(Int location, Int v0, Int v1);
	typedef void(Uniform3iFunc)(Int location, Int v0, Int v1, Int v2);
	typedef void(Uniform4iFunc)(Int location, Int v0, Int v1, Int v2, 
			Int v3);
	typedef void(Uniform1fvFunc)(Int location, Sizei count, const Float *v);
	typedef void(Uniform2fvFunc)(Int location, Sizei count, const Float *v);
	typedef void(Uniform3fvFunc)(Int location, Sizei count, const Float *v);
	typedef void(Uniform4fvFunc)(Int location, Sizei count, const Float *v);
	typedef void(Uniform1ivFunc)(Int location, Sizei count, const Int *v);
	typedef void(Uniform2ivFunc)(Int location, Sizei count, const Int *v);
	typedef void(Uniform3ivFunc)(Int location, Sizei count, const Int *v);
	typedef void(Uniform4ivFunc)(Int location, Sizei count, const Int *v);
	typedef void(UniformMatrix2fvFunc)(Int location, Sizei count, 
			Boolean transpose, const Float *v);
	typedef void(UniformMatrix3fvFunc)(Int location, Sizei count, 
			Boolean transpose, const Float *v);
	typedef void(UniformMatrix4fvFunc)(Int location, Sizei count, 
			Boolean transpose, const Float *v);

	typedef void(GenTexturesFunc)(Sizei count, Uint *texturesRet);
	typedef void(DeleteTexturesFunc)(Sizei count, const Uint *textures);
	typedef void(BindTextureFunc)(Enum target, Uint texture);
	typedef void(TexImage2DFunc)(Enum target, Int level, Int internalFormat, 
			Sizei width, Sizei height, Int border, Enum format, Enum type, 
			const void *data);
	typedef void(TexParameterfFunc)(Enum target, Enum paramName, 
			Float param);
	typedef void(TexParameteriFunc)(Enum target, Enum paramName, Int param);
	typedef void(TexParameterfvFunc)(Enum target, Enum paramName, 
			Float *param);
	typedef void(TexParameterivFunc)(Enum target, Enum paramName, 
			Int *param);
	typedef void(ActiveTextureFunc)(Enum texture);

	typedef void(GenFramebuffersFunc)(Sizei n, Uint *framebuffers);
	typedef void(DeleteFramebuffersFunc)(Sizei n, const Uint *framebuffers);
	typedef void(BindFramebufferFunc)(Enum target, Uint framebuffer);
	typedef void(FramebufferTexture2DFunc)(Enum target, Enum attachment, 
			Enum texTarget, Uint texture, Int level);
	typedef Enum(CheckFramebufferStatusFunc)(Enum target);
	typedef void(ReadPixelsFunc)(Int x, Int y, Sizei width, Sizei height, 
			Enum format, Enum type, Void *pixels);

	typedef void(FrontFaceFunc)(Enum mode);
	typedef void(CullFaceFunc)(Enum mode);
	typedef void(EnableFunc)(Enum capability);
	typedef void(DisableFunc)(Enum capability);


	Gl();


	GetErrorFunc *_GetError = nullptr;

	ViewportFunc *_Viewport = nullptr;
	ClearFunc *_Clear = nullptr;
	GetBooleanvFunc *_GetBooleanv = nullptr;
	GetFloatvFunc *_GetFloatv = nullptr;
	GetIntegervFunc *_GetIntegerv = nullptr;

	GenBuffersFunc *_GenBuffers = nullptr;
	DeleteBuffersFunc *_DeleteBuffers = nullptr;
	BindBufferFunc *_BindBuffer = nullptr;
	BufferDataFunc *_BufferData = nullptr;
	MapBufferFunc *_MapBuffer = nullptr;
	UnmapBufferFunc *_UnmapBuffer = nullptr;
	VertexAttribPointerFunc *_VertexAttribPointer = nullptr;
	EnableVertexAttribArrayFunc *_EnableVertexAttribArray = nullptr;
	DisableVertexAttribArrayFunc *_DisableVertexAttribArray = nullptr;
	DrawArraysFunc *_DrawArrays = nullptr;
	DrawElementsFunc *_DrawElements = nullptr;

	CreateShaderFunc *_CreateShader = nullptr;
	DeleteShaderFunc *_DeleteShader = nullptr;
	ShaderSourceFunc *_ShaderSource = nullptr;
	CompileShaderFunc *_CompileShader = nullptr;
	AttachShaderFunc *_AttachShader = nullptr;
	GetShaderivFunc *_GetShaderiv = nullptr;
	GetShaderInfoLogFunc *_GetShaderInfoLog = nullptr;

	CreateProgramFunc *_CreateProgram = nullptr;
	DeleteProgramFunc *_DeleteProgram = nullptr;
	LinkProgramFunc *_LinkProgram = nullptr;
	ValidateProgramFunc *_ValidateProgram = nullptr;
	UseProgramFunc *_UseProgram = nullptr;
	GetProgramivFunc *_GetProgramiv = nullptr;
	GetProgramInfoLogFunc *_GetProgramInfoLog = nullptr;

	GetUniformLocationFunc *_GetUniformLocation = nullptr;
	GetActiveUniformFunc *_GetActiveUniform = nullptr;
	GetActiveAttribFunc *_GetActiveAttrib = nullptr;
	Uniform1fFunc *_Uniform1f = nullptr;
	Uniform2fFunc *_Uniform2f = nullptr;
	Uniform3fFunc *_Uniform3f = nullptr;
	Uniform4fFunc *_Uniform4f = nullptr;
	Uniform1iFunc *_Uniform1i = nullptr;
	Uniform2iFunc *_Uniform2i = nullptr;
	Uniform3iFunc *_Uniform3i = nullptr;
	Uniform4iFunc *_Uniform4i = nullptr;
	Uniform1fvFunc *_Uniform1fv = nullptr;
	Uniform2fvFunc *_Uniform2fv = nullptr;
	Uniform3fvFunc *_Uniform3fv = nullptr;
	Uniform4fvFunc *_Uniform4fv = nullptr;
	Uniform1ivFunc *_Uniform1iv = nullptr;
	Uniform2ivFunc *_Uniform2iv = nullptr;
	Uniform3ivFunc *_Uniform3iv = nullptr;
	Uniform4ivFunc *_Uniform4iv = nullptr;
	UniformMatrix2fvFunc *_UniformMatrix2fv = nullptr;
	UniformMatrix3fvFunc *_UniformMatrix3fv = nullptr;
	UniformMatrix4fvFunc *_UniformMatrix4fv = nullptr;

	GenTexturesFunc *_GenTextures = nullptr;
	DeleteTexturesFunc *_DeleteTextures = nullptr;
	BindTextureFunc *_BindTexture = nullptr;
	TexImage2DFunc *_TexImage2D = nullptr;
	TexParameterfFunc *_TexParameterf = nullptr;
	TexParameteriFunc *_TexParameteri = nullptr;
	TexParameterfvFunc *_TexParameterfv = nullptr;
	TexParameterivFunc *_TexParameteriv = nullptr;
	ActiveTextureFunc *_ActiveTexture = nullptr;

	GenFramebuffersFunc *_GenFramebuffers = nullptr;
	DeleteFramebuffersFunc *_DeleteFramebuffers = nullptr;
	BindFramebufferFunc *_BindFramebuffer = nullptr;
	FramebufferTexture2DFunc *_FramebufferTexture2D = nullptr;
	CheckFramebufferStatusFunc *_CheckFramebufferStatus = nullptr;
	ReadPixelsFunc *_ReadPixels = nullptr;

	FrontFaceFunc *_FrontFace = nullptr;
	CullFaceFunc *_CullFace = nullptr;
	EnableFunc *_Enable = nullptr;
	DisableFunc *_Disable = nullptr;
};

extern Gl gl;

} /* namespace ATD */


