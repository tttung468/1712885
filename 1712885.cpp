#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<fcntl.h> //_O_U16TEXT
#include<io.h>    //_setmode()
#include<malloc.h>

struct SINHVIEN
{
	wchar_t MSSV[10];
	wchar_t HoTen[30];
	wchar_t Khoa[30];
	wchar_t Email[30];
	int KhoaTuyen;
	wchar_t NgaySinh[12];
	wchar_t HinhAnh[30];
	wchar_t MoTa[1000];
	wchar_t SoThich[1000];
};

long countLines(FILE* fin)
{
	long count = 0;
	wchar_t line[2048];
	while (fgetws(line, 2048, fin) != NULL)
	{
		count++;
	}
	return count;
}

void LayThongtinSV(FILE* fin, SINHVIEN *SV, long count)
{
	wchar_t ch;
	wchar_t tam = fgetwc(fin);	//lấy kí tự trống
	while (1)
	{
		ch = fgetwc(fin);
		if (feof(fin)) break;
		if (ch == '\n')
		{
			break;
		}
		fputwc(ch, stdout);
	}
}

void main()
{
	_setmode(_fileno(stdout), _O_U16TEXT); //needed for output
	_setmode(_fileno(stdin), _O_U16TEXT); //needed for input

	FILE* fin = _wfopen(L"thongtinsv.CSV", L"r, ccs=UTF-8");
	if (!fin)
	{
		wprintf(L"Không mở được file thongtinsv.CSV");
		fclose(fin);
		_getch();
		return;
	}

	long count = countLines(fin);
	rewind(fin);
	SINHVIEN* SV = (SINHVIEN*)calloc(count, sizeof(SINHVIEN*));
	if (SV != NULL)
	{
		LayThongtinSV(fin, SV, count);
	}



	free(SV);
	fclose(fin);
	_getch();
}