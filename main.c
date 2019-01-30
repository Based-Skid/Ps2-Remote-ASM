// All Includes are in Main.h,
#include "main.h" 
// App Strings
#include "strings.h"
// MD5 Shit
#include "md5.h"

extern void loader_elf; // wLaunchELF's loader.elf.

typedef struct {
	u8	ident[16];
	u16	type;
	u16	machine;
	u32	version;
	u32	entry;
	u32	phoff;
	u32	shoff;
	u32	flags;
	u16	ehsize;
	u16	phentsize;
	u16	phnum;
	u16	shentsize;
	u16	shnum;
	u16	shstrndx;
} elf_header_t;

typedef struct {
	u32	type;
	u32	offset;
	void	*vaddr;
	u32	paddr;
	u32	filesz;
	u32	memsz;
	u32	flags;
	u32	align;
} elf_pheader_t;

void menu_header(void)
{
	scr_printf(appName);
	scr_printf(appVer);
	scr_printf(appAuthor);
	//scr_printf(appNotice);
}

void menu_Text(void)
{
	scr_clear();
	scr_printf(appName);
	scr_printf(appVer);
	scr_printf(appAuthor);
	scr_printf(appNotice);
	scr_printf(txtcrossBtn);
	scr_printf(txtselBtn);
	//scr_printf(txtsqrBtn);
	scr_printf(" \n");
}

void ResetIOP()
{
	// Thanks To SP193 For Clarifying This
	SifInitRpc(0);           //Initialize SIFRPC and SIFCMD. Although seemingly unimportant, this will update the addresses on the EE, which can prevent a crash from happening around the IOP reboot.
	SifIopReset("", 0);      //Reboot IOP with default modules (empty command line)
	while(!SifIopSync()){}   //Wait for IOP to finish rebooting.
	SifInitRpc(0);           //Initialize SIFRPC and SIFCMD.
	SifLoadFileInit();       //Initialize LOADFILE RPC.
	fioInit();               //Initialize FILEIO RPC.
	
	// SBV Patches Are Not part of a Normal IOP Reset.
	sbv_patch_enable_lmb(); //SBV Patches
	sbv_patch_disable_prefix_check(); //SBV Patch Load Executable IRX And ELF Files From User-Writable Storage
/*
I should Note That These SBV patches are also known as protokernel patches.
This Specific Patch Allows you To Hijack and Control the Starting Address in User Memory when memory wipe syscalls are triggered by Games and other software
You Can Also Completely Disable The User Memory Wipe as Well.
	//sbv_patch_user_mem_clear(0x00100000); // You Can Specify a Starting Address for the Wipe
	//sbv_patch_user_mem_clear(0x02000000); // Disable Clear Memory With LoadExecPS2() when 0x02000000 is passed as an arg
*/
}

void gotoOSDSYS(int sc)
{
	// The Purpose of this Function is to Provide a Soft Reset and Handle Module Loading Errors. 
	//This Helps With Diagnosing Modules Failing to load
	if (sc != 0)
	{
		scr_printf(appFail);
		if(sc ==1 || sc ==2 || sc ==3 || sc ==4 || sc ==5)
		{
			scr_printf(modloadfail);
		}
		if (sc == 1)
		{
			scr_printf("SIO2MAN\n");
		}
		if (sc == 2)
		{
			scr_printf("CDVDMAN\n");
		}
		if (sc == 3)
		{
			scr_printf("freepad\n");
		}
		if (sc == 4)
		{
			scr_printf("MCMAN\n");
		}
		if (sc == 5)
		{
			scr_printf("MCSERV\n");
		}
		if (sc == 7)
		{
			scr_printf("iomanX\n");
		}
		if (sc == 8)
		{
			scr_printf("fileXio\n");
		}
		if (sc == 9)
		{
			scr_printf("USBD\n");
		}
		if (sc == 10)
		{
			scr_printf("USBHDFSD\n");
		}
		if (sc == 11)
		{
			scr_printf("DEV9\n");
		}
		if (sc == 12)
		{
			scr_printf("NETMAN\n");
		}
		if (sc == 13)
		{
			scr_printf("SMAP\n");
		}
		if (sc == 14)
		{
			scr_printf("PS2IP-NM\n");
		}
		if (sc == 15)
		{
			scr_printf("PS2IPS\n");
		}
		if (sc == 16)
		{
			scr_printf("ps2http HTTP Client File System\n");
		}
		if (sc == 17)
		{
			scr_printf("Poweroff\n");
		}
		if (sc == 141)
		{
			scr_printf("Failed to Load Remote ELF.\n");
		}
		if (sc == 999)
		{
			scr_printf("Unknown Error");
		}
		sleep(5);
	}
	//Terminate Pad
	padPortClose(0, 0);
	padEnd();
	//Terminate Network
	//NetManDeinit();
	ResetIOP();
	//Terminate fileXio
	//fileXioExit();
	//Prints OSD message
	scr_printf(osdmsg);
	//Terminate SIF Services
	//SifExitRpc();
	//Loads OSDSYS (We do this instead of Exiting to browser to Give the Feel of a Console Boot up,)
	LoadExecPS2("rom0:OSDSYS", 0, NULL);
}



void LoadModules(void)
{
	int ret;

	ret = SifExecModuleBuffer(&iomanX, size_iomanX, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load iomanx Module");
		gotoOSDSYS(7);
	}
	
	ret = SifExecModuleBuffer(&fileXio, size_fileXio, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("failed to Load fileXio Module");
		gotoOSDSYS(8);
	}	
	// Init fileXio 
	fileXioInit();
	
	ret = SifExecModuleBuffer(&freesio2, size_freesio2, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load freesio2 Module");
		gotoOSDSYS(1);
		
	}	
	

	ret = SifExecModuleBuffer(&freepad, size_freepad, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load freepad Module");
		gotoOSDSYS(3);
	}
	
	ret = SifExecModuleBuffer(&mcman, size_mcman, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load mcman Module");
		gotoOSDSYS(4);
	}
	
	ret = SifExecModuleBuffer(&mcserv, size_mcserv, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load mcserv Module");
		gotoOSDSYS(5);
	}
	
	
	ret = SifExecModuleBuffer(&USBD, size_USBD, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load USBD Module %d\n", ret);
		gotoOSDSYS(9);
	}
	
	ret = SifExecModuleBuffer(&USBHDFSD, size_USBHDFSD, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load USBHDFSD module");
		gotoOSDSYS(10);
	}
	
		
	ret = SifExecModuleBuffer(&ps2dev9, size_ps2dev9, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load DEV9 Module %d\n", ret);
		gotoOSDSYS(11);
	}
	

	ret = SifExecModuleBuffer(&netman, size_netman, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load netman module %d\n", ret);
		gotoOSDSYS(12);
	}

	ret = SifExecModuleBuffer(&smap, size_smap, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load smap Module %d\n", ret);
		gotoOSDSYS(13);
	}
	
	ret = SifExecModuleBuffer(&ps2ipnm, size_ps2ipnm, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load ps2ip-nm Module %d\n", ret);
		gotoOSDSYS(14);
	}
	
	ret = SifExecModuleBuffer(&ps2ips, size_ps2ips, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load ps2ips Module! %d\n", ret);
		gotoOSDSYS(15);
	}
	ps2ip_init();
	
	ret = SifExecModuleBuffer(&ps2http, size_ps2http, 0, NULL, NULL);
	if (ret < 0)
	{
        scr_printf("Failed to Load ps2http.IRX! %d\n", ret);
		gotoOSDSYS(16);
	}
	
	ret = SifExecModuleBuffer(&poweroff, size_poweroff, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load Poweroff module");
		//gotoOSDSYS(17);
	}
	

	//init MC\n
	mcInit(MC_TYPE_XMC);
 
	}
	


/////////////////////////////////////////////////////////////////////
//waitPadReady
/////////////////////////////////////////////////////////////////////
static int waitPadReady(int port, int slot)
{
	int state;
	int lastState;
	char stateString[16];

	state = padGetState(port, slot);
	lastState = -1;
	while ((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) {
		if (state != lastState) {
			padStateInt2String(state, stateString);
		}
		lastState = state;
		state = padGetState(port, slot);
	}
	// Were the pad ever 'out of sync'?
	if (lastState != -1) {

	}
	return 0;
}



/////////////////////////////////////////////////////////////////////
//initalizePad
/////////////////////////////////////////////////////////////////////
static int initializePad(int port, int slot)
{

	int ret;
	int modes;
	int i;

	waitPadReady(port, slot);
	modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
	if (modes > 0) {
		for (i = 0; i < modes; i++) {
		}

	}
	if (modes == 0) {
		return 1;
	}

	i = 0;
	do {
		if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
			break;
		i++;
	} while (i < modes);
	if (i >= modes) {
		return 1;
	}

	ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);
	if (ret == 0) {
		return 1;
	}
	padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

	waitPadReady(port, slot);
	padInfoPressMode(port, slot);

	waitPadReady(port, slot);
	padEnterPressMode(port, slot);

	waitPadReady(port, slot);
	actuators = padInfoAct(port, slot, -1, 0);

	if (actuators != 0) {
		actAlign[0] = 0;
		actAlign[1] = 1;
		actAlign[2] = 0xff;
		actAlign[3] = 0xff;
		actAlign[4] = 0xff;
		actAlign[5] = 0xff;

		waitPadReady(port, slot);

		padSetActAlign(port, slot, actAlign);
	}
	else {
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////
//buttonStatts
/////////////////////////////////////////////////////////////////////
static void buttonStatts(int port, int slot)
{
	int ret;
	ret = padRead(port, slot, &buttons);

	if (ret != 0) {
		paddata = 0xffff ^ buttons.btns;

		new_pad = paddata & ~old_pad;
		old_pad = paddata;
	}
}

/////////////////////////////////////////////////////////////////////
//checkPadConnected
/////////////////////////////////////////////////////////////////////
void checkPadConnected(void)
{
	int ret, i;
	ret = padGetState(0, 0);
	while ((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1)) {
		if (ret == PAD_STATE_DISCONN) {
			#if defined DEBUG
				scr_printf("Controller(%d, %d) is disconnected\n", 0, 0);
			#endif
		}
		ret = padGetState(0, 0);
	}
	if (i == 1) {
	}
}

/////////////////////////////////////////////////////////////////////
//pad_wat_button
/////////////////////////////////////////////////////////////////////
void pad_wait_button(u32 button)
{
	while (1)
	{
		buttonStatts(0, 0);
		if (new_pad & button) return;
	}
}


void loadGame() 
/* Special Thanks To Dominator For Finding Launch Arguments in Socom 2 That Allow Direct boot to LAN 
and Providing The original PS2LOGO Code to Pass the args the Main Executable,
This Function Does not Call PS2LOGO and Calls LoadExecPS2 instead. 
You Can Customize this to Suit Your game of choice and also pass launch args if you find any
*/
{
	char *largs[3];
	largs[1] = NULL;
	largs[2] = NULL;
	largs[3] = NULL;
	LoadExecPS2("cdrom0:\\SCUS_XXX.XX;1", 3, largs);
}

int DLfile(char *url, FILE *dlfileName)

{
     int fd, size, result;
     void *buffer;
	fd = open(url, O_RDONLY);	
     
     if (fd >= 0)
     {
           size = lseek(fd, 0, SEEK_END);    //Get size of file
           lseek(fd, 0, SEEK_SET);

           if((buffer = malloc(size)) != NULL)
           {
               result = read(fd, buffer, size) == size ? 0 : -EIO; //Check that the whole file was read.

               if(result != 0)
                 free(buffer);
           } else
               result = -ENOMEM;

           close(fd);
           
			fd = fioOpen(dlfileName,O_WRONLY | O_CREAT);
			if(fd < 0) return -6;

			fioWrite(fd,buffer,size);
			fioClose(fd);
			printf("Your File was written sucessfully.\n");// Print Sucess to Debug/pcsx2 Console
           return 0;
           
     } else
        result = fd;
        return -1;

}

int apply_Update(unsigned int *buffer[])
/*
This Function Is what Applies Your Update.dat
a friendly Reminder that update.dat encryption is not supported in the public source code for the RASM Core Application at this time


*/
{
    DI(); // Suspend Interupt Handlers
    ee_kmode_enter(); // Exit Kernel Mode

    // address variable
    unsigned int *address = 0;    
    // data variable
    unsigned int *data = 0;
    // buffer position variable
    int b = 0;

    // Version print
    scr_printf("\nVersion: ");
    scr_printf(&buffer[0]); // this will read until the last byte = 00
    scr_printf(" found, Applying update...\n "); // example: "Version: 59 found. Applying update..."

    // set buffer position to start of patch
    b += 2; // 8 bytes total. Each array element in an int is 4 bytes.
    
    while(buffer[b] != 0x00000000) // check if array element is all zeros, this will ALWAYS check the address, not the data.
    {            
        // decrypt address;
        address = &buffer[b];
        // increment buffer position
        b++;
        // decrypt data
        data = &buffer[b];
        // increment buffer position
        b++;                
        // copy data to address
        // check if addrss is too low
        if(&address >= 0x000820000)
        {
            // write to memory
            memcpy(address, &data, 4);
        }
        else
        {
            // something went wrong
            scr_printf("\n\nUpdate failed, Please restart The PS2 \n.");
            // force loop to stop
            break;
        }
    }    
    ee_kmode_exit(); // Exit Kernel Mode
    EI(); // Resume Interupt Handlers
}
/*
Thanks to 1up For Simplfying the Download Function.
*/
int downloadPatchfile(char *url)
{
     int fd, size, result;
     void *buffer;
	
	fd = open(url, O_RDONLY);	
     
     if (fd >= 0)
     {
           size = lseek(fd, 0, SEEK_END);    //Get size of file
           lseek(fd, 0, SEEK_SET);

           if((buffer = malloc(size)) != NULL)
           {
               result = read(fd, buffer, size) == size ? 0 : -EIO; //Check that the whole file was read.

               if(result != 0)
                 free(buffer);
           } else
               result = -ENOMEM;

           close(fd);
           
           // apply update to memory
           apply_Update(buffer);
           return 0;
           
     } else
        result = fd;
        return -1;

}


int Access_Test(char *arg)
/*
This Function Makes Sure that The Remote ELF File is Accesable and Also Reports The Size of the file in bytes
This could be useful to diagnose Issues with your webserver or the application
*/
{
	int fd, size;

	fd = open(arg, O_RDONLY);

	if(fd >= 0) {
		size = lseek(fd, 0, SEEK_END);
		close(fd);
	} else return fd;

	return size;
}

void BootELF(int lapp)
{
	u8 *pdata, *dest;
	elf_header_t *eh;
	elf_pheader_t *eph;
	int i, j, ret;
	char arg0[256], arg1[256], arg2[256], arg3[256], arg4[256], arg5[256], arg6[256], arg7[256], arg8[256];
	char *exec_args[9] = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8 };
	int argc = 0;
	sleep(2);

	/*
	exec_args[0] == the target ELF's URI. loader.elf will load that ELF.
	exec_args[1] to exec_args[8] == arguments to be passed to the target ELF.
	*/
	if (lapp != 0)
	{
		// if (lapp == 1)
		// {
			// strcpy(exec_args[0], "http://update.ps2.host/ELF/wLE.ELF");
			// strcpy(exec_args[1], "mc0:/SYS-CONF/");
			// strcpy(exec_args[2], "This");
			// strcpy(exec_args[3], "Is");
			// strcpy(exec_args[4], "a");
			// strcpy(exec_args[5], "Test");
			// argc = 6;
		// }
			if (lapp == 1)
			{
				strcpy(exec_args[0], "http://update.ps2.host/ELF/mDU.ELF");
				argc = 1;
			}
			
	} else asm volatile("break\n"); // OUT OF BOUNDS, UNDEFINED ITEM!
	
	//Clear Screen To Make This Look tidy!
	scr_clear();
	menu_header();
	//
	//Access Test (Make sure The Elf can Actually be Loaded So we Dont Crash The PS2)
	scr_printf("Access Test:\n");
	ret = Access_Test(exec_args[0]);
	if(ret < 0) {
		scr_printf(" could not open the file\n");
		printf("Returned from Access_Test(), could not open the file\n");
		gotoOSDSYS(141);//Reboots Ps2 If this Fails
	} else {
		scr_printf(" %d bytes\n", ret);
		printf("Returned from Access_Test(), %d bytes\n", ret);
	}
	//
	
	// Display URL The ELF Is Being Loaded From
	scr_printf("Launching Application from \n %s", arg0);
	sleep(2);
	/* Load the embedded wLaunchELF's loader.elf to its load address, by parsing its ELF header */
	eh = (elf_header_t *)&loader_elf;
	eph = (elf_pheader_t *)(&loader_elf + eh->phoff);

	for(i = 0; i < eh->phnum; i++) {
		dest = (u8*)(eph[i].vaddr);
		pdata = (u8*)(&loader_elf + eph[i].offset);
		for(j = 0; j < eph[i].filesz; j++) dest[j] = pdata[j];
		if(eph[i].memsz > eph[i].filesz) {
			dest = (u8 *)(eph[i].vaddr + eph[i].filesz);
			for(j = 0; j < eph[i].memsz - eph[i].filesz; j++) dest[j] = '\0';
		}
	}
	padPortClose(0, 0);
	padEnd();
	NetManDeinit();
    SifExitRpc();
    FlushCache(0);
    FlushCache(2);

	ExecPS2((void *)eh->entry, 0, argc, exec_args);
}


void endRasm(void)
{
	padPortClose(0, 0);
	padEnd();
	NetManDeinit();
	sleep(3);
	scr_printf(terrorist);
	scr_clear();
	SifExitRpc();
	//Load Game
	loadGame();
}

void initialize(void)
{

	int ret;

	SifInitRpc(0);
	scr_clear();
	// init debug screen
	init_scr();
	scr_clear();
	menu_header();
	scr_printf("Loading... Please Wait. \n");
	// load all modules
	LoadModules();
	

	// init pad
	padInit(0);
	if ((ret = padPortOpen(0, 0, padBuf)) == 0)
	{
		#if defined DEBUG
			scr_printf("padOpenPort failed: %d\n", ret);
		#endif
		SleepThread();
	}

	if (!initializePad(0, 0))
	{
		#if defined DEBUG
			scr_printf("pad initalization failed!\n");
		#endif
		SleepThread();

	}
}

int main(int argc, char *argv[]) 
{

	ResetIOP();
	
	// initialize
	initialize();
	scr_clear();
	menu_header();
	sleep(1);
	scr_printf("Modules Loaded Up. Starting up DHCP \n");
	dhcpmain();
	menu_Text();
	//motd();
	//motdTap();



	while (1)
	{
		//check to see if the pad is still connected
		checkPadConnected();
		//read pad 0
		buttonStatts(0, 0);

		// if (new_pad & PAD_TRIANGLE)
		// {
			// menu_Text();
		// }
		
		if (new_pad & PAD_SELECT)
		{
		 BootELF(1);
		}
		if(new_pad & PAD_CROSS)
		{
			int dlf,dlt,dlbk;
			scr_printf("Applying Patch From the Internet....");
			dlf = downloadPatchfile(mainDownload);
			if (dlf < 0) 
			{
				
				
				scr_printf("Failed \nReverting to Offline Version \n");
				PasteASM();
				endRasm();

				

			}
			else
			{
				printf("\nPatch Loaded Successfully.\n\0");
				endRasm();
			}
		}
	}
	return 0;
}
