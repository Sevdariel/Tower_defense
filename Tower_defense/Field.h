#ifndef FIELD_H
#define FIELD_H

unsigned int fieldVertexCount = 4;

float fieldVertices[] =
{
	
	-20, 0, -20,
	-20, 0, 20,
	20, 0, 20,
	20, 0, -20
	
	
};

float fieldColors[] =
{
	1, 1, 1,
	1, 1, 1,
	1, 1, 1,
	1, 1, 1
};

float fieldTextureCoords[] =
{
	1, 0,
	1, 1,
	0, 1,
	0, 0
	
};

#endif