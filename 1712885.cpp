#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<fcntl.h> //_O_U16TEXT
#include<io.h>    //_setmode()
#include<malloc.h>
#include<string.h>
#define MaxN 2048


typedef struct
{
	wchar_t MSSV[11];
	wchar_t HoTen[31];
	wchar_t Khoa[31];
	wchar_t Email[31];
	int KhoaTuyen;
	wchar_t NgaySinh[13];
	wchar_t HinhAnh[31];
	wchar_t MoTa[1001];
	wchar_t SoThich[1001];
}SINHVIEN;

typedef struct
{
	wchar_t* Title1 = L"<title>";
	wchar_t* NameMSSV = L"\"Personal_FullName\">";
	wchar_t* Khoa = L"\"Personal_Department\">";
	wchar_t* Email = L"\"Personal_Phone\">";
	wchar_t* HinhAnh = L"Personal_HinhcanhanKhung";
	wchar_t* ThongTin = L"TextInListThongTin";
	wchar_t* SoThich = L"TextInListSoThich";
	wchar_t* Mota = L"Description\">";
}TUKHOA;

long countLines(FILE* fin)
{
	long count = 0;
	wchar_t line[MaxN];
	while (fgetws(line, MaxN, fin) != NULL)
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

wchar_t* DatTenFile(wchar_t* MSSV)
{
	wchar_t str2[] = L".html";
	wchar_t str0[] = L"WEBSITE\\";
	int len1 = wcslen(MSSV);
	int len0 = wcslen(str0);
	int len2 = wcslen(str2);
	int len = len0 + len1 + len2;
	wchar_t *FileName = (wchar_t*)calloc(len, sizeof(wchar_t));
	if (FileName != NULL)
	{
		for (int i = 0, j = 0, k = 0; i <= len; ++i)
		{
			if (i < len0)					//ghép chuỗi WEBSITE
			{
				FileName[i] = str0[i];
			}
			else if (i < len0 + len1)				//ghép chuỗi MSSV
			{
				FileName[i] = MSSV[j];
				j++;
			}
			else
			{
				FileName[i] = str2[k];		//ghép chuỗi .HTML
				k++;
			}
		}
	}
	return FileName;
}

wchar_t* LayChuoiCon(wchar_t* ChuoiCha, int len)
{
	wchar_t* str = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
	for (int i = 0; i < len; i++)
	{
		str[i] = ChuoiCha[i];
	}
	str[len] = '\0';
	return str;
}

int TimVtriChuoiCon(wchar_t* ChuoiCha, wchar_t* ChuoiCon)
{
	int lenCha = wcslen(ChuoiCha);
	int lenCon = wcslen(ChuoiCon);
	for (int i = 0; i < lenCha; i++)
	{
		wchar_t* str = LayChuoiCon(ChuoiCha + i, lenCon);
		if (wcscmp(str, ChuoiCon) == 0)
			return i + lenCon;
	}
	return -1;
}

void ChenChuoiCon(wchar_t* ChuoiCha, wchar_t* ChuoiChen, int VtriChen)
{
	int lenCha = wcslen(ChuoiCha);
	int lenChen = wcslen(ChuoiChen);
	wchar_t* temp = (wchar_t*)calloc(lenCha - VtriChen + 1, sizeof(wchar_t));
	wcscpy(temp, ChuoiCha + VtriChen);
	wcscpy(ChuoiCha + VtriChen, ChuoiChen);
	wcscpy(ChuoiCha + VtriChen + lenChen, temp);
	delete[] temp;
}

void GhiFile(FILE* fileMauHTML, FILE* fileOut, SINHVIEN SV)
{
	TUKHOA TuKhoa;
	wchar_t* str = (wchar_t*)calloc(MaxN, sizeof(wchar_t));
	int vitri;
	while (!feof(fileMauHTML))
	{
		fgetws(str, MaxN, fileMauHTML);
		if (wcsstr(str, TuKhoa.Title1))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.Title1);
			ChenChuoiCon(str, L"HCMUS - ", vitri);
			vitri = TimVtriChuoiCon(str, L"HCMUS - ");
			ChenChuoiCon(str, SV.HoTen, vitri);
		}
		else if (wcsstr(str, TuKhoa.NameMSSV))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.NameMSSV);
			ChenChuoiCon(str, SV.HoTen, vitri);
			vitri = TimVtriChuoiCon(str, SV.HoTen);
			ChenChuoiCon(str, L" - ", vitri);
			vitri = TimVtriChuoiCon(str, L" - ");
			ChenChuoiCon(str, SV.MSSV, vitri);
		}
		else if (wcsstr(str, TuKhoa.Khoa))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.Khoa);
			ChenChuoiCon(str, L" Khoa ", vitri);
			vitri = TimVtriChuoiCon(str, L" Khoa ");
			ChenChuoiCon(str, SV.Khoa, vitri);
		}
		else if (wcsstr(str, TuKhoa.Email))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.Email);
			ChenChuoiCon(str, L"Email: ", vitri);
			vitri = TimVtriChuoiCon(str, L"Email: ");
			ChenChuoiCon(str, SV.Email, vitri);
		}
		else if (wcsstr(str, TuKhoa.HinhAnh))
		{
			fputws(str, fileOut);
			fgetws(str, MaxN, fileMauHTML);
			vitri = TimVtriChuoiCon(str, L"<img src=\"");
			ChenChuoiCon(str, SV.HinhAnh, vitri);
		}
		else if (wcsstr(str, TuKhoa.ThongTin))
		{
			fputws(str, fileOut);
			fgetws(str, MaxN, fileMauHTML);
			vitri = TimVtriChuoiCon(str, L"Họ và tên: ");
			ChenChuoiCon(str, SV.HoTen, vitri);
			fputws(str, fileOut);
			fgetws(str, MaxN, fileMauHTML);
			vitri = TimVtriChuoiCon(str, L"MSSV: ");
			ChenChuoiCon(str, SV.MSSV, vitri);
			fputws(str, fileOut);
			fgetws(str, MaxN, fileMauHTML);
			vitri = TimVtriChuoiCon(str, L"Sinh viên khoa ");
			ChenChuoiCon(str, SV.Khoa, vitri);
			fputws(str, fileOut);
			fgetws(str, MaxN, fileMauHTML);
			vitri = TimVtriChuoiCon(str, L"Ngày sinh: ");
			ChenChuoiCon(str, SV.NgaySinh, vitri);
			fputws(str, fileOut);
			fgetws(str, MaxN, fileMauHTML);
			vitri = TimVtriChuoiCon(str, L"Email: ");
			ChenChuoiCon(str, SV.Email, vitri);
		}
		else if (wcsstr(str, TuKhoa.Mota))
		{
			vitri = TimVtriChuoiCon(str, TuKhoa.Mota);
			ChenChuoiCon(str, SV.MoTa, vitri);
		}
		else if (wcsstr(str, TuKhoa.SoThich))
		{

		}
		fputws(str, fileOut);
	}
	delete[] str;
}

void main()
{
	_setmode(_fileno(stdout), _O_U16TEXT); //needed for output
	_setmode(_fileno(stdin), _O_U16TEXT); //needed for input
										  //đọc thông tin từ file CSV
	FILE* fin = _wfopen(L"thongtinsv.CSV", L"r, ccs=UTF-8");
	if (!fin)
	{
		wprintf(L"Không mở được file thongtinsv.CSV");
		fclose(fin);
		_getch();
		return;
	}
	long count = countLines(fin);		//lấy số lượng sinh viên trong file
	rewind(fin);
	SINHVIEN* SV = (SINHVIEN*)calloc(count, sizeof(SINHVIEN));
	if (SV != NULL)
	{
		LayThongtinSV(fin, SV, count);
	}
	//mở file HTML mẫu và xuất ra file HTML mới
	for (int i = 0; i < 1; i++)
	{
		FILE* fileMauHTML = _wfopen(L"fileMauHTML.html", L"r, ccs=UTF-8");			//file HTML mẫu
		if (!fileMauHTML)
		{
			wprintf(L"Không mở được file fileMauHTML.html");
			fclose(fileMauHTML);
			_getch();
			return;
		}
		wchar_t* NameFout = DatTenFile((SV + i)->MSSV);
		if (NameFout == NULL) break;
		FILE* fileOut = _wfopen(NameFout, L"w, ccs=UTF-8");						//file HTML xuất ra
		if (!fileOut)
		{
			wprintf(L"không thể mở file %ls", NameFout);
			fclose(fileOut);
			_getch();
			return;
		}
		GhiFile(fileMauHTML, fileOut, SV[i]);


		fclose(fileOut);
		fclose(fileMauHTML);
	}
	if (SV != NULL) free(SV);
	fclose(fin);
	_getch();
}