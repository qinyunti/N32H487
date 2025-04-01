#include <stdio.h>
#include "shell.h"
#include "shell_func.h"
#include "xprintf.h"
#include "uart.h"
#include "flash_itf.h"
#include "ff.h"
#include "xmodem.h"
#include "kissfft_test.h"

static void helpfunc(uint8_t* param);
static void printflashfunc(uint8_t* param);
static void writeflashfunc(uint8_t* param);
static void printfilefunc(uint8_t* param);
static void writefilefunc(uint8_t* param);
static void fsmountfunc(uint8_t* param);

static void lsfilefunc(uint8_t* param);
static void rmfilefunc(uint8_t* param);
static void renamefilefunc(uint8_t* param);
static void touchfunc(uint8_t* param);
static void mkdirfunc(uint8_t* param);
static void pwdfunc(uint8_t* param);
static void cdfunc(uint8_t* param);
static void cpfilefunc(uint8_t* param);
static void freefunc(uint8_t* param);

static void rxfilefunc(uint8_t* param);
static void sxfilefunc(uint8_t* param);

static void ffttestfunc(uint8_t* param);
static void speexfunc(uint8_t* param);

/**
 * 最后一行必须为0,用于结束判断
*/
const shell_cmd_cfg g_shell_cmd_list_ast[ ] = 
{
  { (uint8_t*)"help",         helpfunc,         (uint8_t*)"help"}, 
  { (uint8_t*)"printflash",   printflashfunc,   (uint8_t*)"printflash addr[hex] len"},
  { (uint8_t*)"writeflash",   writeflashfunc,   (uint8_t*)"writeflash addr[hex] hexstr"}, 	
  { (uint8_t*)"printfile",    printfilefunc,    (uint8_t*)"printfile path addr size"},  
  { (uint8_t*)"writefile",    writefilefunc,    (uint8_t*)"writefile path addr[hex] hexstr"}, 
  { (uint8_t*)"fsmount",      fsmountfunc,      (uint8_t*)"fsmount"}, 
	
  { (uint8_t*)"ls    ",       lsfilefunc,       (uint8_t*)"ls path"},
  { (uint8_t*)"rm    ",       rmfilefunc,       (uint8_t*)"rm path"},  
  { (uint8_t*)"rename    ",   renamefilefunc,   (uint8_t*)"rename path newpath"},  
  { (uint8_t*)"touch     ",   touchfunc,        (uint8_t*)"touch path size"},  
  { (uint8_t*)"mkdir     ",   mkdirfunc,        (uint8_t*)"mkdir path"},  
  { (uint8_t*)"pwd     ",     pwdfunc,          (uint8_t*)"pwd"},  
  { (uint8_t*)"cd     ",      cdfunc,           (uint8_t*)"cd path"},  
  { (uint8_t*)"cp",           cpfilefunc,       (uint8_t*)"cp srcpath dstpath"}, 
  { (uint8_t*)"free",         freefunc,         (uint8_t*)"free path"},
	
  { (uint8_t*)"rxfile",       rxfilefunc,       (uint8_t*)"rxfile name len"},
  { (uint8_t*)"sxfile",       sxfilefunc,       (uint8_t*)"sxfile name len"}, 
	
  { (uint8_t*)"ffttest",      ffttestfunc,      (uint8_t*)"ffttest nfft cnt"}, 	

  { (uint8_t*)"speex",        speexfunc,        (uint8_t*)"speex mic spk out"}, 		
	
  { (uint8_t*)0,		          0 ,               0},
};

static void helpfunc(uint8_t* param)
{
	  (void)param;
    unsigned int i;
    xprintf("\r\n");
    xprintf("**************\r\n");
    xprintf("*   SHELL    *\r\n");
    xprintf("*   V1.0     *\r\n");
    xprintf("**************\r\n");
    xprintf("\r\n");
    for (i=0; g_shell_cmd_list_ast[i].name != 0; i++)
    {
        xprintf("%02d.",i);
        xprintf("%-16s",g_shell_cmd_list_ast[i].name);
        xprintf("%s\r\n",g_shell_cmd_list_ast[i].helpstr);
    }
}

static int ascii2uc(const char c, unsigned char *uc)
{
    if ((c >= '0') && (c <= '9')) {
        *uc = c - '0';
    } else if ((c >= 'a') && (c <= 'f')) {
        *uc = c - 'a' + 10;
    } else if ((c >= 'A') && (c <= 'F')) {
        *uc = c - 'A' + 10;
    } else {
        return -1;
    }

    return 0;
}

static uint32_t str2hex(const char* str, unsigned char *buff, uint32_t len)
{
  uint32_t num = 0;
  uint8_t hex = 0;
  while(1)
  {
    uint8_t tmp1;
    uint8_t tmp2;
    if(ascii2uc(*str++, &tmp1) < 0)
    {
      break;
    }
    if(ascii2uc(*str++, &tmp2) < 0)
    {
      break;
    }
    hex = tmp1*16 + tmp2;
    *buff++ = hex;
    num++;
    if(num >= len)
    {
      break;
    }
  }
  return num;
}


static void printflashfunc(uint8_t* param)
{
  uint8_t buffer[16];
  uint32_t addr;
  uint32_t len;
  if(2 == sscanf((const char*)param, "%*s %x %d", &addr, &len))
  {
    uint32_t toread;
    uint32_t read = 0;
    while(read < len)
    {
      toread = ((len-read) > sizeof(buffer)) ? sizeof(buffer) : (len-read);
			flash_itf_read(buffer, addr+read, toread);
      read += toread;
      for(uint32_t i=0; i<toread ;i++)
      {
        xprintf("%02x ",buffer[i]);
      }
      xprintf("\r\n");
    }
  }
}

static void writeflashfunc(uint8_t* param)
{
  uint8_t buffer[32+1];
  uint8_t hex[16];
	
  uint32_t addr;
  uint32_t len;
  if(2 == sscanf((const char*)param, "%*s %x %s", &addr, buffer))
  {
		len = str2hex((const char*)buffer, hex, strlen((char*)buffer));
		if(len>0)
    {
			flash_itf_write(hex,addr,len);
		}
  }
}

void printfilefunc(uint8_t* param)
{
  char path[128];
  uint8_t tmp[16];
  uint32_t addr;
  uint32_t size;
  FRESULT res;
  FIL fil;
  UINT br;
  uint32_t offset = 0;
  if(3 == sscanf((const char*)param, "%*s %s %x %d", path, &addr, &size))
  {
    xprintf("hexdump %s 0x%x %d\r\n",path,addr,size);
    if(FR_OK == (res=f_open(&fil,(const char*)path, FA_READ)))
    {
      xprintf("\r\n");
      do
      {
        br = 0;
        if(FR_OK == (res = f_read(&fil,tmp,(size>16)?16:size,&br)))
        {
          xprintf("%08x ",offset);
          offset+=br;
          for(uint32_t i=0;i<br;i++)
          {
            xprintf("%02x",(uint32_t)tmp[i]);
          }
          xprintf(":");
          for(uint32_t i=0;i<br;i++)
          {
            xprintf("%c",((tmp[i]>0x1F)&&(tmp[i]<0x7F))?(char)tmp[i]:'.');
          }
          xprintf("\r\n");
          size -= br;
        }
        else
        {
          break;
        }
      }while(br > 0);

      if(FR_OK != (res = f_close(&fil)))
      {
        xprintf("close %s err %d\r\n",path,res); 
      }
    }
    else
    {
      xprintf("open %s err %d",path,res);
    }
  }
  else
  {
    xprintf("param err");
  }
}

void writefilefunc(uint8_t* param)
{
  char path[128];
  uint8_t hexstr[32+1];
  uint8_t tmp[16];
  uint32_t hexnum = 0;
  uint32_t addr;
  FRESULT res;
  FIL fil;
  UINT bw;
  if(3 == sscanf((const char*)param, "%*s %s %x %s", path, &addr, hexstr))
  {
    xprintf("hexwrite %s 0x%x %s\r\n",path,addr,hexstr);
    if(FR_OK == (res=f_open(&fil,(const char*)path, FA_WRITE | FA_CREATE_ALWAYS)))
    {
      xprintf("\r\n");
      hexnum = str2hex((const char*)hexstr,tmp,32);
      if(hexnum > 0)
      {
        if(FR_OK == (res=(f_lseek(&fil,addr))))
        {
          if(FR_OK != (res = f_write(&fil,tmp,hexnum,&bw)))
          {
            xprintf("write err %d\r\n",res);
          }
        }
        else
        {
          xprintf("seek %d err %d\r\n",addr,res);
        }
      }

      if(FR_OK != (res = f_close(&fil)))
      {
        xprintf("close %s err %d\r\n",path,res); 
      }
    }
    else
    {
      xprintf("open %s err %d",path,res);
    }
  }
  else
  {
    xprintf("param err");
  }
}

extern void fs_init(void);
void fsmountfunc(uint8_t* param)
{
	fs_init();
}


/***************
 * 文件操作部分
 **************/

/* List contents of a directory */
static FRESULT list_dir (const char *path)
{
    FRESULT res;
    DIR dir;
    FILINFO fno;
    int nfile, ndir;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        nfile = ndir = 0;
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Error or end of dir */
            if (fno.fattrib & AM_DIR) {            /* Directory */
                xprintf("   <DIR>   %s\n", fno.fname);
                ndir++;
            } else {                               /* File */
                xprintf("%10u %s\n", fno.fsize, fno.fname);
                nfile++;
            }
        }
        f_closedir(&dir);
        xprintf("%d dirs, %d files.\n", ndir, nfile);
    } else {
        xprintf("Failed to open \"%s\". (%u)\n", path, res);
    }
    return res;
}


void lsfilefunc(uint8_t* param)
{
  char path[128];
  if(1 == sscanf((const char*)param, "%*s %s", path))
  {
    list_dir((const char *)path);
  }
  else
  {
    xprintf("param err");
  }
}

void rmfilefunc(uint8_t* param)
{
  FRESULT res;
  char path[128];
  if(1 == sscanf((const char*)param, "%*s %s", path))
  {
    if(FR_OK != (res = f_unlink((const char *)path)))
    {
      xprintf("unlink %s err %d\r\n",path,res);
    }
  }
  else
  {
    xprintf("param err");
  }
}

void renamefilefunc(uint8_t* param)
{
  FRESULT res;
  char path[128];
  char newpath[128];
  if(2 == sscanf((const char*)param, "%*s %s %s", path, newpath))
  {
    if(FR_OK != (res = f_rename((const char *)path, (const char*)newpath)))
    {
      xprintf("rename %s to %s err %d\r\n",path,newpath,res);
    }
  }
  else
  {
    xprintf("param err");
  }
}

void touchfunc(uint8_t* param)
{
  FIL fil;
  char path[128];
  int size;
  if(2 == sscanf((const char*)param, "%*s %s %d", path, &size))
  {
    FRESULT res = f_open(&fil, path, FA_CREATE_NEW | FA_WRITE);
    if (FR_OK == res)
    {
      if(FR_OK != (res = f_expand(&fil,size,1)))
      {
        xprintf("expand %s size to %d err %d\r\n",path,size,res); 
      }

      if(FR_OK != (res = f_close(&fil)))
      {
        xprintf("close %s err %d\r\n",path,res); 
      }
    }
    else
    {
      xprintf("open %s err %d\r\n",path,res);
    }
  }
  else
  {
    xprintf("param err");
  }
}


void mkdirfunc(uint8_t* param)
{
  char path[128];
  FRESULT res;
  if(1 == sscanf((const char*)param, "%*s %s", path))
  {
    if(FR_OK != (res=f_mkdir((const char*)path)))
    {
      xprintf("mkdir %s err %d",path,res);
    }
  }
  else
  {
    xprintf("param err");
  }
}

void pwdfunc(uint8_t* param)
{
  (void)param;
  FRESULT fr;
  TCHAR str[128];
  if(1 == sscanf((const char*)param, "%*s %s", str)){
		if(FR_OK != (fr = f_getcwd(str, 128)))  /* Get current directory path */
		{
			xprintf("getcwd err %d\r\n",fr);
		}
		else
		{
			xprintf("%s\r\n",str);
		}
	}else
  {
    xprintf("param err");
  }
}


void cdfunc(uint8_t* param)
{
  char path[128];
  FRESULT res;
  if(1 == sscanf((const char*)param, "%*s %s", path))
  {
    if(FR_OK != (res=f_chdir((const char*)path)))
    {
      xprintf("chdir to %s err %d",path,res);
    }
  }
  else
  {
    xprintf("param err");
  }
}

void freefunc(uint8_t* param)
{
  char path[128];
  FRESULT res;
  if(1 == sscanf((const char*)param, "%*s %s", path))
  {
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;
    /* Get volume information and free clusters of drive 1 */
    res = f_getfree(path, &fre_clust, &fs);
    if(res == FR_OK)
    {
      /* Get total sectors and free sectors */
      tot_sect = (fs->n_fatent - 2) * fs->csize;
      fre_sect = fre_clust * fs->csize;

      /* Print the free space (assuming 512 bytes/sector) */
      xprintf("%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect / 2, fre_sect / 2);
    }
    else
    {
      xprintf("getfree %s err %d\r\n",path,res);
    }
  }
  else
  {
    xprintf("param err");
  }
}


static int cp(const char* src, const char* dst)
{
	FIL fr;
	FIL fw;
	BYTE buf[64] = {0x00};
	FRESULT res;    
	size_t write_size;  /*剩余待写入字节数*/    
	DWORD pos = 0;
	UINT btr;        /*一次试图写入字节数*/
	UINT br;         /*一次实际读到字节数*/
	UINT bw;         /*一次实际写入字节数*/
    UINT size;
	res = f_open(&fr, src, FA_READ);
	if (res != FR_OK)
	{
		return -1;
	}
	res = f_open(&fw, dst, FA_WRITE | FA_CREATE_ALWAYS);
	if (res != FR_OK) 
	{
		f_close(&fr);
		return -1;
	}    
	write_size = f_size( &fr ); 
    size = write_size; 
	do 
	{   
		btr = (write_size > sizeof(buf)) ? sizeof(buf) : write_size;
        //res = f_lseek(&fr, pos);
		res = f_read (&fr, buf, btr, &br); 
        if(res != FR_OK)
        {
            f_close(&fw);
            f_close(&fr);  
            return -1;
        }
        //res = f_lseek(&fw, pos);		
		res = f_write(&fw, buf, br, &bw);
        if((res != FR_OK) || (br != bw))
        {
            f_close(&fw);
            f_close(&fr); 
            return -1;
        }
		//f_sync(&fw);
		pos += br;
		write_size -= br;
        xprintf("\b\b%02d", 100 * (pos + 1) / size);
	}while(write_size > 0);
    xprintf("\b\b\b%d\r\n", 100);
	res = f_close(&fw);
	res = f_close(&fr);  
    return 0;
}

static void cpfilefunc(uint8_t * cmdbuf)
{
	int len;
	int res;
	char path[32] = {0x00};
	char dst[32] = {0x00};
	char regexp[32]; 
	memset((void *)path, 0, sizeof(path));
  len = sscanf((char const *)cmdbuf, "%*s %31s %31s %31s", path,dst,regexp);
	if (len == 2)
	{
		xprintf("\tcopy \"%s\" to \"%s\"...\r\n", path, dst);
		res = cp(path, dst);
		xprintf("f_cp result = %d\r\n", res);
	}
  else
  {}
}


/***************
 * 文件传输部分
 **************/

static uint8_t rxtx_buf[1029];

extern uint32_t g_sys_tick;
static uint32_t getms(void)
{
    return g_sys_tick;
}

static uint32_t io_read(uint8_t* buffer, uint32_t len)
{
  return uart_read(0,buffer, len);
}

static void io_read_flush(void)
{
  uint8_t tmp;
  while(0 != uart_read(0,&tmp, 1));
}

static uint32_t io_write(uint8_t* buffer, uint32_t len)
{
  uart_send(0,buffer, len);
  return len;
}

static FIL rx_fil;            /* File object */
static int rx_file_open_flag = 0;


static int rx_file_open(char* name)
{
  FRESULT res = f_open(&rx_fil, name, FA_CREATE_NEW | FA_WRITE);
  if (FR_OK == res)
  {
    rx_file_open_flag = 1;
    return 0;
  }
  else
  {
    xprintf("open %s err %d\r\n",name,res);
    return -1;
  }
}

static int rx_file_close(char* name)
{
  (void)name;
  if(rx_file_open_flag != 0)
  {
    rx_file_open_flag = 0;
    FRESULT res = f_close(&rx_fil);
    if(res != FR_OK)
    {
      xprintf("close err %d\r\n",res); 
    }
    return 0;
  }
  else
  {
    return -1;
  }
}

static uint32_t  rx_file_write(uint32_t addr, uint8_t* buffer, uint32_t len)
{
  (void)addr;
  if(rx_file_open_flag != 0)
  {
    UINT bw;
    FRESULT res = f_write(&rx_fil, buffer, len, &bw);
    if ((bw != len) || (res != FR_OK))
    {
      xprintf("write err %d %d\r\n",bw,res);
    }
    return bw;
  }
  else
  {
    return 0;
  }
}

void rxfilefunc(uint8_t* param)
{
  char name[64];
  uint32_t len;
  int res = 0;
  if(2 == sscanf((const char*)param, "%*s %s %d", name, &len))
  {
    xprintf("rxfile %s %ld\r\n",name,len);
    if(0 == rx_file_open(name))
    {
      xmodem_cfg_st cfg=
      {
          .buffer = rxtx_buf,
          .crccheck = 1,
          .getms = getms,
          .io_read = io_read,
          .io_read_flush = io_read_flush,
          .io_write = io_write,
          .start_timeout = 60,
          .packet_timeout = 2000,
          .ack_timeout = 2000,
          .mem_write = rx_file_write,
          .addr = 0,
          .totallen = len,
      };
      xmodem_init_rx(&cfg);
      while((res = xmodem_rx()) == 0);
      rx_file_close(name);
      xprintf("res:%d\r\n",res);
    }
    else
    {
      xprintf("open:%s err\r\n",name);
    }
  }
}


static FIL tx_fil;            /* File object */
static int tx_file_open_flag = 0;

static int tx_file_open(char* name)
{
  FRESULT res = f_open(&tx_fil, name,  FA_READ);
  if (FR_OK == res)
  {
    tx_file_open_flag = 1;
    return 0;
  }
  else
  {
    xprintf("open %s err %d\r\n",name,res);
    return -1;
  }
}

static int tx_file_close(char* name)
{
  (void)name;
  if(tx_file_open_flag != 0)
  {
    tx_file_open_flag = 0;
    FRESULT res = f_close(&tx_fil);
    if(res != FR_OK)
    {
      xprintf("close err %d\r\n",res); 
    }
    return 0;
  }
  else
  {
    return -1;
  }
}

static uint32_t tx_file_read(uint32_t addr, uint8_t* buffer, uint32_t len)
{
  (void)addr;
  UINT br;
  FRESULT res = f_read(&tx_fil, buffer, len, &br);
  if(res != 0)
  {
    xprintf("read err %d\r\n",res); 
  }
  return br;
}

void sxfilefunc(uint8_t* param)
{
  char name[64];
  uint32_t len;
  int res = 0;
  if(2 == sscanf((const char*)param, "%*s %s %d", name, &len))
  {
    xprintf("sxfile %s %ld\r\n",name,len);
    if(0 == tx_file_open(name))
    {
      xmodem_cfg_st cfg=
      {
        .buffer = rxtx_buf,
        .plen = 1024,
        .getms = getms,
        .io_read = io_read,
        .io_read_flush = io_read_flush,
        .io_write = io_write,
        .start_timeout = 60,
        .packet_timeout = 1000,
        .ack_timeout = 5000,
        .mem_read = tx_file_read,
        .addr = 0,
        .totallen = len,
      };
      xmodem_init_tx(&cfg);
      while((res = xmodem_tx()) == 0);
      tx_file_close(name);
      xprintf("res:%d\r\n",res);
    }
    else
    {
      xprintf("open:%s err\r\n",name);
    }
  }
}

void ffttestfunc(uint8_t* param)
{
  int cnt;
	int nfft;
  if(2 == sscanf((const char*)param, "%*s %d %d", &nfft, &cnt))
  {
		kiss_fft_test_cpx(nfft, cnt);
		kiss_fft_test_real(nfft, cnt);
  }
}

extern int speex_main(int argc, char **argv);
void speexfunc(uint8_t* param)
{
	char* cmd = "speex";
  char mic[32];
	char spk[32];
	char out[32];
	char* argv[4]={cmd,mic,spk,out};
  if(3 == sscanf((const char*)param, "%*s %s %s %s", mic, spk, out))
  {
		xprintf("speex %s %s %s\r\n",mic, spk, out);
		speex_main(4, argv);
  }
}
