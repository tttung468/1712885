﻿#define _CRT_SECURE_NO_WARNINGS
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

void TachChuoi(FILE *fin, SINHVIEN* &SV, wchar_t* thongtin, wchar_t KiTuDung)	//lấy thông tin từng trường dữ liệu
{
	wchar_t ch;
	int j = 0;
	while (1)
	{
		ch = fgetwc(fin);
		if (ch == KiTuDung)
		{
			thongtin[j] = '\0';
			break;
		}
		thongtin[j] = ch;
		j++;
	}
}

void LayThongtinSV(FILE* fin, SINHVIEN* &SV, long count)
{
	wchar_t ch = fgetwc(fin);	//lấy kí tự trống không cần thiết, sau đó đọc thông tin để lưu
	int i;		//đếm thứ tự sinh viên
	int j = 0;		//đếm thứ tự phần tử
	for (i = 0; i < count; ++i)
	{
		TachChuoi(fin, SV, (SV + i)->MSSV, ',');
		TachChuoi(fin, SV, (SV + i)->HoTen, ',');
		TachChuoi(fin, SV, (SV + i)->Khoa, ',');
		TachChuoi(fin, SV, (SV + i)->Email, ',');
		fwscanf(fin, L"%d", &(SV + i)->KhoaTuyen);
		ch = fgetwc(fin);	//lấy kí tự ','
		TachChuoi(fin, SV, (SV + i)->NgaySinh, ',');
		TachChuoi(fin, SV, (SV + i)->HinhAnh, ',');
		//lấy thông tin mô tả
		ch = fgetwc(fin);
		if (ch != '"')
		{
			fseek(fin, -1, SEEK_CUR);
			TachChuoi(fin, SV, (SV + i)->MoTa, ',');
		}
		else
		{
			TachChuoi(fin, SV, (SV + i)->MoTa, '"');
			ch = fgetwc(fin);	//lấy kí tự ','
		}
		//lấy thông tin sở thích
		TachChuoi(fin, SV, (SV + i)->SoThich, '\n');
	}
}

void main()
{
	_setmode(_fileno(stdout), _O_U16TEXT); //needed for output
	_setmode(_fileno(stdin), _O_U16TEXT); //needed for input

	FILE* fin = _wfopen(L"thongtinsv.CSV", L"rt+, ccs=UTF-8");
	if (!fin)
	{
		wprintf(L"Không mở được file thongtinsv.CSV");
		fclose(fin);
		_getch();
		return;
	}

	long count = countLines(fin);
	rewind(fin);
	SINHVIEN* SV = (SINHVIEN*)calloc(count * sizeof(SINHVIEN), sizeof(SINHVIEN));
	if (SV != NULL)
	{
		LayThongtinSV(fin, SV, count);
		for (int i = 0; i < count; i++)
		{
			wprintf(L"%ls", (SV + i)->MoTa);
			wprintf(L"%ls\n", (SV + i)->SoThich);
		}
	}



	if (SV != NULL) free(SV);
	fclose(fin);
	_getch();
}