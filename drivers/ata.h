#ifndef ATA_H
#define ATA_H

bool waitForDriveReady();

void waitForDriveBufferReady();

//read a single sector of a drive. lba addresses only. driveNum can only be 0 or 1. numSectors is the number of 512byte sectors to read, allowing you to read entire drive cluster at once if desired
//warning: on a scale of 0-10, this function is about a 10.1 on the bullshit scale. Even looking at it the wrong way will permanently break it and you have to restore a backup for it to work again. If modifying, proceed with extreme caution and make backups. This function can defy all logic and sense sometimes
char* ata_lba_read(unsigned int address, unsigned int driveNum = 0, unsigned int numSectors = 1)
{
    unsigned char numSectors8 = numSectors & 0x000000FF;     //concatenate to an 8 bit value only - avoid all kinds of stupid bugs
    //send bit 24 - 27 and then the lba mode signal to port 0x1F6
    char highhigh = (char)((address && 0x0F000000) >> 24);
    highhigh = (highhigh | 0xE0);
    if (driveNum == 1)
    {
        highhigh = (highhigh | 0x10);       //select second drive by changing highhigh to 0xF0 if the second drive is selected
    }
    asmOutb(highhigh, 0x1F6);

    //send number of sectors to 8 bit port 0x1F2
    //I couldn't get this to work in raw assembly but the code was just complicated enough it could be a programming bug issue
    asmOutb(numSectors8, 0x1F2);

    //send bit 0 - 7 LBA to pot 0x1F3
    char low = (char)(address & 0x000000FF);
    asmOutb(low, 0x1F3);

    //send bit 8 - 15 LBA to port 0x1F4
    char highlow = (char)((address & 0x0000FF00) >> 8);
    asmOutb(highlow, 0x1F4);

    //send bit 16-23 LBA to port 0x1F5
    char lowhigh = (char)((address & 0x00FF0000) >> 16);
    asmOutb(lowhigh, 0x1F5);

    //send command 0x20 to command port 0x1F6
    //command 0x20 is "read wiith retry"
    asmOutb(0x20, 0x1F7);
    waitForDriveReady();        //wait for the drive to be ready. not required on QEMU, required on real hardware even if using a CF card

    //now read from port 0x1F0 using 16 bit reads only. ata mode has to be 16 bits. 
    //They did this just to sell more PC-ATs but also because the first ATA controllers were extremely slow

    // make an array to save all the bytes in
    char *hdd_read = (char *)malloc(sizeof(char) * 512 * numSectors8);
    //for (int s = 0; s < numSectors8; s++)
    //{
        for (int i = 0; i < (256*(numSectors8)); i++)
        {
            if (i != 0 && i % 256 == 0 && numSectors8 > 1)
            {
                //asmOutb(0x20, 0x1F7);
                //waitForDriveReady();
                waitForDriveBufferReady();
            }
            // read the 16 bit value from the ata data port
            // short word = asmInW(0x1F0);

            // split into 8 bit values because that's easier to work with and the compiler optimizes it enough that it probably doesn't matter anyway
            // hdd_read[i * 2] = (char)(word & 0x00FF);
            // hdd_read[(i * 2)+1] = (char)((word & 0xFF00) >> 8);
            // intToE9(word);

            // do a single 16 bit read to make this function less slow
            *(short *)(hdd_read + (i << 1)) = asmInW(0x1F0);
            //waitForDriveReady();
            //on a 486, multiplication opcodes take an eternity to finish
        }
    //}

    //return pointer to newly allocated array without freeing it first
    return hdd_read;

}

//polls the drive until registers indicate that it's ready for a read. returns false if it timed out, returns true if the condition being waited for sucessfully happened
bool waitForDriveReady()
{
    unsigned int retries = 0;
    bool ready = false;
    while(!ready && retries < 1000000)
    {
        //get the status register
        char status = asmInb(0x1F7);

        //we only care about bit 7, the BSY bit
        status = (status & 0x80);
        if (status == 0x80)
        {
            //if the BSY bit is set, the device is not ready
            ready = false;
        }
        else
        {
            //if the BSY bit is 0, the device is ready
            ready = true;
        }
        retries++;
    }
    
    //return result. return value is true if the drive is ready. false if it never became ready even after one million retries
    return ready;
}

void waitForDriveBufferReady()
{
    //asmOutb(0x00, 0x1F7);
    waitForDriveReady();

    bool full = false;
    while (!full)
    {
        //get the status register
        char status = asmInb(0x1F7);

        //figure out the value of bit 3
        if ((status & 0x08) > 0)
        {   
            //if bit 3 is a 1, there is data ready in the buffer
            full = true;
        } 
    }
}

//returns the partition information of the given partition index, mbr only
char* getMbrPartition(unsigned int mbrParitionToCheck, unsigned int driveIndex)
{
    char *sector = ata_lba_read(0, driveIndex);
    unsigned int offset = 0x01BE;       //partition entry 1
    offset += (16*mbrParitionToCheck);
    //offset now contains the index to start reading partition data from
    char *partitionEntry = (char*)malloc(sizeof(char) * 16);
    for (int i = 0; i < 16; i++)
    {
        partitionEntry[i] = sector[offset+i];
    }

    //done with the sector data, free it from memory
    free(sector);

    //return the data
    return partitionEntry;
}

//returns important information about the selected FAT16 or FAT32 partition if the selected drive has a MBR
filesystemInfo partitionInformationMBR(unsigned int driveNum, unsigned int partitionNum)
{
    //char array where the mbr information about this partition is stored i.e. a "partitionEntry" as it's called on wikipedia
    char *partitionEntry = getMbrPartition(partitionNum, driveNum);

    //char* requestedSector = ata_lba_read(partitionNum,driveNum);
    filesystemInfo thisFs;// = (filesystemInfo*)malloc(sizeof(partitionEntry));
    
    //now that we have the starting LBA address, we can fetch the rest of the data
    thisFs.startingLBA = *(unsigned int*)(partitionEntry+8);
    thisFs.partitionType = *(char*)(partitionEntry+4);
    thisFs.numSectors = *(unsigned int*)(partitionEntry+0x0C);

    char *partition = ata_lba_read(thisFs.startingLBA, driveNum);

    thisFs.bytesPerSector = *(short*)(partition+0x0B);
    thisFs.sectorsPerCluster = *(char*)(partition+0x0D);
    if (thisFs.partitionType == 0x06)
    {
        thisFs.sectorsPerFat = *(short*)(partition+0x16);       //0x16 if on FAT16, 0x24 if on FAT32
    }
    else
    {
        thisFs.sectorsPerFat = *(short*)(partition+0x24);       //0x16 if on FAT16, 0x24 if on FAT32
    }
    thisFs.reservedSectors = *(short*)(partition+0x0E);

    thisFs.fsInfoSectorLocation = *(short*)(partition+0x30);
    thisFs.volumeSerialNumber = *(unsigned int*)(partition+0x43);
    thisFs.numFats = *(char*)(partition+0x10);

    free(partitionEntry);
    free(partition);
    return thisFs;
}

//data = 32 byte "directory entry". sys = information about this filesystem required for deriving certain data
fileInfo fileFromData(char *data, filesystemInfo *sys)
{
    fileInfo newFile;
    newFile.isValidFile = true;         //consider every potential file entry to be valid until proven otherwise

    //get the 8 character file name and assign it to a string
    for (int i = 0; i < 8; i++)
    {   
        //generally, anything lower than 34 is bad
        if (data[i] > 32)
        {
            //printChar(data[i], 0x0F);
            newFile.fileName += data[i];
            
        }
        else if (data[i] < 32 || data[i] > 127)
        {
            //if at any point during the 8 char parsing process, it finds something shitty, add this file to the shit list
            newFile.isValidFile = false;
        }
    }

    //fetch the file extension and write it to the respective variable
    newFile.fileExtension = "   ";
    newFile.fileExtension.at(0) = data[8];
    newFile.fileExtension.at(1) = data[9];
    newFile.fileExtension.at(2) = data[10];

    //file attributes at offset 0x0B
    //first, check if it's a directopry or a file
    if ((data[0x0B] & 0x10) > 0)
    {
        newFile.isDirectory = true;
    }
    else
    {
        newFile.isDirectory = false;
    }

    //if any of these bits are equal to 1, then this isn't a file I care about
    if ((data[0x0B] & 0xCA) > 0)
    {
        newFile.isValidFile = false;
    }

    //if this isn't a fat16 filesystem, offset 0x14 contains cluster number high byte
    if (sys->partitionType != 0x06)
    {
        newFile.clusterNumberHigh = (*(short*)(data+0x14))&0x0000FFFF;
    }

    //on fat32 and fat16 filesystems, byte offset 0x1A contains low cluster number of this file or directory
    newFile.clusterNumberLow = (*(short*)(data+0x1A))&0x0000FFFF;

    //now get the date and time
    newFile.fileDateTime.seconds = (*(char*)((data+0x0E)))& 0x1F;
    
    //minutes are more involved because of The Way Things Are (tm)
    //char numMinutes = (((*(char*)((data+0x0E)))& 0xE0)>>5);
    //numMinutes = numMinutes >> 5;
    newFile.fileDateTime.minutes = (((*(char*)((data+0x0E)))& 0xE0)>>5) + ((*(char*)(data+0x0F)&0x07)<<3);
    //newFile.fileDateTime.minutes = numMinutes;
    //newFile.fileDateTime.minutes = *(char*)(data+0x0E)

    //hours
    //newFile.fileDateTime.hours = ((*(char*)((data+0x0F)))& 0xF0)>>4;
    //newFile.fileDateTime.hours = 60;
    newFile.fileDateTime.hours = (*(char*)(data+0x0F))>>3;
    newFile.fileDateTime.hours = newFile.fileDateTime.hours & 0x1F;

    //days months years are just as annoying
    newFile.fileDateTime.day = (*(char*)(data+0x10))&0x1F;
    newFile.fileDateTime.month = ((*(char*)(data+0x10))&0xE0)>>5;
    //newFile.fileDateTime.
    char tempMonth = (*(char*)(data+0x11)&0x01)<<3;
    newFile.fileDateTime.month+= tempMonth;
    newFile.fileDateTime.year = (*(char*)(data+0x11)&0xFE)>>1;

    //am i high or does file size not get recorded anywhere in this function? Cuz I don't see it anywhere (it's 10am and i've had 2 cups of coffee)
    newFile.size = *(unsigned int*)(data+0x1C); //how big the file says it is. Invalid for directories, although directories are supposed to have had their size 0x1C offset set to zero

    return newFile;
}

//input the file's 32 byte directory entry and it will return true or false on if it's a valid file or not.
//faster than parsing the entire file and then checking for validity
//good for both FAT16 and FAT32
bool validFileOrNot(char *data)
{
    //if any of these bits are equal to 1, then this isn't a file I care about
    if ((data[0x0B] & 0xCA) > 0)
    {
        return false;
    }

    for (int i = 0; i < 8; i++)
    {   
        if (data[i] < 32 || data[i] > 127)
        {
            //if at any point during the 8 char parsing process, it finds something shitty, add this file to the shit list
            return false;
        }
    }

    //there, the file passed the invalid file screening process
    return true;
}

//get a list of all files in a directory. Currently this function is slow as absolute ever fucking fuck
rarray<fileInfo> *getFilesInDirectory(unsigned int LBA, unsigned int driveNum, filesystemInfo *sys)
{
    //rarray<fileInfo> *allFoundFiles = new rarray<fileInfo>();
    rarray<fileInfo> *allFoundFiles = new rarray<fileInfo>();
    allFoundFiles->resize(30);      //maybe this will make it faster
    //allFoundFiles->clear();

    // there are always multiple sectors per cluster and directory entries take up at minimum 1 entire cluster each
    // char *theCluster = ata_lba_read(LBA, driveNum);
    //for (int u = 0; u < sys->sectorsPerCluster; u++)
    //{

        //char *theSector = ata_lba_read(LBA+u, driveNum);
        char *theSector = ata_lba_read(LBA, driveNum, sys->sectorsPerCluster);
        //consoleNewLine();
        //printString("got bytes from hard drive", 0x0F);
        //consoleNewLine();
        int entriesToParse = 16*sys->sectorsPerCluster; //make sure it's not redoing that multiplication operation on each loop iteration. That shit is slow on a 486
        for (int i = 0; i < (entriesToParse); i++)
        {
            //fileInfo newFile = fileFromData(theSector + (i * 32), sys);
            //screen for bogus files here to avoid having to move them around in memory later
            //if (newFile.isValidFile)
            //if (validFileOrNot(theSector + (i * 32)))       //check for file validity before producing a file from file data
            if (validFileOrNot(theSector + (i << 5))) 
            {
                //fileInfo newFile = fileFromData(theSector + (i * 32), sys);
                //fileInfo newFile = fileFromData(theSector + (i << 5), sys);     //bit shift for higher speeds
                //allFoundFiles->push_back(newFile);
                allFoundFiles->push_back(fileFromData(theSector + (i << 5), sys));
                //consoleNewLine();
                //printString("pushed back file", 0x0F);
                //it seems like discarding unuses "invalid" files speeds it up
                //i think discarding them without deleting them could in theory cause memory leaks but that doesn't appear to happen for some reason
            }
        }

        //no memory leaks
        free(theSector);
    //}


    //return the pointer without clearing it. iirc this doesn't work on normal c++ but it works here
    return allFoundFiles;
}

//input filesystem information and this returns the LBA address of the root directory of the drive
unsigned int rootLBA(filesystemInfo *sys)
{   
    //oddly enough, this seems to work for both FAT16 and FAT32
    unsigned int result = sys->startingLBA + (sys->reservedSectors) + (sys->sectorsPerFat * 2);
    return result;
}

//return the LBA address of the first FAT
unsigned int FATLBA(filesystemInfo *sys)
{
    //I have officially got to the point where I can no longer fully comprehend what's going on with FAT ass at the same time. 
    //Good thing I put a lot of effort into fixing all the bugs as I encountered them, now I can rely on my previously written code
    unsigned int result = sys->startingLBA + (sys->reservedSectors);      //not yet tested on FAT16. Note that FAT16 FAT entires take up 16 bits and FAT32 FAT entries take up 32 bits
    return result;
}

//return the cluster number where the provided file or directory is located
unsigned int getFileCluster(fileInfo *file, filesystemInfo *sys)
{
    //first, get the low cluster number
    unsigned int cluster = (file->clusterNumberLow & 0x0000FFFF);

    //if this is a FAT32 partition, 
    if (sys->partitionType == 0x0B || sys->partitionType == 0x0C)
    {
        cluster += ((file->clusterNumberHigh & 0x0000FFFF)<<16);
        //cluster -= 2;       //you always have to subtract 2 from this number unless it's 0 in FAT32 mode for some reason
        if (cluster > 1)
        {
            //what a stupid hack
            cluster = cluster - 2;;
        }
        //if you think this sucks, you haven't seem the z80 assembly version of this. The 11th layer of Hell is where Satan forces you to implement write support in the z80 FAT32 driver.
    }

    return cluster;
}

//returns the absolute LBA of the contents of the given file or directory
unsigned int LBAofFile(fileInfo *file, filesystemInfo *sys)
{
    /*//first, get the low cluster number
    unsigned int cluster = (file.clusterNumberLow & 0x0000FFFF);

    //if this is a FAT32 partition, 
    if (sys.partitionType == 0x0B || sys.partitionType == 0x0C)
    {
        cluster += ((file.clusterNumberHigh & 0x0000FFFF)<<16);
        //cluster -= 2;       //you always have to subtract 2 from this number unless it's 0 in FAT32 mode for some reason
        if (cluster > 1)
        {
            //what a stupid hack
            cluster = cluster - 2;;
        }
        //if you think this sucks, you haven't seem the z80 assembly version of this. The 11th layer of Hell is where Satan forces you to implement write support in the z80 FAT32 driver.
    }*/
    //figure out the cluster number
    unsigned int cluster = getFileCluster(file, sys);

    //unsigned int num = (cluster * sys.sectorsPerCluster) + sys.startingLBA + (sys.reservedSectors) + (sys.sectorsPerFat * 2);
    unsigned int num = (cluster * sys->sectorsPerCluster) + rootLBA(sys);

    consoleNewLine();
    printString("calcd lba is ", 0x0F);
    printInt(num, 0x0F);
    consoleNewLine();

    return num;
}

fileInfo getFileByName(unsigned int currentDirectoryLBA, unsigned int driveNum, filesystemInfo *sys, string fileName)
{
    //get all the files in the directory
    rarray<fileInfo> *allFoundFiles = getFilesInDirectory(currentDirectoryLBA, driveNum, sys);
    fileInfo fileToReturn;
    fileToReturn.isValidFile = false;
    //figure out if any of them match
    for (int i = 0; i < allFoundFiles->getSize(); i++)
    {
        //consoleNewLine();
        //printString(allFoundFiles->at(i).fileName, 0x0F);
        if (allFoundFiles->at(i).fileName == fileName)
        {
            fileToReturn = allFoundFiles->at(i);
            //terminate the loop if file found
            i = allFoundFiles->getSize() + 1;
            //fileToReturn.isValidFile = true;
        }
    }

    //we have to erase each string before clearing the rarray because strings contain char arrays
    for (int i = 0; i < allFoundFiles->getSize(); i++)
    {
        allFoundFiles->at(i).~fileInfo();
    }
    //now do all the other auxillary memory leak prevention stuff
    //allFoundFiles->manual_delete();
    /*for (int i = 0; i < allFoundFiles->getSize(); i++)
    {
        allFoundFiles->at(i).~fileInfo();
    }*/
    //lol why is this loop in there twice? (not deleting just in case its important)
    allFoundFiles->manual_delete();
    free(allFoundFiles);

    return fileToReturn;
}

bool isExitCodeFat16(char *data)
{

    short code = (*(short*)(data))&0x0000FFFF;
    if (code > 0xFFF7)      //16 bit values. upper 4 bits is reserved on both FAT16 and FAT32 despite having different FAT entry sizes
    {
        //i THINK this is all you do
        return true;
    }
    else
    {
        return false;
    }
}


bool isExitCodeFat32(char *data)
{
    unsigned int code = *(unsigned int*)(data);
    if (code > 0xFFFFFF7)
    {
        //i THINK this is all you do
        return true;
    }
    else
    {
        return false;
    }
}

bool isExitCodeFAT(char *data, filesystemInfo *sys)
{
    //FAT16 and FAT32 handle their FAT entries a little differently
    if (sys->partitionType == 0x0B || sys->partitionType == 0x0C)
    {
        //if the filesystem is FAT32, figure out if this is a FAT32 exit code
        return isExitCodeFat32(data);
    }
    else
    {
        //if the filesystem is FAT16, figure out if this is a FAT16 exit code
        return isExitCodeFat16(data);
    }
}

//returns a list of cluster numbers that a file takes up, as reported by the FAT, in ascending order. 
//FAT16 currently doesn't work with this, don't know why
//this was actually really hard to make. Not as hard as the memory allocator but still damn hard.
//the worst function. so far.
rarray<unsigned int>* fileClusters(fileInfo *file, filesystemInfo *sys, unsigned int driveNum)
{
    unsigned int fat16or32 = 2;     //FAT16 has 16 bit FATs. FAT32 has 32 bit FATs. Huh. (the vlue gets changed to 4 later on if the filesystem is detected as FAT32)
    //make the array to return. dont forget to clear it the correct way after you're done with the return value
    rarray<unsigned int> *fileFATList = new rarray<unsigned int>();
    fileFATList->resize(30);//resizing rarrays that was created with the new operator to some size greater than 11 slightly improves performance

    //goto the LBA position for the first cluster in the file
    unsigned int FATlocation = ((getFileCluster(file, sys)+2) * 2);     //in FAT16, it's 2 bytes per FAT block. If the volume is later detected as FAT32, this number can be multiplied by 2 since FAT32 is 4 bytes per FAT block
    fileFATList->push_back((getFileCluster(file, sys)+2));

    //get the correct LBA address of where that FAT entry is supposed to be
    unsigned int blocksPerSector = 256 * sys->sectorsPerCluster;
    if (sys->partitionType == 0x0B || sys->partitionType == 0x0C)
    {
        blocksPerSector = 128 * sys->sectorsPerCluster;  //FAT32 blocks are twice as big
        FATlocation *= 2;
        fat16or32 = 4;
    }
    unsigned int LBAOffset = FATlocation / (blocksPerSector/sys->sectorsPerCluster*fat16or32);       //it seems like it rounds down. Good
    char *FATsector = ata_lba_read(FATLBA(sys) + LBAOffset, driveNum);        //Hopefully this is correct. Source: I thought about this like 3x as long as usual and while petting a cat.

    //now figure out the relative location in the char array of where the desired FAT block is
    unsigned int blockLocation = FATlocation % (blocksPerSector/sys->sectorsPerCluster*fat16or32);

    bool lastBlock = isExitCodeFAT((FATsector+blockLocation), sys);
    fileFATList->push_back(*(unsigned int*)(FATsector+blockLocation));
    //fileFATList->push_back(blockLocation);        //no more using the fat list for debug, I have thingg that depend on it now
    //fileFATList->push_back(LBAOffset);
    unsigned int counter = 0;
    while (!lastBlock/* && counter < 7*/)
    {
        /*if (sys.partitionType == 0x0B || sys.partitionType == 0x0C)
        {
            blockLocation = blockLocation / 4;
        }
        else
        {
            blockLocation = blockLocation / 2;
        }*/

        //find the next block
        //FATlocation = *(unsigned int*)((FATsector+blockLocation))*4;      //set FATsector to whatever the new value is the FAT block is
        blockLocation = *(unsigned int*)((FATsector+blockLocation));      //needs to be *2 for FAT16
        FATlocation = blockLocation*fat16or32;
        //if (sys.partitionType == 0x06) FATlocation = FATlocation / 2;
        //LBAOffset = FATlocation / blocksPerSector;
        //FATlocation = FATlocation * blocksPerSector;
        //FATlocation = ((getFileCluster(file, sys)+2) * 2);
        //FATlocation = blockLocation;

        if ((FATlocation / (blocksPerSector/sys->sectorsPerCluster*fat16or32)) != LBAOffset)
        {
            //if the new cluster is at a far away enough position that it's in a different drive sector, recalculate the important values and fetch the new drive sector
            //LBAOffset = FATlocation / blocksPerSector * 4;      //assign the new value to the variable
            LBAOffset = FATlocation / (blocksPerSector/sys->sectorsPerCluster*fat16or32);
            free(FATsector);        //avoid memory leaks
            FATsector = ata_lba_read(FATLBA(sys) + LBAOffset, driveNum);        //fetch the drive sector from the location according to math
        }
        blockLocation = FATlocation % (blocksPerSector/sys->sectorsPerCluster*fat16or32);
        lastBlock = isExitCodeFAT((FATsector+blockLocation), sys);
        fileFATList->push_back(*(unsigned int*)(FATsector+blockLocation));
        //fileFATList->push_back(blockLocation);
        //fileFATList->push_back(LBAOffset);
        //fileFATList->push_back(0);
        //lastBlock = true;
        counter++;
    }

    free(FATsector);

    return fileFATList;
}

//the size of a file on disk, in clusters - the smallest segment a file can be assigned to on a FAT. Clusters are always made out of multiple sectors.
unsigned int sizeOfFile(fileInfo *file, filesystemInfo *sys, unsigned int driveNum)
{
    rarray<unsigned int> *blocks = fileClusters(file, sys, driveNum);
    unsigned int num = blocks->getSize() * sys->bytesPerSector * sys->sectorsPerCluster;
    blocks->manual_delete();
    free(blocks);
    //Sdelete blocks;
    return num;
    //return 69;
}

//load a file into ram and return the address that it's loaded at
void* loadFile(fileInfo *file, unsigned int driveNum, filesystemInfo *sys)
{
    if(file->isValidFile)
    {
        //get a list of clusters that the file takes up
        rarray<unsigned int> *blocks = fileClusters(file, sys, driveNum);

        //allocate a spot in memory to store the file
        //char *fileStore = (char *)malloc(foundFile.size);
        char *fileStore = (char *)malloc(sizeOfFile(file, sys, driveNum));
        //char *fileStore = (char *)malloc(10024);
        //unsigned int debugInt = 0;
        for (int i = 0; i < blocks->getSize(); i++)
        {
            //consoleNewLine();
            //printInt(blocks->at(i)-2, 0x0F);
            char poop[3];
            *(unsigned int*)poop = blocks->at(i);
            //if (!isExitCodeFAT((char*)blocks->at(i), selectedVolume.fsinfo))
            if (!isExitCodeFAT(poop, sys))
            //if (blocks->at(i) < 0xFFFFFF7)
            {
                //for (int s = 0; s < sys->sectorsPerCluster; s++)
                //{
                    unsigned int lbaOfFileCluster = ((blocks->at(i)-2) * sys->sectorsPerCluster) + rootLBA(sys);
                    //lbaOfFileCluster-=2;     //you need to do this for some reason. I'm sure FAT16 will need other special handling but I already had to give up on FAT16 in fileClusters
                    //*(char*)(fileStore+(i*512)) = ata_lba_read(lbaOfFileCluster, MBR_DRIVE_MODE);
                    char *bytes = ata_lba_read(lbaOfFileCluster, driveNum, sys->sectorsPerCluster);
                    /*for (int u = 0; u < 512; u++)
                    {
                        // copy each bytes from the ata lba read into the block of allocated memory designated to store the contents of the file in
                        //fileStore[(i * 512) + u] = bytes[u];
                        *(char*)(fileStore+(((i + s) * 512) + u)) = *(char*)(bytes+u);
                        //printChar('.',0x0F);
                        debugInt++;
                    }*/
                    memcpy((fileStore+(i * sys->bytesPerSector * sys->sectorsPerCluster)), (bytes), sys->bytesPerSector*sys->sectorsPerCluster);        //memcpy is the most optimized memory copy right now
                    //debugInt++;
                    free(bytes); // don't need this anymore
                //}
            }
        }
        consoleNewLine();
        //printInt(debugInt, 0x0F);

        //before any freeing happens, display how much memory is in use
        //printMemoryManagement();

        //free(fileStore);    //avoid memory leak
        //if forgetting to free this doesn't cause major memory leaks then something is really wrong

        /*for (int i = 0; i < blocks->getSize(); i++)
        {
            consoleNewLine();
            printInt(blocks->at(i), 0x0F, true);
        }*/
        blocks->manual_delete();
        free(blocks);
        return fileStore;
    }
    else
    {
        //if the file was invalid, return nullptr
        return nullptr;
    }

    return nullptr;
}

//returns the number of clusters the file or directory takes up as reported by FAT
unsigned int clusterSizeFAT(fileInfo file, filesystemInfo sys)
{

}

//copy file to specified location in ram without using memory management. Use this for copying a boot image into ram before jumping to it
bool fileToRam(unsigned int desiredLocation, fileInfo *file, unsigned int driveNum, filesystemInfo *sys)
{
    if (file->isValidFile)
    {
        //load the file into memory
        void *storedFile = loadFile(file, driveNum, sys);

        //the file would've got loaded to whatever random spot the memory manager decided, so copy it to the specified place now
        memcpy((void*)desiredLocation, storedFile, file->size);
        free(storedFile);
        return true;
    }
    else
    {
        //if invalid, false
        return false;
    }
}

#endif