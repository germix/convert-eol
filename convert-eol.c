////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Programa para convertir el formato de fin de línea para archivos de texto
//
// Germán Martínez - 23/10/2017
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(void)
{
	printf("Usage: [mode] [filename]+\n");
	printf("\n");
	printf("mode:\n");
	printf("  -u       Unix EOL      : <LF>\n");
	printf("  -m       Macintosh EOL : <CR>\n");
	printf("  -w       Windows EOL   : <CR><LF>\n");
	printf("  default  -w\n");
	printf("\n");
	printf("filename:  File name of archive to convert\n");
	printf("\n");
}
void convert_eol(const char* inFilename, const char* eol)
{
	int ok = 0;
	char outFilename[256];
	strcpy(outFilename, inFilename);
	strcat(outFilename, ".tmp");
	
	FILE* fin = fopen(inFilename, "rb");
	FILE* fout = fopen(outFilename, "wb");
	
	if(fin != NULL && fout != NULL)
	{
		ok = 1;
		while(!feof(fin))
		{
			int c;
			while(!feof(fin))
			{
				c = fgetc(fin);
				if(c != EOF)
				{
					if(c == '\r')
					{
						c = fgetc(fin);
						fwrite(eol, 1, strlen(eol), fout);
						if(c == '\n')
						{
						}
						else if(c == '\r')
						{
							fwrite(eol, 1, strlen(eol), fout);
						}
						else if(c != EOF)
						{
							fputc(c, fout);
						}
					}
					else if(c == '\n')
					{
						fwrite(eol, 1, strlen(eol), fout);
					}
					else
					{
						fputc(c, fout);
					}
				}
			}
		}
	}
	if(fin != NULL)
		fclose(fin);
	if(fout != NULL)
		fclose(fout);
	
	if(ok == 1)
	{
		//
		// Eliminar el archivo original
		//
		remove(inFilename);
		
		//
		// Renombrar el archivo temporar con el nombre del archivo original
		//
		rename(outFilename, inFilename);
	}
	
}
int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		int i = 1;
		char* eol = 0;

		if(argv[1][0] == '-')
		{
			i++;
			if(0 == strcmp(argv[1], "-u"))		eol = "\n";
			if(0 == strcmp(argv[1], "-m"))		eol = "\r";
			if(0 == strcmp(argv[1], "-w"))		eol = "\r\n";
			
			if(eol == 0)
			{
				usage();
				return -1;
			}
		}
		else
		{
			eol = "\r\n";
		}
		for( ; i < argc; i++)
		{
			convert_eol(argv[i], eol);
		}
	}
	else
	{
		usage();
	}
	return 0;
}
